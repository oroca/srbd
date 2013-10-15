//----------------------------------------------------------------------------
//    프로그램명 	: KeyPad  관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 2013.10. 1.
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_KeyPad.c
//----------------------------------------------------------------------------


//----- 헤더파일 열기
//
#define  HW_KEYPAD_LOCAL

#include "Hw_KeyPad.h"





//-- 내부 변수 
//

typedef struct
{
	u32 State;
	
	u8  Value;
	
	u32 Event;
	
	u32 Cnt_Dly;
	u32 Cnt_Pressed;
	u32 Cnt_Released;
	u32 Cnt_Repeat;
	
	u32 Dly_Pressing;
	u32 Dly_Releasing;
	u32 Dly_Pressed;
	u32 Dly_Repeat; 
	
	void (*EventFnct)(u32 Event);

} HW_KEYPAD_LOG;


volatile u32 Hw_KeyPad_Data;


HW_KEYPAD_LOG Hw_KeyPad_Log[HW_KEYPAD_MAX];


u32 Hw_KeyPad_PressedCount = 0;
u32 Hw_KeyPad_PressedList[HW_KEYPAD_MAX];
u32 Hw_KeyPad_EventFlag    = 0;
u32 Hw_KeyPad_EventList[HW_KEYPAD_EVENT_MAX];
u32 Hw_KeyPad_EventKeyNum  = 0;



//-- 내부 함수 
//
u8   Hw_KeyPad_StateTick( HW_KEYPAD_LOG *pKey, u8 KeyData, u8 KeyNum  );
void Hw_KeyPad_Tick( void );
u32  Hw_KeyPad_Scan( void );




/*---------------------------------------------------------------------------
     TITLE   : Hw_KeyPad_StateTick
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_KeyPad_StateTick( HW_KEYPAD_LOG *pKey, u8 KeyData, u8 KeyNum  )
{
	u8  Value = 0;
	
	switch( pKey->State )
	{

		case HW_KEYPAD_STATE_IDLE:
		
			Value = 0;
					
			if( KeyData > 0 )
			{
				pKey->State   = HW_KEYPAD_STATE_PRESSING;
				pKey->Cnt_Dly = 0;
			}
			break;


		case HW_KEYPAD_STATE_PRESSING:
			
			Value = 0;
			
			pKey->Cnt_Dly++;
			
			if( pKey->Cnt_Dly > pKey->Dly_Pressing )
			{
				pKey->State        = HW_KEYPAD_STATE_PRESSED;
				pKey->Cnt_Pressed  = 0;
				pKey->Cnt_Released = 0;
				Value              = KeyData;	
				Hw_KeyPad_PressedList[ Hw_KeyPad_PressedCount ] = KeyNum;
				Hw_KeyPad_PressedCount++;
				
				if( IS_CLR_BIT( Hw_KeyPad_EventFlag, HW_KEYPAD_EVENT_PRESSED ) )
				{
					SET_BIT( Hw_KeyPad_EventFlag, HW_KEYPAD_EVENT_PRESSED );
					Hw_KeyPad_EventList[HW_KEYPAD_EVENT_PRESSED] = KeyNum;
				}
				
				if( pKey->EventFnct != NULL )
				{
					(*pKey->EventFnct)(HW_KEYPAD_EVENT_PRESSED);
				} 				
			}			
			break;


		case HW_KEYPAD_STATE_PRESSED:
			
			Value = KeyData;

			pKey->Cnt_Pressed++;
			
			if( KeyData == 0 )	pKey->Cnt_Released++;
			else			    pKey->Cnt_Released = 0;			

			if( pKey->Cnt_Released > pKey->Dly_Releasing )
			{
				pKey->State = HW_KEYPAD_STATE_RELEASING;
				Value       = 0;
			}
			else
			{			
				if( pKey->Cnt_Pressed > pKey->Dly_Pressed )
				{
					pKey->State        = HW_KEYPAD_STATE_REPEAT;
					pKey->Cnt_Pressed  = 0;
					pKey->Cnt_Released = 0;
					pKey->Cnt_Repeat   = 1;
					
					if( IS_CLR_BIT( Hw_KeyPad_EventFlag, HW_KEYPAD_EVENT_REPEAT ) )
					{
						SET_BIT( Hw_KeyPad_EventFlag, HW_KEYPAD_EVENT_REPEAT );
						Hw_KeyPad_EventList[HW_KEYPAD_EVENT_REPEAT] = KeyNum;
					}
					
					if( pKey->EventFnct != NULL )
					{
						(*pKey->EventFnct)(HW_KEYPAD_EVENT_REPEAT);
					} 					
				}
			}
			
			break;


		case HW_KEYPAD_STATE_REPEAT:

			Value = KeyData;

			pKey->Cnt_Pressed++;
			
			if( KeyData == 0 )	pKey->Cnt_Released++;
			else			    pKey->Cnt_Released = 0;			

			if( pKey->Cnt_Released > pKey->Dly_Releasing )
			{
				pKey->State      = HW_KEYPAD_STATE_RELEASING;
				Value            = 0;
				pKey->Cnt_Repeat = 0;
			}
			else
			{			
				if( pKey->Cnt_Pressed > pKey->Dly_Repeat )
				{
					pKey->Cnt_Pressed  = 0;
					pKey->Cnt_Repeat++;

					if( IS_CLR_BIT( Hw_KeyPad_EventFlag, HW_KEYPAD_EVENT_REPEAT ) )
					{
						SET_BIT( Hw_KeyPad_EventFlag, HW_KEYPAD_EVENT_REPEAT );
						Hw_KeyPad_EventList[HW_KEYPAD_EVENT_REPEAT] = KeyNum;						
					}
					
					if( pKey->EventFnct != NULL )
					{
						(*pKey->EventFnct)(HW_KEYPAD_EVENT_REPEAT);
					} 					
				}
			}
			
			break;

			
		case HW_KEYPAD_STATE_RELEASING:
			pKey->State = HW_KEYPAD_STATE_IDLE;
			Value       = 0;			
			
			if( Hw_KeyPad_PressedCount > 0 )
			{
				Hw_KeyPad_PressedCount--;
			}

			if( IS_CLR_BIT( Hw_KeyPad_EventFlag, HW_KEYPAD_EVENT_RELEASED ) )
			{
				SET_BIT( Hw_KeyPad_EventFlag, HW_KEYPAD_EVENT_RELEASED );
				Hw_KeyPad_EventList[HW_KEYPAD_EVENT_RELEASED] = KeyNum;				
			}
			
			if( pKey->EventFnct != NULL )
			{
				(*pKey->EventFnct)(HW_KEYPAD_EVENT_RELEASED);
			} 				

			break;
	}

	return Value;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_KeyPad_Tick
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_KeyPad_Tick( void )
{
	u32 i;
	u32 scan_data = 0;
	u32 key_data  = 0;
	u32 Cnt = 0;
	
	scan_data = Hw_KeyPad_Scan();
	

	
	for( i=0; i<HW_KEYPAD_MAX; i++ )
	{
		if( scan_data & (1<<i) )  key_data |= Hw_KeyPad_StateTick( &Hw_KeyPad_Log[i], 1, i ) << i; 
		else					  key_data |= Hw_KeyPad_StateTick( &Hw_KeyPad_Log[i], 0, i ) << i; 		
		
		if( key_data & (1<<i) ) Cnt++;
	}
	
	Hw_KeyPad_Data         = key_data;
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_KeyPad_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_KeyPad_Init( void )
{
	u8 i;
	
	Hw_KeyPad_Data = 0;


	for( i=0; i<HW_KEYPAD_MAX; i++ )
	{	
		Hw_KeyPad_Log[i].State         = HW_KEYPAD_STATE_IDLE;
		Hw_KeyPad_Log[i].Event		   = 0;
		Hw_KeyPad_Log[i].Value         = 0;
		Hw_KeyPad_Log[i].Cnt_Dly       = 0;
		Hw_KeyPad_Log[i].Cnt_Pressed   = 0; 
		Hw_KeyPad_Log[i].Cnt_Released  = 0;
		Hw_KeyPad_Log[i].Cnt_Repeat    = 0;
		
		Hw_KeyPad_Log[i].Dly_Pressing  = 50  /10; // 50ms
		Hw_KeyPad_Log[i].Dly_Releasing = 50  /10; // 50ms
		Hw_KeyPad_Log[i].Dly_Pressed   = 1000/10; // 1000ms
		Hw_KeyPad_Log[i].Dly_Repeat    = 500 /10; // 500ms	
		
		Hw_KeyPad_Log[i].EventFnct     = NULL;				
	}

	
	//-- KeyPad 처리용 타이머 시작
	//
	Hw_Timer_Set  ( HW_TIMER_CH_KEYPAD, 10, LOOP_TIME, Hw_KeyPad_Tick, NULL );
	Hw_Timer_Start( HW_TIMER_CH_KEYPAD );
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_KeyPad_Scan
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u32 Hw_KeyPad_Scan( void )
{
	u32 ScanData = 0;
	
	
	ScanData = Hw_VBus_ReadData( HW_VBUS_CS_KEY );
		    
	return ScanData;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_KeyPad_SetEventFunc
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_KeyPad_SetEventFunc( u8 KeyNum, void (*EventFnct)(u32 Event) )
{
	if( KeyNum >= HW_KEYPAD_MAX ) return;
	
	Hw_KeyPad_Log[KeyNum].EventFnct = EventFnct;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_KeyPad_ReadCode
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u32 Hw_KeyPad_ReadCode( void )
{
	u32 Ret;

	Ret = Hw_KeyPad_Data;

	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_KeyPad_GetRepeatCnt
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u32 Hw_KeyPad_GetRepeatCnt( u8 KeyNum )
{
	if( KeyNum >= HW_KEYPAD_MAX ) return 0;
	
	
	return Hw_KeyPad_Log[ KeyNum ].Cnt_Repeat;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_KeyPad_GetPressedCnt
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u32 Hw_KeyPad_GetPressedCnt( void )
{
	return Hw_KeyPad_PressedCount;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_KeyPad_GetPressedNum
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_KeyPad_GetPressedNum( u8 PressedIndex )
{
	if( PressedIndex >= HW_KEYPAD_MAX ) return 0;
	
	return Hw_KeyPad_PressedList[ PressedIndex ];
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_KeyPad_WaitForEvent
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u32 Hw_KeyPad_WaitForEvent( u32 TimeOut )
{
	u32 TimeStart;
	u32 TimeEnd;
	u32 Ret = 0;
	
	TimeStart = Hw_Timer_Get_CountValue();

	Hw_KeyPad_EventFlag = 0;
	
	while(1)
	{	
		if( Hw_KeyPad_EventFlag > 0 )
		{
			Ret = Hw_KeyPad_EventFlag ;
			break;
		}
	
		//-- 타임 아웃 처리
		//
		if( TimeOut > 0 )
		{
			TimeEnd = Hw_Timer_Get_CountValue();
			
			if( (TimeEnd - TimeStart) > TimeOut )
			{
				break;
			}
		}
	}

	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_KeyPad_GetEventKeyData
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u32 Hw_KeyPad_GetEventKeyData( u32 EventFlag )
{
	u32 EventKey = 0;
	u32 Ret = 0;
	
	EventKey = Hw_KeyPad_EventList[EventFlag];
	Ret      = (1<<EventKey);
	
	return Ret;
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_KeyPad_GetEventKeyNum
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u32 Hw_KeyPad_GetEventKeyNum( u32 EventFlag )
{
	u32 EventKey = 0;
	u32 Ret = 0;
	
	EventKey = Hw_KeyPad_EventList[EventFlag];
	Ret      = EventKey;
	
	return Ret;
}