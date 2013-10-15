//----------------------------------------------------------------------------
//    프로그램명 : CMD 관련 함수
//
//    만든이     : Cho Han Cheol , LG-Nsys
//
//    날  짜     : 2007.11. 12
//    
//    최종 수정  : 2007.11. 12
//
//    MPU_Type   : 
//
//    파일명     : Cmd_Lib.c
//----------------------------------------------------------------------------





//----- 헤더파일 열기
//
#define  CMD_LIB_LOCAL  

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "Uart_Lib.h"

#include "Cmd_Lib.h"
#include "Uart_Q_Lib.c"
#include "Pwm_Lib.h"
#include "Exam_Lib.h"
#include "BISol_Lib.h"
#include "UNISol_Lib.h"




void Cmd_Menu( void *arg )
{
	//Uart_Print( &Uart_Ch0, "Menu Excuting\n");
	Cmd_ShowTitle();
}



void Cmd_PWM_Exam( void *arg )
{
	Cmd_CmdClear();
	
	
	Cmd_Print("Testing LED\n");
	while( Cmd_Loop() )
	{
		PWM_OUT(0,  PWM_RES/2);
	}
	
	
	PWM_OUT(0,  PWM_RES + 1);
}


/*---------------------------------------------------------------------------
     TITLE   : Cmd_Init

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
void Cmd_Init( void )
{
	int i;
	
	Uart_Q_Init();
	
	Uart_Open( &Uart_Ch0, UART_COM1, UART_115200BPS, UART_48MHZ );

	
	//----- 명령어 사전 포인터 초기화
	//
	for( i=0; i<CMD_DIC_MAX; i++)
	{
		CMD_DIC_FUNC(i)   = NULL_FUNC;  // 모두 어떠한 함수도 가르키지 않는다
	}    
	
	
	Cmd_Size = 0;	
	
	Cmd_AddCmd( "MENU",     "Show Menu"  ,  Cmd_Menu );
	
	Cmd_AddCmd( "LED",      "Test LED"   ,  Exam_LED );
	Cmd_AddCmd( "SEG",      "Test Segment", Exam_SEG );    
	Cmd_AddCmd( "DIP",      "Test Dip Switch", Exam_DipSw );
	Cmd_AddCmd( "KEY",      "Test KeyPad",     Exam_KeyPad );	
	Cmd_AddCmd( "BISTEP",   "BISTEP 0 UP/DOWN",Exam_BIStep );		
	Cmd_AddCmd( "ADC",      "ADC 0",           Exam_Adc );		
	Cmd_AddCmd( "LSEN",     "LSEN 0",          Exam_LSen );		
	Cmd_AddCmd( "SSEN",     "SSEN 0",          Exam_SSen );		
	Cmd_AddCmd( "ISRSEN",   "ISRSEN 0",        Exam_ISRSen );			
	Cmd_AddCmd( "BISOL",    "BISOL  0 UP/DOWN",Exam_BISol );			
	Cmd_AddCmd( "UNISOL",   "UNISOL 0 ON/OFF", Exam_UNISol );			
	Cmd_AddCmd( "ENCODER",  "ENCODER",         Exam_Encoder );			
		
	Cmd_ChSize   = 0;
	Cmd_CmdCheck = 0;
	
}





/*---------------------------------------------------------------------------
     TITLE   : Cmd_AddCmd

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
U16 Cmd_AddCmd( char *NameStr, char *MenuStr,  Cmd_FuncType Func_Ptr )
{
	U16 Ret = 0;

	if( Cmd_Size >= CMD_DIC_MAX ) return 1;  // 사이즈 오버플로어
	
	Util_StrCopy( Cmd_Dic_Ptr[Cmd_Size].NameStr, NameStr );
	Util_StrCopy( Cmd_Dic_Ptr[Cmd_Size].MenuStr, MenuStr );
	CMD_DIC_FUNC_SET( Cmd_Size, Func_Ptr );

	
	Cmd_Size++;
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Cmd_UartPush

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
void Cmd_UartPush( S8 Ch )
{
	//Uart_Q_Push( CMD_UART_CH,  &Ch );
	
	if( Cmd_ChSize >= CMD_STRING_MAX )
	{
		Cmd_ChSize = 0;
	}
	
	Ch = toupper( Ch );
	
	Cmd_CmdStr[ Cmd_ChSize ] = Ch;
	
	Cmd_ChSize++;
	
	Cmd_CmdStr[Cmd_ChSize] = 0;
	
	if( Ch == 0x0D )
	{
		Cmd_ChSize    = 0;
		Cmd_DlyCheck  = 1;
		
		Cmd_CmdCheck  = 1;
		Cmd_LoopCheck = 1;
	}
	
	Uart_Putch( &Uart_Ch0, Ch );

	
	
}






/*---------------------------------------------------------------------------
     TITLE   : Cmd_Clear

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
void Cmd_CmdClear( void )
{
	Cmd_CmdCheck  = 0;
	Cmd_DlyCheck  = 0;
	Cmd_LoopCheck = 0;
	
}




/*---------------------------------------------------------------------------
     TITLE   : Cmd_Clear

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
U8 Cmd_GetCheck( void )
{
	return Cmd_CmdCheck;
}




/*---------------------------------------------------------------------------
     TITLE   : Cmd_UartPop

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
S8 Cmd_UartPop( S8 *Ch )
{
	return Uart_Q_Pop( CMD_UART_CH, Ch );
}





/*---------------------------------------------------------------------------
     TITLE   : Cmd_ShowTitle

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
void Cmd_ShowTitle( void )
{
	U16 i;
	char StrBuf[100];
	
	Cmd_ShowFirstLine();
	Uart_Print( &Uart_Ch0, "\n");
	
	for( i=0; i<Cmd_GetSize(); i++ )
	{
		sprintf( StrBuf, "%02d. %6s - %s\n", i+1, Cmd_Dic_Ptr[i].NameStr, Cmd_Dic_Ptr[i].MenuStr );
		Uart_Print( &Uart_Ch0, StrBuf );
	}
	
	Uart_Print( &Uart_Ch0, "\n");	
	Cmd_ShowLastLine();
}





/*---------------------------------------------------------------------------
     TITLE   : Cmd_ShowCmdLine

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
void Cmd_ShowCmdLine( void )
{
	Uart_Print( &Uart_Ch0, "LG-NSYS>> ");
}






/*---------------------------------------------------------------------------
     TITLE   : Cmd_ShowFirtLine

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
void Cmd_ShowFirstLine( void )
{
	Uart_Print( &Uart_Ch0, "\n");
	Uart_Print( &Uart_Ch0, "*-------------------------------------------------*\n");
	Uart_Print( &Uart_Ch0, "      ezSCRM Main Test Monitor V0.1 \n");
	Uart_Print( &Uart_Ch0, "      Made by Cho Han Cheol\n");
	Uart_Print( &Uart_Ch0, "      LG-NSys\n");
}







/*---------------------------------------------------------------------------
     TITLE   : Cmd_ShowFirtLine

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
void Cmd_ShowLastLine( void )
{
	Uart_Print( &Uart_Ch0, "*-------------------------------------------------*\n");
}






/*---------------------------------------------------------------------------
     TITLE   : Cmd_GetSize

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
U16 Cmd_GetSize( void )
{
	return Cmd_Size;
}




/*---------------------------------------------------------------------------
     TITLE   : Cmd_Print

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
void Cmd_Print( char *Buffer )
{
	Uart_Print( &Uart_Ch0, Buffer );
}






/*---------------------------------------------------------------------------
     TITLE   : Cmd_PutBit

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
void Cmd_PutBit( char BitData )
{
	Uart_U8Bit_PutBit( &Uart_Ch0, BitData );
}





/*---------------------------------------------------------------------------
     TITLE   : Cmd_ExeFunc

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
S16 Cmd_ExeFunc( void )
{
	U16 CmdSize;
	U16 CmdStrSize;
	S16 CmdCheck;
	U16 i;
	
	CmdSize = Cmd_GetSize();
	
	for( i=0; i<CmdSize; i++ )
	{
		CmdStrSize = Util_StrLen( Cmd_Dic_Ptr[i].NameStr );
				   
		CmdCheck = strncmp( Cmd_Dic_Ptr[i].NameStr, Cmd_CmdStr, CmdStrSize );
		
		
		if( CmdCheck == 0 && Cmd_Dic_Ptr[i].Func_Ptr != NULL_FUNC )
		{
		
			Util_StrCopy( Cmd_CmdStrExe, Cmd_CmdStr );
			CMD_DIC_FUNC_EXEC(i, Cmd_CmdStrExe); 
			
			break;
		}
	}
	
	
	if( CmdSize == i )
		Uart_Print( &Uart_Ch0, "Unkown Command\n");
	
	return 0;
}





/*---------------------------------------------------------------------------
     TITLE   : Cmd_Delay

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
S16 Cmd_Delay( long DlyData  )
{
	U32 DlyDataIn = 1000;
	U32 i;
	
	Cmd_DlyCheck = 0;
	
	while( DlyData-- )
	{
		for( i=0; i<DlyDataIn; i++ )
		{
			if( Cmd_DlyCheck == 1 ) break;
		}
	}	
	
	Cmd_DlyCheck = 0;
	
	return 0;
}






/*---------------------------------------------------------------------------
     TITLE   : Cmd_Loop

     WORK    : 

     ARG     : void
              
     RET     : void

     DATE    : 2007. 11. 13
---------------------------------------------------------------------------*/
S16 Cmd_Loop(void)
{		
	if( Cmd_LoopCheck == 1 )
	{
		Cmd_LoopCheck = 0;
		return 0;
	}
	else
		return 1;
}
