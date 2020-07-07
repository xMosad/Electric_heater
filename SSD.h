#ifndef SSD_H
#define SSD_H

#include "types.h"
#include "port.h"
#include "macros.h"

#define SSD_SWITCH_TIME 50 // delay between SSD switch 
#define SSD_BLINK_TIME 1000 // blink period of SSD

void ssd_init(void);
uint8_t display7s(uint8_t v);
void ssd_update(uint8_t temp);
void ssd_turn_off(void);
void ssd_blink(uint8_t e_temp);

typedef enum {
    SSD_LEFT,
    SSD_RIGHT
}SSD_SELECT_t;

typedef enum {
    SSD_ON,
    SSD_OFF
}SSD_BLINK_t;

#endif