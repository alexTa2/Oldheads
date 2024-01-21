//******************************************************************************
//
//  Description: This file contains the ports initializations using macros from
//               from my macros.h file, obtained using the schematics
//
//  Alex Tayyeb
//  Sept 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************
#include  "msp430.h"
#include  "functions.h"
#include <string.h>
#include  "macros.h"

void Init_Ports(void){
  //Function calls all of the other initialization functions 
  Init_Port1();
  Init_Port2();
  Init_Port3(SMCLK);
  Init_Port4();
  Init_Port5();
  Init_Port6();
}

void Init_Port1(void){ // Configure Port 1
  //-------------------------------------------------------
  //Configure Port 1
  // Port 1 Pins
  // RED_LED (0x01) // 0 RED LED 0
  // V_A1_SEEED (0x02) // 1 A1_SEEED
  // V_DETECT_L (0x04) // 2 V_DETECT_L
  // V_DETECT_R (0x08) // 3 V_DETECT_R
  // V_A4_SEEED (0x10) // 4 V_A4_SEEED
  // V_THUMB (0x20) // 5 V_THUMB
  // UCA0RXD (0x40) // 6 Back Channel UCA0RXD
  // UCA0TXD (0x80) // 7 Back Channel UCA0TXD
  //-------------------------------------------------------
  P1OUT = RESET_STATE; //P1 set low
  P1DIR = RESET_STATE; //P1 direction to output
  
  P1SELC &= ~RED_LED;
  P1OUT &= ~RED_LED; //Initial Value is off
  P1DIR |= RED_LED;  //LED is an output
  
  P1SELC |= V_A1_SEEED; // ADC input for A1_SEEED
  
  P1SELC |= V_DETECT_L; // ADC input for V_DETECT_L
  
  P1SELC |= V_DETECT_R; // ADC input for V_DETECT_R
  
  P1SELC |= V_A4_SEEED; // ADC input for V_A4_SEEED
  
  P1SELC |= V_THUMB; // ADC input for V_THUMB

  
  P1SEL0 |= UCA0RXD; //UCA0 Receive
  P1SEL1 &= ~UCA0RXD;
  
  P1SEL0 |= UCA0TXD; //UCA0 transmit
  P1SEL1 &= ~UCA0TXD;
}

void Init_Port2(void){ // Configure Port 2
//------------------------------------------------------------------------------
  P2OUT = RESET_STATE; // P2 set Low
  P2DIR = RESET_STATE; // Set P2 direction to output

  P2SELC &= ~RESET_LCD; // RESET_LCD GPIO operation
  P2OUT &= ~RESET_LCD; // Initial Value = Low / Off
  P2DIR |= RESET_LCD; // Direction = output

  P2SELC &= ~Check_Bat; // Check_Bat GPIO operation
  P2OUT &= ~Check_Bat; // Initial Value = Low / Off
  P2DIR |= Check_Bat; // Direction = output

  P2SELC &= ~IR_LED; // P2_2 GPIO operation
  P2OUT &= ~IR_LED; // Initial Value = Low / Off
  P2DIR |= IR_LED; // Direction = input

  P2SELC &= ~SW2; // SW2 Operation
  P2OUT |= SW2; // Configure pullup resistor
  P2DIR &= ~SW2; // Direction = input
  P2REN |= SW2; // Enable pullup resistor
  
  //Project 5 below
  P2IFG &= ~SW2; // P2.3 IFG SW2 cleared
  P2IE |= SW2; // P2.3 SW2 interrupt enabled
  P2IES |= SW2; // P2.0 Hi/Lo edge interrupt

  P2SELC &= ~IOT_RUN_CPU; // IOT_RUN_CPU GPIO operation
  P2OUT &= ~IOT_RUN_CPU; // Initial Value = Low / Off
  P2DIR &= ~IOT_RUN_CPU; // Direction = output

  P2SELC &= ~DAC_ENB; // DAC_ENB GPIO operation
  P2OUT |= DAC_ENB; // Initial V~alue = High
  P2DIR |= DAC_ENB; // Direction = output

  P2SEL0 &= ~LFXOUT; // LFXOUT Clock operation
  P2SEL1 |= LFXOUT; // LFXOUT Clock operation

  P2SEL0 &= ~LFXIN; // LFXIN Clock operation
  P2SEL1 |= LFXIN; // LFXIN Clock operation
  //------------------------------------------------------------------------------
 }
void Init_Port3(char smclk){  // Configure Port 3
  P3OUT = RESET_STATE; // P3 set Low
  P3DIR = RESET_STATE; // Set P4 direction to output
  
  P3SELC &= ~TEST_PROBE; //GPIO
  // P3DIR &= ~TEST_PROBE; //INPUT
  // P3OUT &= ~TEST_PROBE; //TURNED OFF
  
  P3SELC |= OA2O; //OA20
  
  
  P3SELC |= OA2N; //OA2N

  P3SELC |= OA2P; //OA2O

  P3SEL0 &= ~SMCLK; //SMCLK
  P3SEL1 |= SMCLK; //SMCLK
  P3DIR |= SMCLK;  //SMCLK
  P3OUT &= ~SMCLK;//SMCLK
  
  P3SELC &= ~DAC_CNTL; //GPIO
  //P3DIR &= ~DAC_CNTL; //INPUT
  //P3OUT &= ~DAC_CNTL;
  
  P3SELC &= ~IOT_LINK_CPU; //GPIO
  P3DIR |= IOT_LINK_CPU; //OUTPUT
  P3OUT &= ~IOT_LINK_CPU; //inital low
  
  P3SELC &= ~IOT_EN_CPU; //GPIO
  P3DIR |= ~IOT_EN_CPU; //OUTPUT
  P3OUT &= ~IOT_EN_CPU; //Set as Output Active low for 50ms reset IOT
}

void Init_Port4(void){ // Configure Port 4
//------------------------------------------------------------------------------
  P4OUT = RESET_STATE; // P4 set Low
  P4DIR = RESET_STATE; // Set P4 direction to output

  P4SELC &= ~DAC_CNTL1; // DAC_CNTL1 GPIO operation
  P4OUT |= DAC_CNTL1; // Set DAC_CNTL1 Off [High]
  P4DIR |= DAC_CNTL1; // Set DAC_CNTL1 direction to output

  P4SELC &= ~SW1; // SW1 GPIO operation
  P4OUT |= SW1; // Configure pullup resistor
  P4DIR &= ~SW1; // Direction = input
  P4REN |= SW1; // Enable pullup resistor
  //Project 5 below
  P4IES |= SW1; // P4.2 Hi/Lo edge interrupt
  P4IFG &= ~SW1; // Clear all P2.6 interrupt flags
  P4IE |= SW1; // P4.2 interrupt enabled
  //

  P4SEL0 |= UCA1TXD; // USCI_A1 UART operation
  P4SEL1 &= ~UCA1TXD; // USCI_A1 UART operation

  P4SEL0 |= UCA1RXD; // USCI_A1 UART operation
  P4SEL1 &= ~UCA1RXD; // USCI_A1 UART operation

  P4SELC &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
  P4OUT |= UCB1_CS_LCD; // Set SPI_CS_LCD Off [High]
  P4DIR |= UCB1_CS_LCD; // Set SPI_CS_LCD direction to output

  P4SEL0 |= UCB1CLK; // UCB1CLK SPI BUS operation
  P4SEL1 &= ~UCB1CLK; // UCB1CLK SPI BUS operation

  P4SEL0 |= UCB1SIMO; // UCB1SIMO SPI BUS operation
  P4SEL1 &= ~UCB1SIMO; // UCB1SIMO SPI BUS operation

  P4SEL0 |= UCB1SOMI; // UCB1SOMI SPI BUS operation
  P4SEL1 &= ~UCB1SOMI; // UCB1SOMI SPI BUS operation
//------------------------------------------------------------------------------
}

void Init_Port5(void){ // Configure Port 5
  //--------------------------------------------------------
  // Port 5 Pins
  // V_BAT (0x01) // 0 V_BAT
  // V_5_0 (0x02) // 1 V_5_0
  // V_DAC (0x04) // 2 V_DAC
  // V_3_3 (0x08) // 3 V_3.3
  // IOT_PROG_MODE (0x10) // 4 IOT_PROGRAM_MODE
  //--------------------------------------------------------
  P5OUT = RESET_STATE; // P5 set Low
  P5DIR = RESET_STATE; // Set P5 direction to output
 
  P5SELC |= V_BAT; // ADC input for V_BAT
  
  P5SELC |= V_5_0; // ADC input for V_BAT
  
  P5SELC |= V_DAC; // ADC input for V_DAC
  
  P5SELC |= V_3_3; // ADC input for V_3_3
  
  P5SELC &= ~IOT_BOOT; // IOT_BOOT GPIO operation
  P5OUT |= IOT_BOOT; // Set Output value active
  P5DIR |= IOT_BOOT; // Set direction to output
 
}


//Project 6 PWM Implementations below

void Init_Port6(void){ // Configure Port 6
  P6OUT = RESET_STATE; // P6 set Low
  P6DIR = RESET_STATE; // Set P6 direction to output
  
  P6SEL1 &= ~R_FORWARD; //GPIO
  P6SEL0 |=  R_FORWARD;
  P6DIR  |=  R_FORWARD; //OUTPUT
//P6OUT &= ~R_FORWARD; //INITIAL VALUE = OFF
  
  P6SEL1 &= ~R_REVERSE; //GPIO
  P6SEL0 |=  R_REVERSE;
  P6DIR  |=  R_REVERSE; //OUTPUT
//P6OUT &= ~R_REVERSE; //INITIAL VALUE = OFF
  
  P6SEL1 &= ~L_FORWARD; //GPIO
  P6SEL0 |=  L_FORWARD;
  P6DIR  |=  L_FORWARD; //OUTPUT
//P6OUT &= ~L_FORWARD; //INITIAL VALUE = OFF
  
  P6SEL1 &= ~L_REVERSE; //GPIO
  P6SEL0 |=  L_REVERSE;
  P6DIR  |=  L_REVERSE; //OUTPUT
//P6OUT &= ~L_REVERSE; //INITIAL VALUE = OFF
  
  P6SELC &= ~LCD_BACKLITE; //GPIO
  P6DIR |= LCD_BACKLITE; //OUTPUT
  P6OUT &= ~LCD_BACKLITE; //INITIAL VALUE = OFF
  
  P6SELC &= ~P6_5; //GPIO
  
  P6SELC &= ~GRN_LED; //GPIO
  
  P6DIR |= GRN_LED; //OUTPUT
  P6OUT &= ~GRN_LED; //INITIAL VALUE = OFF

}