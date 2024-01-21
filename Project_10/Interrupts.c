#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"
unsigned int count_debounce_SW1;
unsigned int count_debounce_SW2;
unsigned int sw1_position;
unsigned int sw2_position;
unsigned int okay_to_look_at_switch2;
unsigned int okay_to_look_at_switch1;

#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
// Switch 1
  if (P4IFG & SW1) {
    P4IFG &= ~SW1; // IFG SW1 cleared
  // Set a variable to identify the switch has been pressed.
    sw1_position = PRESSED;
  // Set a variable to identify the switch is being debounced.
    okay_to_look_at_switch1 = NOT_OKAY;
  // Reset the count required of the debounce.
    count_debounce_SW1 = DEBOUNCE_RESTART;
  // Disable the Switch Interrupt.
    P4IE &= ~SW1;
  // Clear any current timer interrupt.
    P3OUT ^= LCD_BACKLITE;
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~L_FORWARD;
    TB0CCTL1 |= CCIE;
    TB0CCTL0 &= ~CCIE;
    TB0CCTL0 &= ~CCIFG;  
    TB0CCR1 = TB0R + TB0CCR1_INTERVAL;
  }
  // Use a Timer Value to control the debounce
}
#pragma vector=PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
// Switch 2
  if (P2IFG & SW2) {
    P2IFG &= ~SW2; // IFG SW2 cleared
  // Set a variable to identify the switch has been pressed.
    sw2_position = PRESSED;
  // Set a variable to identify the switch is being debounced.
    okay_to_look_at_switch2 = NOT_OKAY;
  // Reset the count required of the debounce.
    count_debounce_SW2 = DEBOUNCE_RESTART;
  // Disable the Switch Interrupt.
    P2IE &= ~SW1;
  // Clear any current timer interrupt.
    TB0CCTL2 |= CCIE;
    TB0CCTL0 &= ~CCIE;
    TB0CCTL0 &= ~CCIFG;
    TB0CCR2 = TB0R + TB0CCR2_INTERVAL;
    P3OUT ^= LCD_BACKLITE;
    P6OUT ^= GRN_LED;
  }
}