//------------------------------------------------------------------------------
//
//  Description: This file contains adc interrupts
//
//  Alex Tayyeb
//  Sept 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h" //Created 9/9/22

char ADC_Channel; //variable for switch case 
volatile unsigned int ADC_Left_Detect; //variable to hold adc value of left sens
volatile unsigned int ADC_Right_Detect; //var to hold adc value of right sens 
volatile unsigned int ADC_Batt_Detect; //var to hold adc value of battery detect
volatile unsigned int ADC_Thumb_Detect; //var to hold adc balue of thumb detect 
char volatile adcReady; //adc ready flag to indiciate when adc has gone through all channels 

//State definitions for ADC Channel switch case 
#define LEFT_SENSOR (0x01)
#define RIGHT_SENSOR (0x02)
#define THUMBWHEEL (0x03)
#define BATT_DETECT (0x04)


//Single Channel - Demo Code
#pragma vector=ADC_VECTOR //initialize interrupt
__interrupt void ADC_ISR(void){
  //this interrupts calls on the adc to read the input from several peripherals including
  //the left and right light detectors and the thumb wheel on the power board 
  switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
  case ADCIV_NONE:
    break;
  case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
    
    // before its previous conversion result was read.
    break;
  case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
   
    break;
  case ADCIV_ADCHIIFG: // Window comparator interrupt flags
   
    break;
  case ADCIV_ADCLOIFG: // Window comparator interrupt flag
   
    break;
  case ADCIV_ADCINIFG: // Window comparator interrupt flag
    
    break;
  case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result
   ADCCTL0 &= ~ADCENC; // Disable ENC bit.
   switch (ADC_Channel){
   case LEFT_SENSOR: // Channel A2 Interrupt
       ADCMCTL0 &= ~ADCINCH_2; // Disable Last channel A2
       ADCMCTL0 |= ADCINCH_3; // Enable Next channel A3
       ADC_Left_Detect = ADCMEM0; // Move result into Global
       ADC_Left_Detect = ADC_Left_Detect >> 2; // Divide the result by 4
       hex2bcd(ADC_Left_Detect); // Convert result to String
       if(courseEvent > IOT_MOVING){ //If the main switch case is looking for white, start displaying the values on the screen
       displayADC(4,5); // Place String in Display
       }
       ADC_Channel++; //next channel
     break;
   case RIGHT_SENSOR:
       ADCMCTL0 &= ~ADCINCH_3; // Disable Last channel A2
       ADCMCTL0 |= ADCINCH_5; // Enable Next channel A3
       ADC_Right_Detect = ADCMEM0; // Move result into Global
       ADC_Right_Detect = ADC_Right_Detect >> 2; // Divide the result by 4
       hex2bcd(ADC_Right_Detect); // Convert result to String
       if(courseEvent > IOT_MOVING){ //If the main switch case is looking for white, start displaying the values on the screen
       displayADC(4,0); // Place String in Display
       }
       ADC_Channel++;//next channel

       break;
   case THUMBWHEEL:
       ADCMCTL0 &= ~ADCINCH_5; // Disable Last channel A2
       ADCMCTL0 |= ADCINCH_4; // Enable Next channel A3
       ADC_Thumb_Detect = ADCMEM0; // Move result into Global
       ADC_Thumb_Detect = ADC_Thumb_Detect >> 2; // Divide the result by 4
       hex2bcd(ADC_Thumb_Detect); // Convert result to String
       //displayADC(4,4); // Place String in Display
       ADC_Channel++;//next channel
     break;
   case BATT_DETECT:
       ADC_Channel=0;
       ADCMCTL0 &= ~ADCINCH_4; // Disable Last channel A2
       ADCMCTL0 |= ADCINCH_2; // Enable Next channel A3
       ADC_Batt_Detect = ADCMEM0; // Move result into Global
       ADC_Batt_Detect = ADC_Batt_Detect >> 2; // Divide the result by 4
       hex2bcd(ADC_Batt_Detect); // Convert result to String
       //displayADC(1,6); // Place String in Display
     break;
   default:
     break;
   }
   
     ADCCTL0 |= ADCENC; // Enable Conversions
     ADCCTL0 |= ADCSC; // Start next sample
     adcReady = HI; //Indicate adc is ready
  default:
    break;
  }
}