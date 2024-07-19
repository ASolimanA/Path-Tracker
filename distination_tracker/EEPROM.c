#include "EEPROM.h"

#include "driverlib/sysctl.h"
#include "driverlib/eeprom.h"

#define EEPROM_PROGRAM_SUCCESS 0

uint8_t eeprom_init(void) {
	// Used for initializing the EEPROM without erasing its contents
	
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0); 
	
    uint8_t eeprom_status = EEPROMInit();
	
    if(eeprom_status == EEPROM_INIT_OK) return 0; // Init Success
    
    return 1; // Init Falied
}

uint8_t eeprom_init_reset(void) {
    uint8_t status = eeprom_init();
    if(status != 0) return 1;
    EEPROMMassErase();
    return 0;
}


uint8_t eeprom_write_coords(Coords* data, uint32_t coords_num) {
    if (EEPROMProgram(&coords_num, 0x0, 4) != EEPROM_PROGRAM_SUCCESS) {
        if(EEPROMInit() != EEPROM_INIT_OK) {
            return 1;
        }
    }
    EEPROMProgram(data, 0x4, coords_num *8);
		return 0;
}

void eeprom_read_coords(Coords* data, uint16_t* coords_num) {
    uint32_t num;
    EEPROMRead(&num, 0x0, 4);
    *coords_num = num;
    EEPROMRead(data, 0x4, num * 8);
}

void eeprom_disable(void)
{
	SysCtlPeripheralDisable(SYSCTL_PERIPH_EEPROM0); 
}

