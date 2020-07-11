/*
 * I2C_drive.h
 *
 * Created: 11/27/2019 12:21:31 AM
 *  Author: mosad
  * Description :This is a driver for the I2C module includes five functions
    the driver is to use the PIC as a master when using the driver you must use
    the I2C_intialize() function before anything
 */ 
#ifndef I2C_DRIVE_H_
#define I2C_DRIVE_H_

#include <xc.h>
#include "Main.h"
#include "macros.h"
#include "I2C_drive.h"

void I2C1_Init(uint32_t freq);
void I2C_Wait(void);
void I2C1_Start(void);
void I2C1_Stop(void);
void I2C1_Wr(uint8_t _data);
uint8_t I2C1_Rd(void);

#endif /* I2C_DRIVE_H_ */