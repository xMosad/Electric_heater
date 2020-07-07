#include "I2C_drive.h"

/******************************************************************************
* Description : wait till the required bits are cleared 
* Parameters  : none
* Return type : void                                                                           
*******************************************************************************/
void I2C_Wait(void){
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

/******************************************************************************
* Description : Initialize the I2C in master mode  
* Parameters  : the frequency of the transmission 
* Return type : void                                                                           
*******************************************************************************/
void I2C1_Init(uint32_t freq){
    SET_BIT(SSPCON , 5 ); //Enables the serial port and configures the SDA and SCL 
    SET_BIT(SSPCON , 3 ); //I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
    SSPADD = (8000000 / (freq * 4) ) - 1 ; // setting the clock
    // setting SDA and SCK pins as input
    TRISC |= (1 << 3); 
    TRISC |= (1 << 4);
}

/******************************************************************************
* Description : send start condition
* Parameters  : none
* Return type : void                                                                           
*******************************************************************************/
void I2C1_Start(void){ 
    I2C_Wait();
    SET_BIT(SSPCON2 , 0); 
}

/******************************************************************************
* Description : send stop condition  
* Parameters  : none
* Return type : void                                                                           
*******************************************************************************/
void I2C1_Stop(void){
    I2C_Wait();
    SET_BIT(SSPCON2 , 2);
}

/******************************************************************************
* Description : write the data to the I2C bus 
* Parameters  : the data to be sent 8-bit
* Return type : void                                                                           
*******************************************************************************/
void I2C1_Wr(uint8_t _data){
    I2C_Wait();
    SSPBUF = _data ; // write the date to th buffer reg to be sent 
    I2C_Wait();
}

/******************************************************************************
* Description : read the incoming data from I2C bus 
* Parameters  : none
* Return type : uint8_t                                                                           
*******************************************************************************/
uint8_t I2C1_Rd(void){
    SSPIF = 0;       // Clear The Interrupt Flag Bit
    SET_BIT(SSPCON2 , 3);
    while(!SSPIF);   // Wait Until Completion
    SSPIF = 0;       // Clear The Interrupt Flag Bit
    return SSPBUF;   // Return The Received Byte
}
