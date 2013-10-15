//----------------------------------------------------------------------------
//    프로그램명 	: Edu Menu 관련 함수 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: AP_EduMenu.h
//----------------------------------------------------------------------------




                                                                                                 
#ifndef  _AP_EDUMENU_H
#define  _AP_EDUMENU_H



#include "Ap.h"



#ifdef   AP_EDUMENU_LOCAL
#define  EXT_AP_EDUMENU_DEF
#else
#define  EXT_AP_EDUMENU_DEF     extern
#endif


EXT_AP_EDUMENU_DEF u8 Ap_EduMenu_ExeCmd(void);

#endif
                                                                                                 
                                                                                                 

