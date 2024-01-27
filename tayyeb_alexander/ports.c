//******************************************************************************
//
//  Description: This file contains the ports initializations using macros from
//                my macros.h file, obtained using the schematics
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

void Init_Port1(void){
    P1OUT = 0x00; // P1 set Low
    P1DIR = 0x00; // Set P1 to output

    P1SEL0 &= ~RED_LED;
    P1SEL1 &= ~RED_LED;
    P1OUT |= RED_LED;
    P1DIR |= RED_LED;

    P1SEL0 |= A1_SPEED;
    P1SEL1 |= A1_SPEED;

    P1SEL0 |= V_DETECT_L;
    P1SEL1 |= V_DETECT_L;

    P1SEL0 |= V_DETECT_R;
    P1SEL1 |= V_DETECT_R;

    P1SEL0 &= ~SMCLK_2476;
    P1SEL1 &= ~SMCLK_2476;
    P1DIR &= ~SMCLK_2476;

    P1SEL0 |= V_THUMB;
    P1SEL1 |= V_THUMB;

    P1SEL0 |= UCA0RXD;
    P1SEL1 &= ~UCA0RXD;

    P1SEL0 |= UCA0TXD;
    P1SEL1 &= ~UCA0TXD;
}

void Init_Port2(void){ // Configure Port 2
//------------------------------------------------------------------------------
    P2OUT = 0x00; // P2 set Low
    P2DIR = 0x00; // Set P2 direction to output

    P2SEL0 &= ~RESET_LCD; // RESET_LCD GPIO operation
    P2SEL1 &= ~RESET_LCD; // RESET_LCD GPIO operation
    P2OUT &= ~RESET_LCD; // Initial Value = Low / Off
    P2DIR |= RESET_LCD; // Direction = output

    P2SEL0 &= ~L_REVERSE_2476; // L_REVERSE_2476 GPIO operation
    P2SEL1 &= ~L_REVERSE_2476; // L_REVERSE_2476 GPIO operation
    P2DIR &= ~L_REVERSE_2476; // Direction = input

    P2SEL0 &= ~P2_2; // P2_2 GPIO operation
    P2SEL1 &= ~P2_2; // P2_2 GPIO operation
    P2DIR &= ~P2_2; // Direction = input

    P2SEL0 &= ~SW2; // SW2 Operation
    P2SEL1 &= ~SW2; // SW2 Operation
    P2OUT |= SW2; // Configure pullup resistor
    P2DIR &= ~SW2; // Direction = input
    P4PUD |= SW2; // Configure pull-up resistor SW2
    P2REN |= SW2; // Enable pullup resistor
    P2IES |= SW2; // P2.0 Hi/Lo edge interrupt
    P2IFG &= ~SW2; // Clear all P2.6 interrupt flags
    P2IE |= SW2; // P2.6 interrupt enabled

    P2SEL0 &= ~IOT_RUN_CPU; // IOT_RUN_CPU GPIO operation
    P2SEL1 &= ~IOT_RUN_CPU; // IOT_RUN_CPU GPIO operation
    P2OUT &= ~IOT_RUN_CPU; // Initial Value = Low / Off
    P2DIR |= IOT_RUN_CPU; // Direction = output

    P2SEL0 &= ~DAC_ENB; // DAC_ENB GPIO operation
    P2SEL1 &= ~DAC_ENB; // DAC_ENB GPIO operation
    P2OUT |= DAC_ENB; // Initial Value = High
    P2DIR |= DAC_ENB; // Direction = output

    P2SEL0 &= ~LFXOUT; // LFXOUT Clock operation
    P2SEL1 |= LFXOUT; // LFXOUT Clock operation

    P2SEL0 &= ~LFXIN; // LFXIN Clock operation
    P2SEL1 |= LFXIN; // LFXIN Clock operation

}

void Init_Port3(char bruv){
    P3OUT = 0x00;
    P3DIR = 0x00;

    P3SEL0 &= ~TEST_PROBE;
    P3SEL1 &= ~TEST_PROBE;
    P3DIR &= ~TEST_PROBE;

    P3SEL0 &= ~DAC_CNTL1;
    P3SEL1 &= ~DAC_CNTL1;
    P3DIR &= ~DAC_CNTL1;

    P3SEL0 |= OA2N;
    P3SEL1 |= OA2N;

    P3SEL0 &= ~LCD_BACKLITE;
    P3SEL1 &= ~LCD_BACKLITE;
    P3OUT |= LCD_BACKLITE;
    P3DIR |= LCD_BACKLITE;

    P3SEL0 &= ~SMCLK_2355;
    P3SEL1 &= ~SMCLK_2355;
    P3DIR &= ~SMCLK_2355;

    P3SEL0 &= ~DAC_CNTL;
    P3SEL1 &= ~DAC_CNTL;
    P3DIR &=  ~DAC_CNTL;

    P3SEL0 &= ~IOT_LINK_CPU;
    P3SEL1 &= ~IOT_LINK_CPU;
    P3DIR |= IOT_LINK_CPU;

    P3SEL0 &= ~IOT_EN_CPU;
    P3SEL1 &= ~IOT_EN_CPU;
    P3OUT &= ~IOT_EN_CPU;
    P3DIR |= IOT_EN_CPU;
}

void Init_Port4(void){
    P4OUT = 0x00; // P4 set Low
    P4DIR = 0x00; // Set P4 direction to output
    P4SEL0 &= ~P4_0; // P4_0 GPIO operation
    P4SEL1 &= ~P4_0; // P4_0 GPIO operation
    P4DIR &= ~P4_0; // Set P4_0 direction to input
    P4SEL0 &= ~SW1; // SW1 GPIO operation
    P4SEL1 &= ~SW1; // SW1 GPIO operation
    P4OUT |= SW1; // Configure pullup resistor
    P4DIR &= ~SW1; // Direction = input
    P4PUD |= SW1; // Configure pull-up resistor SW1
    P4REN |= SW1; // Enable pullup resistor
    P4IES |= SW1; // P4.2 Hi/Lo edge interrupt
    P4IFG &= ~SW1; // Clear all P2.6 interrupt flags
    P4IE |= SW1; // P4.2 interrupt enabled
    P4SEL0 |= UCA1TXD; // USCI_A1 UART operation
    P4SEL1 &= ~UCA1TXD; // USCI_A1 UART operation
    P4SEL0 |= UCA1RXD; // USCI_A1 UART operation
    P4SEL1 &= ~UCA1RXD; // USCI_A1 UART operation
    P4SEL0 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
    P4SEL1 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
    P4OUT |= UCB1_CS_LCD; // Set SPI_CS_LCD Off [High]
    P4DIR |= UCB1_CS_LCD; // Set SPI_CS_LCD direction to output
    P4SEL0 |= UCB1CLK; // UCB1CLK SPI BUS operation
    P4SEL1 &= ~UCB1CLK; // UCB1CLK SPI BUS operation
    P4SEL0 |= UCB1SIMO; // UCB1SIMO SPI BUS operation
    P4SEL1 &= ~UCB1SIMO; // UCB1SIMO SPI BUS operation
    P4SEL0 |= UCB1SOMI; // UCB1SOMI SPI BUS operation
    P4SEL1 &= ~UCB1SOMI; // UCB1SOMI SPI BUS operation
//----------------------------------------------------------
}

void Init_Port5(void){
    P5OUT &= 0x00;
    P5DIR &= 0x00;

    P5SEL0 |= CHECK_BAT;
    P5SEL1 |= CHECK_BAT;

    P5SEL0 |= V_BAT;
    P5SEL1 |= V_BAT;

    P5SEL0 |= V_DAC;
    P5SEL1 |= V_DAC;

    P5SEL0 |= ~V_3_3;
    P5SEL1 |= ~V_3_3;

    P5SEL0 &= ~IOT_BOOT_CPU;
    P5SEL1 &= ~IOT_BOOT_CPU;
    P5OUT |= IOT_BOOT_CPU;
    P5DIR |= IOT_BOOT_CPU;

}

void Init_Port6 (void){
    P6OUT &= 0x00;
    P6DIR &= 0x00;

    P6SEL0 |= R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6DIR |= R_FORWARD;

    P6SEL0 |= L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6DIR |= L_FORWARD;

    P6SEL0 |= R_REVERSE;
    P6SEL1 &= ~R_REVERSE;
    P6DIR |= R_REVERSE;

    P6SEL0 |= L_REVERSE_2433;
    P6SEL1 &= ~L_REVERSE_2433;
    P6DIR |= L_REVERSE_2433;

    P6SEL0 &= ~IR_LED;
    P6SEL1 &= ~IR_LED;
    P6DIR |= IR_LED;
    P6OUT |= IR_LED;

    P6SEL0 &= ~P6_5;
    P6SEL1 &= ~P6_5;
    P6DIR &= ~P6_5;

    P6SEL0 &= ~GRN_LED;
    P6SEL1 &= ~GRN_LED;
    P6OUT |= GRN_LED;
    P6DIR |= GRN_LED;
}

void Init_Ports(void){
    Init_Port1();
    Init_Port2();
    Init_Port3('j');
    Init_Port4();
    Init_Port5();
    Init_Port6();
}