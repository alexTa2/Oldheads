//------------------------------------------------------------------------------
//
//  Description: This file contains timer service routines.
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

//------------------------------------------------------------------------------
// Timer B0 initialization sets up both B0_0, B0_1-B0_2 and overflow

void Init_Timers(void){
  //This function initializes all of the timers for use 
  Init_Timer_B0(); //Initiaize B0
  Init_Timer_B1(); //Initialize B1
  Init_Timer_B2(); //Initialize B2
  Init_Timer_B3(); //Initialize B3
  
}

//Updating display 200 ms
void Init_Timer_B0(void) {
  //This function is the initialization function of timer B0
  //It is initialized to ACLK with the CCR0 and CCR2 registers turned on at startup
  TB0CTL = TBSSEL__ACLK; // ACLK source 32768Hz
  TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
  TB0CTL |= MC__CONTINOUS; // Continuous up
  TB0CTL |= ID__1; // Divide clock by 8
  
  TB0EX0 = TBIDEX__1; // Divide clock by an additional 8
  
  TB0CCR0 = TB0CCR0_INTERVAL; // CCR0 = 200 ms
  TB0CCTL0 |= CCIE; // CCR0 enable interrupt //Display update
  
   TB0CCR1 = TB0CCR1_INTERVAL; // CCR1 == .5 seconds
  // TB0CCTL1 |= CCIE; // CCR1 enable interrupt //IOT Ping 
   
   TB0CCR2 = TB0CCR2_INTERVAL; // CCR2 == .1 seconds
   TB0CCTL2 |= CCIE; // CCR2 enable interrupt //IOT enable delay
   
  TB0CTL &= ~TBIE; // Disable Overflow Interrupt
  TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
 
}

//LCD and Debouncing switches
void Init_Timer_B1(void) {
   //This function is the initialization function of timer B1 at SMCLK 
  
  TB1CTL = TBSSEL__SMCLK; // SMCLK source 32768Hz
  TB1CTL |= TBCLR; // Resets TB0R, clock divider, count direction
  TB1CTL |= MC__CONTINOUS; // Continuous up
  TB1CTL |= ID__8; // Divide clock by 8
  
  TB1EX0 = TBIDEX__8; // Divide clock by an additional 8
  
  TB1CCR0 = TB1CCR0_INTERVAL; // CCR0
  //TB1CCTL0 |= CCIE; // CCR0 enable interrupt
  
  TB1CCR1 = TB1CCR1_INTERVAL; // CCR1
  //TB1CCTL1 |= CCIE; // CCR1 enable interrupt
  
  TB1CCR2 = TB1CCR2_INTERVAL; // CCR2
  //TB1CCTL2 |= CCIE; // CCR2 enable interrupt
  
  TB1CTL &= ~TBIE; // Disable Overflow Interrupt
  TB1CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}
void Init_Timer_B2(void) {
  //This function intializes timer B2 at ACLK
  //and turns on CCR0 to enable ADC capture at startup 
  TB2CTL = TBSSEL__ACLK; // ACLK source 32768Hz
  TB2CTL |= TBCLR; // Resets TB0R, clock divider, count direction
  TB2CTL |= MC__CONTINOUS; // Continuous up
  TB2CTL |= ID__1; // Divide clock by 1
  
  TB2EX0 = TBIDEX__1; // Divide clock by an additional 1
  
  TB2CCR0 = TB2CCR0_INTERVAL; // CCR0
  TB2CCTL0 |= CCIE; // CCR0 enable interrupt ((ADC Sample captures))
  
   TB2CCR1 = TB2CCR1_INTERVAL; // CCR1
  // TB2CCTL1 |= CCIE; // CCR1 enable interrupt
  
   TB2CCR2 = TB2CCR2_INTERVAL; // CCR2
  // TB2CCTL2 |= CCIE; // CCR2 enable interrupt
   
  
  TB2CTL &= ~TBIE; // Disable Overflow Interrupt
  TB2CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}

void Init_Timer_B3(void) {
  //Timer B3 is used as a way to implement PWM on the motors 
  
  //------------------------------------------------------------------------------
  // SMCLK source, up count mode, PWM Right Side
  // TB3.1 P6.0 R_FORWARD
  // TB3.2 P6.1 R_REVERSE
  // TB3.3 P6.2 L_FORWARD
  // TB3.4 P6.3 L_REVERSE
  //------------------------------------------------------------------------------
  TB3CTL = TBSSEL__SMCLK; // SMCLK
  TB3CTL |= MC__UP; // Up Mode
  TB3CTL |= TBCLR; // Clear TAR
  
  TB3CCR0 = WHEEL_PERIOD; // PWM Period
  
  TB3CCTL1 = OUTMOD_7; // CCR1 reset/set
  RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM duty cycle
  
  TB3CCTL2 = OUTMOD_7; // CCR2 reset/set
  RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.1 Right Reverse PWM duty cycle
  
  TB3CCTL3 = OUTMOD_7; // CCR3 reset/set
  LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
  
  TB3CCTL4 = OUTMOD_7; // CCR4 reset/set
  LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM duty cycle
  //------------------------------------------------------------------------------
}
