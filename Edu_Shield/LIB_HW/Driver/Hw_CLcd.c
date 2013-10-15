//----------------------------------------------------------------------------
//    프로그램명 : CLCD 관련 함수
//
//    만든이     : Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  : 
//
//    MPU_Type   : 
//
//    파일명     : Hw_CLcd.c
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  HW_CLCD_LOCAL  

#include "Hw_CLcd.h"




#define HW_CLCD_EN_SET()	Hw_VBus_SetBit(HW_VBUS_CS_BUSIO, 0)	
#define HW_CLCD_EN_CLR()	Hw_VBus_ClrBit(HW_VBUS_CS_BUSIO, 0)

#define HW_CLCD_RS_SET()	Hw_VBus_SetBit(HW_VBUS_CS_BUSIO, 1)	
#define HW_CLCD_RS_CLR()	Hw_VBus_ClrBit(HW_VBUS_CS_BUSIO, 1)

#define HW_CLCD_BUS_WRITE(Data)	Hw_VBus_WriteData(HW_VBUS_CS_LCD, Data)
#define HW_CLCD_BUS_READ()		Hw_VBus_ReadData(HW_VBUS_CS_LCD)




//----- 내부 함수
void Hw_CLcd_Delay(u32 DlyData);
u16  Hw_CLcd_WriteCmd( u8 CmdData );
u16  Hw_CLcd_WriteByte( u8 ByteData );
u16  Hw_CLcd_WriteChar( u8 CharData );
u16  Hw_CLcd_WriteStr( char *pStr );
u16  Hw_CLcd_Goto( u8 x, u8 y );



//----- 내부 변수











/*---------------------------------------------------------------------------
     TITLE   : Hw_CLcd_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_CLcd_Init(void)
{	
	HW_CLCD_EN_CLR();
	HW_CLCD_RS_CLR();
	
	Hw_CLcd_Delay(10000);	// delay 10ms
	
	Hw_CLcd_WriteCmd(0x3C);	// 2-line mode, display on
	Hw_CLcd_WriteCmd(0x0C);	// display on, cursor off, blink off
	Hw_CLcd_WriteCmd(0x01);	// Display Clear
	Hw_CLcd_Delay(2000);	// delay 2ms
	Hw_CLcd_WriteCmd(0x06);	// increment mode, entire shift off

	Hw_CLcd_Goto(0,0);		
	Hw_CLcd_Clear();	
	
	Hw_CLcd_Print( 0, 0, "SmartRobot BD");
	Hw_CLcd_Print( 0, 1, "Baram");
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_CLcd_Delay
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_CLcd_Delay(u32 DlyData)
{
	Hw_Wait_Usec(DlyData);
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_CLcd_WriteCmd
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_CLcd_WriteCmd( u8 CmdData )
{
	u16 Ret = ERR_NONE;
	
	HW_CLCD_RS_CLR();
	HW_CLCD_EN_SET();
	
	HW_CLCD_BUS_WRITE( CmdData );
	
	HW_CLCD_EN_CLR();
	
	Hw_CLcd_Delay(50);
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_CLcd_WriteByte
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_CLcd_WriteByte( u8 ByteData )
{
	u16 Ret = ERR_NONE;

	HW_CLCD_RS_SET();
	HW_CLCD_EN_SET();
	
	HW_CLCD_BUS_WRITE( ByteData );
	
	HW_CLCD_EN_CLR();
	
	Hw_CLcd_Delay(50);
			
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_CLcd_Goto
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_CLcd_Goto( u8 x, u8 y )
{
	u16 Ret = ERR_NONE;
	u8  Cmd;
	
	Cmd = 0x80 | 0x40*y | x;
	
	Hw_CLcd_WriteCmd( Cmd );

	return Ret;
}






/*---------------------------------------------------------------------------
     TITLE   : Hw_CLcd_WriteChar
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_CLcd_WriteChar( u8 CharData )
{
	u16 Ret = ERR_NONE;
	
	Ret = Hw_CLcd_WriteByte(CharData);
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_CLcd_Clear
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_CLcd_Clear( void )
{
	u16 Ret = ERR_NONE;
	
	Hw_CLcd_WriteCmd(0x01);
	Hw_CLcd_Delay(2000);
	
	return Ret;
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_CLcd_WriteStr
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_CLcd_WriteStr( char *pStr )
{
	u16 Ret = ERR_NONE;
		
	while( *pStr != '\0' )
	{
		Ret |= Hw_CLcd_WriteChar( *pStr );
		pStr++;
	}
		
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_CLcd_Print
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_CLcd_Print( u8 x_str, u8 y_str,  char *format, ... )
{
	u16 Ret = ERR_NONE;
	char Str[200];
	Lb_va_list ap;

	Lb_va_start( ap, format );
		
	Lb_vsprintf(Str, format, ap);	

	Hw_CLcd_Goto( x_str, y_str );
	Hw_CLcd_WriteStr( Str );		
		
	Lb_va_end(ap);		
	
	return Ret;
}



