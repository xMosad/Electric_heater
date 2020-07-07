#include "adc_drive.h"

/******************************************************************************
* Description : Initialize the adc with vref - > vcc 
* Parameters  : none
* Return type : void                                                                           
*******************************************************************************/
void ADC_Init(void){ 
    // selecting the frequency to rc
    SET_BIT(ADCON0 , 7);
    SET_BIT(ADCON0 , 6);
    SET_BIT(ADCON1 , 6);
    // value is right justfied
    SET_BIT(ADCON1 , 7);
    // vref
    CLEAR_BIT(ADCON1 , 0);
    CLEAR_BIT(ADCON1 , 1);
    CLEAR_BIT(ADCON1 , 2);
    CLEAR_BIT(ADCON1 , 3);
    // adc on
    SET_BIT(ADCON0 , 0);
}

/******************************************************************************
* Description : read the value of the adc selected channel (0 - > 1023) 
* Parameters  : the channel number
* Return type : uint16_t                                                                           
*******************************************************************************/
uint16_t ADC_Read (uint8_t channel){
    CLEAR_BIT(ADCON0 , 3);
    CLEAR_BIT(ADCON0 , 4);
    CLEAR_BIT(ADCON0 , 5);
    ADCON0 |= (channel << 3);
    SET_BIT (ADCON0 , 2); // start conversion 
    while (ADCON0 & (1 << 2)){}
    return ( ADRESL + (ADRESH* 256) ) ;
}