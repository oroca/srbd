//----------------------------------------------------------------------------
//    프로그램명 	: Radio Menu 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type		:
//
//    파일명     	: Ap_UartMenu.h
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  AP_RADIOMENU_LOCAL

#include "Ap_RadioMenu.h"



//-- 내부 선언
//



//-- 내부 변수
//



//-- 내부 함수
//
void Ap_RadioMenu_ShowMenu(void);



void TimerDebug_Tick( void )
{
	static u32 Cnt = 0;
	
	printf("Timer Tick\n");
}



/*---------------------------------------------------------------------------
     TITLE   : Ap_RadioMenu_ShowMenu
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Ap_RadioMenu_ShowMenu(void)
{
	u8 key;

	printf("\n\n");
	printf("*******************************************************\n");
	printf("                      Radio V0.3                       \n");
	printf("*******************************************************\n");
	printf("* 1.  Band Select                                     *\n");
	printf("* 2.  CH ++                                           *\n");
	printf("* 3.  CH --                                           *\n");
	printf("* 4.  Seek +                                          *\n");
	printf("* 5.  Seek -                                          *\n");
	printf("* 6.  Volume +                                        *\n");
	printf("* 7.  Volume -                                        *\n");
	printf("* 8.  Korea Ch ++                                     *\n");
	printf("* 9.  Korea Ch --                                     *\n");
	printf("* m.  Menu                                            *\n");
	printf("*******************************************************\n");
	printf("\n");
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_RadioMenu_GetCmd
     WORK    : 
     ARG     : void
     RET     : 
				u8 : 눌려진 메뉴키 데이터 
---------------------------------------------------------------------------*/
u8 Ap_RadioMenu_GetCmd(void)
{
	u8  key;
	u16 Volume;
	u16 Channel;
	
	
	if( Ap_Radio_BandGet() == MODE_RADIO_BAND_FM )
	{
		Ap_Radio_VolumeGet( &Volume );
		Ap_Radio_ChannelGet( &Channel );
		
		printf("FM %d %d%% : ", Channel, Volume*100/63 );
	}
	else
	if( Ap_Radio_BandGet() == MODE_RADIO_BAND_AM )	
	{
		Ap_Radio_VolumeGet( &Volume );
		Ap_Radio_ChannelGet( &Channel );
		
		printf("AM %d %d%% : ", Channel, Volume*100/63 );		
	}
	else
	{
		printf("BAND ERROR : ");
	}
    key=get_byte();
	
	printf("\n");
	
    return key;
}




/*---------------------------------------------------------------------------
     TITLE   : Ap_RadioMenu_ShowMenu
     WORK    : 
     ARG     : void
     RET     : 
				u8 : 눌려진 메뉴키 데이터 
---------------------------------------------------------------------------*/
u8 Ap_RadioMenu_ExeCmd(void)
{
	u8 key;
	u8 Status;
	static u8 ExeFirst = TRUE;
	u16 Ret;
	u16 Channel;
	u16 ChannelGap;
	u8  FreqValid;
	static u8  Band_Mode = 0;
	static u16 FM_Freq_Index = 0;
	static u16 FM_Freq[] = 
			{
				8775,
				8910,
				8970,
				9070,
				9190,
				9310,
				9390,
				9450,
				9510,
				9590,
				9670,
				9730,
				9810,
				9910,
				9990,
				10050,
				10130,
				10190,
				10270,
				10350,
				10450,
				10490,
				10530,
				10610,
				10690,
				10770,
				0 };
				
	static u16 AM_Freq_Index = 0;
	static u16 AM_Freq[] = 
			{
				603,
				639,
				711,
				792,
				837,
				900,
				972,
				1134,
				1188,
				0 };				
				
	
	if( ExeFirst == TRUE )
	{
		Ap_RadioMenu_ShowMenu();	
	}

    while( (key = Ap_RadioMenu_GetCmd()) != 0 )
    {
        switch(key)
        {
           case '1':
           		if( Band_Mode == 0 )
           		{
               		printf("FM Config : %x \n", Ap_Radio_BandSetFM());     
               		Ap_Radio_ChannelSetFirst();      
               	}
               	else
               	{
               		printf("AM Config : %x \n", Ap_Radio_BandSetAM());           
               		Ap_Radio_ChannelSetFirst();
               	}
               	Band_Mode ^= 1;
               	break;

           case '2':
           		if( Ap_Radio_BandGet() == MODE_RADIO_BAND_FM )	ChannelGap = 10;
           		else											ChannelGap = 1;
           		Ret  = Ap_Radio_ChannelSetNext( ChannelGap );
           		Ret |= Ap_Radio_ChannelGet( &Channel );
				printf("Freq : %d  Ret : %d\n", Channel, Ret );
					         	
               break;

           case '3':
           		if( Ap_Radio_BandGet() == MODE_RADIO_BAND_FM )	ChannelGap = 10;
           		else											ChannelGap = 1;
           		Ret  = Ap_Radio_ChannelSetPrev( 10 );
           		Ret |= Ap_Radio_ChannelGet( &Channel );
				printf("Freq : %d  Ret : %d\n", Channel, Ret );
               break; 

           case '4':
				Ret = Ap_Radio_SeekToLast( &FreqValid );
				
				if( Ret == ERR_NONE )
				{
					if( FreqValid == 1 )
					{
           				Ret = Ap_Radio_ChannelGet( &Channel );
						printf("Found Freq : %d  Ret : %d\n", Channel, Ret );
					}
					else
					{
						printf("Not Found\n");
					}
				}
				else
				{
					printf("ErrCode  Ret : %d\n", Ret );
				}
				
               break;  

           case '5':
				Ret = Ap_Radio_SeekToFirst( &FreqValid );
				
				if( Ret == ERR_NONE )
				{
					if( FreqValid == 1 )
					{
           				Ret = Ap_Radio_ChannelGet( &Channel );
						printf("Found Freq : %d  Ret : %d\n", Channel, Ret );
					}
					else
					{
						printf("Not Found\n");
					}
				}
				else
				{
					printf("ErrCode  Ret : %d\n", Ret );
				}    

               break;  

           case '6':
               printf("Volume Up : %d \n", Ap_Radio_VolumeSetUp(10));               
               break;  

           case '7':
               printf("Volume Down : %d \n", Ap_Radio_VolumeSetDown(10));               
               break;  

           case '8':           		
           		if( Ap_Radio_BandGet() == MODE_RADIO_BAND_FM )
           		{
	        		Ret  = Ap_Radio_ChannelSet( FM_Freq[FM_Freq_Index] );
	           		Ret |= Ap_Radio_ChannelGet( &Channel );
					printf("Korea FM Freq : %d  Ret : %d\n", Channel, Ret );				
					
	           		FM_Freq_Index++;
	           		
	           		if( FM_Freq[FM_Freq_Index] == 0 )
	           		{
	           			FM_Freq_Index--;
	           		}				
	           	}
	           	else
	           	{
	        		Ret  = Ap_Radio_ChannelSet( AM_Freq[AM_Freq_Index] );
	           		Ret |= Ap_Radio_ChannelGet( &Channel );
					printf("Korea AM Freq : %d  Ret : %d\n", Channel, Ret );				
					
	           		AM_Freq_Index++;
	           		
	           		if( AM_Freq[AM_Freq_Index] == 0 )
	           		{
	           			AM_Freq_Index--;
	           		}					           		
	           	}
               break;

           case '9': 
           		if( Ap_Radio_BandGet() == MODE_RADIO_BAND_FM )
           		{				           		           		
	        		Ret  = Ap_Radio_ChannelSet( FM_Freq[FM_Freq_Index] );
	           		Ret |= Ap_Radio_ChannelGet( &Channel );
					printf("Korea FM Freq : %d  Ret : %d\n", Channel, Ret );
					
	           		if( FM_Freq_Index > 0 )
	           			FM_Freq_Index--;
				}
				else
				{
	        		Ret  = Ap_Radio_ChannelSet( AM_Freq[AM_Freq_Index] );
	           		Ret |= Ap_Radio_ChannelGet( &Channel );
					printf("Korea AM Freq : %d  Ret : %d\n", Channel, Ret );
					
	           		if( AM_Freq_Index > 0 )
	           			AM_Freq_Index--;
				}				
               break;

           case '0':
               printf("No.0\n");
               break;

			case 'm':
			case 'M':
				Ap_RadioMenu_ShowMenu();
				break;
				
           default :
               break;
        }
    }
    
    return key;
}
