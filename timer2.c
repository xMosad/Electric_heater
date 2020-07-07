#include "timer2.h"

/******************************************************************************
* Description : Initialize timer 2 to overflow every 1 ms  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void timer2_init(void){
    CLEAR_BIT(T2CON , 2) ;        // stop timer
    SET_BIT(T2CON , 1);           // prescaller 16
    CLEAR_BIT(T2CON , 0);         // prescaller 16
    TMR2 = TIMER_INITIAL_VALUE ;  // intial value to fire every 1 ms
    SET_BIT(INTCON , 7);          // GIE = 1 global interrupt enable
    SET_BIT(INTCON , 6);          // PEIE = 1 Peripheral Interrupt Enable
    CLEAR_BIT(PIR1 , 1);          // clear flag
    SET_BIT(PIE1 , 1);            // enable timer2 interrupt
    SET_BIT(T2CON , 2) ;          // start timer
}

/******************************************************************************
* Description : stop timer 2  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void stop_timer(void){
    CLEAR_BIT(T2CON , 2) ;   // stop timer
}

/******************************************************************************
* Description : start timer 2 
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void start_timer(void){
    SET_BIT(T2CON , 2) ;   // start timer
    TMR2 = TIMER_INITIAL_VALUE ;            // intial value to fire every 1 ms
}