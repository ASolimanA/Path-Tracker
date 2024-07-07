#include "TIVA_HEADERS.h"

#include "UART.h"
#include "GPIO_TIMER.h"
#include "EEPROM.h"


#include "inc/hw_memmap.h"
#include "driverlib/rom.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define M_PI 3.14159265358979323846

//------------------------//
//--- Global Variables ---//
//------------------------//

const double Earth_Radius = 6371000; // 6,357 MIN  6,378 MAX
unsigned char button_in1;	// Variable to receive input from SW1
unsigned char button_in2; // Variable to receive input from SW2
unsigned char button_prev1 = 0; // Used for FALLING EDGE Condition of SW1

unsigned char LED_current; // Variable to get current LED data

float previouslat=0;
float previouslong=0;
float currentlat=0;
float currentlong=0;
float total_distance=0;
char GPS_COUNTER;
char GPS[80];   //Array to store readings from GPS
char GPS_logName[] = "$GPGLL,"; //Array to make sure we are reading the correct sentence
char GPS_formated[10][22];
char xycoo[23];  // variable to store latitude and longitude
char * token;
char pointscount=0; //indicate how many cooredinates we have recieved ( make sure not to calc distance before 2 cooredinates at least )
char flagco=0; //indicate if new coordinates came
float coords[2][256] = {0};
uint16_t coords_idx = 0;


// sentence is  $GPGLL,3003.91117,N,03116.71060,E,085049.00,A,A*75

//------------------------//
//--- GPS_READ_FUNCTION---//
//------------------------//


void GPS_READ(void)  //we will use GPGLL
{
	int k;
	int r;
	char i;
    char RECIEVED_CHAR;
    char flag = 1;
    do
    {
        flag = 1;

        for (i = 0; i < 7; i++) //"$GPGLL,"
        {
          if (UART_InChar_gps() != GPS_logName[i])
          {
            flag = 0; // break and exit from for loop
            break;
          }
        }
				
    } while (flag == 0);// i checked recieved output true or not  will continue checking untill the desired LOG arrives

   strcpy(GPS, ""); // INIT GPS ARRAY
   GPS_COUNTER =0;
	 
do{

    RECIEVED_CHAR=UART_InChar_gps();
    GPS[GPS_COUNTER++]=RECIEVED_CHAR;

} while(RECIEVED_CHAR!='*');

}
//----------------------------//
//--- GPS FORMAT function  ---//
//----------------------------//

void GPS_format(void)
{
	int j;
	int i ;
	int noOfTokenStrings;
	noOfTokenStrings=0;


   for (i = 0; i < 10; i++) {
            for (j = 0; j < 22; j++) {
            GPS_formated[i][j] = '\0'; // Reset each character to null terminator
        }
    }
	//GPS  3003.91117,N,03116.71060,E,085049.00,A,A*
		
	token=strtok(GPS,","); //token pointer is now pointing at first element
		
	do
	{
		strcpy(GPS_formated[noOfTokenStrings],token);
		token=strtok(NULL,",");
		noOfTokenStrings++;
	}while(token!=NULL);   // Now GPS_formated array has {' 3003.91117','N','03116.71060','E','085049.00','A','A*'}*/

	if(noOfTokenStrings==7)  //when we finish recieving all characters before '*'

	{
		if(strcmp(GPS_formated[5],"A")==0)   // check that the LOG is VALID

		{
			flagco=1;//flag that I have new valid coordinates
      pointscount++;//this counter is incremented when I recieve valid coordinated

			previouslat=currentlat;
      previouslong=currentlong;

			currentlat=(atof(GPS_formated[0])); //1st location in recieved Log
			currentlong=(atof(GPS_formated[2])); //3rd location in recieved Log
			coords[0][coords_idx] = currentlat;
			coords[1][coords_idx] = currentlong;
			coords_idx++;
			
      //all Egypt lies on North and East
			
      strcat(xycoo,GPS_formated[0]);// lat
			strcat(xycoo,",");//lat,
      strcat(xycoo,GPS_formated[2]); //lat,long
      strcat(xycoo," "); //lat,longspace
		}
	}
}

//----------------------------------------//
//--- CONVERT_T0_DEGREE_ANGLE_FUNTION  ---//
//---------------------------------------//

float convert_to_degree_angle(float angle)
{
    int degree = (int)angle / 100;
    float minutes = angle - (float)degree * 100;
    return (degree + (minutes / 60));
}

//------------------------------------//
//--- CONVERT_T0_RAD_ANGLE_FUNTION ---//
//------------------------------------//
float convert_to_rad_angle(float angle)
{

    return angle * M_PI / 180;
}
//-------------------------//
//--- distance_FUNCTION ---//
//-------------------------//
float GPS_getDistance(float clong, float clat, float dlong, float dlat)
{

    float currentLongRad = convert_to_rad_angle(convert_to_degree_angle(clong));
    float currentLatRad = convert_to_rad_angle(convert_to_degree_angle(clat));
    float destLongRad = convert_to_rad_angle(convert_to_degree_angle(dlong));
    float destLatRad = convert_to_rad_angle(convert_to_degree_angle(dlat));
    // Get Difference
    float longDiff = destLongRad - currentLongRad;
    float latDiff = destLatRad - currentLatRad;
    // calculate distance
    float a = pow(sin(latDiff / 2), 2) + cos(currentLatRad) * cos(destLatRad) * pow(sin(longDiff / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return Earth_Radius * c;
}


//----------------------------------------------------------------------//
//----------------------------- Main Code ------------------------------//
//----------------------------------------------------------------------//
int main(){
	
// Setting main variables
	unsigned char i;
	unsigned int idx;
	unsigned char INIT_OK;
	int k;
	int kk = 0;
	char test[4] = {0};
	unsigned char mainflag = 0;
	unsigned char flagg = 0;
	
  previouslat=0;
	previouslong=0;
	currentlat=0;
	currentlong=0;
	total_distance=0;
	pointscount=0;
	memset(xycoo, '\0', sizeof(xycoo));  //make sure that xy is reset every time
	flagco=0;

	
// Initializing GPS UART, PC UART, LED, SWITCHES, TIMER, EEPROM to use
	
	UART_Init_gps();
	LED_Init();
	SW_Init();
	SysTick_Init();
	UART_Init_pc();
	
	//INIT_OK = EEPROM_Init();
	
	/*if(INIT_OK)
	{
		EEPROM_EEBLOCK_R = 0;
		EEPROM_EEOFFSET_R = 0;
	}
	else
	{
		turn_On_LED(Red);
		while(1);
	}*/
	
 // INIT END

  while(1)
	{
		if(SW1_ON())
		{
			SysTick_Delay10ms();
			LED_Reset();
			turn_On_LED(Blue); // BLUE IF GPS MODE
			mainflag = 1;
			Delay_in_seconds(1);
			LED_Reset();
			break;
		}

		else if(SW2_ON())
		{
			turn_Off_LED(Red); // PINK IF PC MODE
			Delay_in_seconds(1);
			mainflag = 2;
			LED_Reset();
			break;
		}
	}



	// ============================================
	if(mainflag == 1)
	{
		while(1)
		{
      memset(xycoo, '\0', sizeof(xycoo));

			GPS_READ();
			turn_On_LED(Blue);
			Delay_in_seconds(1);

			GPS_format();
			LED_Reset();
			turn_On_LED(Blue);
			turn_On_LED(Green);
			LED_Reset();


			if ((pointscount>=2) && (flagco==1))
				//make sure I have 2 valid points at least and that I am not calculating distance on same 2 points again
			{
				// Distance calculation:
				total_distance += GPS_getDistance(previouslong,previouslat,currentlong,currentlat);
				
				// Block of code to indicate successful calculation (LED is RED)
				LED_Reset();
				turn_On_LED(Red);
				SysTick_Delay10ms();
				SysTick_Delay10ms();
				LED_Reset();
			}
			
      flagco=0;	//to prevent adding same distance to itself if no new coordinates came
      SysTick_Delay10ms();
			SysTick_Delay10ms();
			
      //EEPROM_WriteArray(xycoo,23);

      if((SW1_ON()) || total_distance>=100)
			// if switch 1 is pressed or total distance exceededd 100 meters
			{
				//EEPROM_WriteChar('-');
				turn_On_LED(Green);
				turn_On_LED(Red);
				SysTick_Delay10ms();
				SysTick_Delay10ms();
				LED_Reset();
				break;
			}

      LED_Reset();
			turn_On_LED(Green);
			Delay_in_seconds(1);
			LED_Reset();
		}
		while(1); // END OF OPERATION IN GPS MODE
	}
	
	else if(mainflag == 2)
	{
		/*idx = EEPROM_Read(DATA);
		EEPROM_TO_PC(DATA,idx);
		turn_On_LED(Green);
		while(1);*/
	}


}
