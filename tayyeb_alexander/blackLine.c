//******************************************************************************
//
//  Description: This file contains the black line detect functions
//
//  Alex Tayyeb
//  Oct 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h" //Created 9/9/22




char blackLineEvents;



void detectionStates(void){
 
  switch(blackLineEvents){ 
    
  case IDLE: 
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM OFF
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.1 Right Reverse PWM OFF
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM OFF
    //Timer Interrupt waits 1 second and then send to next state
    
    break;
  case SEARCH:
    
    RIGHT_FORWARD_SPEED = CRUISE;
    LEFT_FORWARD_SPEED = CRUISE; 
    
    if(ADC_Left_Detect >= BLACK_LINE_FOUND){
      RIGHT_FORWARD_SPEED = WHEEL_OFF;
      LEFT_FORWARD_SPEED = WHEEL_OFF; 
      leftFound = HI;
      blackLineEvents = FOUND;
    }
    if(ADC_Right_Detect >= BLACK_LINE_FOUND){
      RIGHT_FORWARD_SPEED = WHEEL_OFF;
      LEFT_FORWARD_SPEED = WHEEL_OFF; 
      rightFound = HI;
      blackLineEvents = FOUND;
    }
    
    break;
  case FOUND:
    P6OUT |= GRN_LED;
    //Sets timer for 2 seconds
    
    TB2CCTL1 |= CCIE; // CCR1 enable interrupt (2 second timer B2 CCR1)
    
    //Goes to next state at the end of timer
    
    break;
    
  case ADJUST:
    
    P6OUT &= ~GRN_LED;
    if(leftFound){
      sensorDetect = LEFT;
      
      LEFT_FORWARD_SPEED = WHEEL_OFF;
      RIGHT_FORWARD_SPEED = DEAD;
      LEFT_REVERSE_SPEED = SLOW; 
      
      
      if(ADC_Right_Detect >= BLACK_LINE_FOUND){
        RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM OFF
        RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.1 Right Reverse PWM OFF
        LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM OFF
        LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM OFF
        //Lined up
        blackLineEvents = FINISH;

      }
    }else if(rightFound){
      sensorDetect = RIGHT;
      
 
      RIGHT_FORWARD_SPEED = WHEEL_OFF;
      RIGHT_REVERSE_SPEED = SLOW;
      LEFT_FORWARD_SPEED = DEAD; 

        if(ADC_Left_Detect >= BLACK_LINE_FOUND){
          RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM OFF
          RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.1 Right Reverse PWM OFF
          LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM OFF
          LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM OFF
          //Lined up
          blackLineEvents = FINISH;
        }
    }
    break;
  case FINISH:
    
    bangBang();
    
    if(lineUp == LO){
      bangEvent = IDLE_BANG;
      lineUp++;
    }
    
    break;
  default:
    break;
  }
}