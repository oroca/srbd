//----------------------------------------------------------------------------
//    프로그램명 	: Virtual BUS 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_VBus.c
//----------------------------------------------------------------------------


//----- 헤더파일 열기
//
#define  HW_VBUS_LOCAL


#include "Hw_VBus.h"




#define HW_VBUS_BUG_FIX_FND		1



//----- 내부 함수
void Hw_VBus_SetIdle( void );
void Hw_VBus_SetAddress( u32 Address );
void Hw_VBus_SetData   ( u8  Data  );
u8   Hw_VBus_GetData   ( void );



void Hw_VBus_SetDataPortOutput( void );
void Hw_VBus_SetDataPortInput ( void );


//----- 내부 변수

#define HW_VBUS_PORT_DIR_OUT	0
#define HW_VBUS_PORT_DIR_IN		1
#define HW_VBUS_PORT_DIR_INOUT	2



typedef struct
{
	u8	Dir;
	u8  Data;
} HW_VBUS_MODE;



HW_VBUS_MODE Hw_VBus_AddrTbl[256];





/*---------------------------------------------------------------------------
     TITLE   : Hw_VBus_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_VBus_Init( void )
{
	u32 i;
	
	for( i=0; i<256; i++ )
	{
		Hw_VBus_AddrTbl[i].Dir = HW_VBUS_PORT_DIR_OUT;
	}
	
	

	Hw_VBus_AddrTbl[ HW_VBUS_CS_FND_0 ].Dir = HW_VBUS_PORT_DIR_OUT;
	Hw_VBus_AddrTbl[ HW_VBUS_CS_FND_2 ].Dir = HW_VBUS_PORT_DIR_OUT;
	Hw_VBus_AddrTbl[ HW_VBUS_CS_LCD   ].Dir = HW_VBUS_PORT_DIR_OUT;
	Hw_VBus_AddrTbl[ HW_VBUS_CS_KEY   ].Dir = HW_VBUS_PORT_DIR_IN;
	Hw_VBus_AddrTbl[ HW_VBUS_CS_LED_1 ].Dir = HW_VBUS_PORT_DIR_OUT;
	Hw_VBus_AddrTbl[ HW_VBUS_CS_LED_2 ].Dir = HW_VBUS_PORT_DIR_OUT;
	Hw_VBus_AddrTbl[ HW_VBUS_CS_BUSIO ].Dir = HW_VBUS_PORT_DIR_OUT;
	
	Hw_VBus_AddrTbl[ HW_VBUS_CS_FND_0 ].Data = 0;
	Hw_VBus_AddrTbl[ HW_VBUS_CS_FND_2 ].Data = 0;
	Hw_VBus_AddrTbl[ HW_VBUS_CS_LCD   ].Data = 0;
	Hw_VBus_AddrTbl[ HW_VBUS_CS_KEY   ].Data = 0;
	Hw_VBus_AddrTbl[ HW_VBUS_CS_LED_1 ].Data = 0;
	Hw_VBus_AddrTbl[ HW_VBUS_CS_LED_2 ].Data = 0;
	Hw_VBus_AddrTbl[ HW_VBUS_CS_BUSIO ].Data = 0;	
	
	
	
	//-- JTAG 핀을 Disable하고 PB3,4를 GPIO로 사용한다.(핀구성에 따라 사용하지 않을수 있음)
	//
	REG_AFIO_MAPR &= ~(0x07 << 24);
	REG_AFIO_MAPR |=  (0x04 << 24);
	
	
	//-- 데이터 버스 기본으로 출력으로 설정
	//
	Hw_VBus_SetDataPortOutput();
	
	
	//-- A0 PA.13
	//
	REG_GPIOA_CRH &= ~(0x0F << (20));	// Clear
	REG_GPIOA_CRH |=  (0x03 << (20));	// MODE, PA.13 Output mode, max speed 50Mhz
	REG_GPIOA_CRH |=  (0x00 << (22));	// CNF,  PA.13 General purpose output push-pul			
	

	//-- A1 PA.14
	//
	REG_GPIOA_CRH &= ~(0x0F << (24));	// Clear
	REG_GPIOA_CRH |=  (0x03 << (24));	// MODE, PA.14 Output mode, max speed 50Mhz
	REG_GPIOA_CRH |=  (0x00 << (26));	// CNF,  PA.14 General purpose output push-pul			

	//-- A2 PA.15
	//
	REG_GPIOA_CRH &= ~(0x0F << (28));	// Clear
	REG_GPIOA_CRH |=  (0x03 << (28));	// MODE, PA.15 Output mode, max speed 50Mhz
	REG_GPIOA_CRH |=  (0x00 << (30));	// CNF,  PA.15 General purpose output push-pul			

	//-- A3 PC.14
	//
	REG_GPIOC_CRH &= ~(0x0F << (24));	// Clear
	REG_GPIOC_CRH |=  (0x03 << (24));	// MODE, PC.14 Output mode, max speed 50Mhz
	REG_GPIOC_CRH |=  (0x00 << (26));	// CNF,  PC.14 General purpose output push-pul			

	//-- A4 PC.15
	//
	REG_GPIOC_CRH &= ~(0x0F << (28));	// Clear
	REG_GPIOC_CRH |=  (0x03 << (28));	// MODE, PC.15 Output mode, max speed 50Mhz
	REG_GPIOC_CRH |=  (0x00 << (30));	// CNF,  PC.15 General purpose output push-pul		
	
	
	//-- 버스 상태를 Idle상태로 만든다.
	//
	Hw_VBus_SetIdle();
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_VBus_SetDataPortOutput
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_VBus_SetDataPortOutput( void )
{
	//-- D0 PB.0
	//
	REG_GPIOB_CRL &= ~(0x0F << ( 0));	// Clear
	REG_GPIOB_CRL |=  (0x03 << ( 0));	// MODE, PB.0 Output mode, max speed 50Mhz
	REG_GPIOB_CRL |=  (0x00 << ( 2));	// CNF,  PB.0 General purpose output push-pul	

	//-- D1 PB.1
	//
	REG_GPIOB_CRL &= ~(0x0F << ( 4));	// Clear
	REG_GPIOB_CRL |=  (0x03 << ( 4));	// MODE, PB.1 Output mode, max speed 50Mhz
	REG_GPIOB_CRL |=  (0x00 << ( 6));	// CNF,  PB.1 General purpose output push-pul	

	//-- D2 PB.2
	//
	REG_GPIOB_CRL &= ~(0x0F << ( 8));	// Clear
	REG_GPIOB_CRL |=  (0x03 << ( 8));	// MODE, PB.2 Output mode, max speed 50Mhz
	REG_GPIOB_CRL |=  (0x00 << (10));	// CNF,  PB.2 General purpose output push-pul	

	//-- D3 PB.3
	//
	REG_GPIOB_CRL &= ~(0x0F << (12));	// Clear
	REG_GPIOB_CRL |=  (0x03 << (12));	// MODE, PB.2 Output mode, max speed 50Mhz
	REG_GPIOB_CRL |=  (0x00 << (14));	// CNF,  PB.2 General purpose output push-pul	

	//-- D4 PB.4
	//
	REG_GPIOB_CRL &= ~(0x0F << (16));	// Clear
	REG_GPIOB_CRL |=  (0x03 << (16));	// MODE, PB.2 Output mode, max speed 50Mhz
	REG_GPIOB_CRL |=  (0x00 << (18));	// CNF,  PB.2 General purpose output push-pul	
	
	//-- D5 PB.5
	//
	REG_GPIOB_CRL &= ~(0x0F << (20));	// Clear
	REG_GPIOB_CRL |=  (0x03 << (20));	// MODE, PB.6 Output mode, max speed 50Mhz
	REG_GPIOB_CRL |=  (0x00 << (22));	// CNF,  PB.6 General purpose output push-pul	
	
	//-- D6 PB.6
	//
	REG_GPIOB_CRL &= ~(0x0F << (24));	// Clear
	REG_GPIOB_CRL |=  (0x03 << (24));	// MODE, PB.6 Output mode, max speed 50Mhz
	REG_GPIOB_CRL |=  (0x00 << (26));	// CNF,  PB.6 General purpose output push-pul	

	//-- D7 PB.7
	//
	REG_GPIOB_CRL &= ~(0x0F << (28));	// Clear
	REG_GPIOB_CRL |=  (0x03 << (28));	// MODE, PB.7 Output mode, max speed 50Mhz
	REG_GPIOB_CRL |=  (0x00 << (30));	// CNF,  PB.7 General purpose output push-pul				
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_VBus_SetDataPortInput
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_VBus_SetDataPortInput( void )
{
	//-- D0 PB.0
	//
	REG_GPIOB_CRL &= ~(0x0F << ( 0));	// Clear
	REG_GPIOB_CRL |=  (0x00 << ( 0));	// MODE, PB.0 Input Mode
	REG_GPIOB_CRL |=  (0x01 << ( 2));	// CNF,  PB.0 Floating input
		
	//-- D1 PB.1
	//
	REG_GPIOB_CRL &= ~(0x0F << ( 4));	// Clear
	REG_GPIOB_CRL |=  (0x00 << ( 4));	// MODE, PB.1 Input Mode
	REG_GPIOB_CRL |=  (0x01 << ( 6));	// CNF,  PB.1 Floating input

	//-- D2 PB.2
	//
	REG_GPIOB_CRL &= ~(0x0F << ( 8));	// Clear
	REG_GPIOB_CRL |=  (0x00 << ( 8));	// MODE, PB.2 Input Mode
	REG_GPIOB_CRL |=  (0x01 << (10));	// CNF,  PB.2 Floating input

	//-- D3 PB.3
	//
	REG_GPIOB_CRL &= ~(0x0F << (12));	// Clear
	REG_GPIOB_CRL |=  (0x00 << (12));	// MODE, PB.2 Input Mode
	REG_GPIOB_CRL |=  (0x01 << (14));	// CNF,  PB.2 Floating input

	//-- D4 PB.4
	//
	REG_GPIOB_CRL &= ~(0x0F << (16));	// Clear
	REG_GPIOB_CRL |=  (0x00 << (16));	// MODE, PB.2 Input Mode
	REG_GPIOB_CRL |=  (0x01 << (18));	// CNF,  PB.2 Floating input
	
	//-- D5 PB.5
	//
	REG_GPIOB_CRL &= ~(0x0F << (20));	// Clear
	REG_GPIOB_CRL |=  (0x00 << (20));	// MODE, PB.6 Input Mode
	REG_GPIOB_CRL |=  (0x01 << (22));	// CNF,  PB.6 Floating input
	
	//-- D6 PB.6
	//
	REG_GPIOB_CRL &= ~(0x0F << (24));	// Clear
	REG_GPIOB_CRL |=  (0x00 << (24));	// MODE, PB.6 Input Mode
	REG_GPIOB_CRL |=  (0x01 << (26));	// CNF,  PB.6 Floating input

	//-- D7 PB.7
	//
	REG_GPIOB_CRL &= ~(0x0F << (28));	// Clear
	REG_GPIOB_CRL |=  (0x00 << (28));	// MODE, PB.7 Input Mode
	REG_GPIOB_CRL |=  (0x01 << (30));	// CNF,  PB.7 Floating input
		
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_VBus_SetIdle
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_VBus_SetIdle( void )
{
	SET_BIT( REG_GPIOC_ODR, 14 );
	SET_BIT( REG_GPIOC_ODR, 15 );	
	
	#if HW_VBUS_BUG_FIX_FND == 1

	Hw_VBus_SetData(Hw_VBus_AddrTbl[ HW_VBUS_CS_FND_2 ].Data);
	Hw_VBus_SetAddress( HW_VBUS_CS_FND_2 );
		
	#else
	Hw_VBus_SetData(0xFF);
	#endif
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_VBus_SetAddress
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_VBus_SetAddress( u32 Address )
{
	if( Address & (1<<0) )	SET_BIT( REG_GPIOA_ODR, 13 );
	else					CLR_BIT( REG_GPIOA_ODR, 13 );

	if( Address & (1<<1) )	SET_BIT( REG_GPIOA_ODR, 14 );
	else					CLR_BIT( REG_GPIOA_ODR, 14 );

	if( Address & (1<<2) )	SET_BIT( REG_GPIOA_ODR, 15 );
	else					CLR_BIT( REG_GPIOA_ODR, 15 );
	
	if( Address & (1<<3) )	SET_BIT( REG_GPIOC_ODR, 14 );
	else					CLR_BIT( REG_GPIOC_ODR, 14 );

	if( Address & (1<<4) )	SET_BIT( REG_GPIOC_ODR, 15 );
	else					CLR_BIT( REG_GPIOC_ODR, 15 );
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_VBus_SetData
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_VBus_SetData( u8 Data  )
{
	u32 RegData;
	
	RegData  = REG_GPIOB_ODR;
	RegData &= ~(0xFF);
	RegData |= Data;
	
	REG_GPIOB_ODR = RegData; 
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_VBus_GetData
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_VBus_GetData( void  )
{
	u8 Data;
		
	Data = (u8)REG_GPIOB_IDR;
		
	return Data;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_VBus_WriteData
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_VBus_WriteData( u32 Address, u8 ByteData )
{
	u8 Dir;
	
	Dir = Hw_VBus_AddrTbl[ Address ].Dir;
	
	Hw_VBus_AddrTbl[ Address ].Data = ByteData;
	
	if( Dir == HW_VBUS_PORT_DIR_OUT || Dir == HW_VBUS_PORT_DIR_INOUT )
	{ 
		Disable_ISR();
		
		#if HW_VBUS_BUG_FIX_FND == 1
		SET_BIT( REG_GPIOC_ODR, 14 );
		SET_BIT( REG_GPIOC_ODR, 15 );	
		#endif
		
		Hw_VBus_SetData   ( ByteData );
		Hw_VBus_SetAddress( Address  );
		Hw_VBus_SetIdle();		
		Enable_ISR();		
	}

	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_VBus_ReadData
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_VBus_ReadData( u32 Address )
{
	u8 Ret = 0;
	u8 Dir;
	
	Dir = Hw_VBus_AddrTbl[ Address ].Dir;
	
	if( Dir == HW_VBUS_PORT_DIR_IN || Dir == HW_VBUS_PORT_DIR_INOUT )
	{
		Disable_ISR();
		
		#if HW_VBUS_BUG_FIX_FND == 1
		SET_BIT( REG_GPIOC_ODR, 14 );
		SET_BIT( REG_GPIOC_ODR, 15 );
		#endif
					
		Hw_VBus_SetDataPortInput();
		Hw_VBus_SetAddress( Address  );
		
		Ret = Hw_VBus_GetData();
	
		Hw_VBus_SetIdle();
		
		Hw_VBus_SetDataPortOutput();
	
		Enable_ISR();
	}
	else
	{
		Ret = Hw_VBus_AddrTbl[ Address ].Data;
	}
		
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_VBus_SetBit
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_VBus_SetBit( u32 Address, u8 BitNum )
{
	u8 Data;
	
	Data = Hw_VBus_ReadData( Address );
	
	SET_BIT( Data, BitNum );
	
	Hw_VBus_WriteData( Address, Data );
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_VBus_ClrBit
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_VBus_ClrBit( u32 Address, u8 BitNum )
{
	u8 Data;
	
	Data = Hw_VBus_ReadData( Address );
	
	CLR_BIT( Data, BitNum );
	
	Hw_VBus_WriteData( Address, Data );
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_VBus_TglBit
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_VBus_TglBit( u32 Address, u8 BitNum )
{
	u8 Data;
	
	Data = Hw_VBus_ReadData( Address );
	
	TGL_BIT( Data, BitNum );
	
	Hw_VBus_WriteData( Address, Data );
}

