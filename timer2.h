#ifndef TIMER0_H
#define TIMER0_H

#include "macros.h"
#include "port.h"

#define TIMER_INITIAL_VALUE 131

void timer2_init(void);
void stop_timer(void);
void start_timer(void);

#endif