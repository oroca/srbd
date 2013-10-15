//----------------------------------------------------------------------------
//    프로그램명 	: LED 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_Led.c
//----------------------------------------------------------------------------


//----- 헤더파일 열기
//
#define  HW_LED_LOCAL


#include "Hw_Led.h"





/*---------------------------------------------------------------------------
     TITLE   : Hw_Led_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Led_Init( void )
{		
	Hw_Led_Off(0);
	Hw_Led_Off(1);
	Hw_Led_Off(2);
	Hw_Led_Off(3);
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Led_On
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Led_On( u8 Ch )
{	
	switch( Ch )
	{
		case 0:
			Hw_VBus_ClrBit( HW_VBUS_CS_LED_1, 0 );
			break;

		case 1:
			Hw_VBus_ClrBit( HW_VBUS_CS_LED_1, 1 );
			break;
			
		case 2:
			Hw_VBus_ClrBit( HW_VBUS_CS_LED_1, 2 );
			break;

		case 3:
			Hw_VBus_ClrBit( HW_VBUS_CS_LED_1, 3 );
			break;			
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Led_On
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Led_Off( u8 Ch )
{
	switch( Ch )
	{
		case 0:
			Hw_VBus_SetBit( HW_VBUS_CS_LED_1, 0 );
			break;

		case 1:
			Hw_VBus_SetBit( HW_VBUS_CS_LED_1, 1 );
			break;

		case 2:
			Hw_VBus_SetBit( HW_VBUS_CS_LED_1, 2 );
			break;

		case 3:
			Hw_VBus_SetBit( HW_VBUS_CS_LED_1, 3 );
			break;			
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Led_Toggle
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Led_Toggle( u8 Ch )
{
	switch( Ch )
	{
		case 0:
			Hw_VBus_TglBit( HW_VBUS_CS_LED_1, 0 );
			break;

		case 1:
			Hw_VBus_TglBit( HW_VBUS_CS_LED_1, 1 );
			break;

		case 2:
			Hw_VBus_TglBit( HW_VBUS_CS_LED_1, 2 );
			break;

		case 3:
			Hw_VBus_TglBit( HW_VBUS_CS_LED_1, 3 );
			break;			
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Led_Wait
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Led_Wait( u32 delay )
{
    volatile u32 i;
    for ( i = 0 ; i < delay ; i++ ){ };
}



















 
