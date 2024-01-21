//******************************************************************************
//
//  Description: This file contains the straight motion functions
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

char sCase;
extern char shapeEvent;
extern char time_change;
extern unsigned int cycle_time;

extern unsigned int right_motor_count;
extern unsigned int left_motor_count;
extern unsigned int delay_start;
extern char segment_count;


  void startStraight_case(void){
  cycle_time = 0;
  right_motor_count = 0;
  left_motor_count = 0;
  driveForward();
  segment_count = 0;
  sCase = RUN;
}

void runStraight_case(void){
  if(time_change){
    time_change = 0;
    if(segment_count <= TRAVEL_DISTANCE){
      if(right_motor_count++ >= RIGHT_COUNT_TIME){
        P6OUT &= ~R_FORWARD;
      }
      if(left_motor_count++ >= LEFT_COUNT_TIME){
        P6OUT &= ~L_FORWARD;
      }
      if(cycle_time >= WHEEL_COUNT_TIME){
        cycle_time = 0;
        right_motor_count = 0;
        left_motor_count = 0;
        segment_count++;
        driveForward();
      }
    }else{
      sCase = END;
    }
  }
}

void waitStraight_case(void){
  if(time_change){
    time_change = 0;
    if(delay_start++ >= WAITING2START){
      delay_start = 0;
      sCase = START;
    }
  }
}



void endStraight_case(void){
  stop();
  shapeEvent = NONE;
}



void Straight(void) {
  
  switch(sCase){
  case WAIT: // Begin
    waitStraight_case();
    break; //
  case START: // Begin
    startStraight_case();
    break; //
  case RUN: // Run
    runStraight_case();
      break; //
  case END: // End
    endStraight_case();
    break; //
  default: break;
  }
}



