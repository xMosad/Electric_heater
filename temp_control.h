#ifndef TEMP_CONTROL_H
#define TEMP_CONTROL_H

#include "Main.h"
#include "port.h"
#include "macros.h"

// system constants
#define TEMP_ERROR 5
#define MINI_TEMP 35
#define MAX_TEMP 75

typedef enum {
    HEATER_ON ,
    HEATER_OFF,
    }TEMP_STATE_t ;

void temp_control_off(void);
void temp_control_init(void);
void temp_set( void );
void led(void);
    
#endif