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
#include "Sch_16f.h"


/*              global variables                  */

uint8_t set_temp ; // store the setting temperature 
uint8_t measured_temp ; // store the measured temperature 

MODE_STATE_t mode = NORMAL_MODE ; // variable will hold the system state 
POWER_MODES_t power_mode = OFF_STATE ; // variable will hold the power state 


void main() {
    // system initialize 
    temp_sensor_init();
    temp_control_init();
    switch_init();
    ssd_init();
    EEPROM_init();
    SCH_Init(); // initialize the scheduler 
    
    // tasks
    /*
     * we have 7 tasks will be handled by the scheduler 
     * the scheduler uses timer1 an isr is fired every 10 ms 
     * - > get_set_temp task : 
     *      one shot task only invoked at the beginning to read set_temp
     * - > temp_sensor_read task : 
     *      a periodic task invoked every (GET_TEMP_TASK_PERIOD) 
     *      only in ( NORMAL_MODE ) take temp from LM35 and calculate  
     *      the average of the last ten values
     * - > ssd_update task : 
     *      a periodic task invoked every (SSD_UPDATE_TASK_PERIOD)
     *      only in ( NORMAL_MODE ) write to one of SSD at a time 
     * - > led task : 
     *      a periodic task invoked every (LED_TASK_PERIOD) 
     *      only in ( NORMAL_MODE ) and the heater is on  
     * - > temp_set task : 
     *      a periodic task invoked every (TEMP_SET_TASK_PERIOD) 
     *      only in ( NORMAL_MODE ) and compare the measured_temp with set_temp
     *      and open heater or cooler based on that 
     * - > switch_scan task : 
     *      a periodic task invoked every (SWITCH_SCAN_TASK_PERIOD) 
     *      in both modes check on buttons and take action based on its state
     * - > ssd_blink task : 
     *      a periodic task invoked every (SSD_BLINK_TASK_PERIOD) 
     *      only in (SETTING_MODE) to blink the SSD       
     */
    SCH_Add_Task(get_set_temp , 0 , 0); 
    SCH_Add_Task(temp_sensor_read , 0 , GET_TEMP_TASK_PERIOD);
    SCH_Add_Task(ssd_update , 0 , SSD_UPDATE_TASK_PERIOD);
    SCH_Add_Task(led , 0 , LED_TASK_PERIOD);
    SCH_Add_Task(temp_set , 0 , TEMP_SET_TASK_PERIOD);
    SCH_Add_Task(switch_scan , 0 , SWITCH_SCAN_TASK_PERIOD);
    SCH_Add_Task(ssd_blink , 0 , SSD_BLINK_TASK_PERIOD);
    
    //super loop 
    while(1){
        // call the dispatcher 
        SCH_Dispatch_Tasks();
    }
}



