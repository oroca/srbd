//----------------------------------------------------------------------------
//    프로그램명 : N5110 DMA 그래픽 LCD 관련 함수 헤더
//
//    만든이     : Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  : 
//
//    MPU_Type   : 
//
//    파일명     : Hw_N5110G.h
//----------------------------------------------------------------------------
                       
#ifndef __HW_N5110G_H__
#define __HW_N5110G_H__ 




#ifdef   HW_N5110G_LOCAL
#define  EXT_HW_N5110G_DEF 
#else
#define  EXT_HW_N5110G_DEF     extern
#endif


#include "Hw.h"
#include "Hw_SPI.h"

 

#define HW_N5110G_WIDTH			84
#define HW_N5110G_HEIGHT		48

#define HW_N5110G_BUF_WIDTH		84
#define HW_N5110G_BUF_HEIGHT	(HW_N5110G_HEIGHT/8)


EXT_HW_N5110G_DEF void Hw_N5110G_Init(void);

EXT_HW_N5110G_DEF u16  Hw_N5110G_Clear( void );
EXT_HW_N5110G_DEF u16  Hw_N5110G_Print( u8 x_str, u8 y_str,  char *format, ... );

EXT_HW_N5110G_DEF void Hw_N5110G_SetPixel( u16 x, u16 y );
EXT_HW_N5110G_DEF void Hw_N5110G_ClrPixel( u16 x, u16 y );
EXT_HW_N5110G_DEF void Hw_N5110G_XorPixel( u16 x, u16 y );
EXT_HW_N5110G_DEF u8   Hw_N5110G_GetPixel( u16 x, u16 y );

EXT_HW_N5110G_DEF u16  Hw_N5110G_DrawClear( void );
EXT_HW_N5110G_DEF void Hw_N5110G_DrawClearRect( u16 s_x, u16 s_y, u16 e_x, u16 e_y );
EXT_HW_N5110G_DEF u16  Hw_N5110G_DrawPrint( u8 x_str, u8 y_str,  char *format, ... );
EXT_HW_N5110G_DEF void Hw_N5110G_DrawCircle( u16 Xpos, u16 Ypos, u16 Radius, u8 Mode );


#endif
