//----------------------------------------------------------------------------
//    프로그램명 : DHT11 관련 함수
//
//    만든이     : Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  : 
//
//    MPU_Type   : 
//
//    파일명     : Hw_DHT11.c
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  HW_DHT11_LOCAL  

#include "Hw_DHT11.h"



#define HW_DHT11_DEBUG		0



//----- 내부 함수
void Hw_DHT11_Timer_Setup(void);
void Hw_DHT11_IO_Setup(void);
void Hw_DHT11_IO_SetupISR(void);
void Hw_DHT11_IO_SetOutput(void);
void Hw_DHT11_IO_SetInput(void);
void Hw_DHT11_IO_SetHigh(void);
void Hw_DHT11_IO_SetLow(void);
u8   Hw_DHT11_IO_Get(void);

void Hw_DHT11_IO_EnableISR(void);
void Hw_DHT11_IO_DisableISR(void);


//----- 내부 변수
u32 Hw_DHT11_BitIndex = 0;
u16 Hw_DHT11_BitTbl[45];


HW_DHT11_DATA_OBJ Hw_DHT11_Log;



void Hw_DHT11_Tick(void)
{
	static u32 State = 0;
	static u32 Cnt = 0;
	u32 i;
	u32 RegNum;
	u32 BitNum;
	u8  CheckSum;

	switch( State )
	{
		//-- PowerON 후 1초를 기다린다.
		//
		case 0:

			if( Cnt > 1000/10 )
			{
				Cnt   = 0;
				State = 1;
			}
			break;

		//-- Start Pulse (20ms)
		//
		case 1:

			if( Cnt == 1 )
			{
				Hw_DHT11_IO_SetLow();	
			}
			
			if( Cnt >= 20/10 )	
			{
				Hw_DHT11_BitIndex = 0;
				Hw_DHT11_IO_SetHigh();
				Hw_DHT11_IO_SetInput();				
				Hw_DHT11_IO_EnableISR();				
				State = 2;	
			}
			break;

		//-- End 
		//
		case 2:
			if( Cnt == 500/10 )
			{
				Hw_DHT11_IO_DisableISR();								
				Hw_DHT11_IO_SetHigh();
				Hw_DHT11_IO_SetOutput();
			}

			//-- 1초마다 센서 데이터를 업데이트 한다.
			//
			if( Cnt >= 1200/10 )
			{
				Cnt   = 0;
				State = 1;
				Hw_DHT11_Log.Data[0] = 0;
				Hw_DHT11_Log.Data[1] = 0;
				Hw_DHT11_Log.Data[2] = 0;
				Hw_DHT11_Log.Data[3] = 0;
				Hw_DHT11_Log.Data[4] = 0;

				for( i=0; i<40; i++ )
				{
					RegNum = i/8;
					BitNum = i%8;

					if( Hw_DHT11_BitTbl[i] > 100 )
					{
						SET_BIT( Hw_DHT11_Log.Data[RegNum], 7-BitNum );
					}
				}

				CheckSum = 0;

				for( i=0; i<4; i++ )
				{
					CheckSum += Hw_DHT11_Log.Data[i];
				}

				// CheckSum 확인 
				if( CheckSum == Hw_DHT11_Log.Data[4] )
				{
					Hw_DHT11_Log.Error       = 0;
					Hw_DHT11_Log.Humidity    = Hw_DHT11_Log.Data[0];
					Hw_DHT11_Log.Temperature = Hw_DHT11_Log.Data[2];

					#if HW_DHT11_DEBUG == 1
					Lb_printf("H %d T %d \n", Hw_DHT11_Log.Humidity, Hw_DHT11_Log.Temperature );
					#endif
				}
				else
				{
					Hw_DHT11_Log.Error = 1;

					#if HW_DHT11_DEBUG == 1
					Lb_printf("Fail\n");
					for( i=0; i<5; i++ )
					{
						Lb_printf("%d %x \n", i, Hw_DHT11_Log.Data[i] );
					}

					for( i=0; i<40; i++ )
					{
						Lb_printf("Bit %d %d \n", i, Hw_DHT11_BitTbl[i] );
					}					
					#endif

				}
			}
			
			break;
	}

	Cnt++;
}





void Hw_DHT11_IO_ISR(void)
{
	static u16 TimerCntSaved = 0;
	u16 TimerCntCur;

	//-- 인터럽트 팬딩 레지스터 Clear
	//
	REG_EXTI_PR = (1<<6);


	if( Hw_DHT11_BitIndex >= 2 && Hw_DHT11_BitIndex <= 41 )
	{
		TimerCntCur = REG_TIM2_CNT;
			
		Hw_DHT11_BitTbl[Hw_DHT11_BitIndex-2] = TimerCntCur - TimerCntSaved;
	}


	Hw_DHT11_BitIndex++;


	if( Hw_DHT11_BitIndex >= 42 )
	{
		Hw_DHT11_BitIndex = 42;	
	}

	TimerCntSaved = REG_TIM2_CNT;
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_DHT11_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_DHT11_Init(void)
{
	Hw_DHT11_Log.Data[0] = 0;
	Hw_DHT11_Log.Data[1] = 0;
	Hw_DHT11_Log.Data[2] = 0;
	Hw_DHT11_Log.Data[3] = 0;
	Hw_DHT11_Log.Error		 = 0;
	Hw_DHT11_Log.DataValid   = FALSE;
	Hw_DHT11_Log.Humidity    = 0;
	Hw_DHT11_Log.Temperature = 0;


	Hw_DHT11_IO_Setup();
	Hw_DHT11_Timer_Setup();

	Hw_DHT11_IO_SetupISR();


	Hw_Timer_Set  ( HW_TIMER_CH_DHT11, 10, LOOP_TIME, Hw_DHT11_Tick, NULL );	
	Hw_Timer_Start( HW_TIMER_CH_DHT11 );
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DHT11_GetHumidity
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_DHT11_GetHumidity(void)
{
	u8 Humidity;

	
	Disable_ISR();
	Humidity = Hw_DHT11_Log.Humidity;
	Enable_ISR();

	return Humidity;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DHT11_GetTemperature
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_DHT11_GetTemperature(void)
{
	u8 Temperature;

	
	Disable_ISR();
	Temperature = Hw_DHT11_Log.Temperature;
	Enable_ISR();

	return Temperature;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DHT11_Start
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_DHT11_Start(void)
{
	Hw_Timer_Reset( HW_TIMER_CH_DHT11 );
	Hw_Timer_Start( HW_TIMER_CH_DHT11 );
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DHT11_Stop
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_DHT11_Stop(void)
{
	Hw_Timer_Stop( HW_TIMER_CH_DHT11 );
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DHT11_Timer_Setup
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_DHT11_Timer_Setup(void)
{
	SET_BIT( REG_RCC_APB1ENR, 0 );		// TIM2 Clock Enable APB1 = 36Mhz

	REG_TIM2_PSC = 72-1; 				// 36Mhz/36 = 1Mhz

	SET_BIT( REG_TIM2_CR1, 0 );			// Counter Enable 		 
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DHT11_TimerInit
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_DHT11_IO_Setup(void)
{
	Hw_DHT11_IO_SetOutput();
	Hw_DHT11_IO_SetHigh();		
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DHT11_IO_SetupISR
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_DHT11_IO_SetupISR(void)
{

	// 1. 인터럽트 핀 설정 
	//
	REG_AFIO_EXTICR2 &= ~(0x0F<<8);
	REG_AFIO_EXTICR2 |=  (0x00<<8);	// EXTI6, PA.6을 외부 인터럽트로 설정 

	// 2. Falling trigger로 설정
	// 
	REG_EXTI_FTSR |= (1<<6);	 


	Hw_ISR_SetIRQFunc( 23, (u32)Hw_DHT11_IO_ISR, 0 );	
	SET_BIT( REG_NVIC_ISER( 23/32 ), 23%32 );	// NVIC 37번 EXTI0 인터럽트 활성화
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DHT11_IO_EnableISR
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_DHT11_IO_EnableISR(void)
{
	// EXTI6 Enable
	// 
	REG_EXTI_EMR |= (1<<6);	// Event Enable
	REG_EXTI_IMR |= (1<<6);	// Interrupt nable	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DHT11_IO_DisableISR
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_DHT11_IO_DisableISR(void)
{
	// EXTI6 Disable
	// 
	REG_EXTI_EMR &= ~(1<<6);	// Event Enable
	REG_EXTI_IMR &= ~(1<<6);	// Interrupt nable	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DHT11_IO_SetOutput
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_DHT11_IO_SetOutput(void)
{
	//-- PA.6
	//
	REG_GPIOA_CRL &= ~(0x0F << (24));	// Clear
	REG_GPIOA_CRL |=  (0x03 << (24));	// MODE, PA.6 Output mode, max speed 50Mhz
	REG_GPIOA_CRL |=  (0x00 << (26));	// CNF,  PA.6 General purpose output push-pul		
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DHT11_IO_SetInput
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_DHT11_IO_SetInput(void)
{
	REG_GPIOA_CRL &= ~(0x0F << (24));	// Clear
	REG_GPIOA_CRL |=  (0x00 << (24));	// MODE, PA.6 Input Mode
	REG_GPIOA_CRL |=  (0x01 << (26));	// CNF,  PA.6 Floating input	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DHT11_IO_SetHigh
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_DHT11_IO_SetHigh(void)
{
	SET_BIT( REG_GPIOA_ODR, 6 );		
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DHT11_IO_SetLow
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_DHT11_IO_SetLow(void)
{
	CLR_BIT( REG_GPIOA_ODR, 6 );		
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DHT11_IO_Get
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_DHT11_IO_Get(void)
{
	if( IS_SET_BIT( REG_GPIOA_IDR, 6 ) ) return 1;
	else								 return 0;	
}


