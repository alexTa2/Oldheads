//******************************************************************************
//
//  Description: This file contains the serial transmit functions
//
//  Alex Tayyeb
//  Oct 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h" //Created 9/9/22



void USCI_A1_transmit(void){ // Transmit Function for USCI_A1
// Contents must be in process_buffer 
// End of Transmission is identified by NULL character in process_buffer
// process_buffer includes Carriage Return and Line Feed
iot_tx = 0; // Set Array index to first location
//UCA1TXBUF = 0x00; // Prime the Pump
UCA1IE |= UCTXIE; // Enable TX interrupt
}

void USCI_A0_transmit(void){ // Transmit Function for USCI_A1
// Contents must be in process_buffer 
// End of Transmission is identified by NULL character in process_buffer
// process_buffer includes Carriage Return and Line Feed
usb_tx = 0; // Set Array index to first location
UCA0IE |= UCTXIE; // Enable TX interrupt
}

  
      