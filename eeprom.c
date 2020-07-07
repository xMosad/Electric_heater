#include "eeprom.h"

void EEPROM_init(void){
     I2C1_Init(100000);
}

void EEPROM_write (uint16_t address , uint8_t _x){
    I2C1_Start();
    I2C1_Wr(0XA2);
    I2C1_Wr(address);
    I2C1_Wr(_x);
    I2C1_Stop();
}
uint8_t EEPROM_read(uint16_t address ){
    uint8_t _x = 0 ;
    I2C1_Start();
    I2C1_Wr(0XA2);
    I2C1_Wr(address);
    I2C1_Start();
    I2C1_Wr(0XA3);
    _x = I2C1_Rd();
    I2C1_Stop();
    return _x ;
}