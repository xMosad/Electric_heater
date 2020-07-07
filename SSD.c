#include "SSD.H"

uint8_t tens , ones ;
extern uint8_t ssd_delay ;
extern uint16_t ssd_timer;
extern uint8_t set_temp  ;
SSD_SELECT_t ssd_select = SSD_LEFT ;
SSD_BLINK_t blink = SSD_ON ;

void ssd_init(void){

     // set portd for ssd
     CLEAR_REG(SSD_DIR);
     CLEAR_REG(SSD_PORT);
     
     // set the selections pins
     CLEAR_BIT(SSD_SELECT_DIR  , SSD_PIN_LEFT);
     CLEAR_BIT(SSD_SELECT_PORT , SSD_PIN_LEFT);
     CLEAR_BIT(SSD_SELECT_DIR  , SSD_PIN_RIGHT);
     CLEAR_BIT(SSD_SELECT_PORT , SSD_PIN_RIGHT);
}

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

void ssd_update(uint8_t temp){
    ssd_delay  += TIC_TIME;
    if (ssd_delay == SSD_SWITCH_TIME){
        switch (ssd_select){
           case SSD_LEFT :
                tens = temp / 10 ;
                SET_BIT  (SSD_SELECT_PORT , SSD_PIN_LEFT);
                CLEAR_BIT(SSD_SELECT_PORT , SSD_PIN_RIGHT);
                SSD_PORT = display7s(tens);
                ssd_delay = 0 ;
                ssd_select = SSD_RIGHT ;
                break ;
            case SSD_RIGHT :
                 ones = temp % 10 ;
                 SET_BIT  (SSD_SELECT_PORT , SSD_PIN_RIGHT);
                 CLEAR_BIT(SSD_SELECT_PORT , SSD_PIN_LEFT);
                 SSD_PORT = display7s(ones);
                 ssd_delay = 0 ;
                 ssd_select = SSD_LEFT ;
                 break;
            default : break ;
        }
    }
}

void ssd_turn_off(void){
    CLEAR_REG(SSD_PORT);
    CLEAR_BIT(SSD_SELECT_PORT , SSD_PIN_LEFT);
    CLEAR_BIT(SSD_SELECT_PORT , SSD_PIN_RIGHT);
}

void ssd_blink(uint8_t e_temp){
     ssd_timer  += TIC_TIME;
     switch (blink){
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