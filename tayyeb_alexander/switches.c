//******************************************************************************
//
//  Description: This file contains the switches functions
//
//  Alex Tayyeb
//  Sept 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************
#include  "msp430.h"
#include  "functions.h"
#include <string.h>
#include  "macros.h"



char switch1_Pressed = RESET_STATE;
extern char secTimer;
char dirToggle = HI;

//------------------------------------------------------------------------------
void Switches_Process(void){
//------------------------------------------------------------------------------
// This function calls the individual Switch Functions
//------------------------------------------------------------------------------
Switch1_Process();
Switch2_Process();
}
//------------------------------------------------------------------------------

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
      // do what you want with button press
      
      switch1_Pressed++;
      
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
      sw2_position = PRESSED;
      okay_to_look_at_switch2 = NOT_OKAY;
      count_debounce_SW2 = DEBOUNCE_RESTART;
      // do what you want with button press
      
     // dirToggle = dirToggle * -1;
      
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