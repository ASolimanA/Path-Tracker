#ifndef GPS_H
#define GPS_H

typedef struct
{
    float latitude;
    float longitude;
} Coords;

void GPS_READ(char* GPS);

void GPS_format(char* GPS, float* currentlat, float* currentlong, float* previouslat, float* previouslong);

void GPS_FUNCTION(float* currentlat, float* currentlong, float* previouslat, float* previouslong);

float convert_to_degree_angle(float angle);

float convert_to_rad_angle(float angle);

float GPS_getDistance(float clong, float clat, float dlong, float dlat);

float GPS_getDistance(float clong, float clat, float dlong, float dlat);


#endif