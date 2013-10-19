//----------------------------------------------------------------------------
//    프로그램명 : DHT11 관련 함수 헤더
//
//    만든이     : Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  : 
//
//    MPU_Type   : 
//
//    파일명     : Hw_DHT11.h
//----------------------------------------------------------------------------
                       
#ifndef __HW_DHT11_H__
#define __HW_DHT11_H__ 




#ifdef   HW_DHT11_LOCAL
#define  EXT_HW_DHT11_DEF 
#else
#define  EXT_HW_DHT11_DEF     extern
#endif


#include "Hw.h"


typedef struct
{

	u8 Data[5];

	u8 Error;
	u8 DataValid;
	u8 Humidity;
	u8 Temperature;

} HW_DHT11_DATA_OBJ;


EXT_HW_DHT11_DEF  void Hw_DHT11_Init(void);
EXT_HW_DHT11_DEF  void Hw_DHT11_Start(void);
EXT_HW_DHT11_DEF  void Hw_DHT11_Stop(void);

EXT_HW_DHT11_DEF  u8   Hw_DHT11_GetHumidity(void);
EXT_HW_DHT11_DEF  u8   Hw_DHT11_GetTemperature(void);

#endif
