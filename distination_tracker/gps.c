#include "GPS.h"
#include "UART.h"
#include "GPIO_TIMER.h"
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define M_PI 3.14159265358979323846

const uint32_t Earth_Radius = 6371000;
//char xycoo[23];  // variable to store latitude and longitude

extern uint16_t pointscount;//indicate how many coordinates we have recieved ( make sure not to calc distance before 2 cooredinates at least )
extern unsigned char flagco;//indicate if new coordinates came

// sentence is  $GPGLL,3003.91117,N,03116.71060,E,085049.00,A,A*75

void GPS_READ(char* GPS)  //we will use GPGLL
{
		char i;
		char GPS_COUNTER;
		char RECIEVED_CHAR;
		char GPS_logName[] = "$GPGLL,"; //Array to make sure we are reading the correct sentence
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

	
void GPS_format(char* GPS, float* currentlat, float* currentlong, float* previouslat, float* previouslong)
{
		unsigned char j;
		unsigned char i ;
		
		char long_arr[10];
		char lat_arr[10];

		char validation = GPS[37]; // "A"
	
		if (validation == 'A')
		{
			flagco=1;
			pointscount++;
			for(i = 0; i<10; i++) lat_arr[i] = GPS[i];
			for(i = 0; i<11; i++)	long_arr[i] = GPS[13+i];
			
			*previouslat = *currentlat;
			*previouslong = *currentlong;
			
			*currentlat=(atof(lat_arr));
			*currentlong=(atof(long_arr));
			
		}
}

void GPS_FUNCTION(float* currentlat, float* currentlong, float* previouslat, float* previouslong, Coords* DATA, unsigned char* coords_idx)
{
	char GPS[60];   //Array to store readings from GPS
	//-------------------------//
	//--- GPS_READ_FUNCTION ---//
	//-------------------------//
	GPS_READ(GPS);
	turn_On_LED(Blue); // Successful
	Delay_in_seconds(1);
	
	//-------------------------------------//
	//-------- GPS FORMAT function --------//
	//-------------------------------------//
	
	GPS_format(GPS, currentlat, currentlong, previouslat, previouslong);
	
	DATA[*coords_idx].latitude = *currentlat;
	DATA[*coords_idx].longitude = *currentlong;
	
	*coords_idx +=1;
	LED_Reset();
	turn_On_LED(Blue);
	turn_On_LED(Green);
	Delay_in_seconds(1);
	
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
//------------------------------------//
//--- distance_FUNCTION ---//
//------------------------------------//
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
		return Earth_Radius;
}

