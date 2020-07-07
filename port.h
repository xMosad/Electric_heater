#ifndef PORT_H
#define PORT_H

/*
 * LM5              - > A2
 * HEATER           - > C5
 * COOLER           - > C2
 * LED              - > B4
 * UP BUTTON        - > B2
 * DOWN BUTTPN      - > B1
 * OFF BUTTON       - > B0
 * SSD LEFT SELECT  - > A4
 * SSD RIGHT SELECT - > A5
 * SSD PORT         - > D
 */

#include <xc.h>
// temperature sensor
#define TEMP_SENSOR_DIR  TRISA
#define TEMP_SENSOR_PORT PORTA
#define TEMP_SENSOR_PIN  2

// heater
#define HEATER_DIR  TRISC
#define HEATER_PORT PORTC
#define HEATER_PIN  5

// cooler
#define COOLER_DIR  TRISC
#define COOLER_PORT PORTC
#define COOLER_PIN  2

// seven segemnt display
#define SSD_DIR          TRISD
#define SSD_PORT         PORTD
#define SSD_SELECT_DIR   TRISA
#define SSD_SELECT_PORT  PORTA
#define SSD_PIN_LEFT     4
#define SSD_PIN_RIGHT    5

//led
#define LED_DIR  TRISB
#define LED_PORT PORTB
#define LED_PIN  4

// UP BUUTON
#define UP_BUTTON_DIR  TRISB
#define UP_BUTTON_PORT PORTB
#define UP_BUTTON_PIN  2

// down BUUTON
#define DOWN_BUTTON_DIR  TRISB
#define DOWN_BUTTON_PORT PORTB
#define DOWN_BUTTON_PIN  1

// on/off BUUTON
#define POWER_BUTTON_DIR TRISB
#define POWER_BUTTON_PORT PORTB
#define POWER_BUTTON_PIN 0

#define TIC_TIME 1

#endif