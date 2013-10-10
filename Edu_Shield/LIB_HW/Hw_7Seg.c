//----------------------------------------------------------------------------
//    프로그램명 	: 7Segment  관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 2013.10. 4.
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_7Seg.c
//----------------------------------------------------------------------------


//----- 헤더파일 열기
//
#define  HW_7SEG_LOCAL

#include "Hw_7Seg.h"




//----- 내부 함수




//----- 내부 변수

u8	Hw_7Seg_Data[ HW_7SEG_MAX ];
 
                       //     0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
u8  Hw_7Seg_DotTbl[16] = { 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x67,0x5F,0x7C,0x39,0x5E,0x79,0x71 };   






/*---------------------------------------------------------------------------
     TITLE   : Hw_7Seg_Tick
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_7Seg_Tick( void )
{
	static u8 Column_Cnt = 0;
	
	
	//-- 7Seg 데이터 출력
	//
	Hw_VBus_WriteData( HW_VBUS_CS_FND_0, Hw_7Seg_Data[Column_Cnt] );
	
	
	//-- 7Seg 4개를 번갈아가며 On 시켜준다.
	//
	Hw_VBus_WriteData( HW_VBUS_CS_FND_2, ~(1<<Column_Cnt) );
	
	Column_Cnt++;
	
	if( Column_Cnt>=HW_7SEG_MAX )
	{
		Column_Cnt = 0;
	}
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_7Seg_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_7Seg_Init( void )
{
	u32 i;
	
	//-- 7Seg 값 초기화
	//
	for( i=0; i<HW_7SEG_MAX; i++ )
	{
		Hw_7Seg_Data[i] = 0x00;
	}
	
	
	Hw_VBus_WriteData( HW_VBUS_CS_FND_0, 0x00 );
	Hw_VBus_WriteData( HW_VBUS_CS_FND_2, 0x0F );
			
	
	//-- 7Seg 처리용 타이머 시작(50Hz x 4 = 200Hz)
	//
	Hw_Timer_Set  ( HW_TIMER_CH_7SEG, 1000/200, LOOP_TIME, Hw_7Seg_Tick, NULL );
	Hw_Timer_Start( HW_TIMER_CH_7SEG );
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_7Seg_SetDot
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_7Seg_SetDot( u8 Index, u8 DotData  )
{
	u8 SegOut;
	
	if( Index >= HW_7SEG_MAX ) return;
	
	SegOut  = Hw_7Seg_Data[ Index ];
		
	if( DotData == 0 ) CLR_BIT( SegOut, 7 );
	else			   SET_BIT( SegOut, 7 );
	
	Hw_7Seg_Data[ Index ] = SegOut;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_7Seg_SetNumber
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_7Seg_SetNumber( u8 Index, u8 Number  )
{
	u8 SegOut;
	
	if( Index >= HW_7SEG_MAX ) return;


	if( Number == ' ' )
	{
		SegOut  = Hw_7Seg_Data[ Index ] & (1<<7);	// Dot 데이터는 건드리지 않는다.
		SegOut |= 0x00;
				
		Hw_7Seg_Data[ Index ] = SegOut;
	}


	if( Number == '-' )
	{
		SegOut  = Hw_7Seg_Data[ Index ] & (1<<7);	// Dot 데이터는 건드리지 않는다.
		SegOut |= (1<<6);
				
		Hw_7Seg_Data[ Index ] = SegOut;
	}

	
	if( Number >= '0' && Number <= '9' )
	{
		SegOut  = Hw_7Seg_Data[ Index ] & (1<<7);	// Dot 데이터는 건드리지 않는다.
		SegOut |= Hw_7Seg_DotTbl[ Number - '0' ];
		
		Hw_7Seg_Data[ Index ] = SegOut;
	}	

	
	if( Number >= 'A' && Number <= 'F' )
	{
		SegOut  = Hw_7Seg_Data[ Index ] & (1<<7);	// Dot 데이터는 건드리지 않는다.
		SegOut |= Hw_7Seg_DotTbl[ Number - 'A' + 10 ];
		
		Hw_7Seg_Data[ Index ] = SegOut;
	}	


	if( Number >= 'a' && Number <= 'f' )
	{
		SegOut  = Hw_7Seg_Data[ Index ] & (1<<7);	// Dot 데이터는 건드리지 않는다.
		SegOut |= Hw_7Seg_DotTbl[ Number - 'a' + 10 ];
		
		Hw_7Seg_Data[ Index ] = SegOut;
	}	
	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_7Seg_Print
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_7Seg_Print( u8 Index, char *format, ... )
{
	char Str[200];
	u32 StrSize;
	u32 i;
		
	Lb_va_list ap;

	Lb_va_start( ap, format );
		
	Lb_vsprintf(Str, format, ap);	
	
	StrSize = 0;
	
	for( i=0; i<100; i++ )
	{
		if( Str[i] == 0 ) break;
	}
	
	StrSize = i;
	
	if( StrSize > 0 && StrSize < 100 )
	{
		for( i=Index; i<HW_7SEG_MAX; i++)
		{
			if( i < StrSize ) 
				Hw_7Seg_SetNumber( i, Str[StrSize-i-Index-1] ); 
			else
				Hw_7Seg_SetNumber( i, ' ' ); 
		}
	} 
	
	Lb_va_end(ap);		
	
}