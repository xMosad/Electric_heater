#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include "port.h"
#include "macros.h"
#include "types.h"
#include "adc_drive.h"

#define TEMP_PERIOD 100
#define TEMP_SAMPLES 10
void temp_sensor_init(void);
uint8_t average (void);
uint8_t temp_sensor_read (void);


#endif