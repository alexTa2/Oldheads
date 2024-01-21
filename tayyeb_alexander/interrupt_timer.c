//------------------------------------------------------------------------------
//
//  Description: This file contains interrupt service routines for timers.
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


char interrCount; //counter for amounts of time 


//different counters for amounts of time 
int pauseCount; 
char commandCount;
char pingCount;
char circleCount;


char pingFlag; //flag for the ping
char DYNAMIC_TIME = 50; // amount of time to get into the circle on the course 



////////////TIMER 0/////////////////
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
  //------------------------------------------------------------------------------
  // TimerB0 0 Interrupt handler
  //------------------------------------------------------------------------------
  
  TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0
  update_display = TRUE;     //LCD UPDATES TIMER B0 0

  
}
#pragma vector= TIMER_B0_1_OVFL_VECTOR
__interrupt void TIMER0_B1_ISR(void){
  //----------------------------------------------------------------------------
  // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
  //----------------------------------------------------------------------------
  
  //----------------------------------------------------------------------------
  switch(__even_in_range(TB0IV,14)){
  case 0: break; // No interrupt
  
  case 2: // CCR1 Used for ping
    if(pingCount++ == TEN_SECOND_COUNT){
      pingCount = RESET_STATE;
      pingFlag = TRUE; //set ping flag high after 10 seconds 
    }
    TB0CCR1 += TB0CCR1_INTERVAL;
    
    
    break;
    
  case 4: // CCR2 Used for for the delay of the IOT enable
     TB0CCR2 += TB0CCR2_INTERVAL; // Add Offset to TBCCR0
     P3OUT |= IOT_EN_CPU; //Set as Output Active HI for 50ms reset IOT
     setUpFlag = TRUE; //start initiating IOT
     readyForNext = TRUE; //ready for next command
     TB0CCTL2 &= ~CCIE; //Disable itself
    break;
    

    
  default: break;
  }
  //----------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
//////////////////TIMER 1/////////////////////////////////////////
//---------------------------------------------------------------------------
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void){
  //------------------------------------------------------------------------------
  // TimerB1 0 Interrupt handler
  //------------------------------------------------------------------------------
  
  //------------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  
}
#pragma vector= TIMER_B1_1_OVFL_VECTOR
__interrupt void TIMER1_B1_ISR(void){
  //----------------------------------------------------------------------------
  // TimerB1 1-2, Overflow Interrupt Vector (TBIV) handler
  //----------------------------------------------------------------------------
  
  //----------------------------------------------------------------------------
  switch(__even_in_range(TB1IV,14)){
  case 0: break; // No interrupt
  
  case 2: // CCR1 Used for SW1 Debounce
    
    if(interrCount > LO){ //if there is still debounce time 
      interrCount--; //decrement counter 
    }
    if(interrCount == RESET_STATE){
      TB1CCTL1 &= ~CCIE;// 1. Disable Timer B1 CCR1
      P4IFG &= ~SW1;// 2. Clear SW1 Interrupt Flag
      P4IE |= SW1; // SW1 interrupt Enabled   3. Enable SW1 Interrupt
      interrCount = FALSE; //reset count 
    }
    
    TB1CCR1 += TB1CCR1_INTERVAL; // Add Offset to TBCCR1
    break;
    
  case 4: // CCR2 Used for SW2 Debounce
    if(interrCount > LO){ //if there is still debounce time 
      interrCount--; //decrement count 
    }
    if(interrCount == RESET_STATE){
      TB1CCTL2 &= ~CCIE; // 1. Disable Timer B0 CCR2
      P2IFG &= ~SW2; //  2. Clear SW2 Interrupt Flag
      P2IE |= SW2; //  3. Enable SW2 Interrupt
      interrCount = FALSE; // reset count 
    }
    
    TB1CCR1 += TB1CCR1_INTERVAL; // Add Offset to TBCCR1
    break;
    
    
  default: break;
  }
  //----------------------------------------------------------------------------
}

//---------------------------------------------------------------------------
//////////////////TIMER 2/////////////////////////////////////////
//---------------------------------------------------------------------------
#pragma vector = TIMER2_B0_VECTOR
__interrupt void Timer2_B0_ISR(void){
  //------------------------------------------------------------------------------
  // TimerB2 0 Interrupt handler
  //------------------------------------------------------------------------------
  
  //Project 6 ADC Conversions (5 Times a second)
  TB2CCR0 += TB2CCR0_INTERVAL; // Add Offset to TBCCR0
  ADCCTL0 |= ADCENC; // Enable ADC
  ADCCTL0 |= ADCSC; // Start next sample
  
}
#pragma vector= TIMER_B2_1_OVFL_VECTOR
__interrupt void TIMER2_B1_ISR(void){
  //----------------------------------------------------------------------------
  // TimerB2 1-2, Overflow Interrupt Vector (TBIV) handler
  //----------------------------------------------------------------------------
  
  //----------------------------------------------------------------------------
  switch(__even_in_range(TB2IV,14)){
  case 0: break; // No interrupt
  
  case 2: // CCR1 Used for 5 second delay of main events
    
    if(pauseCount == FIVE_SECOND_COUNT){ //wait till it hits a second
    pauseCount = RESET_STATE; //reset the count
    courseEvent = nextEvent; //go to next event in the main.c FSM
    TB2CCTL1 &= ~CCIE; //Turn off timer
  }
  pauseCount++;//increment counter 
  RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM OFF
  RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.1 Right Reverse PWM OFF
  LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM OFF
  LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM OFF
  P6OUT &= ~LCD_BACKLITE; //turn off LCD
  TB2CCR1 += TB2CCR1_INTERVAL; //increment time 

    
    break;
   
  case 4: // CCR2 Used for timing when in circle
    if(circleCount == DYNAMIC_TIME){ //wait till it hits a second
      circleCount = RESET_STATE; //reset the count
      courseEvent = FIRST_CIRCLE;
      TB2CCTL2 &= ~CCIE; //Turn off timer
    }
    circleCount++;
    TB2CCR2 += TB2CCR2_INTERVAL;//increment time 
    
    

    break;
    
//  case 14: // overflow available for greater than 1 second timer
//    break;
    
  default: break;
  }
  //----------------------------------------------------------------------------
}

