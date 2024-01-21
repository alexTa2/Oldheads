//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//
//  Alex Tayyeb
//  Nov 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h" //Created 9/9/22


unsigned char usbProcess[LARGE_RING_SIZE];
unsigned char iotProcess[LARGE_RING_SIZE];

char usb_rx_rd_2ndLast;
char usb_rx_rd_last;
char iot_rx_rd_2ndLast;
char iot_rx_rd_last;
char iotCommandCounter;
char validCommand;



void serialManager(void){
  unsigned int temp_usb_rx_wr = usb_rx_wr;                                      //saves where the usb write pointer is
  unsigned int temp_iot_rx_wr = iot_rx_wr;                                      //saves where the iot write pointer is
  
  if(usb_rx_rd != temp_usb_rx_wr){                                              //checks if a new char is in
    char currentChar = USB_Rx[usb_rx_rd];                                       //saves the new char
    USB_Rx[usb_rx_rd] = NULL;                                                   //nulls the char in the usb recieve buffer
    usbProcess[usb_rx_rd] = currentChar;                                        //saves the new char in a usb process
    usb_rx_rd_2ndLast = usb_rx_rd_last;
    usb_rx_rd_last = usb_rx_rd;    
    usb_rx_rd++;                                                                //increments to the next read point
    
    if(usb_rx_rd >= (LARGE_RING_SIZE)){                                         //checks if the read pointer is at the end
      usb_rx_rd = BEGINNING;                                                    //resets the read pointer
    }
    
    if((usbProcess[usb_rx_rd_2ndLast] == CR) && (usbProcess[usb_rx_rd_last] == LN)){ //checks if the message is done
      for(char i = 0; i < LARGE_RING_SIZE; i++){
        if(usbProcess[i] != NULL){                                              //checks if the char isn't a NULL
          IOT_Tx_buf[i] = usbProcess[i];                                            //copies the process for transmitting
          usbProcess[i] = NULL;                                                 //clears usb process buffer for more commands
        }
      }
      if(IOT_Tx_buf[BEGINNING] != '!'){
        UCA0IE |= UCTXIE;                                                         //turns on the IOT tx interrupt for transmitting
      }else{
        for(char i = 0; IOT_Tx_buf[i] != NULL ; i++){
          IOT_Rx[i] = IOT_Tx_buf[i];
          IOT_Tx_buf[iot_rx_wr] = NULL;
          iot_rx_wr++;
        }
      }
      
    }
  }

//  if(iot_rx_rd != temp_iot_rx_wr){                                              //checks if a new char is in
//    char currentChar = IOT_Rx[iot_rx_rd];                                       //saves the new char
//    IOT_Rx[iot_rx_rd] = NULL;                                                   //nulls the char in the iot recieve buffer
//    iotProcess[iot_rx_rd] = currentChar;                                        //saves the new char in a iot process
//    iot_rx_rd_2ndLast = iot_rx_rd_last;
//    iot_rx_rd_last = iot_rx_rd;
//    iot_rx_rd++;                                                                //increments to the next read point
//    if(iot_rx_rd >= (LARGE_RING_SIZE)){                                         //checks if the read pointer is at the end
//      iot_rx_rd = BEGINNING;                                                    //resets the read pointer
//    }
//   
//    if((iotProcess[iot_rx_rd_2ndLast] == CR) && (iotProcess[iot_rx_rd_last] == LN)){ //checks if the message is done
//      for(char i = 0; i < LARGE_RING_SIZE; i++){
//        if(iotProcess[i] != NULL){
//          USB_Tx[i] = iotProcess[i];                                              //copies the process for transmitting
//        }
//      }
//      iotCommandCounter++;
//      UCA1IE |= UCTXIE;                                                         //turns on the usb tx interrupt for transmitting
//    }
//  }
//  
}