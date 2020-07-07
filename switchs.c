#include "switchs.h"

extern uint8_t switch_delay;
extern uint16_t switch_wait;
extern uint8_t set_temp  ;
SWITCH_STATE_t up_sw = RELEASED ;
SWITCH_STATE_t down_sw = RELEASED ;
SWITCH_STATE_t power_sw = RELEASED ;

extern MODE_STATE_t mode ;

void switch_init(void){
     // setting all the pins for buttons as input
     SET_BIT(UP_BUTTON_DIR , UP_BUTTON_PIN);
     SET_BIT(DOWN_BUTTON_DIR , DOWN_BUTTON_PIN);
     SET_BIT(POWER_BUTTON_DIR , POWER_BUTTON_PIN);
     SET_BIT(INTCON , 4); // enable external interrupt on b0
}

void switch_scan(void){
     switch_delay += TIC_TIME ;
    if (switch_delay == SCANE_INTERVAL){
       if ( !(IS_SET(UP_BUTTON_PORT , UP_BUTTON_PIN))){
          up_sw = PRESSED ;
       }
       else if ( !(IS_SET(DOWN_BUTTON_PORT , DOWN_BUTTON_PIN))){
          down_sw = PRESSED ;
       }
       else {
       
       }
       switch_delay = 0 ;
    }
}

void sw_action(void){
     switch (mode){
         case NORMAL_MODE :
              if ( (up_sw == PRESSED ) || (down_sw == PRESSED) ) {
                  mode = SETTING_MODE ;
                  up_sw   = RELEASED ;
                  down_sw = RELEASED ;
              }
              else{
              
              }
              break;
         case SETTING_MODE :
              if (switch_wait < SWITCH_MAX_WAIT){
                  if ( (up_sw == PRESSED) && (set_temp < 75)){
                      set_temp += TEMP_CHANGE ;
                      switch_wait = 0 ;
                      up_sw = RELEASED ;
                  }
                  else if ( (down_sw == PRESSED) && (set_temp > 35)){
                      set_temp -= TEMP_CHANGE ;
                      switch_wait = 0 ;
                      down_sw = RELEASED ;
                  }
                  else {
                      switch_wait += TIC_TIME ;
                  }
              }
              else {
                   mode = NORMAL_MODE ;
                   switch_wait = 0 ;
                   EEPROM_write(0xff , set_temp);
              }
              break;
     }
}
