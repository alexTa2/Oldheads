//******************************************************************************
//
//  Description: This file contains the wheels related functions.
//
//  Alex Tayyeb
//  Sept 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************
#include  "msp430.h"
#include  "functions.h"
#include <string.h>
#include  "macros.h"


extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int Time_Sequence;
extern char display_line[4][11];
extern char *display[4];


extern unsigned int myTime;
extern unsigned int blinkTime;
extern unsigned int oldTime_Sequence; 
extern char move;

unsigned int states = RESET_STATE;
unsigned int blinkState = RESET_STATE;
unsigned int counter = RESET_STATE;
unsigned int blinks = RESET_STATE;
unsigned int state_count = RESET_STATE;
extern char time_change;

void driveForward(void) {
  
  P6OUT &= ~L_REVERSE;
  P6OUT &= ~R_REVERSE;
  
  P6OUT |= L_FORWARD;
  P6OUT |= R_FORWARD;
  
}

void driveLeft(void) { 
    
    P6OUT &= ~L_REVERSE;
    
    P6OUT |= L_FORWARD;
    strcpy(display_line[0], "    LF    ");
    display_changed = TRUE;
}

void leftReverse(void) { 
    
    P6OUT |= L_REVERSE;
    
    P6OUT &= ~L_FORWARD;
    strcpy(display_line[0], "    LR    ");
    display_changed = TRUE;
}

void rightReverse(void) { 
  
    P6OUT |= R_REVERSE;
    
    P6OUT &= ~R_FORWARD;
        strcpy(display_line[0], "    RR    ");
    display_changed = TRUE;
   
}

void stopLeft(void) {
 
  P6OUT &= ~L_FORWARD;
  P6OUT &= ~L_REVERSE;
      strcpy(display_line[0], "LEFT STOP ");
    display_changed = TRUE;
  
}

void stopRight(void) {
 
  P6OUT &= ~R_FORWARD;
  P6OUT &= ~R_REVERSE;
        strcpy(display_line[0], "RIGHT STOP");
    display_changed = TRUE;
}


void driveRight(void) { 
  
    P6OUT &= ~R_REVERSE;
    
    P6OUT |= R_FORWARD;
          strcpy(display_line[0], "   RF   ");
    display_changed = TRUE;
   
}


void driveReverse(void) {
  
  P6OUT &= ~L_FORWARD;
  P6OUT &= ~R_FORWARD;
  
  P6OUT |= R_REVERSE;
  P6OUT |= L_REVERSE;
}

void stop(void){
  
  P6OUT &= ~R_FORWARD;
  P6OUT &= ~L_FORWARD;
  
  P6OUT &= ~R_REVERSE;
  P6OUT &= ~L_REVERSE;
}

void lcdOFF(void){ 
  
  P6OUT &= ~LCD_BACKLITE; //Turn lcd backlite off
  
}

void lcdON(void) { 
  
  P6OUT |= LCD_BACKLITE; // Turn lcd on
}

void lcdBlink(void) {
  switch(blinkState){
  case LCD_ON:                        //
    
    P6OUT |= LCD_BACKLITE; // Turn lcd on
    if(time_change){
      time_change = 0;
      if(counter++ > WAIT_TIME) {
        counter = 0;
        blinks++;
        blinkState = LCD_OFF;
      }
    }
    
    
    break;
  case LCD_OFF:                        //
    
    if(blinks == 4){
      states = STOP_EVENT; 
      break;
    }
    
    else{ 
      P6OUT &= ~LCD_BACKLITE; //Turn lcd backlite off
      if(time_change){
        time_change = 0;
        if(counter++ > WAIT_SMALL) {
          counter = 0;
          blinkState = LCD_ON;
        }
      }
    }
    
    
    break;
    
  default: break;
  }
  
  
}




void movementFive(void){
  
  
  switch(states) {
    
  case RESET_STATE:              //200 milliseconds
    //
    if(time_change){
      time_change = 0;
      if(counter++ > WAIT_TIME) {
        counter = 0;
        driveForward();                //200 ms after switch press: GO
        states = WAIT_STATE;
      }
    }
    break;
  
  case WAIT_STATE: 
      if(time_change){
      time_change = 0;
      if(counter++ > TIME_1000){ 
        counter = 0;
        state_count++;
        if(state_count == 1) states = TIME_1000;
        else states = TIME_2000;
      }
    }
    
    break;
    
  case TIME_1000:              //1000 milliseconds
    //
    if(time_change){
      time_change = 0;
      if(counter++ > WAIT_TIME) {
        counter = 0;
        lcdOFF();                //Turn lcd off after a second to conserve pwr
        states = WAIT_STATE;
      }
    }
    break;
    
  case TIME_2000:                         // 2 seconds
        lcdBlink();
        break;
        
  case STOP_EVENT:
    
    stop();   //Stop Wheels
    move = RESET_STATE;
    states = RESET_STATE;
    break;
    
  default: break;
  }
}

