//******************************************************************************
//
//  Description: This file contains the black line following state machine
//
//  Alex Tayyeb
//  Oct 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h" //Created 9/9/22

char bangEvent;
char bangTimer;


void bangBang(void){
  
 
  switch(bangEvent){
  case IDLE_BANG:
    idleEvent = BLACK_LINE_FOLLOW;
    TB1CCTL0 |= CCIE;
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM OFF
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.1 Right Reverse PWM OFF
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM OFF
    //Timer Interrupt waits 1 second and then send to next state
    
    break;
  case GO:
    //Initiate 2 revolution TIMER
    TB2CCTL2 |= CCIE;
    RIGHT_FORWARD_SPEED = SLOW;
    LEFT_FORWARD_SPEED = SLOW ; 
    
    if((ADC_Left_Detect >= BLACK_LINE_FOUND) || (ADC_Left_Detect >= BLACK_LINE_FOUND)) {
      RIGHT_FORWARD_SPEED = WHEEL_OFF;
      LEFT_FORWARD_SPEED = WHEEL_OFF; 
      bangEvent = BANG;
    }
    break;
  case BANG:
    switch(sensorDetect){
    case LEFT:

      RIGHT_FORWARD_SPEED = SLOW;
      LEFT_FORWARD_SPEED = DEAD;
      
      if((ADC_Right_Detect < BLACK_LINE_FOUND) && (ADC_Left_Detect < BLACK_LINE_FOUND) ){
        RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM OFF
        RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.1 Right Reverse PWM OFF
        LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM OFF
        LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM OFF
        
        bangEvent = GO;
      }
        break;
      case RIGHT:
        
        LEFT_FORWARD_SPEED = SLOW;
        RIGHT_FORWARD_SPEED = DEAD;
        
        if((ADC_Right_Detect < BLACK_LINE_FOUND) && (ADC_Left_Detect < BLACK_LINE_FOUND)){
          RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM OFF
          RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.1 Right Reverse PWM OFF
          LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM OFF
          LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM OFF
          
          bangEvent = GO;
        }
      break;
    default: break;
    }
    break;
  case BREAK:
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM OFF
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.1 Right Reverse PWM OFF
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM OFF
    //Turn 90 degrees
    LEFT_FORWARD_SPEED = SLOW;
    bangTimer = RIGHT_ANGLE;
    TB0CCTL2 |= CCIE;

    break;
  case MIDDLE:
    bangTimer = RADIUS;
    TB0CCTL2 |= CCIE; //Turn off timer
    LEFT_FORWARD_SPEED = SLOW;
    RIGHT_FORWARD_SPEED = SLOW;
    break;
  case BANG_END:
    
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM OFF
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.1 Right Reverse PWM OFF
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM OFF
    
    strcpy(display_line[0], "DONE.     ");
    display_changed = TRUE;
    break;
    
    
  default: break;
  
  }
}