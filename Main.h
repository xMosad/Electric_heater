#ifndef MAIN_H
#define MAIN_H

// tasks period
#define GET_TEMP_TASK_PERIOD      (100) 
#define SSD_UPDATE_TASK_PERIOD    (50)
#define LED_TASK_PERIOD           (1000)
#define TEMP_SET_TASK_PERIOD      (100)
#define SWITCH_SCAN_TASK_PERIOD   (20)
#define SSD_BLINK_TASK_PERIOD     (1000)

// system types 
typedef unsigned char uint8_t;
typedef signed char sint8_t;
typedef unsigned int uint16_t;
typedef signed int sint16_t;
typedef unsigned long uint32_t;
typedef signed long sint32_t;

// system mode
typedef enum {
    NORMAL_MODE,
    SETTING_MODE
}MODE_STATE_t;

//power mode
typedef enum {
    ON_STATE,
    OFF_STATE
}POWER_MODES_t;

#endif

