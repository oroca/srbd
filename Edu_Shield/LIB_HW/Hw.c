//----------------------------------------------------------------------------
//    프로그램명 	: Hw 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw.c
//----------------------------------------------------------------------------


//----- 헤더파일 열기
//
#define  HW_LOCAL  

#include "Hw.h"






/*---------------------------------------------------------------------------
     TITLE   : Hw_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Init( void )
{
	//-- PLL 설정
	//
	Hw_PLL_Init();
	
	
	//-- 인터럽트 관련 초기화
	//
	Hw_ISR_Init();
	
		
	//-- GPIO 기본값 설정
	//
	REG_GPIOA_CRL = 0x33333333;	// PA0 - 7
	REG_GPIOA_CRH = 0x333334B3;	// PA8 - 15    //PA9,10,11,12는 가각 UART, USB에 할당됨
	REG_GPIOB_CRL = 0x33333333;	// PB0 - 7
	REG_GPIOB_CRH = 0x33333333;	// PB8 - 15
	REG_GPIOC_CRH = 0x33444444;	// PC14, 15
	
	//Hw_DMA_Init();
	Hw_Uart_Init();
	Hw_Timer_Init();

	Hw_Adc_Init();

	//Hw_Si47xx_Init();
	//Hw_N5110G_Init();
	Hw_VBus_Init();
	Hw_VCom_Init();
	
	Hw_Led_Init();
	Hw_KeyPad_Init();
	Hw_7Seg_Init();
	Hw_CLcd_Init();
	Hw_Cds_Init();
	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Wait
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Wait( u32 delay )
{
    volatile u32 i;
    for ( i = 0 ; i < delay ; i++ ){ };
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Wait_Usec
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Wait_Usec( u32 usec )
{
    Hw_Wait( usec * 6 );
}

















 
