//----------------------------------------------------------------------------
//    프로그램명 	: Cds 헤더
//
//    만든이     	: Cho Han Cheol
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type	:
//
//    파일명     	: Hw_Cds.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_CDS_H__
#define __HW_CDS_H__


#ifdef   HW_CDS_LOCAL
#define  EXT_HW_CDS_DEF
#else
#define  EXT_HW_CDS_DEF     extern
#endif



#include "Hw.h"


#define HW_CDS_MAX_LIMIT	100



EXT_HW_CDS_DEF void Hw_Cds_Init( void );
EXT_HW_CDS_DEF u16  Hw_Cds_GetData( void );

#endif
