//----------------------------------------------------------------------------
//    프로그램명 	: Virtual BUS 관련 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_VBus.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_VBUS_H__
#define __HW_VBUS_H__


#ifdef   HW_VBUS_LOCAL
#define  EXT_HW_VBUS_DEF
#else
#define  EXT_HW_VBUS_DEF     extern
#endif



#include "Hw.h"



#define HW_VBUS_CS_FND_0		0x0A
#define HW_VBUS_CS_FND_2		0x0C
#define HW_VBUS_CS_LCD			0x10
#define HW_VBUS_CS_KEY			0x11
#define HW_VBUS_CS_LED_1		0x12
#define HW_VBUS_CS_LED_2		0x13
#define HW_VBUS_CS_BUSIO		0x14

#define HW_VBUS_CS_MAX			0xFF







EXT_HW_VBUS_DEF void Hw_VBus_Init( void );


EXT_HW_VBUS_DEF void Hw_VBus_WriteData( u32 Address, u8 ByteData );
EXT_HW_VBUS_DEF u8   Hw_VBus_ReadData ( u32 Address );
EXT_HW_VBUS_DEF void Hw_VBus_SetBit   ( u32 Address, u8 BitNum );
EXT_HW_VBUS_DEF void Hw_VBus_ClrBit   ( u32 Address, u8 BitNum );
EXT_HW_VBUS_DEF void Hw_VBus_TglBit   ( u32 Address, u8 BitNum );


#endif
