//******************************************************************************
//
//  Description: This file contains the serial communication functions/interrupts
//
//  Alex Tayyeb
//  Sept 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************

#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h" //Created 9/9/22

char displayIndex;
char dispString[DISPLAY_SIZE];
char transmitFlag;
char transmitToPC;
char message;
char serialFlag;
char step;

// Globals IOT
// Serial




void serialComms(void){
  switch(step){
    
  case WAITING:
    strcpy(display_line[0], " WAITING  ");
    display_changed = TRUE;
    //UCA0IE |= UCRXIE; // Enable RX interrupt
    serialFlag = HI;
    break;
    
  case RECEIVED:
    strcpy(display_line[0], " RECEIVED ");
    strcpy(display_line[1], "          ");
    display_changed = TRUE;
    serialFlag = LO;
    break;
    
  case TRANSMIT:
    strcpy(display_line[0], " TRANSMIT ");
    display_changed = TRUE;
    strcpy(display_line[1], dispString);
    strcpy(display_line[3], "          ");
    display_changed = TRUE;
   
    if(transmitFlag){
    USCI_A0_transmit();
    }
    
    
    break;
    
  default: break;
  }
  
}





void serialDisplay(void){
  
  char temp_wr = iot_rx_wr;
  char currChar = NULL;
  
  if(iot_rx_rd != temp_wr){
    currChar = IOT_Rx[iot_rx_rd];
    iot_rx_rd++;
  }
  
  if(iot_rx_rd >= sizeof(IOT_Rx)){
    iot_rx_rd = BEGINNING;
  }
  
  if(currChar != NULL){
    process_buffer[displayIndex] = currChar;
    displayIndex++;
    memcpy(dispString, process_buffer, DISPLAY_SIZE);
    strcpy(display_line[3], dispString);
    display_changed = 1;
    message = TRUE;
  }
  
  if(displayIndex >= DISPLAY_SIZE){
    displayIndex = RESET_STATE;
    message = FALSE;
    step = RECEIVED;
  }
}



