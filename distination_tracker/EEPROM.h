#ifndef EEPROM_H
#define EEPROM_H

#include <stdbool.h>
#include <stdint.h>

#include "gps.h"

uint8_t eeprom_init(void);
uint8_t eeprom_init_reset(void);
uint8_t eeprom_write_coords(Coords* data, uint16_t coords_num);
void eeprom_read_coords(Coords* data, uint16_t* coords_num);

#endif