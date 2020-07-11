#include "temp_control.h"

// system variables
extern uint8_t set_temp ;
extern uint8_t measured_temp ; // store the measured temperature
extern MODE_STATE_t mode  ; // variable will hold the system state

// private 
TEMP_STATE_t state ;

/******************************************************************************
* Description : Initialize the temp_control moduler
*               Initialize cooler , heater , indicator led    
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void temp_control_init(void){
     // heater initialize
     CLEAR_BIT (HEATER_DIR  , HEATER_PIN);  // set the pin output
     CLEAR_BIT (HEATER_PORT , HEATER_PIN); // heater off
     
     // cooler initialize
     CLEAR_BIT (COOLER_DIR  , COOLER_PIN); //set the pin output
     CLEAR_BIT (COOLER_PORT , COOLER_PIN); //cooler off
     
     // led initialize
     CLEAR_BIT (LED_DIR  , LED_PIN);
     CLEAR_BIT (LED_PORT , LED_PIN);
}

/******************************************************************************
* Description : the task to turn on the cooler or the heater based 
 *              on the measured temp . will be called for every new temp taken 
* Parameters  : the measured temp (uint8_t); 
* Return type : void                                                                           
*******************************************************************************/
void temp_set( void ){
    if (mode == NORMAL_MODE){
        if ( measured_temp < (set_temp - TEMP_ERROR ) ){
            SET_BIT(HEATER_PORT , HEATER_PIN); // heater on
            CLEAR_BIT (COOLER_PORT , COOLER_PIN); //cooler off
            state = HEATER_ON ; // will blink the led
        }
        else if ( measured_temp > (set_temp + TEMP_ERROR ) ){
            SET_BIT(COOLER_PORT , COOLER_PIN); //cooler on
            CLEAR_BIT (HEATER_PORT , HEATER_PIN); // heater off
            SET_BIT(LED_PORT , LED_PIN);
            state = HEATER_OFF ;
        }
        else if ( measured_temp == set_temp){
            CLEAR_BIT (COOLER_PORT , COOLER_PIN); //cooler off
            CLEAR_BIT (HEATER_PORT , HEATER_PIN); // heater off
            CLEAR_BIT (LED_PORT , LED_PIN);
            state = HEATER_OFF ;
        }
        else {
              /* shouldn't be here */
        }
    }
    else {
        temp_control_off();
    }
}

/******************************************************************************
* Description : set the led indecator for the right operation  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void led(void){
    if (mode == NORMAL_MODE){
        if (state == HEATER_ON){
            TOGGLE_BIT(LED_PORT , LED_PIN);
        }
        else {

        }
    }
}

/******************************************************************************
* Description : turn off cooler , heater , led  
* Parameters  : none
* Return type : void                                                                           
*******************************************************************************/
void temp_control_off(void){
    CLEAR_BIT (HEATER_PORT , HEATER_PIN); // heater off
    CLEAR_BIT (COOLER_PORT , COOLER_PIN); //cooler off
    CLEAR_BIT (LED_PORT , LED_PIN);       //led off
}