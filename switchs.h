#ifndef SWITCHS_H
#define SWITCHS_H

#include "types.h"
#include "port.h"
#include "macros.h"
#include "eeprom.h"

#define SCANE_INTERVAL 200
#define SWITCH_MAX_WAIT 5000
#define TEMP_CHANGE 5

typedef enum{
    PRESSED,
    RELEASED
}SWITCH_STATE_t;

void switch_init(void);
void switch_scan(void);
void sw_action(void);


#endif