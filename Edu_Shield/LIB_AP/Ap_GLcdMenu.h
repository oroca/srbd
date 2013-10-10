//----------------------------------------------------------------------------
//    프로그램명 	: GLcd Menu 관련 함수 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: AP_GLcdMenu.h
//----------------------------------------------------------------------------




                                                                                                 
#ifndef  _AP_GLCDMENU_H
#define  _AP_GLCDMENU_H



#include "Ap.h"



#ifdef   AP_GLCDMENU_LOCAL
#define  EXT_AP_GLCDMENU_DEF
#else
#define  EXT_AP_GLCDMENU_DEF     extern
#endif


EXT_AP_GLCDMENU_DEF u8 Ap_GLcdMenu_ExeCmd(void);

#endif
                                                                                                 
                                                                                                 

