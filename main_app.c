/*
 * File      : main_app.c
 * Author    : Abdelrhman Mosad Abdelhady
 * Project   : Electric water heater
 * simulator : picsimlab - > https://sourceforge.net/projects/picsim/  
 * Created on July 6, 2020, 10:12 PM
 */
// CONFIG
#pragma config FOSC   = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE   = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE  = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN  = OFF       // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP    = ON        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD    = OFF       // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT    = OFF       // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP     = OFF       // Flash Program Memory Code Protection bit (Code protection off)4

/*                  includes                      */
#include <xc.h>
#include "timer2.h"
#include "temp_sensor.h"
#include "temp_control.h"
#include "SSD.h"
#include "switchs.h"
#include "eeprom.h"

/*              global variables                  */

//timers
uint8_t temp_timer ; // to take the temp every 100 ms
uint8_t ssd_delay ; // to power on one display at the time every 50 ms
uint8_t switch_delay; // to check on the switches every 200 ms
uint16_t switch_wait; // to count till 5000ms if no buttons pressed in setting mode
uint16_t ssd_timer; // to blink ssd every 1000 ms 
uint16_t led_timer ; // to blink led every 1000 ms

uint8_t set_temp ; // store the setting temperature 
uint8_t hold_temp ; // store the measured temperature 

MODE_STATE_t mode = NORMAL_MODE ; // variable will hold the system state 
POWER_MODES_t power_mode = OFF_STATE ; // variable will hold the power state 

/*
 * The ISR handle two interrupts :
 *   - > the timer 2 interrupt :
 *        * The timer will overflow every 1 ms.
 *        * Timer 2 is the main OS handling all the tasks in a cooperative style.
 *        * the system has 7 main tasks handled in the ISR.
 *        * a main state variable named *mode* will determine which tasks will be
 *        * executed every tick. 
 * 
 *        *  - > We have two main modes the normal mode where :  
 *        *    --> read temp from lm35 every 100 ms.
 *        *    --> update the SSD with the current temp.
 *        *    --> change the temp by turning on the heater or the cooler.
 *        *    --> set the indicator led will blink every 1 s if heater is on.
 *        *    --> scan the switches every 200 ms.
 *        *    --> if a switch is pressed an action will be taken in sw_action.
 * 
 *        *  - > The second mode is the setting mode  :
 *        *    --> scan the switches every 200 ms.
 *        *    --> if a switch is pressed an action will be taken in sw_action.
 *        *    --> blink the SSD every 1 s.
 *        * will exit the mode if no buttons are pressed for 5 s.
 * 
 *   - > the external interrupt :
 *        * handle the power button.
 *        * check on the current power state of the system.
 *        * if the system is on and the ISR is fired will power off the system.
 *        * if the system is off and the ISR is fired will power on the system.
 *        * if the system is off : SSD , led cooler , heater are off + the
 *        * timer will be off. 
 */

void __interrupt () isr (void){
    TMR2 = TIMER_INITIAL_VALUE ;     // initial value to fire every 1 ms
    // timer interrupt
    if(PIR1 & (1 << 1)){
       CLEAR_BIT(PIR1 , 1);    // clear flag
       switch (mode){
           // this mode run when no switches are pressed 
           case NORMAL_MODE  :
                //tasks 
                hold_temp = temp_sensor_read ();
                ssd_update(hold_temp);
                temp_set (hold_temp);
                led();
                switch_scan();
                sw_action();
                break;
            // this mode will be entered when the up or down buttons are pressed    
           case SETTING_MODE :
                //tasks
                switch_scan();
                sw_action();
                ssd_blink(set_temp);
                break;
           default : 
               
               break ;
       }
    }
    
    // external interrupt on b0
    if (INTCON & (1 << 1)){
        CLEAR_BIT(INTCON , 1); // clear the external interrupt flag 
        switch (power_mode){
            // if we were in the on state and the power button was pressed
            case ON_STATE :
                power_mode = OFF_STATE ;
                stop_timer();
                ssd_turn_off();
                temp_control_off();
                break;
             // if we were in the off state and the power button was pressed    
            case OFF_STATE :
                power_mode = ON_STATE ;
                start_timer();
                break;
        }
    }

}
void main() {
    // system initialize 
    temp_sensor_init();
    temp_control_init();
    switch_init();
    ssd_init();
    EEPROM_init();
    timer2_init();
    // if a reading is stored in the eeprom we will read it 
    // if not the default temperature is 60 
    if (EEPROM_read(0xff) > 100){
       set_temp = 60 ;
    }
    else {
       set_temp = EEPROM_read(0xff) ;
    }
    
    
    // in off state 
    stop_timer();
    ssd_turn_off();
    temp_control_off();
    
    //super loop 
    while(1){
        // sleep
        system_sleep();
    }
}
