//----------------------------------------------------------------------------
//    프로그램명 	: KeyPad 헤더
//
//    만든이     	: 
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_KeyPad.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_KEYPAD_H__
#define __HW_KEYPAD_H__


#ifdef   HW_KEYPAD_LOCAL
#define  EXT_HW_KEYPAD_DEF
#else
#define  EXT_HW_KEYPAD_DEF     extern
#endif



#include "Hw.h"


#define HW_KEYPAD_MAX				4


#define HW_KEYPAD_STATE_IDLE		0
#define HW_KEYPAD_STATE_PRESSING	1
#define HW_KEYPAD_STATE_PRESSED		2
#define HW_KEYPAD_STATE_REPEAT		3
#define HW_KEYPAD_STATE_RELEASING	4


#define HW_KEYPAD_EVENT_MAX			3

#define HW_KEYPAD_EVENT_PRESSED		0
#define HW_KEYPAD_EVENT_REPEAT		1
#define HW_KEYPAD_EVENT_RELEASED	2





EXT_HW_KEYPAD_DEF void Hw_KeyPad_Init( void );
EXT_HW_KEYPAD_DEF void Hw_KeyPad_SetEventFunc( u8 KeyNum, void (*EventFnct)(u32 Event) );
EXT_HW_KEYPAD_DEF u32  Hw_KeyPad_ReadCode( void );

EXT_HW_KEYPAD_DEF u32  Hw_KeyPad_GetPressedCnt( void );
EXT_HW_KEYPAD_DEF u8   Hw_KeyPad_GetPressedNum( u8 PressedIndex );
EXT_HW_KEYPAD_DEF u32  Hw_KeyPad_GetRepeatCnt( u8 KeyNum );

EXT_HW_KEYPAD_DEF u32  Hw_KeyPad_WaitForEvent( u32 TimeOut );
EXT_HW_KEYPAD_DEF u32  Hw_KeyPad_GetEventKeyData( u32 EventFlag );
EXT_HW_KEYPAD_DEF u32  Hw_KeyPad_GetEventKeyNum( u32 EventFlag );


#endif
