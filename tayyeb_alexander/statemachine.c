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





char ncstates = RESET_STATE; 
char lastState = RESET_STATE;
char round = RESET_STATE;
char secTimer; 

void stateMachine(void); 

void stateMachine(void){
  
  switch(ncstates){ 
  case IDLE: 
    
    strcpy(display_line[1], "   IDLE   ");
    display_changed = TRUE;
    
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~L_FORWARD;
    
    P6OUT &= ~R_REVERSE;
    P6OUT &= ~L_REVERSE;
    
    break;
    //////////////////////////////////////
    ////////////////////////////////////
  case PAUSE:
    strcpy(display_line[1], "PAUSE 1SEC");
    display_changed = TRUE;
    
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~L_FORWARD;
    
    P6OUT &= ~R_REVERSE;
    P6OUT &= ~L_REVERSE;
    
    if(oneSecond && lastState == FORWARD){
    fifthSecTimer = RESET_STATE;
    ncstates = REVERSE;
  }else if(oneSecond && lastState == REVERSE){
    fifthSecTimer = RESET_STATE;
    ncstates = FORWARD;
  }else if(round == 2 && lastState == FORWARD && oneSecond){
    fifthSecTimer = RESET_STATE;
    ncstates = CLOCKWISE;
  }else if(lastState == CLOCKWISE && secTimer == 2){
    fifthSecTimer = RESET_STATE;
    ncstates = C_CLOCKWISE;
    }else if(lastState == C_CLOCKWISE && secTimer == 2){
    fifthSecTimer = RESET_STATE;
  strcpy(display_line[0], "PROJECT 5 ");
  strcpy(display_line[1], "          ");
  strcpy(display_line[2], "   DONE   ");
  strcpy(display_line[3], "  TAYYEB  ");
  display_changed = TRUE;
  break;
  }
  
  break;
  //////////////////////////////////////
  //////////////////////////////////////
case FORWARD: 

  strcpy(display_line[1], "  FORWARD  ");
  display_changed = TRUE;
  
  P6OUT &= ~L_REVERSE;
  P6OUT &= ~R_REVERSE;
  
  P6OUT |= L_FORWARD;
  P6OUT |= R_FORWARD;
  
  if(oneSecond){
    lastState = FORWARD;
    round++;
    ncstates = PAUSE;
    fifthSecTimer = RESET_STATE;
  }
  
  break;
case REVERSE:

  strcpy(display_line[1], " REVERSE  ");
  display_changed = TRUE;
  
  P6OUT &= ~L_FORWARD;
  P6OUT &= ~R_FORWARD;
  
  P6OUT |= R_REVERSE;
  P6OUT |= L_REVERSE;
  if(oneSecond) secTimer++;
  if(secTimer == 2){
    lastState = REVERSE;
    secTimer = RESET_STATE;
    ncstates = PAUSE;
  }
  
    
  break;
  
case CLOCKWISE:
      strcpy(display_line[1], "    CW    ");
  display_changed = TRUE;
  
  P6OUT &= ~L_FORWARD;
  P6OUT &= ~R_FORWARD;
  
  P6OUT |= R_REVERSE;
  P6OUT &= ~L_REVERSE;
  
  if(oneSecond) secTimer++;
    if(secTimer == 3){
    lastState = CLOCKWISE;
    secTimer = RESET_STATE;
    ncstates = PAUSE;
  }
  
    break;
case C_CLOCKWISE:

  strcpy(display_line[1], "   CCW    ");
  display_changed = TRUE;
  
  P6OUT &= ~L_FORWARD;
  P6OUT &= ~R_FORWARD;
  
  P6OUT |= L_REVERSE;
  P6OUT &= ~R_REVERSE;
  
  if(oneSecond) secTimer++;
  if(secTimer == 3){
    lastState = C_CLOCKWISE;
    secTimer = RESET_STATE;
    ncstates = PAUSE;
  }
    
  break;
default: break;
}
}

  
  