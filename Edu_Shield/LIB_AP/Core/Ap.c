//----------------------------------------------------------------------------
//    프로그램명 	: Ap 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Ap.c
//----------------------------------------------------------------------------


//----- 헤더파일 열기
//
#define  AP_LOCAL

#include "Ap.h"








/*---------------------------------------------------------------------------
     TITLE   : Ap_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Ap_Init( void )
{
	/*
	if( Ap_Radio_Init() == ERR_NONE )
	{
		Lb_printf("Radio Init OK : FM Mode\n");
	}
	else
	{
		Lb_printf("Radio Init Fail\n");
	}
	*/

	Ap_StrCmd_Init();
}



 
