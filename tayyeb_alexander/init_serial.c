//******************************************************************************
//
//  Description: This file contains the initialization of serial communication functions/interrupts
//
//  Alex Tayyeb
//  Sept 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************

#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h" //Created 9/9/22

void Init_Serial_UCA0(void){ //was (char speed)
//------------------------------------------------------------------------------
//                                          TX error (%) RX error (%)
// BRCLK   Baudrate UCOS16  UCBRx   UCFx   UCSx    neg    pos   neg   pos
// 8000000    4800     1     104      2    0xD6   -0.08  0.04  -0.10  0.14
// 8000000    9600     1      52      1    0x49   -0.08  0.04  -0.10  0.14
// 8000000   19200     1      26      0    0xB6   -0.08  0.16  -0.28  0.20
// 8000000   57600     1       8     10    0xF7   -0.32  0.32  -1.00  0.36
// 8000000  115200     1       4      5    0x55   -0.80  0.64  -1.12  1.76
// 8000000  460800     0      17      0    0x4A   -2.72  2.56  -3.76  7.28
//------------------------------------------------------------------------------
    
// Configure eUSCI_A0 for UART mode
  UCA0CTLW0 = 0;
  UCA0CTLW0 |= UCSWRST;                        // Put eUSCI in reset
  UCA0CTLW0 |= UCSSEL__SMCLK;                   // Set SMCLK as fBRCLK
  UCA0CTLW0 &= ~UCMSB;                          // MSB, LSB select
  UCA0CTLW0 &= ~UCSPB;                          // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
  UCA0CTLW0 &= ~UCPEN;                          // No Parity
  UCA0CTLW0 &= ~UCSYNC;
  UCA0CTLW0 &= ~UC7BIT;
  UCA0CTLW0 |= UCMODE_0;
  
  //  BRCLK   Baudrate  UCOS16  UCBRx   UCFx   UCSx    neg   pos   neg   pos
  //  8000000   115200     1      4      5     0x55  -0.80  0.64  -1.12  1.76
  //  UCA?MCTLW = UCSx + UCFx + UCOS16
  UCA0BRW = 4;                                 // 115,200 baud
  UCA0MCTLW = 0x5551;                          // 115,200 baud
  
  UCA0CTLW0 &= ~UCSWRST;                       // release from reset
  UCA0TXBUF = 0x00;                             // Prime the Pump
  UCA0IE |= UCRXIE;                             // Enable RX interrupt
  UCA0IE &= ~UCTXIE;                             //disable TX interrupt
//------------------------------------------------------------------------------
}








void Init_Serial_UCA1(void){ //was (char speed)
//------------------------------------------------------------------------------
//                                          TX error (%) RX error (%)
// BRCLK   Baudrate UCOS16  UCBRx   UCFx   UCSx    neg    pos   neg   pos
// 8000000    4800     1     104      2    0xD6   -0.08  0.04  -0.10  0.14
// 8000000    9600     1      52      1    0x49   -0.08  0.04  -0.10  0.14
// 8000000   19200     1      26      0    0xB6   -0.08  0.16  -0.28  0.20
// 8000000   57600     1       8     10    0xF7   -0.32  0.32  -1.00  0.36
// 8000000  115200     1       4      5    0x55   -0.80  0.64  -1.12  1.76
// 8000000  460800     0      17      0    0x4A   -2.72  2.56  -3.76  7.28
//------------------------------------------------------------------------------
  int i;
  
  //Project 9 Global
  

  usb_rx_wr = BEGINNING; //reset pointer 
  usb_rx_rd = BEGINNING; //reset pointer 
  
  for(i=0; i<LARGE_RING_SIZE; i++){             // May not use this
    USB_Tx[i] = NULL;                      // USB Tx Buffer        //was 0x00
    USB_Rx[i] = NULL;                      // USB Rx Buffer        //was 0x00
    IOT_Tx_buf[i] = NULL;
    IOT_Rx[i] = NULL;
    
  }
  
  for(i = 0; i < DATA_SIZE; i++){ //nullify all of IOT_Data Process buffers 
    IOT_Data[0][i] = NULL;
    IOT_Data[1][i] = NULL;
    IOT_Data[2][i] = NULL;
    IOT_Data[3][i] = NULL;
    IOT_Data[4][i] = NULL;
    commandArray[i] = NULL;
  }
  
  usb_tx_wr = BEGINNING; //reset pointer 
  usb_tx_rd = BEGINNING; //reset pointer 
  
// Configure eUSCI_A1 for UART mode
  UCA1CTLW0 = 0;
  UCA1CTLW0 |= UCSWRST ;                        // Put eUSCI in reset
  UCA1CTLW0 |= UCSSEL__SMCLK;                   // Set SMCLK as fBRCLK
  UCA1CTLW0 &= ~UCMSB;                          // MSB, LSB select
  UCA1CTLW0 &= ~UCSPB;                          // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
  UCA1CTLW0 &= ~UCPEN;                          // No Parity
  UCA1CTLW0 &= ~UCSYNC;
  UCA1CTLW0 &= ~UC7BIT;
  UCA1CTLW0 |= UCMODE_0;
  
  //  BRCLK   Baudrate  UCOS16  UCBRx   UCFx   UCSx    neg   pos   neg   pos
  //  8000000   115200     1      4      5     0x55  -0.80  0.64  -1.12  1.76
  //  UCA?MCTLW = UCSx + UCFx + UCOS16
  UCA1BRW = 4;                                 // 115,200 baud
  UCA1MCTLW = 0x5551;                          // 115,200 baud
  
  UCA1CTLW0 &= ~UCSWRST;                       // release from reset
  //UCA1TXBUF = 0x00;                             // Prime the Pump
  UCA1IE |= UCRXIE;                             // Enable RX interrupt
  UCA1IE &= ~UCTXIE;                             //disable TX interrupt
  
//------------------------------------------------------------------------------
}