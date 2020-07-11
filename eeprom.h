#ifndef EEPROM_H
#define EEPROM_H

#include "Main.h"
#include "I2C_drive.h"

void EEPROM_init(void);
void EEPROM_write (uint16_t address , uint8_t _x);
uint8_t EEPROM_read(uint16_t address );
void get_set_temp(void);

#endif