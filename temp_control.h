#ifndef TEMP_CONTROL_H
#define TEMP_CONTROL_H

#include "types.h"
#include "port.h"
#include "macros.h"

// system constants
#define TEMP_ERROR 5
#define MINI_TEMP 35
#define MAX_TEMP 75
#define LED_TOOGLE_TIME 1000



typedef enum {
    HEATER_ON ,
    COOLER_ON,
    IDEL_STATE
    }TEMP_STATE_t ;

void temp_control_off(void);
void temp_control_init(void);
void temp_set( uint8_t temp );
void led(void);
    

#endif