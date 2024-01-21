#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"
extern volatile unsigned int fully_recieved;
extern volatile unsigned int usb_rx_ring_wr;
extern volatile unsigned int usb_rx_ring_rd;
extern volatile unsigned int usb_tx_ring_wr;
extern volatile unsigned int usb_tx_ring_rd;
extern volatile unsigned char IOT_2_PC[SMALL_RING_SIZE];
extern volatile unsigned int iot_rx_wr;
extern volatile unsigned int iot_rx_rd;
extern volatile unsigned char PC_2_IOT[SMALL_RING_SIZE];
extern volatile unsigned int iot_tx_wr;
extern volatile unsigned int iot_tx_rd;
extern volatile char ringBuffa1[LARGE_RING_SIZE];
extern volatile unsigned int clear_buffer;
extern volatile unsigned int RnT;
extern volatile unsigned int commandRecieved;
extern unsigned int IotInitSM;
extern unsigned int gotIP;
extern unsigned char IPAddress[32];
#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
//------------------------------------------------------------------------------
// Echo back RXed character, confirm TX buffer is ready first
unsigned int temp;
switch(__even_in_range(UCA1IV,0x08)){
case 0: break; // Vector 0 - no interrupt
case 2: // Vector 2 - RXIFG
  temp = iot_rx_wr;
  ringBuffa1[temp] = UCA1RXBUF; // Rx -> PC_2_IOT character array
  PC_2_IOT[temp] = ringBuffa1[temp];
  if(PC_2_IOT[iot_rx_wr++] == '\n'){
  UCA1IE &= ~UCRXIE;
  UCA0IE |= UCTXIE; // Enable Tx interrupt
  P2OUT ^= IOT_RUN_CPU;
  } 
  //UCA0TXBUF = UCA1RXBUF;
break;
case 4: // Vector 4 - TXIFG  
  UCA1TXBUF = IOT_2_PC[iot_tx_rd];
  if(IOT_2_PC[iot_tx_rd++] == '\n'){
    iot_rx_wr = BEGINNING; // Circular buffer back to beginning
    iot_tx_rd = BEGINNING;
    UCA1IE &= ~UCTXIE;
    fully_recieved = 1;
    commandRecieved = 1;
    RnT = 0;
  } 
break;
default: break;
}
//------------------------------------------------------------------------------
}

void Init_Serial_UCA1(void){
int i;
for(i=0; i<SMALL_RING_SIZE; i++){
IOT_2_PC[i] = 0x00; // USB Rx Buffer
}
iot_rx_wr = BEGINNING;
iot_rx_rd = BEGINNING;
for(i=0; i<LARGE_RING_SIZE; i++){ // May not use this
PC_2_IOT[i] = 0x00; // USB Tx Buffer
}
iot_tx_wr = BEGINNING;
iot_tx_rd = BEGINNING;
// Configure UART 0
UCA1CTLW0 = 0; // Use word register
UCA1CTLW0 |= UCSWRST; // Set Software reset enable
UCA1CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
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
UCA1BRW = 4; // 9,600 Baud
// UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
// UCA0MCTLW = 0x49 concatenate 1 concatenate 1;
UCA1MCTLW = 0x5551 ;
UCA1CTLW0 &= ~ UCSWRST; // Set Software reset enable
UCA1IE |= UCRXIE; // Enable RX interrupt
UCA1TXBUF = 0x00; // Prime the Pump
}


void message_received(void){
      for(int i = 0; i < sizeof(IOT_2_PC); i++){
        if(IOT_2_PC[i] == 'I'){
          if(IOT_2_PC[i+1] =='P'){
            if(IOT_2_PC[i+2] == ','){
                  gotIP = 1;
                }
              }
        }
       if(gotIP){
         for(int i =0; i < sizeof(IPAddress); i++){
           for(int j = 0; j < 14; j++){
             if(IOT_2_PC[j] > 0x2e && IOT_2_PC[j] < 0x3a){
              IPAddress[i] = IOT_2_PC[j];
              break;
             }
          }
       }
       }
       if(i < 10){
        display_line[2][i] = IOT_2_PC[i];
       }
        if(IOT_2_PC[i] == '\0'){
          display_line[2][i] = ' ';
        }
      }   
}