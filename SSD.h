#ifndef SSD_H
#define SSD_H

// includes 
#include "Main.h"
#include "port.h"
#include "macros.h"

void ssd_init(void);
uint8_t display7s(uint8_t v);
void ssd_update(void);
void ssd_turn_off(void);
void ssd_blink(void);

// to determine which SSD will be on 
typedef enum {
    SSD_LEFT,
    SSD_RIGHT
}SSD_SELECT_t;

// to determine the state of SSD to blink 
typedef enum {
    SSD_ON,
    SSD_OFF
}SSD_BLINK_t;

#endif