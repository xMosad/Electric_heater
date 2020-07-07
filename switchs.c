#include "switchs.h"

extern uint8_t switch_delay;
extern uint16_t switch_wait;
extern uint8_t set_temp  ;
SWITCH_STATE_t up_sw = RELEASED ; // up buuton state
SWITCH_STATE_t down_sw = RELEASED ; // down buuton state
extern MODE_STATE_t mode ;

/******************************************************************************
* Description : Initialize switch module  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void switch_init(void){
     // setting all the pins for buttons as input
     SET_BIT(UP_BUTTON_DIR , UP_BUTTON_PIN);
     SET_BIT(DOWN_BUTTON_DIR , DOWN_BUTTON_PIN);
     SET_BIT(POWER_BUTTON_DIR , POWER_BUTTON_PIN);
     SET_BIT(INTCON , 4); // enable external interrupt on b0
}

/******************************************************************************
* Description : check if a button is pressed and change its state  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void switch_scan(void){
    switch_delay += TIC_TIME ;
    // will enter the if every 200 ms  
    if (switch_delay == SCANE_INTERVAL){
       if ( !(IS_SET(UP_BUTTON_PORT , UP_BUTTON_PIN))){
          up_sw = PRESSED ; // the up button is pressed
       }
       else if ( !(IS_SET(DOWN_BUTTON_PORT , DOWN_BUTTON_PIN))){
          down_sw = PRESSED ; // the down button is pressed
       }
       else {
           /* shouldn't reach here */
       }
       switch_delay = 0 ; // initialize the counter 
    }
}

/******************************************************************************
* Description : take action based on the system mode and the pressed button  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void sw_action(void){
    switch (mode){
        // in the normal mode and on a button is pressed
        case NORMAL_MODE :
            if ( (up_sw == PRESSED ) || (down_sw == PRESSED) ) {
                mode    = SETTING_MODE ; // to enter the setting mode.
                up_sw   = RELEASED ;     // initialize the up button state.
                down_sw = RELEASED ;     // initialize the down button state.
            }
            else{
                /* shouldn't be here */
            }
            break;
        // in setting mode and a button is pressed    
        case SETTING_MODE :
            // will remain here for 5 s if no button is pressed
            if (switch_wait < SWITCH_MAX_WAIT){
                if ( (up_sw == PRESSED) && (set_temp < 75)){
                    set_temp += TEMP_CHANGE ;
                    switch_wait = 0 ; // to begin counting 5s from the beginning
                    up_sw = RELEASED ;
                }
                else if ( (down_sw == PRESSED) && (set_temp > 35)){
                    set_temp -= TEMP_CHANGE ;
                    switch_wait = 0 ; // to begin counting 5s from the beginning
                    down_sw = RELEASED ;
                }
                else {
                    switch_wait += TIC_TIME ;
                }
            }
            else {
                mode = NORMAL_MODE ; // switch to normal mode
                switch_wait = 0 ;    // initialize the counter 
                EEPROM_write(0xff , set_temp); // write the new set temp to the eeprom
            }
            break;
        default : 
            /* shouldn't be here */
            break ;
    }
}
