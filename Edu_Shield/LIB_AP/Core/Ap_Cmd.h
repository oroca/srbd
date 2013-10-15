//----------------------------------------------------------------------------
//    프로그램명 	: Cmd 관련 함수
//
//    만든이     	: HanCheol Cho
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type		:
//
//    파일명     	: Ap_Cmd.h
//----------------------------------------------------------------------------
                                                                                                 
                                                                                                 
#ifndef  _AP_CMD_H_
#define  _AP_CMD_H_


#ifdef   AP_CMD_LOCAL
#define  EXT_AP_CMD_DEF 
#else
#define  EXT_AP_CMD_DEF     extern
#endif


#include "Ap.h"




#define  CMD_DIC_MAX    20

#define  CMD_STRING_MAX 100


//----- 상수 선언
//
#ifndef    NULL_FUNC
#define    NULL_FUNC    ((void *)0)     // 함수가 할당되어있는지 검사
#endif


typedef void (*Cmd_FuncType)( void*);


//----- 프로그램 사전에 연결된 함수 실행
//
#define    CMD_DIC_FUNC_EXEC(Opcode,arg)                     ( *Cmd_Dic_Ptr[ Opcode ].Func_Ptr )( arg )       

#define    CMD_DIC_FUNC_SET(Opcode,FuncData)             Cmd_Dic_Ptr[Opcode].Func_Ptr = FuncData;          
#define    CMD_DIC_FUNC(Opcode)                          Cmd_Dic_Ptr[Opcode].Func_Ptr





typedef struct
{
	char NameStr[10];
	char MenuStr[50];
	
	//void  ( *Func_Ptr )( void *);
	Cmd_FuncType  Func_Ptr;

} CMD_OBJ;


EXT_CMD_LIB_DEF CMD_OBJ Cmd_Dic_Ptr[CMD_DIC_MAX];
EXT_CMD_LIB_DEF volatile U16     Cmd_Size;

EXT_CMD_LIB_DEF UART_OBJ Uart_Ch0;

EXT_CMD_LIB_DEF volatile U16     Cmd_ChSize;
EXT_CMD_LIB_DEF volatile char    Cmd_CmdCheck;
EXT_CMD_LIB_DEF char    Cmd_CmdStr[CMD_STRING_MAX];
EXT_CMD_LIB_DEF char    Cmd_CmdStrExe[CMD_STRING_MAX];
                              
EXT_CMD_LIB_DEF volatile U8     Cmd_DlyCheck;
EXT_CMD_LIB_DEF volatile U8     Cmd_LoopCheck;


EXT_CMD_LIB_DEF void Cmd_Init( void );
EXT_CMD_LIB_DEF void Cmd_UartPush( S8 Ch );
EXT_CMD_LIB_DEF void Cmd_ShowTitle( void );
EXT_CMD_LIB_DEF void Cmd_ShowCmdLine( void );
EXT_CMD_LIB_DEF void Cmd_ShowFirstLine( void );
EXT_CMD_LIB_DEF void Cmd_ShowLastLine( void );
EXT_CMD_LIB_DEF U16  Cmd_GetSize( void );
EXT_CMD_LIB_DEF U16  Cmd_AddCmd( char *NameStr, char *MenuStr,  Cmd_FuncType Func_Ptr );

EXT_CMD_LIB_DEF void Cmd_CmdClear( void );
EXT_CMD_LIB_DEF U8   Cmd_GetCheck( void );

EXT_CMD_LIB_DEF S16  Cmd_ExeFunc( void );

EXT_CMD_LIB_DEF S16 Cmd_Loop(void);
EXT_CMD_LIB_DEF S16 Cmd_Delay( long DlyData  );

EXT_CMD_LIB_DEF void Cmd_Print( char *Buffer );

EXT_CMD_LIB_DEF void Cmd_PutBit( char BitData );





#endif
                                                                                                 
                                                                                                 

