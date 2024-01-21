//******************************************************************************
//
//  Description: This file contains the iot to fram communication functions and scheduler
//
//  Alex Tayyeb
//  Nov 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************

#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h" //Created 9/9/22

#define DATA_SIZE (50) //size of process buffer 
unsigned char commandArray[DATA_SIZE]; //array to hold commands 
int commandPointer; //pointer to point at current char in commandarray
char instantCommandChar; //char to be read in instant command function
char endCommandChar; //char to decide if command has ended or is a multicommand
char continueFlag; //flag to talk to main when commandarray needs to be cleared and readied for next command
char commandSet; //flag to talk to main to tell commandsReceived to execute
char stopFlag; //flag to prevent magic smoke, forward and reverse at same time

char arriveString [DISPLAY_SIZE] = "ARRIVED   "; //string to output to display
char arriveNum = HEX_ZERO; //initialize the number to display 0





void commandsReceived(void){
  //commandsReceived() is triggered when the IOT process sense a valid command
  //this function checks if the command is spam or not and acts accordingly
  // if it is a good command that passes the password, it will tell the FRAM what to do based on the command
  
  commandSet = FALSE; //reset commandSet flag
  commandPointer = BEGINNING; //point to beginning of command array 
  
  //I just received a command like !2367@F)
  //or !2367F!2367R)
  
  //check if I got a )
  if(commandArray[commandPointer] == ')'){
    clearCommand(); //If end of command clear pointer 
    return;
  }
  
  //check password and increment along array
  
  if(commandArray[commandPointer++] != firstPass){ //check first letter of array 
    clearCommand(); //if wrong password clear 
    return; 
  }
  if(commandArray[commandPointer++] != secondPass){ //check second letter of array 
    clearCommand(); //if wrong password clear
    return; 
  }
  if(commandArray[commandPointer++] != thirdPass){ //check third letter of array 
    clearCommand(); //if wrong password clear 
    return; 
  }
  if(commandArray[commandPointer++] != fourthPass){ //check fourth password of array 
    clearCommand(); //if wrong password clear 
    return; 
  }
  
  //check the character after the passcode 
  //@ means instant command 

  if(commandArray[commandPointer] == '@'){ //if @ sign that means instant char 
    commandPointer++; //increment pointer 
    runCommandInstant(commandArray[commandPointer]); //run the command using the letter after the @
    return;
  }
  
    
  //end of commandReceived function
  
}


void endOfCommand(char endCommandChar){
  //endOfCommand(char) checks the character after the signature command character and sees if the command has ended
  //it is mainly used to see if a multicommand was sent from the IOT
  
  switch(endCommandChar){ //check if multi command or not 
  case PARANTH: //This signifies the end of a command
    //end of command
    clearCommand(); //clears array and sets command flag low
    
    break;
  default:
    //some other letter just clear it
    clearCommand(); //clear command and sets command flag low 
    break;
  }
  continueFlag = RESET_STATE; //rest toggle continue flag
  interrCount = RESET_STATE; //reset the interrCount from the timer 
}


//CLEAR COMMAND FUNCTION----------------------
void clearCommand(void){
  //This function is used to nullify the command array and prime it for a new command 
  for(commandPointer = BEGINNING; commandArray[commandPointer] != NULL; commandPointer++){ //clears array until a null character 
    commandArray[commandPointer] = NULL; //clear array
  }
  
  commandPointer = BEGINNING; //set pointer to beginning 
  commandSet = FALSE; //reset commandSet flag
  return;
}
//---------------------------------------------


//Run instant commands
void runCommandInstant(char instantCommandChar){
  //runCommandInstant(char) reads the character that comes into the function and puts it into a switch case
  //the switch case decides what the motors and display should do based on the character 
  //after the character is executed the continueFlag is toggled and the endOfCommand function is started 
  
  switch(instantCommandChar){ //switch depending on the character brought in 
  case FORWARD: //If the character is F
    if(stopFlag){ //Check if the car is stopped to prevent magic smoke
      stopFlag = LO; //toggle flag off because we are moving now 
    //------ DISPLAY-----
    lcd_BIG_mid(); //change lcd to 3 line 
    strcpy(display_line[1], " FORWARD  "); //Display "FORWARD" on the big line
    strcpy(display_line[2], "CONTINUOUS"); //display "CONTINUOUS" on the bottom line 
    display_changed = TRUE; //update display process 
    //-------------------
    
        //control wheels here
    
      RIGHT_FORWARD_SPEED = MID_2; //change speed of right wheel to go straight 
      LEFT_FORWARD_SPEED = MID; //change speed of left wheel to go straight 
      RIGHT_REVERSE_SPEED = WHEEL_OFF; //turn off reverse 
      LEFT_REVERSE_SPEED = WHEEL_OFF; //turn off reverse 
    
    continueFlag = TRUE; //command is done, flag to clear array in main 
    }

    break;
  case BEGIN: //If the character is an A
    if(stopFlag){ //check if car has stopped 
      stopFlag = LO; //reset stop flag
      lcd_4line(); //change lcd to 4 lines 
      strcpy(display_line[0], "          "); //clear first line 
      strcpy(display_line[1], "  BEGIN   "); // BEGIN on second line 
      strcpy(display_line[2], "          "); //clear third line 
      strcpy(display_line[3], "          "); //clear fourth line 
  
      display_changed = TRUE; //update display process 
      TB2CCTL1 |= CCIE; //initiate 5 second timer to begin main FSM in main.c
      
      continueFlag = TRUE; //flag clear the commandArray 
    }
    break;
  case EXIT_CIRC: // if letter is X
    
    courseEvent = EXIT_CIRCLE; //change current courseEvent in main.c to exit the circle
      
      continueFlag = TRUE; //flag to clear the commandArray 
    break;
    
  case REVERSE: //If letter is B 
    if(stopFlag){ //check if car has stopped 
      stopFlag = LO; //reset stop flag 
    //------ DISPLAY-----
    lcd_BIG_mid(); //3 line LCD  
    strcpy(display_line[1], " REVERSE  "); //big line says REVERSE
    strcpy(display_line[2], "CONTINUOUS"); //bottom line says CONTINUOUS 
    display_changed = TRUE; //update display process
    //-------------------
    
        //control wheels here
    
      RIGHT_FORWARD_SPEED = WHEEL_OFF; //turn off forward wheels 
      LEFT_FORWARD_SPEED = WHEEL_OFF; //turn off forward 
      RIGHT_REVERSE_SPEED = MID; //turn on reverse both wheels 
      LEFT_REVERSE_SPEED = MID;
      
      continueFlag = TRUE; //flag to clear commandArray 
    }
    

    
    break;
  case RIGHT_TURN: // if letter is R 
    if(stopFlag){ //check if car has stopped 
      stopFlag = LO; //reset stop flag 
    //------ DISPLAY-----
    lcd_BIG_mid(); //lcd 3 lines 
    strcpy(display_line[1], " RIGHT T  "); //big line says RIGHT T 
    strcpy(display_line[2], "CONTINUOUS"); //bottom line says CONTINUOUS 
    display_changed = TRUE; //update display process 
    //-------------------
    
        //control wheels here
    RIGHT_FORWARD_SPEED = CRUISE; // only have right forward on at a medium speed
    LEFT_FORWARD_SPEED = WHEEL_OFF; //left off
    RIGHT_REVERSE_SPEED = WHEEL_OFF; //right reverse off 
    LEFT_REVERSE_SPEED = WHEEL_OFF; //left off 
    
    continueFlag = TRUE; //flag to clear commandArray 
    }

    break;
  case LEFT_TURN: //if letter is an L
    if(stopFlag){ //check if car has stopped moving 
      stopFlag = LO; //reset stopFlag 
    //------ DISPLAY-----
    lcd_BIG_mid(); //3 line display 
    strcpy(display_line[1], "  LEFT T  "); //big line says LEFT T 
    strcpy(display_line[2], "CONTINUOUS"); // bottom line says CONTINUOUS 
    display_changed = TRUE; //update display process 
    //-------------------
    
        //control wheels here
    RIGHT_FORWARD_SPEED = WHEEL_OFF; //turn off all directions except left forward 
    LEFT_FORWARD_SPEED = CRUISE; //left forward at a medium speed 
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    
    continueFlag = TRUE; //flag to clear commandArray 
    }
    

    break;
  case STOP: //if Letter is an S
    //------ DISPLAY-----
    lcd_BIG_mid(); //3 line display 
    strcpy(display_line[1], "   STOP   "); //big line says STOP
    strcpy(display_line[2], "          "); // clear second line 
    display_changed = TRUE; //update display process 
    //-------------------
    
    //control wheels here
    stopFlag = HI; // toggle stop flag
    RIGHT_FORWARD_SPEED = WHEEL_OFF; //turn off all wheels 
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    
    continueFlag = TRUE; // flag to clear commandArray 
    
    break;
  default: break;
  }
}
