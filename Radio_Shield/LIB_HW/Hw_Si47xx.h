//----------------------------------------------------------------------------
//    프로그램명 	: Si47xx 관련 헤더
//
//    만든이     	: 
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_Si47xx.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_SI47XX_H__
#define __HW_SI47XX_H__


#ifdef   HW_SI47XX_LOCAL
#define  EXT_HW_SI47XX_DEF
#else
#define  EXT_HW_SI47XX_DEF     extern
#endif



#include "Hw.h"



#define HW_SI47XX_OK					0
#define HW_SI47XX_ERR_NO_ACK			1
#define HW_SI47XX_ERR_TIME_OUT			2
#define HW_SI47XX_ERR_RESP				3


#define HW_SI47XX_BAND_MODE_FM			0
#define HW_SI47XX_BAND_MODE_AM			1




#define HW_SI47XX_PRINTF	printf





EXT_HW_SI47XX_DEF void Hw_Si47xx_Init( void );

EXT_HW_SI47XX_DEF u16  Hw_Si47xx_FM_Config( void );
EXT_HW_SI47XX_DEF u16  Hw_Si47xx_AM_Config( void );



EXT_HW_SI47XX_DEF u16  Hw_Si47xx_POWER_UP( u8 Mode );
EXT_HW_SI47XX_DEF u16  Hw_Si47xx_SET_PROPERTY( u16 PropertyName, u16 PropertyValue );
EXT_HW_SI47XX_DEF u16  Hw_Si47xx_GET_PROPERTY( u16 PropertyName, u16 *pPropertyValue );
EXT_HW_SI47XX_DEF u16  Hw_Si47xx_FM_TUNE_FREQ( u16 Frequency );
EXT_HW_SI47XX_DEF u16  Hw_Si47xx_FM_SEEK_START( u8 Dir, u8 Mode );
EXT_HW_SI47XX_DEF u16  Hw_Si47xx_FM_TUNE_STATUS( u8 Option );

EXT_HW_SI47XX_DEF u16  Hw_Si47xx_AM_TUNE_FREQ ( u16 Frequency );
EXT_HW_SI47XX_DEF u16  Hw_Si47xx_AM_SEEK_START( u8 Dir, u8 Mode );
EXT_HW_SI47XX_DEF u16  Hw_Si47xx_AM_TUNE_STATUS( u8 Option );

EXT_HW_SI47XX_DEF u16  Hw_Si47xx_GET_INT_STATUS( u8 *pStatus );


EXT_HW_SI47XX_DEF u16 Hw_Si47xx_WaitForSTC( u32 TimeOut );
EXT_HW_SI47XX_DEF u8  Hw_Si47xx_ReadResp( u8 Index );


#endif
