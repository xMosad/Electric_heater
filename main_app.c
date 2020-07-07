/*
 * File:   main_app.c
 * Author: mosad
 *
 * Created on July 6, 2020, 10:12 PM
 */
// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)4

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
POWER_MODES_t power_mode = ON_STATE ; // variable will hold the power state 


void __interrupt () isr (void){
    TMR2 = TIMER_INITIAL_VALUE ;     // initial value to fire every 1 ms
    if(PIR1 & (1 << 1)){
       CLEAR_BIT(PIR1 , 1);    // clear flag
       switch (mode){
           case NORMAL_MODE  :
                hold_temp = temp_sensor_read ();
                ssd_update(hold_temp);
                temp_set (hold_temp);
                led();
                switch_scan();
                sw_action();
                break;
           case SETTING_MODE :
                switch_scan();
                sw_action();
                ssd_blink(set_temp);
                break;
       }
    }
    if (INTCON & (1 << 1)){
        CLEAR_BIT(INTCON , 1);
        switch (power_mode){
             case ON_STATE :
                 power_mode = OFF_STATE ;
                 stop_timer();
                 ssd_turn_off();
                 temp_control_off();
                 break;
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
    if (EEPROM_read(0xff) > 100){
       set_temp = 60 ;
    }
    else {
       set_temp = EEPROM_read(0xff) ;
    }
    timer2_init();
    
    //super loop 
    while(1){
        // sleep 
    }
}
