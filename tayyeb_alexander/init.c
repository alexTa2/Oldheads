//******************************************************************************
//
//  Description: This file contains the initialization functions
//
//  Alex Tayyeb
//  Sept 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************
#include  "msp430.h"
#include  "functions.h"
#include <string.h>
#include  "macros.h"

extern char display_line[4][11];
extern volatile unsigned char update_display;
extern char *display[4];

void Init_Conditions(void){
  //Initializes the display and enables interrupts 
//------------------------------------------------------------------------------

  int i;
  for(i=0;i<11;i++){
    display_line[0][i] = RESET_STATE;
    display_line[1][i] = RESET_STATE;
    display_line[2][i] = RESET_STATE;
    display_line[3][i] = RESET_STATE;
  }
  display_line[0][DISPLAY_SIZE] = RESET_STATE;
  display_line[1][DISPLAY_SIZE] = RESET_STATE;
  display_line[2][DISPLAY_SIZE] = RESET_STATE;
  display_line[3][DISPLAY_SIZE] = RESET_STATE;

  display[0] = &display_line[0][BEGINNING];
  display[1] = &display_line[1][BEGINNING];
  display[2] = &display_line[2][BEGINNING];
  display[3] = &display_line[3][BEGINNING];
  update_display = 0;


// Interrupts are disabled by default, enable them.
  enable_interrupts();
  stopFlag = HI; //Car is stopped initially 
//------------------------------------------------------------------------------
}

void Init_LEDs(void){
  //Turns off both LEDS
//------------------------------------------------------------------------------
// LED Configurations
//------------------------------------------------------------------------------
// Turns off both LEDs
  P1OUT &= ~RED_LED;
  P6OUT &= ~GRN_LED;
//------------------------------------------------------------------------------
}

void Init_ADC(void){
  //Primes the ADC to start working 
  
  
  //------------------------------------------------------------------------------
  // V_DETECT_L (0x04) // Pin 2 A2
  // V_DETECT_R (0x08) // Pin 3 A3
  // V_THUMB (0x20) // Pin 5 A5
  //------------------------------------------------------------------------------
  // ADCCTL0 Register
  ADCCTL0 = 0; // Reset
  ADCCTL0 |= ADCSHT_2; // 16 ADC clocks
  ADCCTL0 |= ADCMSC; // MSC
  ADCCTL0 |= ADCON; // ADC ON
  
  // ADCCTL1 Register
  ADCCTL2 = 0; // Reset
  ADCCTL1 |= ADCSHS_0; // 00b = ADCSC bit
  ADCCTL1 |= ADCSHP; // ADC sample-and-hold SAMPCON signal from sampling timer.
  ADCCTL1 &= ~ADCISSH; // ADC invert signal sample-and-hold.
  ADCCTL1 |= ADCDIV_0; // ADC clock divider - 000b = Divide by 1
  ADCCTL1 |= ADCSSEL_0; // ADC clock MODCLK
  ADCCTL1 |= ADCCONSEQ_0; // ADC conversion sequence 00b = Single-channel single-conversion
  // ADCCTL1 & ADCBUSY identifies a conversion is in process
  
  // ADCCTL2 Register
  ADCCTL2 = 0; // Reset
  ADCCTL2 |= ADCPDIV0; // ADC pre-divider 00b = Pre-divide by 1
  ADCCTL2 |= ADCRES_2; // ADC resolution 10b = 12 bit (14 clock cycle conversion time)
  ADCCTL2 &= ~ADCDF; // ADC data read-back format 0b = Binary unsigned.
  ADCCTL2 &= ~ADCSR; // ADC sampling rate 0b = ADC buffer supports up to 200 ksps
  
  // ADCMCTL0 Register
  ADCMCTL0 |= ADCSREF_0; // VREF - 000b = {VR+ = AVCC and VR– = AVSS }
  ADCMCTL0 |= ADCINCH_2; // V_THUMB (0x20) Pin 5 A5
 
  ADCIE |= ADCIE0; // Enable ADC conv complete interrupt
  ADCCTL0 |= ADCENC; // ADC enable conversion.
  ADCCTL0 |= ADCSC; // ADC start conversion.
}



