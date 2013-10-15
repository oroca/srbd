//----------------------------------------------------------------------------
//    프로그램명 : N5110 그래픽 LCD 관련 함수
//
//    만든이     : Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  : 
//
//    MPU_Type   : 
//
//    파일명     : Hw_N5110G.c
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  HW_N5110G_LOCAL  

#include "Hw_N5110G.h"
#include "Hw_N5110G_Font.h"




#define HW_N5110G_RESET_ON()		CLR_BIT(REG_GPIOB_ODR, 10)
#define HW_N5110G_RESET_OFF()		SET_BIT(REG_GPIOB_ODR, 10)

#define HW_N5110G_SEL_DATA()		SET_BIT(REG_GPIOB_ODR, 11)
#define HW_N5110G_SEL_CMD()			CLR_BIT(REG_GPIOB_ODR, 11)


//----- 내부 함수
void Hw_N5110G_Reset(void);

u16 Hw_N5110G_WriteCmd( u8 CmdData );
u16 Hw_N5110G_WriteByte( u8 ByteData );
u16 Hw_N5110G_WriteChar( u8 CharData );
u16 Hw_N5110G_WriteStr( char *pStr );
u16 Hw_N5110G_Goto( u8 x, u8 y );

//----- 내부 변수



//----- DMA 사용 관련

void Hw_N5110G_DrawRequest(void);


u16 Hw_N5110G_Frame_Index;
u32 Hw_N5110G_Frame_DrawReq;
u8  Hw_N5110G_TxFrameBuf[ HW_N5110G_BUF_WIDTH * HW_N5110G_BUF_HEIGHT ]  __attribute__((aligned(4)));
u8  Hw_N5110G_RxFrameBuf[ HW_N5110G_BUF_WIDTH * HW_N5110G_BUF_HEIGHT ]  __attribute__((aligned(4)));




/*---------------------------------------------------------------------------
     TITLE   : Hw_N5100G_FrameTick
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_N5100G_FrameTick( void )
{
	static u8 Index = 0;
	
	if( Hw_DMA_SPI2_IsTransFinished() == 1 ) 
	{
		if( Hw_N5110G_Frame_DrawReq > 0 )
		{
			Hw_N5110G_Frame_DrawReq--;
		}
	}
	
	if( Hw_N5110G_Frame_DrawReq > 0 )
	{
		Hw_DMA_SPI2_Setup( (u32)Hw_N5110G_TxFrameBuf, (u32)Hw_N5110G_RxFrameBuf, HW_N5110G_BUF_WIDTH * HW_N5110G_BUF_HEIGHT );				
		Hw_SPI_StartDMA( HW_SPI_CH2 );			
	}	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_5110G_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_N5110G_Init(void)
{
	u16 i;
	char Str[100];
	
	Hw_SPI_Init();	
	
	//-- RESET PB.10
	//
	REG_GPIOB_CRH &= ~(0x0F << ( 8));	// Clear
	REG_GPIOB_CRH |=  (0x03 << ( 8));	// MODE, PB.10 Output mode, max speed 50Mhz
	REG_GPIOB_CRH |=  (0x00 << (10));	// CNF,  PB.10 General purpose output push-pul	

	//-- DNC PB.11
	//
	REG_GPIOB_CRH &= ~(0x0F << (12));	// Clear
	REG_GPIOB_CRH |=  (0x03 << (12));	// MODE, PB.11 Output mode, max speed 50Mhz
	REG_GPIOB_CRH |=  (0x00 << (14));	// CNF,  PB.11 General purpose output push-pul	


	HW_N5110G_RESET_OFF();
	HW_N5110G_SEL_CMD();
	
	Hw_N5110G_Reset();
	
	Hw_N5110G_WriteCmd(0x21);	// H = 1
	Hw_N5110G_WriteCmd(0xC6);	// Write Vop
	Hw_N5110G_WriteCmd(0x06);	// Set Temperature Coefficient
	Hw_N5110G_WriteCmd(0x13);	// Set Bias System
	Hw_N5110G_WriteCmd(0x20);	// H = 0
	Hw_N5110G_WriteCmd(0x0C);	// Set display configuration
								//  - normal mode							
	Hw_N5110G_Goto(0,0);
	
	
	HW_N5110G_SEL_DATA();
	Hw_N5110G_Frame_Index    = 0;	
	Hw_N5110G_Frame_DrawReq	= 0;						
	
	
	Hw_Timer_Set  ( HW_TIMER_CH_GLCD, 33, LOOP_TIME, Hw_N5100G_FrameTick, NULL );
	Hw_Timer_Start( HW_TIMER_CH_GLCD );		
	
	
	Hw_N5110G_Clear();	
	Hw_N5110G_DrawRequest();
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_5110G_Reset
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_N5110G_Reset(void)
{
	HW_N5110G_RESET_ON();
	Hw_Timer_Delay_ms(10);
	HW_N5110G_RESET_OFF();
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_5110G_Reset
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110G_WriteCmd( u8 CmdData )
{
	u16 Ret;
	
	HW_N5110G_SEL_CMD();
	Ret = Hw_SPI_Write8Bit( HW_SPI_CH2, CmdData );
	HW_N5110G_SEL_DATA();
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110G_WriteByte
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110G_WriteByte( u8 ByteData )
{
	u16 Ret = ERR_NONE;
	
	Hw_N5110G_TxFrameBuf[ Hw_N5110G_Frame_Index++ ] = ByteData;
	Hw_N5110G_Frame_Index %= (HW_N5110G_BUF_WIDTH * HW_N5110G_BUF_HEIGHT);
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110G_Goto
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110G_Goto( u8 x, u8 y )
{
	u16 Ret = ERR_NONE;
	
	Ret |= Hw_N5110G_WriteCmd( 0x40|y );
	Ret |= Hw_N5110G_WriteCmd( 0x80|x );

	return Ret;
}






/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110G_WriteChar
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110G_WriteChar( u8 CharData )
{
	u16 Ret = ERR_NONE;
	u16 i;
	u8  Ch;
	
	for( i=0; i<6; i++ )
	{
		Ch = Hw_N5110G_Font6_8[CharData-32][i];
		Ret |= Hw_N5110G_WriteByte(Ch);
	}	
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110G_Clear
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110G_Clear( void )
{
	u16 Ret;
	u16 i;
	
	for( i=0; i<6*84; i++ )
	{
		Hw_N5110G_TxFrameBuf[i] = 0;
	}
	
	Hw_N5110G_DrawRequest();
	
	return Ret;
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110_WriteStr
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110G_WriteStr( char *pStr )
{
	u16 Ret = ERR_NONE;
		
	while( *pStr != '\0' )
	{
		Ret |= Hw_N5110G_WriteChar( *pStr );
		pStr++;
	}
		
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110G_Print
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110G_Print( u8 x_str, u8 y_str,  char *format, ... )
{
	u16 Ret = ERR_NONE;
	char Str[200];
	Lb_va_list ap;

	Lb_va_start( ap, format );
		
	Lb_vsprintf(Str, format, ap);	

	Hw_N5110G_Frame_Index = y_str*HW_N5110G_BUF_WIDTH +x_str*6;

	Hw_N5110G_WriteStr( Str );		
	
	Hw_N5110G_DrawRequest();
	
	Lb_va_end(ap);		
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110G_DrawRequest
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_N5110G_DrawRequest(void)
{		
	Hw_N5110G_Frame_DrawReq++;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110G_SetPixel
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_N5110G_SetPixel( u16 x, u16 y )
{
	u8  Buf;
	u32 xy_offset; 
	u32 y_offset = y/8;
	u32 y_remain = y%8;
	
	xy_offset = y_offset*HW_N5110G_BUF_WIDTH + x;
	
	Buf = Hw_N5110G_TxFrameBuf[ xy_offset ];

	SET_BIT( Buf, y_remain );
	
	Hw_N5110G_TxFrameBuf[ xy_offset ] = Buf;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110G_ClrPixel
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_N5110G_ClrPixel( u16 x, u16 y )
{
	u8  Buf;
	u32 xy_offset; 
	u32 y_offset = y/8;
	u32 y_remain = y%8;
	
	xy_offset = y_offset*HW_N5110G_BUF_WIDTH + x;
	
	Buf = Hw_N5110G_TxFrameBuf[ xy_offset ];

	CLR_BIT( Buf, y_remain );
	
	Hw_N5110G_TxFrameBuf[ xy_offset ] = Buf;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110G_XorPixel
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_N5110G_XorPixel( u16 x, u16 y )
{
	u8  Buf;
	u32 xy_offset; 
	u32 y_offset = y/8;
	u32 y_remain = y%8;
	
	xy_offset = y_offset*HW_N5110G_BUF_WIDTH + x;
	
	Buf = Hw_N5110G_TxFrameBuf[ xy_offset ];

	Buf ^= (1<<y_remain);
	
	Hw_N5110G_TxFrameBuf[ xy_offset ] = Buf;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110G_GetPixel
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_N5110G_GetPixel( u16 x, u16 y )
{
	u8  Buf;
	u32 xy_offset; 
	u32 y_offset = y/8;
	u32 y_remain = y%8;
	
	xy_offset = y_offset*HW_N5110G_BUF_WIDTH + x;
	
	Buf = Hw_N5110G_TxFrameBuf[ xy_offset ];

	if( Buf & (1<<y_remain) ) return 1;
	else					  return 0;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110G_DrawClear
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110G_DrawClear( void )
{
	u16 Ret;
	u16 i;
	
	for( i=0; i<6*84; i++ )
	{
		Hw_N5110G_TxFrameBuf[i] = 0;
	}
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110G_DrawClearRect
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_N5110G_DrawClearRect( u16 s_x, u16 s_y, u16 e_x, u16 e_y )
{
	u16 i,j;
	
	for( i=s_x; i<=e_x; i++ )
	{
		for( j=s_y; j<=e_y; j++ )
		{
			Hw_N5110G_ClrPixel( i, j );
		}
	}
}



/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110G_DrawPrint
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110G_DrawPrint( u8 x_str, u8 y_str,  char *format, ... )
{
	u16 Ret = ERR_NONE;
	char Str[200];
	Lb_va_list ap;

	Lb_va_start( ap, format );
		
	Lb_vsprintf(Str, format, ap);	

	Hw_N5110G_Frame_Index = y_str*HW_N5110G_BUF_WIDTH +x_str*6;

	Hw_N5110G_WriteStr( Str );		
		
	Lb_va_end(ap);		
	
	return Ret;
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110G_DrawCircle
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_N5110G_DrawCircle( u16 Xpos, u16 Ypos, u16 Radius, u8 Mode )
{
	s16  D;		/* Decision Variable */ 
	u16  CurX;	/* Current X Value */
	u16  CurY;	/* Current Y Value */ 
  
	D    = 3 - (Radius << 1);
	CurX = 0;
	CurY = Radius;
	
	while (CurX <= CurY)
	{
		if( Mode == 0 )
		{
			Hw_N5110G_SetPixel(Xpos + CurX, Ypos + CurY);
			Hw_N5110G_SetPixel(Xpos + CurX, Ypos - CurY);
			Hw_N5110G_SetPixel(Xpos - CurX, Ypos + CurY);
			Hw_N5110G_SetPixel(Xpos - CurX, Ypos - CurY);
			Hw_N5110G_SetPixel(Xpos + CurY, Ypos + CurX);
			Hw_N5110G_SetPixel(Xpos + CurY, Ypos - CurX);
			Hw_N5110G_SetPixel(Xpos - CurY, Ypos + CurX);
			Hw_N5110G_SetPixel(Xpos - CurY, Ypos - CurX);
		}
		else
		if( Mode == 1 )
		{
			Hw_N5110G_ClrPixel(Xpos + CurX, Ypos + CurY);
			Hw_N5110G_ClrPixel(Xpos + CurX, Ypos - CurY);
			Hw_N5110G_ClrPixel(Xpos - CurX, Ypos + CurY);
			Hw_N5110G_ClrPixel(Xpos - CurX, Ypos - CurY);
			Hw_N5110G_ClrPixel(Xpos + CurY, Ypos + CurX);
			Hw_N5110G_ClrPixel(Xpos + CurY, Ypos - CurX);
			Hw_N5110G_ClrPixel(Xpos - CurY, Ypos + CurX);
			Hw_N5110G_ClrPixel(Xpos - CurY, Ypos - CurX);			
		}
		if( Mode == 2 )
		{
			Hw_N5110G_XorPixel(Xpos + CurX, Ypos + CurY);
			Hw_N5110G_XorPixel(Xpos + CurX, Ypos - CurY);
			Hw_N5110G_XorPixel(Xpos - CurX, Ypos + CurY);
			Hw_N5110G_XorPixel(Xpos - CurX, Ypos - CurY);
			Hw_N5110G_XorPixel(Xpos + CurY, Ypos + CurX);
			Hw_N5110G_XorPixel(Xpos + CurY, Ypos - CurX);
			Hw_N5110G_XorPixel(Xpos - CurY, Ypos + CurX);
			Hw_N5110G_XorPixel(Xpos - CurY, Ypos - CurX);			
		}
				
		if (D < 0)
		{ 
			D += (CurX << 2) + 6;
		}
		else
		{
			D += ((CurX - CurY) << 2) + 10;
			CurY--;
		}
		CurX++;
	}
}






