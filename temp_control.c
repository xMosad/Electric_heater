#include "temp_control.h"

extern uint8_t set_temp ;
extern uint16_t led_timer ;
TEMP_STATE_t state ;

void temp_control_init(void){
     // heater intitialize
     CLEAR_BIT (HEATER_DIR  , HEATER_PIN);  // set the pin output
     CLEAR_BIT (HEATER_PORT , HEATER_PIN); // heater off
     
     // cooler initialize
     CLEAR_BIT (COOLER_DIR  , COOLER_PIN); //set the pin output
     CLEAR_BIT (COOLER_PORT , COOLER_PIN); //cooler off
     
     // led initialize
     CLEAR_BIT (LED_DIR  , LED_PIN);
     CLEAR_BIT (LED_PORT , LED_PIN);
}

void temp_set( uint8_t temp ){
     if ( temp < (set_temp - TEMP_ERROR ) ){
          SET_BIT(HEATER_PORT , HEATER_PIN); // heater on
          CLEAR_BIT (COOLER_PORT , COOLER_PIN); //cooler off
          state = HEATER_ON ;
     }
     else if ( temp > (set_temp + TEMP_ERROR ) ){
          SET_BIT(COOLER_PORT , COOLER_PIN); //cooler on
          CLEAR_BIT (HEATER_PORT , HEATER_PIN); // heater off
          state = COOLER_ON ;
     }
     else {
          //CLEAR_BIT (HEATER_PORT , HEATER_PIN); // heater off
          //CLEAR_BIT (COOLER_PORT , COOLER_PIN); //cooler off
          //state = IDEL_STATE ;
     }
}

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
         case IDEL_STATE : CLEAR_BIT (LED_PORT , LED_PIN); break;
         
         default         : break ;
     }
}

void temp_control_off(void){
     CLEAR_BIT (HEATER_PORT , HEATER_PIN); // heater off
     CLEAR_BIT (COOLER_PORT , COOLER_PIN); //cooler off
     CLEAR_BIT (LED_PORT , LED_PIN);
}