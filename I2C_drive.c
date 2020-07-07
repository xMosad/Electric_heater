#include "I2C_drive.h"

void I2C_Wait(void){
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C1_Init(long int freq){
    SET_BIT(SSPCON , 5 ); //Enables the serial port and configures the SDA and SCL 
    SET_BIT(SSPCON , 3 ); //I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
    SSPADD = (8000000 / (freq * 4) ) - 1 ; // setting the clock
    TRISC |= (1 << 3);
    TRISC |= (1 << 4);
}

void I2C1_Start(void){ 
    I2C_Wait();
    SET_BIT(SSPCON2 , 0); 
}

void I2C1_Stop(void){
    I2C_Wait();
    SET_BIT(SSPCON2 , 2);
}

void I2C1_Wr(uint8_t _data){
    I2C_Wait();
    SSPBUF = _data ;
    I2C_Wait();
}

uint8_t I2C1_Rd(void){
    SSPIF = 0;       // Clear The Interrupt Flag Bit
    SET_BIT(SSPCON2 , 3);
    while(!SSPIF);   // Wait Until Completion
    SSPIF = 0;       // Clear The Interrupt Flag Bit
    return SSPBUF;   // Return The Received Byte
}
