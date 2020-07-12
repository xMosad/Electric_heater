
#include "switchs.h"

// system variables
extern uint8_t set_temp  ;
extern MODE_STATE_t mode ;
extern SSD_BLINK_t blink  ; // to store the ssd state on or off

// private variables 
static uint16_t switch_wait;

typedef struct
{
    unsigned char samples[2];
    SWITCH_STATE_t state;
}SWITCH_INFO_t;

static SWITCH_INFO_t sw_info[2];


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
    sw_info[0].state = RELEASED;
    sw_info[1].state = RELEASED;
}

/******************************************************************************
* Description : check if a button is pressed and change its state  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void switch_scan(void){
    
    SW_t current_button = SW_UP;
    for (current_button = SW_UP; current_button <= SW_DOWN; current_button++)
    {
        /* Update samples */
        sw_info[current_button].samples[0] = sw_info[current_button].samples[1];

        switch(current_button)
        {
         case SW_UP:
            sw_info[current_button].samples[1] = ((PORTB >> 2) & 1);
        break;
        case SW_DOWN:
            sw_info[current_button].samples[1] = ((PORTB >> 1) & 1);
            break;
        default:
            /* Should not be here */
            break;
        }

        /* Update state */
        switch(sw_info[current_button].state)
        {
        case RELEASED:
            if((sw_info[current_button].samples[0] == 0) &&
               (sw_info[current_button].samples[1] == 0))
            {
                sw_info[current_button].state = PRE_PRESSED;
            }
            break;
        case PRE_PRESSED:
            if(sw_info[current_button].samples[1] == 0)
            {
                sw_info[current_button].state = PRESSED;
            }
            break;
        case PRESSED:
            if((sw_info[current_button].samples[0] == 1) &&
               (sw_info[current_button].samples[1] == 1))
            {
                sw_info[current_button].state = PRE_RELEASED;
            }
            break;
        case PRE_RELEASED:
            if(sw_info[current_button].samples[1] == 1)
            {
                sw_info[current_button].state = RELEASED;
            }
            break;
            //break;
        default:
            /* Should not be here */
            break;
        }

    }
    
   sw_action();   
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
            if ( (sw_info[0].state == PRE_RELEASED ) ||
                (sw_info[1].state == PRE_RELEASED) ) {
                mode    = SETTING_MODE ; // to enter the setting mode.
            }
            else{
                /* shouldn't be here */
            }
            break;
        // in setting mode and a button is pressed    
        case SETTING_MODE :
            // will remain here for 5 s if no button is pressed
            if (switch_wait < SWITCH_MAX_WAIT){
                if ( (sw_info[0].state == PRE_RELEASED) && (set_temp < 75)){
                    set_temp += TEMP_CHANGE ;
                    switch_wait = 0 ; // to begin counting 5s from the beginning                   
                }
                else if ( (sw_info[1].state == PRE_RELEASED) && (set_temp > 35)){
                    set_temp -= TEMP_CHANGE ;
                    switch_wait = 0 ; // to begin counting 5s from the beginning
                }
                else {
                    switch_wait += SWITCH_SCAN_TASK_PERIOD ;
                }
            }
            else {
                mode = NORMAL_MODE ; // switch to normal mode
                switch_wait = 0 ;    // initialize the counter 
                EEPROM_write(0xff , set_temp); // write the new set temp to the eeprom
                blink = SSD_ON ;
            }
            break;
        default : 
            /* shouldn't be here */
            break ;
    }    
}
