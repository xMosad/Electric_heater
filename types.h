#ifndef TYPES_H
#define TYPES_H

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