//----------------------------------------------------------------------------
//    프로그램명 	: 7Segment 헤더
//
//    만든이     	: 
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_7Seg.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_7SEG_H__
#define __HW_7SEG_H__


#ifdef   HW_7SEG_LOCAL
#define  EXT_HW_7SEG_DEF
#else
#define  EXT_HW_7SEG_DEF     extern
#endif



#include "Hw.h"


#define HW_7SEG_MAX					4




EXT_HW_7SEG_DEF void Hw_7Seg_Init( void );

EXT_HW_7SEG_DEF void Hw_7Seg_SetDot( u8 Index, u8 DotData  );
EXT_HW_7SEG_DEF void Hw_7Seg_SetNumber( u8 Index, u8 Number  );
EXT_HW_7SEG_DEF void Hw_7Seg_Printf( u8 Index, char *format, ... );

#endif
