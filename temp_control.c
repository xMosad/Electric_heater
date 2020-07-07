#include "temp_control.h"

extern uint8_t set_temp ;
extern uint16_t led_timer ;
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
* Description : turn on the cooler or the heater based on the measured temp  
* Parameters  : the measured temp (uint8_t); 
* Return type : void                                                                           
*******************************************************************************/
void temp_set( uint8_t temp ){
    if ( temp < (set_temp - TEMP_ERROR ) ){
        SET_BIT(HEATER_PORT , HEATER_PIN); // heater on
        CLEAR_BIT (COOLER_PORT , COOLER_PIN); //cooler off
        state = HEATER_ON ; // will blink the led
    }
    else if ( temp > (set_temp + TEMP_ERROR ) ){
        SET_BIT(COOLER_PORT , COOLER_PIN); //cooler on
        CLEAR_BIT (HEATER_PORT , HEATER_PIN); // heater off
        state = COOLER_ON ; // the led will be on
    }
    else if ( temp == set_temp){
        CLEAR_BIT (COOLER_PORT , COOLER_PIN); //cooler off
        CLEAR_BIT (HEATER_PORT , HEATER_PIN); // heater off
        state = IDEL_STATE ; //the led will be off
    }
    else {
          /* shouldn't be here */
    }
}

/******************************************************************************
* Description : set the led indecator for the right operation  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void led(void){
    switch (state){
        // toggle the led if the heater is on
        case HEATER_ON :
            led_timer += TIC_TIME;
                if (led_timer == LED_TOOGLE_TIME){
                    TOGGLE_BIT(LED_PORT , LED_PIN);
                    led_timer = 0 ; }
            break ;             
        // turn the led on if the cooler is on
        case COOLER_ON  :
                SET_BIT(LED_PORT , LED_PIN);
                led_timer = 0 ;
                break ;
        // turn the led off if the cooler is on
        case IDEL_STATE :
            CLEAR_BIT (LED_PORT , LED_PIN);
            break; 
        default : 
            /* shouldn't be here */
            break ;
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
     led_timer = 0 ; // initialize the counter
}