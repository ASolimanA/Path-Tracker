#include "RTE_Components.h"
#include CMSIS_device_header
#include "EEPROM.h"
#include <stdint.h>
#include "gps.h"

Coords trajectory[256] = {};
uint16_t coord_idx = 0;

int main() {
    eeprom_init();
    trajectory[0].latitude = 10.16549;
    trajectory[0].longitude = 49.16879;
    coord_idx++;
    eeprom_write_coords(&trajectory, coord_idx);
    for (;;) {
    }
}
