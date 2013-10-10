//----------------------------------------------------------------------------
//    프로그램명 	: Si47xx 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_Si47xx.c
//----------------------------------------------------------------------------


//----- 헤더파일 열기
//
#define  HW_SI47XX_LOCAL


#include "Hw_Si47xx.h"
#include "Hw_I2C.h"

//#define HW_SI47XX_ADDR				(0x22 >> 1)		// Si47xx Slave 주소
#define HW_SI47XX_ADDR		(0xC6 >> 1)		// Si47xx Slave 주소

#define HW_SI47XX_MAX_LENGTH_CMD	8
#define HW_SI47XX_MAX_LENGTH_RESP	16



#define HW_SI47XX_DEBUG_SHOW		0
#define HW_SI47XX_DEBUG_SHOW_RESP	0



//-- 내부 변수
//

u8   Hw_Si47xx_LastResp[HW_SI47XX_MAX_LENGTH_RESP];


//-- 내부 함수
//
void Hw_Si47xx_Reset( void );

u16  Hw_Si47xx_CommandResp( u8 *pCmd, u16 CmdLength, u8 *pResp, u16 RespLength, u32 TimeOut );
u16  Hw_Si47xx_SendCommand( u8 *pCmd, u16 Length );
u16  Hw_Si47xx_ReceiveResponse( u8 *pResp, u16 Length, u32 TimeOut );
u16  Hw_Si47xx_WaitForCTS( void  );




/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Si47xx_Init( void )
{
	Hw_I2C_Init();


	//-- Reset 핀 GPIO 초기화 PB.5
	//
	//-- RSTB PB.5
	//
	REG_GPIOB_CRL &= ~(0x0F << (20));	// Clear
	REG_GPIOB_CRL |=  (0x03 << (20));	// MODE, PB.5 Output mode, max speed 50Mhz
	REG_GPIOB_CRL |=  (0x00 << (22));	// CNF,  PB.5 General purpose output push-pul	
	

	Hw_Si47xx_Reset();
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_Reset
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Si47xx_Reset( void )
{
	Hw_Wait(100);
	SET_BIT( REG_GPIOB_ODR, 5 );		// RSTB = 1	
	Hw_Wait(100);
	CLR_BIT( REG_GPIOB_ODR, 5 );		// RSTB = 0	
	Hw_Wait(500);	
	SET_BIT( REG_GPIOB_ODR, 5 );		// RSTB = 1		
	Hw_Wait(100);
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_FM_Config
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_FM_Config( void )
{
	u16 Ret;
	u8  Status;
	u32 i;
	u8  Cmd [HW_SI47XX_MAX_LENGTH_CMD];
	u8  Resp[HW_SI47XX_MAX_LENGTH_RESP];	

	u16 PropertyValue;
	
	if( (Ret = Hw_Si47xx_POWER_UP(HW_SI47XX_BAND_MODE_FM)) != ERR_NONE )	return Ret;
		
	Hw_Timer_Delay_ms(120);
	
	
	//-- GET_REV
	//
	#if HW_SI47XX_DEBUG_SHOW == 1
	HW_SI47XX_PRINTF("GET_REV\n");
	#endif

	Cmd[0] = 0x10;
	if( (Ret = Hw_Si47xx_CommandResp( Cmd, 1, Resp, 8, 1000 )) != ERR_NONE ) return Ret;
	
	
	Ret = Hw_Si47xx_SET_PROPERTY( 0x0001, 0x0005 );		// Seek/Tune Complete Interrupt Enable
	Ret = Hw_Si47xx_SET_PROPERTY( 0x0201, 32768 );		// REFCLK_FREQ
	Ret = Hw_Si47xx_SET_PROPERTY( 0x0202, 1     );		// PRESCALE

		
	Ret = Hw_Si47xx_SET_PROPERTY( 0x1502, 0     );		// FM_RDS_CONFIG
	Ret = Hw_Si47xx_SET_PROPERTY( 0x1100, 1     );		// FM_DEEMPHASIS
	Ret = Hw_Si47xx_SET_PROPERTY( 0x1400, 7600 );		// FM_SEEK_BAND_BOTTOM
	Ret = Hw_Si47xx_SET_PROPERTY( 0x1401, 10790 );		// FM_SEEK_BAND_TOP
	Ret = Hw_Si47xx_SET_PROPERTY( 0x1402, 10    );		// FM_SEEK_FREQ_SPACING
	//Ret = Hw_Si47xx_SET_PROPERTY( 0x1403, 3     );		// FM_SEEK_TUNE_SNR_THRESHOLD   3
	//Ret = Hw_Si47xx_SET_PROPERTY( 0x1404, 20    );		// FM_SEEK_TUNE_RSSI_THRESHOLD


	Ret = Hw_Si47xx_SET_PROPERTY( 0x4001, 0  );
	Ret = Hw_Si47xx_SET_PROPERTY( 0x4000, 63 );				

	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_AM_Config
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_AM_Config( void )
{
	u16 Ret;
	u8  Status;
	u32 i;
	u8  Cmd [HW_SI47XX_MAX_LENGTH_CMD];
	u8  Resp[HW_SI47XX_MAX_LENGTH_RESP];	

	u16 PropertyValue;
	
	if( (Ret = Hw_Si47xx_POWER_UP(HW_SI47XX_BAND_MODE_AM)) != ERR_NONE )	return Ret;
		
	Hw_Timer_Delay_ms(120);
	
	
	//-- GET_REV
	//
	#if HW_SI47XX_DEBUG_SHOW == 1
	HW_SI47XX_PRINTF("GET_REV\n");
	#endif

	Cmd[0] = 0x10;
	if( (Ret = Hw_Si47xx_CommandResp( Cmd, 1, Resp, 8, 1000 )) != ERR_NONE ) return Ret;
	
	
	Ret = Hw_Si47xx_SET_PROPERTY( 0x0001, 0x0005 );		// Seek/Tune Complete Interrupt Enable
	Ret = Hw_Si47xx_SET_PROPERTY( 0x0201, 32768 );		// REFCLK_FREQ
	Ret = Hw_Si47xx_SET_PROPERTY( 0x0202, 1     );		// PRESCALE

		
	Ret = Hw_Si47xx_SET_PROPERTY( 0x3102, 1     );		// AM_CHANNEL_FILTER - 4khz
	Ret = Hw_Si47xx_SET_PROPERTY( 0x3100, 1     );		// AM_DEEMPHASIS
	Ret = Hw_Si47xx_SET_PROPERTY( 0x3400, 520 	);		// AM_SEEK_BAND_BOTTOM
	Ret = Hw_Si47xx_SET_PROPERTY( 0x3401, 1710 	);		// AM_SEEK_BAND_TOP
	Ret = Hw_Si47xx_SET_PROPERTY( 0x3402, 1    	);		// AM_SEEK_FREQ_SPACING
	//Ret = Hw_Si47xx_SET_PROPERTY( 0x3403, 11    );		// AM_SEEK_TUNE_SNR_THRESHOLD   3
	//Ret = Hw_Si47xx_SET_PROPERTY( 0x3404, 42    );		// AM_SEEK_TUNE_RSSI_THRESHOLD


	Ret = Hw_Si47xx_SET_PROPERTY( 0x4001, 0  );
	Ret = Hw_Si47xx_SET_PROPERTY( 0x4000, 63 );				

	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_ReadResp
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_Si47xx_ReadResp( u8 Index )
{
	if( Index > HW_SI47XX_MAX_LENGTH_RESP ) Index = HW_SI47XX_MAX_LENGTH_RESP;
	
	return Hw_Si47xx_LastResp[ Index ];
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_WaitForSTC
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_WaitForSTC( u32 TimeOut )
{
	u16 Ret;
	u32 TimeStart;
	u32 TimeEnd;
	u8  Status;
	
	TimeStart = Hw_Timer_Get_CountValue();
	
	while(1)
	{	
		Ret = Hw_Si47xx_GET_INT_STATUS( &Status );
		
		if( Ret == ERR_NONE )
		{
			if( Status & (1<<0) )
			{				
				break;
			}
		}
	
		//-- 타임 아웃 처리
		//
		TimeEnd = Hw_Timer_Get_CountValue();
		
		if( (TimeEnd - TimeStart) > TimeOut )
		{
			Ret = HW_SI47XX_ERR_TIME_OUT;
			break;
		}
	}
	
	return Ret;		
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_POWER_UP
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_POWER_UP( u8 Mode )
{
	u16 Ret;
	u8  Cmd [HW_SI47XX_MAX_LENGTH_CMD];
	u8  Resp[HW_SI47XX_MAX_LENGTH_RESP];
	u16 i;

	//-- 보드 리셋
	//
	Hw_Si47xx_Reset();


	Cmd[0] = 0x01;		// POWER_UP 명령
	
	if( Mode == HW_SI47XX_BAND_MODE_FM )
	{
		Cmd[1] = 0			// FM
				| (0<<7)	// CTS interrupt enabled
				| (1<<4);	// Crystal Oscillator Enable
				
	}
	else
	{
		Cmd[1] = 1			// AM
				| (0<<7)	// CTS interrupt enabled 
				| (1<<4);	// Crystal Oscillator Enable
	}

	
	Cmd[2] = 0xB5;		// Analog Output



	
	Ret = Hw_Si47xx_SendCommand( Cmd, 3 );
		
	if( Ret == ERR_NONE )
	{
		Ret = Hw_Si47xx_ReceiveResponse( Resp, 0+1, 1000 );
	}	
	
	#if HW_SI47XX_DEBUG_SHOW == 1
	Lb_printf("Ret : %x , St : %x \n", Ret, Resp[0] );
	#endif
	
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_SET_PROPERTY
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_SET_PROPERTY( u16 PropertyName, u16 PropertyValue )
{
	u16 Ret;
	u16 Index;
	u8  Cmd [HW_SI47XX_MAX_LENGTH_CMD];
	u8  Resp[HW_SI47XX_MAX_LENGTH_RESP];
	

	#if HW_SI47XX_DEBUG_SHOW == 1
	HW_SI47XX_PRINTF("SET_PROPERTY : %x, %x\n", PropertyName, PropertyValue );
	#endif

	Index = 0;
	
	Cmd[Index++] = 0x12;
	Cmd[Index++] = 0x00;
	Cmd[Index++] = ( PropertyName  >> 8 ) & 0xFF;
	Cmd[Index++] = ( PropertyName  >> 0 ) & 0xFF;
	Cmd[Index++] = ( PropertyValue >> 8 ) & 0xFF;
	Cmd[Index++] = ( PropertyValue >> 0 ) & 0xFF;
	
	Ret = Hw_Si47xx_CommandResp( Cmd, Index, Resp, 0, 1000 );
	
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_GET_PROPERTY
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_GET_PROPERTY( u16 PropertyName, u16 *pPropertyValue )
{
	u16 Ret;
	u16 Index;
	u8  Cmd [HW_SI47XX_MAX_LENGTH_CMD];
	u8  Resp[HW_SI47XX_MAX_LENGTH_RESP];
	

	#if HW_SI47XX_DEBUG_SHOW == 1
	HW_SI47XX_PRINTF("GET_PROPERTY : %x\n", PropertyName );
	#endif

	Index = 0;
	
	Cmd[Index++] = 0x13;
	Cmd[Index++] = 0x00;
	Cmd[Index++] = ( PropertyName  >> 8 ) & 0xFF;	
	Cmd[Index++] = ( PropertyName  >> 0 ) & 0xFF;
	
	Ret = Hw_Si47xx_CommandResp( Cmd, Index, Resp, 3, 1000 );
	
	if( Ret == ERR_NONE )
	{
		*pPropertyValue = (Resp[2]<<8) | (Resp[3]<<0);
	}
	
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_FM_TUNE_FREQ
     WORK    : 
     			FM 주파수 설정으로 10Khz 단위로 설정
     			107.7Mhz -> 10770
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_FM_TUNE_FREQ( u16 Frequency )
{
	u16 Ret;
	u16 Index;
	u8  Cmd [HW_SI47XX_MAX_LENGTH_CMD];
	u8  Resp[HW_SI47XX_MAX_LENGTH_RESP];
	

	#if HW_SI47XX_DEBUG_SHOW == 1
	HW_SI47XX_PRINTF("FM_TUNE_FREQ\n" );
	#endif

	Index = 0;
	
	Cmd[Index++] = 0x20;
	Cmd[Index++] = 0x00;
	Cmd[Index++] = ( Frequency >> 8 ) & 0xFF;			// FREQ_h
	Cmd[Index++] = ( Frequency >> 0 ) & 0xFF;			// FREQ_l
	Cmd[Index++] = 0x00; 								// ANTCAP
	
	Ret = Hw_Si47xx_CommandResp( Cmd, Index, Resp, 0, 1000 );
		
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_AM_TUNE_FREQ
     WORK    : 
     			AM 주파수 설정
     			520khz -> 1710khz
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_AM_TUNE_FREQ( u16 Frequency )
{
	u16 Ret;
	u16 Index;
	u8  Cmd [HW_SI47XX_MAX_LENGTH_CMD];
	u8  Resp[HW_SI47XX_MAX_LENGTH_RESP];
	

	#if HW_SI47XX_DEBUG_SHOW == 1
	HW_SI47XX_PRINTF("AM_TUNE_FREQ\n" );
	#endif

	Index = 0;
	
	Cmd[Index++] = 0x40;
	Cmd[Index++] = 0x00;
	Cmd[Index++] = ( Frequency >> 8 ) & 0xFF;			// FREQ_h
	Cmd[Index++] = ( Frequency >> 0 ) & 0xFF;			// FREQ_l
	Cmd[Index++] = 0x00; 								// ANTCAP
	Cmd[Index++] = 0x00; 								// ANTCAP
	
	Ret = Hw_Si47xx_CommandResp( Cmd, Index, Resp, 0, 1000 );
		
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_FM_SEEK_START
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_FM_SEEK_START( u8 Dir, u8 Mode )
{
	u16 Ret;
	u16 Index;
	u8  Cmd [HW_SI47XX_MAX_LENGTH_CMD];
	u8  Resp[HW_SI47XX_MAX_LENGTH_RESP];
	

	#if HW_SI47XX_DEBUG_SHOW == 1
	HW_SI47XX_PRINTF("FM_SEEK_START\n" );
	#endif

	Index = 0;
	
	Cmd[Index++] = 0x21;
	Cmd[Index++] = (Dir<<3) | (Mode<<2);
	
	Ret = Hw_Si47xx_CommandResp( Cmd, Index, Resp, 0, 1000 );
		
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_AM_SEEK_START
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_AM_SEEK_START( u8 Dir, u8 Mode )
{
	u16 Ret;
	u16 Index;
	u8  Cmd [HW_SI47XX_MAX_LENGTH_CMD];
	u8  Resp[HW_SI47XX_MAX_LENGTH_RESP];
	

	#if HW_SI47XX_DEBUG_SHOW == 1
	HW_SI47XX_PRINTF("AM_SEEK_START\n" );
	#endif

	Index = 0;
	
	Cmd[Index++] = 0x41;
	Cmd[Index++] = (Dir<<3) | (Mode<<2);
	
	Ret = Hw_Si47xx_CommandResp( Cmd, Index, Resp, 0, 1000 );
		
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_GET_INT_STATUS
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_GET_INT_STATUS( u8 *pStatus )
{
	u16 Ret;
	u16 Index;
	u8  Cmd [HW_SI47XX_MAX_LENGTH_CMD];
	u8  Resp[HW_SI47XX_MAX_LENGTH_RESP];
	

	#if HW_SI47XX_DEBUG_SHOW == 1
	HW_SI47XX_PRINTF("GET_INT_STATUS\n" );
	#endif

	Index = 0;
	
	Cmd[Index++] = 0x14;	
	Ret = Hw_Si47xx_CommandResp( Cmd, Index, Resp, 0, 1000 );
	
	*pStatus = Resp[0];
		
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_FM_TUNE_STATUS
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_FM_TUNE_STATUS( u8 Option )
{
	u16 Ret;
	u16 Index;
	u8  Cmd [HW_SI47XX_MAX_LENGTH_CMD];
	u8  Resp[HW_SI47XX_MAX_LENGTH_RESP];
	

	#if HW_SI47XX_DEBUG_SHOW == 1
	HW_SI47XX_PRINTF("FM_TUNE_STATUS\n" );
	#endif

	Index = 0;
	
	Cmd[Index++] = 0x22;	
	Cmd[Index++] = Option;	
	Ret = Hw_Si47xx_CommandResp( Cmd, Index, Resp, 7, 1000 );
		
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_AM_TUNE_STATUS
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_AM_TUNE_STATUS( u8 Option )
{
	u16 Ret;
	u16 Index;
	u8  Cmd [HW_SI47XX_MAX_LENGTH_CMD];
	u8  Resp[HW_SI47XX_MAX_LENGTH_RESP];
	

	#if HW_SI47XX_DEBUG_SHOW == 1
	HW_SI47XX_PRINTF("AM_TUNE_STATUS\n" );
	#endif

	Index = 0;
	
	Cmd[Index++] = 0x42;	
	Cmd[Index++] = Option;	
	Ret = Hw_Si47xx_CommandResp( Cmd, Index, Resp, 7, 1000 );
		
	return Ret;	
}






/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_CommandResp
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_CommandResp( u8 *pCmd, u16 CmdLength, u8 *pResp, u16 RespLength, u32 TimeOut )
{
	u16 Ret;
	#if HW_SI47XX_DEBUG_SHOW_RESP == 1
	u32 i;
	#endif
	
	Ret = Hw_Si47xx_WaitForCTS();

	if( Ret == ERR_NONE )
	{
		Ret = Hw_Si47xx_SendCommand( pCmd, CmdLength );
		
		if( Ret == ERR_NONE )
		{
			Ret = Hw_Si47xx_ReceiveResponse( pResp, RespLength+1, TimeOut );			
			
			if( Ret == ERR_NONE )
			{
				if( pResp[0] & (1<<6) )
				{
					Ret = HW_SI47XX_ERR_RESP;
				}
			}
		}	
	}
	
	#if HW_SI47XX_DEBUG_SHOW_RESP == 1	
	if( Ret == ERR_NONE )
	{
		for( i=0; i<RespLength+1; i++ )
		{
			HW_SI47XX_PRINTF("  %d, %x\n", i, pResp[i] );
		}
	}
	else
	{
		HW_SI47XX_PRINTF("  Fail\n");
	}
	#endif
	
	
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_SendCommand
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_SendCommand( u8 *pCmd, u16 Length )
{
	u16 Ret;
	
	Ret = Hw_I2C_Write( 0, HW_SI47XX_ADDR, pCmd, Length );
	
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_ReceiveResponse
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_ReceiveResponse( u8 *pResp, u16 Length, u32 TimeOut )
{
	u16 Ret;
	u32 TimeStart;
	u32 TimeEnd;
	u32 i;
	
	TimeStart = Hw_Timer_Get_CountValue();
	
	while(1)
	{	
		Ret = Hw_I2C_Read( 0, HW_SI47XX_ADDR, pResp, Length );
		
		if( Ret == ERR_NONE )
		{
			if( pResp[0] & (1<<7) )
			{				
				break;
			}
		}
	
		//-- 타임 아웃 처리
		//
		TimeEnd = Hw_Timer_Get_CountValue();
		
		if( (TimeEnd - TimeStart) > TimeOut )
		{
			Ret = HW_SI47XX_ERR_TIME_OUT;
			break;
		}
	}
	
	if( Ret == ERR_NONE )
	{
		for( i=0; i<Length; i++)
		{
			Hw_Si47xx_LastResp[i] = pResp[i];
		}
	}
	
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Si47xx_WaitForCTS
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Si47xx_WaitForCTS( void  )
{
	u16 Ret;
	u8  Resp[1];

	Ret = Hw_Si47xx_ReceiveResponse( Resp, 1, 1000 );	
	
	return Ret;	
}