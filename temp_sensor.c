#include "temp_sensor.h"

static uint16_t temp_hold_array[10] ;
extern uint8_t temp_timer ;
static uint8_t i = 0 ;
uint8_t temp = 0 ;

void temp_sensor_init(void){
     ADC_Init();
     SET_BIT(TEMP_SENSOR_DIR , TEMP_SENSOR_PIN); //input
}

uint8_t average (void){
     uint8_t i = 0 ;
     uint16_t sum = 0 ;
     for (i = 0 ; i < TEMP_SAMPLES ; i++){
         sum = sum + temp_hold_array[i] ;
     }
     return (sum / TEMP_SAMPLES) ;
}

uint8_t temp_sensor_read (void) {
    temp_timer += TIC_TIME;
    if( (temp_timer == TEMP_PERIOD) ){
        temp_hold_array[i] = (ADC_Read(TEMP_SENSOR_PIN) * 4.89 )  ;
        temp_hold_array[i] = temp_hold_array[i] / 10.0 ;
        i++ ;
        temp_timer = 0 ;
        if (i == TEMP_SAMPLES){i = 0 ;}
        temp = average();
        return temp ;
    }
    else {
        return temp ;
    }
}