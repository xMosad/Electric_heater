#include "timer2.h"

void timer2_init(void){
     CLEAR_BIT(T2CON , 2) ;   // stop timer
     SET_BIT(T2CON , 1);      // prescaller 16
     CLEAR_BIT(T2CON , 0);    // prescaller 16
     TMR2 = TIMER_INITIAL_VALUE ;            // intial value to fire every 1 ms
     SET_BIT(INTCON , 7);     // GIE = 1 global interrupt enable
     SET_BIT(INTCON , 6);     // PEIE = 1 global interrupt enable
     CLEAR_BIT(PIR1 , 1);    // clear flag
     SET_BIT(PIE1 , 1);     // enable timer2 interrupt
     SET_BIT(T2CON , 2) ;   // start timer
}

void stop_timer(void){
    CLEAR_BIT(T2CON , 2) ;   // stop timer
}

void start_timer(void){
    SET_BIT(T2CON , 2) ;   // start timer
    TMR2 = TIMER_INITIAL_VALUE ;            // intial value to fire every 1 ms
}