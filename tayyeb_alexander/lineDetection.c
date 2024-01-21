//******************************************************************************
//
//  Description: This file contains the color detect functions
//
//  Alex Tayyeb
//  Oct 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h" //Created 9/9/22

//Macros for the color detect values 
#define BLACK_LINE_FOUND (560)
#define WHITE_LINE_FOUND (200)


char blackLineEvents; //states in switch statement for blackLine
char whiteLineEvents; //states in the switch statement for whiteLine
char whiteFound; //whiteFound flag

char turnEvent; //event in turning the car to line up

//State parameters for the turn event switch statement 
#define ADJUST_LINE (0)
#define FINISH_TURN (1)




void detectWhite(void){
  //detectWhite() is the first function of the main switch FSM
  //It detects white and then moves the next state to detect black

  switch(whiteLineEvents){ 
    
  case IDLE: //Initializes wheels 
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM OFF
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.1 Right Reverse PWM OFF
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM OFF
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM OFF
    whiteLineEvents = SEARCH_WHITE; //moves to next state 
    
    break;
  case SEARCH_WHITE: //arcing motion to the right to fight the white part of the course 
    
    RIGHT_FORWARD_SPEED = WHITE_RIGHT; //speed to find white line right turn
    LEFT_FORWARD_SPEED = WHITE_LEFT; //speed to find white line left 
    
    if( (ADC_Left_Detect < WHITE_LINE_FOUND) && (ADC_Right_Detect < WHITE_LINE_FOUND) ){ //check if both sensors have found white 
      whiteFound = HI; //when found white toggle hi
      strcpy(display_line[1], "   WHITE  "); //display on first line "WHITE"
      display_changed = 1; //update display process 
      whiteLineEvents = FOUND; //move to next state in white line events 
    }
    
    break;
  case FOUND:
    courseEvent = FIND_BLACK; //move main course event to find black
    break;
  default:
    break;
  }
}

 void detectBlack(void){
   //This function is the next function to execute after the white line is found
   //the function goes in a straight line until a sensor picks up black
   //then it moves to the next state 

  switch(blackLineEvents){ //case statement to find the black line 

  case SEARCH: //searching for the black line, just moving forward 
    strcpy(display_line[0], " INTERCEPT"); //first line of display says "STRAIGHT"
    strcpy(display_line[1], "          "); //clear second line 
    display_changed = 1; //update display process 
    
    RIGHT_FORWARD_SPEED = CRUISE_BLACK; //move both wheels straight 
    LEFT_FORWARD_SPEED = CRUISE_BLACK; 
    
    if(ADC_Left_Detect >= BLACK_LINE_FOUND){ //if one of the sensors find black 
      RIGHT_FORWARD_SPEED = WHEEL_OFF; //stop wheels 
      LEFT_FORWARD_SPEED = WHEEL_OFF; 
      blackLineEvents = FINISH; //go to next state in blackLineEvents
    }
    if(ADC_Right_Detect >= BLACK_LINE_FOUND){ //if one of the sensors finds black
      RIGHT_FORWARD_SPEED = WHEEL_OFF; //stop wheels
      LEFT_FORWARD_SPEED = WHEEL_OFF; 
      blackLineEvents = FINISH; //go to next state in blackLineEvents
    }
    
    break;
    
  case FINISH:// end of detectBlack function
    nextEvent = LINE_UP; //prime next courseEvent in main.c
    TB2CCTL1 |= CCIE; //next event timer  (timer goes to next state)
    
    
    break;
  default:
    break;
  }
}


void lineTurn(void){
  //This is a simple function that turns the car until it is lined up with the blackline 
  switch(turnEvent){
      case ADJUST_LINE: //first statement of the FSM
    
      //turn to follow line on left
      LEFT_FORWARD_SPEED = SLOW_ADJUST; //turn left 
      RIGHT_REVERSE_SPEED = DEAD_ADJUST; //turn left 
      
      
      if((ADC_Right_Detect >= BLACK_LINE_FOUND) && (ADC_Left_Detect >= BLACK_LINE_FOUND)){ //if both sensors detect black STOP
        RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM OFF
        RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.1 Right Reverse PWM OFF
        LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM OFF
        LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM OFF
        //Lined up
        turnEvent = FINISH_TURN; //last state of lineTurn
      }
      
    break;
  case FINISH_TURN:
    nextEvent = FOLLOW_LINE; //prime next courseEvent in main.c
    TB2CCTL1 |= CCIE; //next event timer (timer moves courseEvent = nextEvent)
    
    break;
  default:
    break;
  }
  
}
