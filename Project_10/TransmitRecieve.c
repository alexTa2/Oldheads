#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

extern volatile unsigned int fully_recieved;
extern volatile unsigned int usb_rx_ring_wr;
extern volatile unsigned int usb_rx_ring_rd;
extern volatile unsigned int usb_tx_ring_wr;
extern volatile unsigned int usb_tx_ring_rd;
extern volatile unsigned int UCA0_index;
extern volatile char USB_Char_Rx[SMALL_RING_SIZE];
extern volatile char USB_Char_Tx[SMALL_RING_SIZE];
extern volatile char test_command[SMALL_RING_SIZE];
extern char process_buffer[25]; // Size for appropriate Command Length
extern unsigned int pb_index;
extern volatile unsigned char IOT_2_PC[SMALL_RING_SIZE];
extern volatile unsigned int iot_rx_wr;
extern volatile unsigned char PC_2_IOT[32];
extern volatile char ringBuffa0[LARGE_RING_SIZE];
extern volatile unsigned int commandRecieved;
extern unsigned int IotInitSM;
extern volatile unsigned int IOTSMTmr;
#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
unsigned int temp;
switch(__even_in_range(UCA0IV,0x08)){
case 0: // Vector 0 - no interrupt
break;
case 2: // Vector 2 - RXIFG
temp = usb_rx_ring_wr;
ringBuffa0[temp] = UCA0RXBUF; // Rx -> IOT_2_PC character array
IOT_2_PC[temp] = ringBuffa0[temp];
if (ringBuffa0[usb_rx_ring_wr++] == '\n'){
  usb_rx_ring_wr = BEGINNING; // Circular buffer back to beginning
  UCA1IE |= UCTXIE; // Enable Tx interrupt
  P3OUT ^= IOT_LINK_CPU;
}
//UCA1TXBUF = UCA0RXBUF;
break;
case 4: // Vector 4 – TXIFG
  UCA0TXBUF = PC_2_IOT[usb_tx_ring_rd]; // Transmit Current Indexed value
  if(PC_2_IOT[usb_tx_ring_rd++] == '\n'){ // Is the next pb_index location NULL - End of Command
      UCA0IE &= ~UCTXIE; // Disable TX interrupt
      UCA0IE |= UCRXIE;
      usb_tx_ring_rd = BEGINNING;
  } 
break;
default: break;
}
}

void out_character(char character){//------------------------------------------------------------------------------
// The while loop will stall as long as the Flag is not set [port is busy]
while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
UCA0TXBUF = character;
//------------------------------------------------------------------------------
}

void Init_Serial_UCA0(void){
int i;
for(i=0; i<SMALL_RING_SIZE; i++){
USB_Char_Rx[i] = 0x00; // USB Rx Buffer
}
usb_rx_ring_wr = BEGINNING;
usb_rx_ring_rd = BEGINNING;
for(i=0; i<LARGE_RING_SIZE; i++){ // May not use this
USB_Char_Tx[i] = 0x00; // USB Tx Buffer
}
usb_tx_ring_wr = BEGINNING;
usb_tx_ring_rd = BEGINNING;
// Configure UART 0
UCA0CTLW0 = 0; // Use word register
UCA0CTLW0 |= UCSWRST; // Set Software reset enable
UCA0CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
// ***** Continuation from previous slide
// 9,600 Baud Rate
// 1. Calculate N = fBRCLK / Baudrate
// N = SMCLK / 9,600 => 8,000,000 / 9,600 = 833.3333333
// if N > 16 continue with step 3, otherwise with step 2
// 2. OS16 = 0, UCBRx = INT(N)
// continue with step 4
// 3. OS16 = 1,
// UCx = INT(N/16),
// = INT(N/16) = 833.333/16 => 52
// UCFx = INT([(N/16) – INT(N/16)] × 16) = ([833.333/16-INT(833.333/16)]*16) = (52.08333333-52)*16 = 0.083*16 = 1
// 4. UCSx is found by looking up the fractional part of N (= N-INT(N)) in table Table 18-4
// Decimal of SMCLK / 8,000,000 / 9,600 = 833.3333333 => 0.333 yields 0x49 [Table]
// 5. If OS16 = 0 was chosen, a detailed error calculation is recommended to be performed
// TX error (%) RX error (%)
// BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
// 8000000 9600 1 52 1 0x49 -0.08 0.04 -0.10 0.14
UCA0BRW = 4; // 9,600 Baud
// UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
// UCA0MCTLW = 0x49 concatenate 1 concatenate 1;
UCA0MCTLW = 0x5551 ;
UCA0CTLW0 &= ~ UCSWRST; // Set Software reset enable
UCA0IE |= UCRXIE; // Enable RX interrupt
UCA0TXBUF = 0x00; // Prime the Pump
}

void USCI_A0_transmit(void){ // Transmit Function for USCI_A0
UCA0IE |= UCTXIE; // Enable TX interrupt
UCA1IE |= UCTXIE; // Enable TX interrupt
}

void Init_IOT(void){
  switch(IotInitSM){
case 0:
  char * sendingMessage = "AT+CIPMUX=1\r\n";
  for(int i = 0; i < sizeof(PC_2_IOT); i++){
    PC_2_IOT[i] = sendingMessage[i];
  }
  if(IOTSMTmr >= 400){
    UCA0IE |= UCTXIE;
    IotInitSM++;
    IOTSMTmr = 0;
  }
  break;
case 1:
  sendingMessage = "AT+CIPSERVER=1,42069\r\n";
  for(int i = 0; i < sizeof(PC_2_IOT); i++){
    PC_2_IOT[i] = sendingMessage[i];
  }
   if(IOTSMTmr >= 400){
    UCA0IE |= UCTXIE;
    IotInitSM++;
    IOTSMTmr = 0;
  }
  break;
  case 2:
    sendingMessage = "AT+CIFSR\r\n";
  for(int i = 0; i < sizeof(PC_2_IOT); i++){
    PC_2_IOT[i] = sendingMessage[i];
  }
  if(IOTSMTmr >= 400){
  UCA0IE |= UCTXIE;
  IotInitSM++;
  IOTSMTmr = 0;
  }
  break;
  default:
    break;
  }
}  