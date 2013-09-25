//----------------------------------------------------------------------------
//    프로그램명 	:
//
//    만든이     	: Cho Han Cheol
//
//    날  짜     	:
//
//    최종 수정  	:
//
//    MPU_Type	:
//
//    파일명     	: Hw_Define.h
//----------------------------------------------------------------------------


#ifndef _DEFINE_H_
#define _DEFINE_H_



/*
typedef long long            	U64;
typedef  unsigned int        	U32;
typedef  unsigned short int  	U16;
typedef  signed   int        	S32;
typedef  short    int        	S16;
typedef  unsigned char       	U8;
typedef  signed   char       	S8;
typedef volatile unsigned int  	VU32;
typedef volatile unsigned short	VU16;
typedef volatile unsigned char	VU8;

typedef volatile signed int		VS32;
typedef volatile signed short	VS16;
typedef volatile signed char	VS8;

typedef           float      	F32;
*/


typedef long long        		u64;
typedef unsigned int        	u32;
typedef unsigned short       	u16;
typedef unsigned char       	u8;

typedef signed int            	s32;
typedef signed short         	s16;
typedef signed char          	s8;

typedef volatile unsigned int 	vu32;
typedef volatile unsigned short	vu16;
typedef volatile unsigned char	vu8;

typedef volatile signed int  	vs32;
typedef volatile signed short 	vs16;
typedef volatile signed char  	vs8;

typedef           float      	f32;

//typedef 			int			bool;


typedef enum
{
  FALSE = 0, TRUE  = !FALSE
}
bool;

//-- For USB
//
//CHCBARAM
#define __IO volatile 


typedef unsigned       int uint32_t;
typedef unsigned short int uint16_t;
typedef unsigned char      uint8_t;

typedef signed       int    int32_t;
typedef signed short int    int16_t;
typedef signed char         int8_t;




#define ENABLE			1
#define DISABLE			0

#define SWAP8(A)		(A)
#define SWAP16(A)		((((A)&0x00ff)<<8) | ((A)>>8))
#define SWAP32(A)		((((A)&0x000000ff)<<24) | (((A)&0x0000ff00)<<8) | (((A)&0x00ff0000)>>8) | (((A)&0xff000000)>>24))





#define SET_BIT(BitPort, BitNum)      BitPort |= (  1<<(BitNum))
#define CLR_BIT(BitPort, BitNum)      BitPort &= (~(1<<(BitNum)))     


#define TGL_BIT(val,bit)	          (val)   ^= (1<<(bit))

#define IS_CLR_BIT(BitPort, BitNum)   (!((BitPort) & (1<<(BitNum))))
#define IS_SET_BIT(BitPort, BitNum)   (  (BitPort) & (1<<(BitNum)))




// 전체 인터럽트 On/Off
//
#define   Enable_ISR()     __enable_interrupt()
#define   Disable_ISR()    __disable_interrupt()


#ifndef TRUE 
#define TRUE 			1
#endif

#ifndef FALSE
#define FALSE 			0
#endif


#ifndef true
#define	true			1
#endif

#ifndef false
#define false			0
#endif

#define ERR_NONE		0


#ifndef NULL
#define NULL    ((void *)0)
#endif



#endif
