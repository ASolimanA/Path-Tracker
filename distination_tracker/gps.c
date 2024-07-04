#include "gps.h"
#include <stdint.h>


const uint32_t Earth_radius = 6371000;
const float DtR = 0.01745329252;        // degree to rad constant (pi/180)
const char GPS_logName[] = "$GPGLL,";
