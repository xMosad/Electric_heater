#include "temp_sensor.h"

static uint16_t temp_hold_array[TEMP_SAMPLES] ;
extern uint8_t temp_timer ;
static uint8_t i = 0 ;
uint8_t temp = 0 ;

/******************************************************************************
* Description : Initialize lm35 an pin A2  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void temp_sensor_init(void){
    ADC_Init(); // initialize the adc
    SET_BIT(TEMP_SENSOR_DIR , TEMP_SENSOR_PIN); //input
}

/******************************************************************************
* Description : calculate the average of the last ten measured temp values 
* Parameters  : none 
* Return type : uint8_t                                                                           
*******************************************************************************/
uint8_t average (void){
    uint8_t i = 0 ; // a counter for the for loop
    uint16_t sum = 0 ;//hold the sum of the ten samples
    for (i = 0 ; i < TEMP_SAMPLES ; i++){
        sum = sum + temp_hold_array[i] ;
    }
    return (sum / TEMP_SAMPLES) ;
}

/******************************************************************************
* Description : read the temp every 100 ms  
* Parameters  : none 
* Return type : uint8_t                                                                           
*******************************************************************************/
uint8_t temp_sensor_read (void) {
    temp_timer += TIC_TIME;
    // will enter here every 100 ms 
    if( temp_timer == TEMP_PERIOD ){
        // reading the value + equation 
        temp_hold_array[i] = (ADC_Read(TEMP_SENSOR_PIN) * 4.89 )  ;
        temp_hold_array[i] = temp_hold_array[i] / 10.0 ;
        i++ ;
        temp_timer = 0 ; // initialize the timer counter
        if (i == TEMP_SAMPLES){i = 0 ;} 
        temp = average(); // take the average 
        return temp ;
    }
    else {
        return temp ;
    }
}