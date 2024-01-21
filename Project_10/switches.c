/*#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

unsigned int okay_to_look_at_switch1 = 1;
unsigned int sw1_position = 0;
unsigned int count_debounce_SW1 = 0;
unsigned int okay_to_look_at_switch2 = 1;
unsigned int count_debounce_SW2 = 0;
unsigned int sw2_position = 0;
extern char event;
int button_count = 0;

//------------------------------------------------------------------------------
void Switch1_Process(void){
//------------------------------------------------------------------------------
// Switch 1 Configurations
//------------------------------------------------------------------------------
if (okay_to_look_at_switch1 && sw1_position){
    if (!(P4IN & SW1)){
      sw1_position = PRESSED;
        okay_to_look_at_switch1 = NOT_OKAY;
        count_debounce_SW1 = DEBOUNCE_RESTART;
        P4IFG |= SW1;
        switch(button_count){
        case 0:
          event = NONE;
          button_count = 0;
          break;
        case 1:
          event = STRAIGHT;
          button_count = 0;
          break;
        case 2:
          event = CIRCLE;
          button_count = 0;
          break;
        case 3:
          event = FIG8;
          button_count = 0;
          break;
        default:
          event = NONE;
          button_count = 0;
          break;
          //event = STRAIGHT;
        }
    }
}
if (count_debounce_SW1 <= DEBOUNCE_TIME){
count_debounce_SW1++;
}else{
okay_to_look_at_switch1 = OKAY;
if (P4IN & SW1){
sw1_position = RELEASED;
}
}
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Switch2_Process(void){
//------------------------------------------------------------------------------
// Switch 2 Configurations
//------------------------------------------------------------------------------
if (okay_to_look_at_switch2 && sw2_position){
if (!(P2IN & SW2)){
/*sw2_position = PRESSED;
okay_to_look_at_switch2 = NOT_OKAY;
count_debounce_SW2 = DEBOUNCE_RESTART;
button_count++;
  P2IFG |= SW2;
}
}
if (count_debounce_SW2 <= DEBOUNCE_TIME){
count_debounce_SW2++;
}else{
okay_to_look_at_switch2 = OKAY;
if (P2IN & SW2){
sw2_position = RELEASED;
}
}
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Switches_Process(void){
//------------------------------------------------------------------------------
// This function calls the individual Switch Functions
//------------------------------------------------------------------------------
Switch1_Process();
Switch2_Process();
}
//------------------------------------------------------------------------------
*/