//----------------------------------------------------------------------------
//    프로그램명 : CLCD 관련 함수 헤더
//
//    만든이     : Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  : 
//
//    MPU_Type   : 
//
//    파일명     : Hw_CLcd.h
//----------------------------------------------------------------------------
                       
#ifndef __HW_CLCD_H__
#define __HW_CLCD_H__ 




#ifdef   HW_CLCD_LOCAL
#define  EXT_HW_CLCD_DEF 
#else
#define  EXT_HW_CLCD_DEF     extern
#endif


#include "Hw.h"

 


EXT_HW_CLCD_DEF void Hw_CLcd_Init(void);

EXT_HW_CLCD_DEF u16  Hw_CLcd_Clear( void );
EXT_HW_CLCD_DEF u16  Hw_CLcd_Printf( u8 x_str, u8 y_str,  char *format, ... );

#endif
