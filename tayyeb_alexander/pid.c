//******************************************************************************
//
//  Description: This file contains the PID control for the line following portion of the project
//
//  Alex Tayyeb
//  Dec 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h" //Created 9/9/22

int Error, LastError; //Variables for holding error and last error
int proportional, integral, derivative; //Variables for holding the PID variables
double Kp = 14.2; //Gain of the proportional
double Ki = 0;  //Gain of the integral
double Kd = 18.4; //Gain of the derivative
double pCorrection, iCorrection, dCorrection; // Variables to hold sum after gain
double Correction; //Variable to some pCorrect, iCorrection, and dCorrection
int leftSens, rightSens; //Variable to hold sensor values from ADC grab
char direction; //state variable for case statement

double minusCorrect, plusCorrect; //Correction factor to be applied to opposite wheels

//State parameters for direction switch statement
#define LEFT_CORRECT (0)
#define RIGHT_CORRECT (1)

//Offset due to sensor differential 
#define offset (3)

void lineFollowerPID(void){
  //This function takes in the values of both sensors and calculates it's error.
  //Using the error the function implements PID control on both wheels
  //This function provides smooth and accurate line following based on the input from the ADC
  
  adcReady = LO; //Toggle flag so the ADC can set it high again 
  leftSens = ADC_Left_Detect + offset; //Left sensor value
  rightSens = ADC_Right_Detect;        //Right sensor value
  
    Error = leftSens - rightSens; //Error calculation between sensors 
    
    if(Error < 0){                      //Check which direction the wheels need to correct
      direction = RIGHT_CORRECT;        //If Error is negative the left sensor sees white
      Error = Error * -1; //absolute value //Correct the error value for future use
    }else{ //If error is positve the right sensor is seeing white and needs to correct left
      direction = LEFT_CORRECT;
    }
    
    proportional = Error; //Proportional portion of the PID
    integral = integral + Error; //Integral Portion of PID
    derivative = Error - LastError; //Derivative portion of PID
    
    //Tune values
    pCorrection = proportional * Kp; //Apply gain to proportion
    iCorrection = integral * Ki; //Apply gain to Integral
    dCorrection = derivative * Kd; //Apply gain to derivative
    
    Correction = pCorrection + iCorrection + dCorrection; //Sum up all the correctors
    
    LastError = Error; //Update error value
    
    minusCorrect = PID - Correction; //Create speed values for motors
    plusCorrect = PID + Correction; 
    
    if(minusCorrect > WHEEL_PERIOD){ //Check if greater than wheel period to avoid glitches
      minusCorrect = WHEEL_PERIOD; //If creater than wheel period, make wheel max speed
    }
    if(minusCorrect < LO){ // If negative 
      minusCorrect = LO; //make correction 0
    }
    if(plusCorrect > WHEEL_PERIOD){ //if greater than max speed
      plusCorrect = WHEEL_PERIOD; //make it max speed
    }
    if(plusCorrect < LO){ //if negative 
      plusCorrect = LO; //make correction 0
    }
    
    switch(direction){ //depending on which direction the car needs to correct
    case LEFT_CORRECT: //Left correction case 
      RIGHT_FORWARD_SPEED = (int)minusCorrect; //Slow down right 
      LEFT_FORWARD_SPEED = (int)plusCorrect;  //Speed up left 
      break;
    case RIGHT_CORRECT: //Right correction case 
      RIGHT_FORWARD_SPEED = (int)plusCorrect; //Speed up right 
      LEFT_FORWARD_SPEED = (int)minusCorrect;  //Slow down left 
      break;
    default: break; //default just break out 
    
    }
    
    //Check left wheel
    if(LEFT_FORWARD_SPEED > WHEEL_PERIOD){ //Check if speed is greater than max allowed speed 
      if(LEFT_FORWARD_SPEED > 65000){ //this will be negative since this is unsigned 
         LEFT_FORWARD_SPEED = LO; //Make it 0 if negative
      }
      LEFT_FORWARD_SPEED = WHEEL_PERIOD; //otherwise make it max speed 
    }

    //Now check right wheel 
    if(RIGHT_FORWARD_SPEED > WHEEL_PERIOD){ //Check if speed is greater than max allowed speed 
      if(RIGHT_FORWARD_SPEED > 65000){ //this will be negative since this is unsigned 
         RIGHT_FORWARD_SPEED = LO; //Make it 0 if negative
      }
      RIGHT_FORWARD_SPEED = WHEEL_PERIOD; //Otherwise make it max speed 
    }
}