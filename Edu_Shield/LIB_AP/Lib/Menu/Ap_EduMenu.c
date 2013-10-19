//----------------------------------------------------------------------------
//    프로그램명 	: Edu Menu 관련 함수
//
//    만든이     	: Cho Han Cheol
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type		:
//
//    파일명     	: Ap_EduMenu.h
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  AP_EDUMENU_LOCAL

#include "Ap_EduMenu.h"



//-- 내부 선언
//



//-- 내부 변수
//
u16 Seg_Count = 0;


//-- 내부 함수
//
void Ap_EduMenu_ShowMenu(void);



void KeyPad_UpKeyEvent( u32 Event )
{
	if( Event == HW_KEYPAD_EVENT_RELEASED || Event == HW_KEYPAD_EVENT_REPEAT )
	{
		Hw_7Seg_Printf( 0, "%d", Seg_Count++ );
	}
}

void KeyPad_DownKeyEvent( u32 Event )
{
	if( Event == HW_KEYPAD_EVENT_RELEASED || Event == HW_KEYPAD_EVENT_REPEAT )
	{
		Seg_Count -= 2;
		
		
		Hw_7Seg_Printf( 0, "%d", Seg_Count++ );
	}	
}



/*---------------------------------------------------------------------------
     TITLE   : Ap_EduMenu_ShowMenu
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Ap_EduMenu_ShowMenu(void)
{
	u8 key;

	Lb_printf("\n\n");
	Lb_printf("*******************************************************\n");
	Lb_printf("                      Edu V0.7                         \n");
	Lb_printf("*******************************************************\n");
	Lb_printf("* 1. Test.. LED                                       *\n");
	Lb_printf("* 2. Test.. KeyPad                                    *\n");
	Lb_printf("* 3. Test.. 7Seg                                      *\n");
	Lb_printf("* 4. Test.. CLcd                                      *\n");
	Lb_printf("* 5. Test.. BT                                        *\n");
	Lb_printf("* 6. Test.. Cds                                       *\n");
	Lb_printf("* 7. Test.. DHT11                                     *\n");
	Lb_printf("* 8.                                                  *\n");
	Lb_printf("* 9.                                                  *\n");
	Lb_printf("* m.  Menu                                            *\n");
	Lb_printf("*******************************************************\n");
	Lb_printf("\n");
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_EduMenu_GetCmd
     WORK    : 
     ARG     : void
     RET     : 
				u8 : 눌려진 메뉴키 데이터 
---------------------------------------------------------------------------*/
u8 Ap_EduMenu_GetCmd(void)
{
	u8  key;
	

	Lb_printf(">> ");

    key=get_byte();
	
	Lb_printf("\n");
	
    return key;
}




/*---------------------------------------------------------------------------
     TITLE   : Ap_EduMenu_ExeCmd
     WORK    : 
     ARG     : void
     RET     : 
				u8 : 눌려진 메뉴키 데이터 
---------------------------------------------------------------------------*/
u8 Ap_EduMenu_ExeCmd(void)
{
	u8  key;
	u8  UartData;
	u8  i;
	u32 KeyPressedCnt;
	u32 KeyEvent;
	u32 KeyNum;
	u16 SegCnt = 0;
	s16 Temp = 0;
	
	static u8 ExeFirst = TRUE;
	u16 Ret = ERR_NONE;
			
	
	if( ExeFirst == TRUE )
	{
		Ap_EduMenu_ShowMenu();		
		
		Hw_KeyPad_SetEventFunc( 2, KeyPad_UpKeyEvent );
		Hw_KeyPad_SetEventFunc( 0, KeyPad_DownKeyEvent );	

		ExeFirst = FALSE;
	}

	key = 0;

	if( Hw_Uart_GetchNoWait( HW_USE_UART_CH_MENU, &UartData ) == TRUE )
	{
		Lb_printf(">> %c\n", UartData);
		key = UartData;
	}
	else
	{
		return 0;
	}

    if( key > 0 )
    {
        switch(key)
        {
           case '1':
           		Hw_Led_Toggle( 0 );
           		Hw_Timer_Delay_ms(500);
           		Hw_Led_Toggle( 1 );
           		Hw_Timer_Delay_ms(500);
           		Hw_Led_Toggle( 2 );;
           		Hw_Timer_Delay_ms(500);
           		Hw_Led_Toggle( 3 );
           		Hw_Timer_Delay_ms(500);
               	break;

           case '2':
           		Lb_printf("KeyPad Start : press two keys to exit\n");
           		while(1)
           		{
           			KeyEvent = Hw_KeyPad_WaitForEvent(1000);
           			 
           			if( IS_SET_BIT( KeyEvent, HW_KEYPAD_EVENT_PRESSED) )
           			{ 
           				Lb_printf("%x KeyPressed\n", Hw_KeyPad_GetEventKeyData(HW_KEYPAD_EVENT_PRESSED) );           		           			
					}										
					else
					if( IS_SET_BIT( KeyEvent, HW_KEYPAD_EVENT_RELEASED) )
					{
						Lb_printf("%x KeyReleased\n", Hw_KeyPad_GetEventKeyData(HW_KEYPAD_EVENT_RELEASED) );           		           									
					}
					else
					if( IS_SET_BIT( KeyEvent, HW_KEYPAD_EVENT_REPEAT) )
					{
						KeyNum = Hw_KeyPad_GetEventKeyNum(HW_KEYPAD_EVENT_REPEAT);
						
						Lb_printf("%x KeyRepeat Cnt : %d\n",Hw_KeyPad_GetEventKeyData(HW_KEYPAD_EVENT_REPEAT), Hw_KeyPad_GetRepeatCnt( KeyNum ) );           		           									
					}
					
					KeyPressedCnt = Hw_KeyPad_GetPressedCnt();
						
					if( KeyPressedCnt >= 2 )
					{
						Lb_printf("Key Exit\n");
						break;
					}	
				}
				break;

           case '3':	
           		Hw_7Seg_Printf( 0, "%d", Seg_Count++ );
               break; 

           case '4':
           		Hw_CLcd_Printf( 0, 1, "Cnt : %d", Seg_Count++);
				break;  

           case '5':
           		Lb_printf("BT Test Start, press x to exit\n");

           		while(1)
           		{
           			if( Hw_Uart_GetchNoWait( HW_USE_UART_CH_BT, &UartData) == TRUE )
           			{
           				Lb_printf( "BT Rxd : %c \n", UartData );
           			}

           			if( Hw_Uart_GetchNoWait( HW_USE_UART_CH_MENU, &UartData ) == TRUE )
           			{
           				if( UartData == 'x' )
           				{
           					break;
           				}
           				else
           				{
           					Hw_Uart_Printf( HW_USE_UART_CH_BT, "Menu:Send : %c\r\n",UartData );
           				}
           			}

				}
				
				Lb_printf("BT Test Exit\n");
				break; 

           case '6':
           		Lb_printf("Cds Test Start, press x to exit\n");
           		
           		while(1)
           		{

           			if( Hw_Uart_GetchNoWait( HW_USE_UART_CH_MENU, &UartData ) == TRUE )
           			{
           				if( UartData == 'x' )
           				{
           					break;
           				}
           			}

           			Hw_CLcd_Printf( 0, 1, "Cds : %d", Hw_Cds_GetData() );

				}      

				Lb_printf("Cds Test Exit\n");     		
               break;  

           case '7':
           		Lb_printf("H : %d, T : %d \n", Hw_DHT11_GetHumidity(), Hw_DHT11_GetTemperature() );
           		Hw_CLcd_Printf( 0, 1, "H:%d, T:%d", Hw_DHT11_GetHumidity(), Hw_DHT11_GetTemperature() );
               break;  

           case '8':           		
               break;

           case '9': 
               break;

           case '0':
               break;

			case 'm':
			case 'M':
				Ap_EduMenu_ShowMenu();
				break;
				
           default :
               break;
        }
    }
    
    return Ret;
}
