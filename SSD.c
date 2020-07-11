#include "SSD.h"

// system variables
extern uint8_t set_temp  ;
extern uint8_t measured_temp ; // store the measured temperature
extern MODE_STATE_t mode  ; // variable will hold the system state
// private variabes
uint8_t tens , ones ; // hold the number to be written to each SSD 
SSD_SELECT_t ssd_select = SSD_LEFT ; // to switch between the two ssd
uint8_t temp ;
// global variables
SSD_BLINK_t blink = SSD_ON ; // to store the ssd state on or off


/******************************************************************************
* Description : Initialize the SSD 
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void ssd_init(void){
     // set portd as output for ssd and turn it off
     CLEAR_REG(SSD_DIR);
     CLEAR_REG(SSD_PORT);
     // set the selections pins as output and clear it 
     CLEAR_BIT(SSD_SELECT_DIR  , SSD_PIN_LEFT);
     CLEAR_BIT(SSD_SELECT_PORT , SSD_PIN_LEFT);
     CLEAR_BIT(SSD_SELECT_DIR  , SSD_PIN_RIGHT);
     CLEAR_BIT(SSD_SELECT_PORT , SSD_PIN_RIGHT);
}

/******************************************************************************
* Description : to get the value to be written to the SSD  
* Parameters  : number (0 - > 15) ( uint8_t ) 
* Return type : uint8_t                                                                           
*******************************************************************************/
uint8_t display7s(uint8_t v)
{
  switch(v)
  {
    case 0:
      return 0x3F;
      break;
    case 1:
      return 0x06;
      break;
    case 2:
      return 0x5B;
      break;
    case 3:
      return 0x4F;
      break;
    case 4:
      return 0x66;
      break;
    case 5:
      return 0x6D;
      break;
    case 6:
      return 0x7D;
      break;
    case 7:
      return 0x07;
      break;
    case 8:
      return 0x7F;
      break;
    case 9:
      return 0x6F;
      break;
    case 10:
      return 0x77;
      break;
    case 11:
      return 0x7c;
      break;
    case 12:
      return 0x58;
      break;
    case 13:
      return 0x5E;
      break;
    case 14:
      return 0x79;
      break;
    case 15:
      return 0x71;
      break;
    default:
      return 0;
      break;
  }

}

/******************************************************************************
* Description : a task to write to the SSD will switch between the two SSD 
* Parameters  : the value of the temp (uint8_t)
* Return type : void                                                                           
*******************************************************************************/
void ssd_update(void){
    if (blink == SSD_ON){
        switch (mode){
            case NORMAL_MODE : 
                temp = measured_temp ;
                break ;
            case SETTING_MODE : 
                temp = set_temp;
                break ;
        }
        //choose to write to the left or right SSD
        switch (ssd_select){
            case SSD_LEFT :
                tens = temp / 10 ;
                SET_BIT  (SSD_SELECT_PORT , SSD_PIN_LEFT);
                CLEAR_BIT(SSD_SELECT_PORT , SSD_PIN_RIGHT);
                SSD_PORT = display7s(tens);
                ssd_select = SSD_RIGHT ; // update the ssd_select
                break ;
            case SSD_RIGHT :
                 ones = temp % 10 ;
                 SET_BIT  (SSD_SELECT_PORT , SSD_PIN_RIGHT);
                 CLEAR_BIT(SSD_SELECT_PORT , SSD_PIN_LEFT);
                 SSD_PORT = display7s(ones);                
                 ssd_select = SSD_LEFT ; // update the ssd_select
                 break;
            default :
                /* shouldn't be here*/
                break ;
        }
    }
}

/******************************************************************************
* Description : turn off the SSD 
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void ssd_turn_off(void){
    CLEAR_REG(SSD_PORT);
    CLEAR_BIT(SSD_SELECT_PORT , SSD_PIN_LEFT);
    CLEAR_BIT(SSD_SELECT_PORT , SSD_PIN_RIGHT);
    
}

/******************************************************************************
* Description : a task to blink the SSD every 1 s in the setting mode  
* Parameters  : temp (uint8_t) 
* Return type : void                                                                           
*******************************************************************************/
void ssd_blink(void){
    if (mode == SETTING_MODE){ 
        //choose to write to the left or right SSD
        switch (blink){
           // in SSD_ON state will keep updating the SSD 
           case SSD_ON :              
               blink = SSD_OFF ;
               ssd_turn_off();
               break ;

           case SSD_OFF :           
               blink = SSD_ON ;
               break;
           default :
               /* shouldn't be here*/
               break ;
        }
    }
}