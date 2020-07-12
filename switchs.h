#ifndef SWITCHS_H
#define SWITCHS_H

#include "Main.h"
#include "port.h"
#include "macros.h"
#include "eeprom.h"
#include "Sch_16f.h"
#include "SSD.h"

#define SWITCH_MAX_WAIT 5000
#define TEMP_CHANGE 5
#define SWITCH_SAMPLES 2 
#define PRESSED_VOLTAGE 0
#define RELEASED_VOLTAGE 1

typedef enum
{
    SW_UP,
    SW_DOWN
}SW_t;

typedef enum
{
    RELEASED,
    PRE_PRESSED,
    PRESSED,
    PRE_RELEASED
}SWITCH_STATE_t;

void switch_init(void);
void switch_scan(void);
void sw_action(void);

#endif