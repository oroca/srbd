//----------------------------------------------------------------------------
//    프로그램명 	: Radio
//
//    만든이     	: Cho Han Cheol (Baram)
//
//    날  짜     	: 2013. 8.20.
//
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Main.c
//----------------------------------------------------------------------------
/*
	130820 
		- 초기 프로젝트 작업 시작
	130823 
		- Uart 인터럽트 구현
		
*/




//----- 헤더파일 열기
//
#define  MAIN_LOCAL

#include "Main.h"




//-- 내부 선언
//



//-- 내부 변수
//



//-- 내부 함수
//
static void Main_Init( void );



void LED_Tick( void )
{
	Hw_Led_Toggle(0);
}



/*---------------------------------------------------------------------------
     TITLE   :	main          
     WORK    : 	
     			메인 함수
     ARG     : 	void
     RET     : 	
     			int
---------------------------------------------------------------------------*/
int main(void)
{
	int key;
	u32 TimeStart;
	u32 TimeEnd;
	
	Main_Init();
	

    printf("Radio V0.2\n");


	//-- 명령어 처리
	//
	Ap_RadioMenu_ExeCmd();    


    while(1);
    
    return 0;
}





/*---------------------------------------------------------------------------
     TITLE   : Main_Init
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Main_Init( void )
{
	Hw_Init();
	Ap_Init();
	
	Hw_Timer_Set  ( HW_TIMER_CH_LED, 1000, LOOP_TIME, LED_Tick, NULL );
	Hw_Timer_Start( HW_TIMER_CH_LED );	
}



