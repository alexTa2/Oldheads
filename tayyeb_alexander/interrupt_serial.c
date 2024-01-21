//******************************************************************************
//
//  Description: This file contains the  serial communication interrupts
//
//  Alex Tayyeb
//  Oct 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************

#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h" //Created 9/9/22

volatile unsigned int usb_rx_wr;
unsigned int usb_rx_rd;

volatile unsigned int usb_tx_wr;
unsigned int usb_tx_rd;

volatile unsigned int iot_rx_wr;
unsigned int iot_rx_rd;

volatile unsigned char IOT_Tx_buf[LARGE_RING_SIZE];
volatile unsigned char IOT_Rx[LARGE_RING_SIZE];
volatile char iot_tx;
volatile char usb_tx;
char transmitArray[LARGE_RING_SIZE];
volatile char USB_Rx[LARGE_RING_SIZE] ;
volatile char USB_Tx[LARGE_RING_SIZE] ;
const char pingMessage [7] = "ping!";

#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
//------------------------------------------------------------------------------
// Echo back RXed character, confirm TX buffer is ready first
  //unsigned int temp;
  char iot_receive;
  //unsigned int temp2;
  switch(__even_in_range(UCA0IV,0x08)){
  case 0:                                       // Vector 0 - no interrupt
    break;
    
  case 2:     // Vector 2 - RXIFG
    
    
      iot_receive = UCA0RXBUF;
      IOT_Rx[iot_rx_wr++] = iot_receive;                 // Rx -> IOT_2_PC character array
      if (iot_rx_wr >= (LARGE_RING_SIZE)){
        iot_rx_wr = BEGINNING;                    // Circular buffer back to beginning
      }
      UCA1TXBUF = iot_receive;
    
    // UCA1IE = UCTXIE; //Enable TX interrupt
      
    
  break;
  
  case 4:                                       // Vector 4 - TXIFG

    UCA0TXBUF = IOT_Tx_buf[iot_tx];
    IOT_Tx_buf[iot_tx++] = NULL;
    if(IOT_Tx_buf[iot_tx] == NULL){
      UCA0IE &= ~UCTXIE;
      iot_tx = RESET_STATE;
      
    }

   
    break;
  default: break;
  }
//------------------------------------------------------------------------------
}


//-------------------
//              USB
//-------------------

//------------------------------------------------------------------------------
#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
  unsigned int usb_value;
  //unsigned int temp;
  switch(__even_in_range(UCA1IV,0x08)){
  case 0:                                               // Vector 0 - no interrupt
    break;
  case 2:                                               // Vector 2 - RXIFG
    usb_value = UCA1RXBUF;
    USB_Rx[usb_rx_wr++] = usb_value;                      // RX -> USB_Char_Rx character
    if (usb_rx_wr >= (sizeof(USB_Rx))){
      usb_rx_wr = BEGINNING;                       // Circular buffer back to beginning
    }
    
    UCA0TXBUF = usb_value;
    

    break;
    
  case 4:                                               // Vector 4 – TXIFG
    UCA1TXBUF = USB_Tx[usb_tx];
      USB_Tx[usb_tx++] = 0;
    if(USB_Tx[usb_tx] == 0x00){
      usb_tx = 0;
      UCA1IE &= ~UCTXIE;
      strcpy(display_line[2], "          ");
      display_changed = TRUE;
    }
    
  default: break;
  }
}
//------------------------------------------------------------------------------



