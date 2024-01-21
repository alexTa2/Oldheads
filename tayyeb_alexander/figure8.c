//******************************************************************************
//
//  Description: This file contains the figure 8 functions
//
//  Alex Tayyeb
//  Sept 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************
#include  "msp430.h"
#include  "functions.h"
#include <string.h>
#include  "macros.h"

////Functions from wheels////
void driveForward(void);
void driveReverse(void);
void stopLeft(void);
void stopRight(void);
void driveLeft(void);
void driveRight(void);
void stop(void);
void lcdOFF(void);
void lcdON(void);
void lcdBlink(void);
///////////////////////////
char fCase;
extern char shapeEvent;
extern char time_change;
extern unsigned int cycle_time;

extern unsigned int right_motor_count;
extern unsigned int left_motor_count;
extern unsigned int delay_start;
extern char segment_count;
char circleCount = RESET_STATE;
char switchMod;

 void startFigure8_case(void){
  cycle_time = 0;
  right_motor_count = 0;
  left_motor_count = 0;
  driveForward();
  segment_count = 0;
  fCase = RUN;
}

void runFigure8_case(void){
    if(time_change){
    time_change = 0;
    
    if(circleCount < 4){
      
      
      switchMod = circleCount%2;
    
      if(segment_count == TRAVEL_F8){
      segment_count = LO;
      circleCount++;
    }
    switch(switchMod){
    case LO: 
     
      if(right_motor_count++ >= RIGHT_COUNT_TIME_F8){
        P6OUT &= ~R_FORWARD;
      }
      if(left_motor_count++ >= LEFT_COUNT_TIME_F8){
        P6OUT &= ~L_FORWARD;
      }
      if(cycle_time >= WHEEL_COUNT_TIME_F8){
        cycle_time = 0;
        right_motor_count = 0; //higher value -> more stoppage -> slower
        left_motor_count = 0;
        segment_count++;
        driveForward();
      }
      break;
    case HI:
      
           if(right_motor_count++ >= RIGHT_COUNT_TIME_F8){
        P6OUT &= ~L_FORWARD; //swapped RIGHT
      }
      if(left_motor_count++ >= LEFT_COUNT_TIME_F8){
        P6OUT &= ~R_FORWARD; //swapped LEFT
      }
      if(cycle_time >= WHEEL_COUNT_TIME_F8){
        cycle_time = 0;
        right_motor_count = 0; //higher value -> more stoppage -> slower
        left_motor_count = 0;
        segment_count++;
        driveForward();
      }
      break;
    default: break;
    }
      
    }else{
      fCase = END;
    }
  }
 
}



void waitFigure8_case(void){
  if(time_change){
    time_change = 0;
    if(delay_start++ >= WAITING2START){
      delay_start = 0;
      fCase = START;
    }
  }
}


void endFigure8_case(void){
  stop();
  shapeEvent = NONE;
}


void Figure8(void){
    switch(fCase){
  case WAIT: // Begin
    waitFigure8_case();
    break; //
  case START: // Begin
    startFigure8_case();
    break; //
  case RUN: // Run
    runFigure8_case();
      break; //
  case END: // End
    endFigure8_case();
    break; //
  default: break;
  }

}
