#include "temp_sensor.h"

//system variables
extern MODE_STATE_t mode  ; // variable will hold the system state
extern uint8_t measured_temp ; // store the measured temperature

//private variables
static uint16_t temp_hold_array[TEMP_SAMPLES]; // The array hold the last 10 temp readings
static uint8_t i = 0 ;


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
* Description : TASK - > read the temp every 100 ms  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void temp_sensor_read (void) {
    if (mode == NORMAL_MODE){
        // reading the value + equation 
        temp_hold_array[i] = (ADC_Read(TEMP_SENSOR_PIN) * 4.89 )  ;
        temp_hold_array[i] = temp_hold_array[i] / 10.0 ;
        i++ ;
        if (i == TEMP_SAMPLES){i = 0 ;} 
        measured_temp = average(); // take the average 
    }
}