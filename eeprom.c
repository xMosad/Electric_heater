#include "eeprom.h"

extern uint8_t set_temp ; // store the setting temperature 
/******************************************************************************
* Description : Initialize the EEPROM 24c04  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void EEPROM_init(void){
     I2C1_Init(100000); // intialize the 12c with freq 100 KHZ
}

/******************************************************************************
* Description : write to the 24c04  
* Parameters  : the address to be written into (uint16_t)
*               the data to be written         (uint8_t)
* Return type : void                                                                           
*******************************************************************************/
void EEPROM_write (uint16_t address , uint8_t _x){
    I2C1_Start();     //send start
    I2C1_Wr(0XA2);    // write the 24c04 (write address) 
    I2C1_Wr(address); // write address of the data
    I2C1_Wr(_x);      // write the data in the address
    I2C1_Stop();      // send stop
}

/******************************************************************************
* Description : read from a specific address in 24c04  
* Parameters  : the address where we read from  
* Return type : uint8_t                                                                           
*******************************************************************************/
uint8_t EEPROM_read(uint16_t address ){
    uint8_t _x = 0 ;  // variable to hold the written data 
    I2C1_Start();     // send start 
    I2C1_Wr(0XA2);    // write the 24c04 (write address)
    I2C1_Wr(address); // write address of the data
    I2C1_Start();     // send repeated start
    I2C1_Wr(0XA3);    // write the 24c04 (read address) 
    _x = I2C1_Rd();   // read the data
    I2C1_Stop();      // send stop
    return _x ;
}
/******************************************************************************
* Description : get the set_temp when powering up (one - shot)  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void get_set_temp(void){
    // if a reading is stored in the eeprom we will read it 
    // if not the default temperature is 60 
    if (EEPROM_read(0xff) > 100){
       set_temp = 60 ;
    }
    else {
       set_temp = EEPROM_read(0xff) ;
    }
}