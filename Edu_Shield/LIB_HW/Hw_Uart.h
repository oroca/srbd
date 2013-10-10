//----------------------------------------------------------------------------
//    프로그램명 	: Uart 관련 함수 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: Hw_Uart.h
//----------------------------------------------------------------------------




                                                                                                 
#ifndef  _HW_UART_H
#define  _HW_UART_H



#include "Hw.h"



#ifdef   HW_UART_LOCAL
#define  EXT_HW_UART_DEF
#else
#define  EXT_HW_UART_DEF     extern
#endif


#define  HW_UART_COM1   0
#define  HW_UART_COM2   1
#define  HW_UART_COM3   2
#define  HW_UART_COM4   3
#define  HW_UART_COM5   4


#define HW_UART_MAX_CH  5



typedef struct 
{
	u32 Port;
	u32 Baud;	
	
	void (*ISR_FuncPtr)(char Ch);
	
} HW_UART_OBJ;




EXT_HW_UART_DEF void Hw_Uart_Init( void );
EXT_HW_UART_DEF void Hw_Uart_Open( u8 Ch, u32 BaudData, void (*ISR_FuncPtr)(char Ch) );
EXT_HW_UART_DEF u8   Hw_Uart_Getch( u8 Ch );
EXT_HW_UART_DEF void Hw_Uart_Putch( u8 Ch,  char Uart_PutData );
EXT_HW_UART_DEF void Hw_Uart_Print( u8 Ch, char *UartPrintData, ... );
EXT_HW_UART_DEF u8   Hw_Uart_GetchNoWait( u8 Ch, u8 *cReturn );

EXT_HW_UART_DEF char get_byte(void);                                            

#endif
                                                                                                 
                                                                                                 

