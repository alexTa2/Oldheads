//******************************************************************************
//
//  Description: This file contains the triangle motion functions
//
//  Alex Tayyeb
//  Sept 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************
#include  "msp430.h"
#include  "functions.h"
#include <string.h>
#include  "macros.h"

char tCase;
extern char shapeEvent;
extern char time_change;
extern unsigned int cycle_time;

extern unsigned int right_motor_count;
extern unsigned int left_motor_count;
extern unsigned int delay_start;
extern char segment_count;
char triState = RESET_STATE;
char tCount;

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

void triStraight(void){

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
    }
}

void triTurn(void){

    if(segment_count <= 1){
      if(right_motor_count++ >= RIGHT_COUNT_TIME_F8){
        P6OUT &= ~R_FORWARD;
      }
      if(left_motor_count++ >= LEFT_COUNT_TIME_F8){
        P6OUT &= ~L_FORWARD;
      }
      if(cycle_time >= WHEEL_COUNT_TIME){
        cycle_time = 0;
        right_motor_count = 0;
        left_motor_count = 0;
        segment_count++;
        driveForward();
  
      }
    }
}


  
 void startTriangle_case(void){
  cycle_time = 0;
  right_motor_count = 0;
  left_motor_count = 0;
  driveForward();
  segment_count = 0;
  tCase = RUN;
}

void runTriangle_case(void){
    if(time_change){
    time_change = 0;
    
    if(tCount < 6) {
      
    switch(triState){
      
    case T_STRAIGHT: 
     
       if(segment_count == TRAVEL_DISTANCE){
         segment_count = LO;
         tCount++;
         triState = T_TURN;
       }
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
      break;
      
      
    case T_TURN:
      
      if(segment_count == TRAVEL_TURN){
        segment_count = LO;
        triState = T_STRAIGHT;
      }
      if(right_motor_count++ >= RIGHT_COUNT_TIME_T){
        P6OUT &= ~R_FORWARD;
      }
      if(left_motor_count++ >= LEFT_COUNT_TIME_T){
        P6OUT &= ~L_FORWARD;
      }
      if(cycle_time >= WHEEL_COUNT_TIME_T){
        cycle_time = 0;
        right_motor_count = 0;
        left_motor_count = 0;
        segment_count++;
        driveForward();
  
      }

      break;
    default: break;
    }
    }else{
      tCase = END;
  }
    }
}


void waitTriangle_case(void){
  
  if(time_change){
    time_change = 0;
    if(delay_start++ >= WAITING2START){
      delay_start = 0;
      tCase = START;
    }
  }
}

void endTriangle_case(void){
  stop();
  shapeEvent = NONE;
}



void Triangle(void){
    switch(tCase){
  case WAIT: // Begin
    waitTriangle_case();
    break; //
  case START: // Begin
    startTriangle_case();
    break; //
  case RUN: // Run
    runTriangle_case();
      break; //
  case END: // End
    endTriangle_case();
    break; //
  default: break;
  }

}


