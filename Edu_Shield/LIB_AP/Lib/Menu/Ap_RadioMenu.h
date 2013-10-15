//----------------------------------------------------------------------------
//    프로그램명 	: Radio Menu 관련 함수 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: AP_RadioMenu.h
//----------------------------------------------------------------------------




                                                                                                 
#ifndef  _AP_RADIOMENU_H
#define  _AP_RADIOMENU_H



#include "Ap.h"



#ifdef   AP_RADIOMENU_LOCAL
#define  EXT_AP_RADIOMENU_DEF
#else
#define  EXT_AP_RADIOMENU_DEF     extern
#endif


EXT_AP_RADIOMENU_DEF u8 Ap_RadioMenu_ExeCmd(void);

#endif
                                                                                                 
                                                                                                 

