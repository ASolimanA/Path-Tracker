#include "TIVA_HEADERS.h"
#include "UART.h"
#include "GPIO_TIMER.h"
#include "EEPROM.h"
#include "GPS.h"

//------------------------//
//--- Global Variables ---//
//------------------------//

uint16_t pointscount;
unsigned char flagco;
float total_distance;

//----------------------------------------------------------------------//
//----------------------------- Main Code ------------------------------//
//----------------------------------------------------------------------//
int main(){
	
	float previouslat=0;
	float previouslong=0;
	float currentlat=0;
	float currentlong=0;
	unsigned char coord_IDX = 0;
	total_distance=0;
	pointscount=0;
	flagco=0;
	
	//memset(xycoo, '\0', sizeof(xycoo));  //make sure that xy is reset every time

	UART_Init_gps();
	LED_Init();
	SW_Init();
	SysTick_Init();
	UART_Init_pc();
	
	// ============================================
	while(1)
		{
			Coords DATA[256] ;
			
			GPS_FUNCTION(&currentlat, &currentlong, &previouslat, &previouslong, DATA, &coord_IDX);
			
			if ((pointscount>=2) && (flagco==1))
				//make sure I have 2 valid points at least and that I am not calculating distance on same 2 points again
			{
				total_distance+=GPS_getDistance(previouslong,previouslat,currentlong,currentlat);
				LED_Reset();
				turn_On_LED(Red);
				SysTick_Delay10ms();
				SysTick_Delay10ms();
				LED_Reset();
			}

      flagco=0;	//to prevent adding same distance to itself if no new coordinates came
      SysTick_Delay10ms();
			SysTick_Delay10ms();

      if((SW1_ON()) || total_distance>=100)
			// if switch 1 is pressed or total distance exceededd 100 meters
			{
				turn_On_LED(Green);
				turn_On_LED(Red);
				Delay_in_seconds(1);
				LED_Reset();
				break;
			}
			
      LED_Reset();
			turn_On_LED(Green);
			Delay_in_seconds(1);
			LED_Reset();
		}
		
		
		//while(1); // END OF OPERATION IN GPS MODE
	
	}
	
	

