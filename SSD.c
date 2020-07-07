#include "SSD.h"

uint8_t tens , ones ; // hold the number to be written to each SSD 
extern uint8_t ssd_delay ;
extern uint16_t ssd_timer;
extern uint8_t set_temp  ;
SSD_SELECT_t ssd_select = SSD_LEFT ; // to switch between the two ssd
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
* Description : write to the SSD will switch between the two SSD every 50 ms
* Parameters  : the value of the temp (uint8_t)
* Return type : void                                                                           
*******************************************************************************/
void ssd_update(uint8_t temp){
    ssd_delay  += TIC_TIME;
    // enter the if every 50 ms
    if (ssd_delay == SSD_SWITCH_TIME){
        //choose to write to the left or right SSD
        switch (ssd_select){
            case SSD_LEFT :
                tens = temp / 10 ;
                SET_BIT  (SSD_SELECT_PORT , SSD_PIN_LEFT);
                CLEAR_BIT(SSD_SELECT_PORT , SSD_PIN_RIGHT);
                SSD_PORT = display7s(tens);
                ssd_delay = 0 ;          // initialize counter
                ssd_select = SSD_RIGHT ; // update the ssd_select
                break ;
            case SSD_RIGHT :
                 ones = temp % 10 ;
                 SET_BIT  (SSD_SELECT_PORT , SSD_PIN_RIGHT);
                 CLEAR_BIT(SSD_SELECT_PORT , SSD_PIN_LEFT);
                 SSD_PORT = display7s(ones);
                 ssd_delay = 0 ;         // initialize counter
                 ssd_select = SSD_LEFT ; // update the ssd_select
                 break;
            default :
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
    ssd_delay = 0 ;         // initialize counter
}

/******************************************************************************
* Description : to blink the SSD every 1 s in the setting mode  
* Parameters  : temp (uint8_t) 
* Return type : void                                                                           
*******************************************************************************/
void ssd_blink(uint8_t e_temp){
     ssd_timer  += TIC_TIME;
     //choose to write to the left or right SSD
     switch (blink){
        // in SSD_ON state will keep updating the SSD 
        case SSD_ON :
            if (ssd_timer < SSD_BLINK_TIME){
                ssd_update(e_temp);
                }
            else{
                blink = SSD_OFF ;
                ssd_timer = 0 ;
                ssd_turn_off();
            }
            break ;
        // in SSD_OFF state will shut down SSD for 1 s    
        case SSD_OFF :
            if (ssd_timer == SSD_BLINK_TIME){
                blink = SSD_ON ;
                ssd_timer = 0 ;
            }
            else {
                 
            }
            break;
        default : break ;
     }
}