//------------------------------------------------------------------------------
//
//  Description: This file contains interrupt service routines for switches.
//
//
//  Alex Tayyeb
//  Sept 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h" //Created 9/9/22



#pragma vector=PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
  // Switch 2 
  //This function is triggered when switch 2 is pressed
  
  if (P2IFG & SW2) {
    //======DEBOUNCE ==================================================//
    interrCount = DEBOUNCE_1_SECOND;
    P2IFG &= ~SW2;  // IFG SW2 cleared (Cleared switch 2 flag)
    P2IE &= ~SW2;    //Disabled switch 2
    TB1CCTL2 &= ~CCIFG;   //Clear any current time interrupt
    TB1CCR2 = TB0R + TB1CCR2_INTERVAL;// Add Offset to TBCCR2 (a second)
    TB1CCTL2 |= CCIE; // Enable B1 CCR2
   //====================================================================//
  
  }
}

//------------------------------------------------------------------------------
// Port 4 interrupt. For switches, they are disabled for the duration
// of the debounce timer. Flag is set that user space can check.

#pragma vector=PORT4_VECTOR
__interrupt void switch1_interrupt(void) {
  // Switch 1
  //This function is triggered when switch 1 is pressed 
  if (P4IFG & SW1) {
    //======DEBOUNCE ==================================================//
    interrCount = DEBOUNCE_1_SECOND;
    P4IFG &= ~SW1;  // IFG SW1 cleared (Cleared switch 1 flag)
    P4IE &= ~SW1;    //Disabled switch 1
    TB1CCTL1 &= ~CCIFG; //Clear any current time interrupt
    TB1CCR1 = TB0R + TB1CCR1_INTERVAL;// Add Offset to TBCCR1 (a second)
    TB1CCTL1 |= CCIE; // Enable B1 CCR1 
    //====================================================================//
    

  
}

//------------------------------------------------------------------------------

}



