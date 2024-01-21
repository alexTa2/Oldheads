//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
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



// Function Prototypes
// Functions I will use in the main.c file need to be declared
void main(void); //Main Prototype function


//Project 10 Main variables
char courseEvent; //Describes the current stage of the main course
char nextEvent; //Used to define next state which a timer can use to switch states

  
void main(void){
//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
  PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings

  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
  Init_ADC();                          // Initialize ADC
  Init_Serial_UCA1();                  // Initialize Serial Port for USB
  Init_Serial_UCA0();                  //Initialize Serial Port for IOT
  Init_IOT();                          //Initialize IOT commands to initialize IOT

// Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[0], "  TAYYEB  "); //Displays TAYYEB on first line of display
  strcpy(display_line[1], "  NO WIFI "); //Display NO WIFI on second line of display
  strcpy(display_line[2], "          "); //Displays nothing on third line 
  strcpy(display_line[3], "      PJ10"); //Displays PJ10 on 4th, (project 10)

  display_changed = TRUE; // This line identifies the text on the Display is to change.

//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
    
    switch(courseEvent){               //Main state machine to run through the course
    case START: // Start case is setting up the IOT, after IOT is set up it moves to next state
      if(setUpFlag){ //If IOT is ready 
        IOT_Setup(); //set up IOT and prime it for commands 
        if(readyForNext){ //if IOT is ready to receive next init command
          nextCommand(); //Prime next init command 
        }
      }
      nextEvent = IOT_MOVING; //set pointer to next state 
      break;
    case IOT_MOVING: //Case to complete the IOT part of the course, goes to next state off of begin command from IOT
      nextEvent = FIND_WHITE; //set pointer to next state 
      IOT_Process(); //continuously is listening for commands from phone to IOT to fram
    
    if(commandSet){ // IOT process turns on this flag when there is a command
      commandsReceived(); //Function to process said command 
    }
    if(continueFlag){ //If command is registered properly check if there is a multi command
      endOfCommand(commandArray[commandPointer + 1]); //Checks for multiple commands/Clears command flag and nulls array
    }
      
      
      break;
      
    case FIND_WHITE: //After "begin" command from phone, the car begins to look for white 
       P2OUT |= IR_LED; //turn on IR led
      strcpy(display_line[0], " BL START "); //indicate on "BL START" on first line
      strcpy(display_line[1], "          "); //clear second line
      display_changed = TRUE; //update the display process function
      
       P6OUT |= LCD_BACKLITE; //turn LCD back on from timer turning it off
       
       detectWhite(); // function to detect white and move to the next state 
       
      nextEvent = FIND_BLACK; //Prime next state
      break;
    case FIND_BLACK: //After finding white, the car then looks to find black
      
      P6OUT |= LCD_BACKLITE; //turn on the LCD after timer turns it off
      detectBlack(); //function to find the black line, if found moves to next state
      break;
    case LINE_UP: //After black line is found, the EV car must line up with the black line
      strcpy(display_line[0], "  BL TURN "); //Display on first line "BL TURN"
      display_changed = TRUE; //Update display process function 
      lineTurn(); //function that lines up the car to the left 
      P6OUT |= LCD_BACKLITE; //turns LCD back on after timer turns it off 
      nextEvent = FOLLOW_LINE; //primes next state 
      break;
    case FOLLOW_LINE: //After the car is lined up with the line, the PID takes over and follows the line
      strcpy(display_line[0], " BL TRAVEL"); //Display on first line of LCD "BL TRAVEL"
      display_changed = TRUE; //update display process 
      P6OUT |= LCD_BACKLITE; //turn LCD back on after timer turns it off
      
      if(adcReady){ //check if the adc has gone off and finished reading the new values 
        lineFollowerPID(); //if adc is ready then execute the linefollower function 
      }
      nextEvent = FIRST_CIRCLE; //prime next state 
      TB2CCTL2 |= CCIE; //Timer that waits 5 seconds and proceeds to next state 
      break;
    case FIRST_CIRCLE: //After following the line the EV is to pause while doing a circle 
      nextEvent = CONTINUE_CIRCLE; //prime next state 
      strcpy(display_line[0], " BL CIRCLE"); //display one first line "BL CIRCLE"
      RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM OFF
      RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.1 Right Reverse PWM OFF
      LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM OFF
      LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM OFF
      P6OUT &= ~LCD_BACKLITE; //turn off screen
      display_changed = TRUE; //update display function
      TB2CCTL1 |= CCIE; //next event is continue the circle (5 second timer) 
      break;
      
    case CONTINUE_CIRCLE: //After pausing in the circle the EV continues to follow the line until told otherwise
      strcpy(display_line[0], " BL CIRCLE"); //keep displaying "BL Circle" on first line 
      display_changed = TRUE; //update display function 
      
      
      if(adcReady){ //continue following line 
        lineFollowerPID(); //continue following line if adc is ready 
      }
      
      P6OUT |= LCD_BACKLITE; //turn it back on after timer turns LCD off
      
      IOT_Process(); //start reading in commands again from the IOT
      
      if(commandSet){ //if there is a command ready 
        commandsReceived(); //if there is a command ready process it in commandsReceived()
      }
      if(continueFlag){ //if the command is processed and ready to exit
        endOfCommand(commandArray[commandPointer + 1]); //Checks for multiple commands/Clears command flag and nulls array
      }
      
      break;
    case EXIT_CIRCLE: //If the IOT received a command to exit the circle
      TB2CCTL1 = CCIE; //timer for next event
      strcpy(display_line[0], " BL EXIT  "); //display on first line of LCD "BL Exit"
      display_changed = TRUE; //update display prcoess 
      
      ADCCTL0 &= ~ADCENC; // Disable conversions
      ADCCTL0 &= ~ADCSC; // Stop next sample ADC
      ADCIE &= ~ADCIE0; //Disable ADC to save power 
      
      RIGHT_FORWARD_SPEED = VIBE; //Medium speed of wheels 
      LEFT_FORWARD_SPEED = VIBE; //Medium speed of wheels 
      P6OUT |= LCD_BACKLITE; //turn LCD back on after timer turns it off 
      nextEvent = AHHH; //prime next event 
      break;
    case AHHH: //Final event of ECE 306, stop the car and display "BL Stop"
      strcpy(display_line[0], " BL STOP  "); //display "BL Stop" on first line
      display_changed = TRUE; //update display process 
      RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM OFF
      RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.1 Right Reverse PWM OFF
      LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM OFF
      LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM OFF
      
      break;
      
    default:break;
    }
    
    if(pingFlag){ //if the server needs to be pinged
      pingServer(); //ping the server so the IOT can stay connected to network 
    }
    
    //////////////////////////////////////////////
    Display_Process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
    //////////////////////////////////////////////
    
 }
  

//------------------------------------------------------------------------------


}


