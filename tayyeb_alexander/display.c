//******************************************************************************
//
//  Description: This file contains the thumbwheel and ADC functions
//
//  Alex Tayyeb
//  Oct 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************
#include  "msp430.h"
#include  "functions.h"
#include <string.h>
#include  "macros.h"

// globals
char adc_char[FIVE]; //variable to hold value to display

char line; //variable to hold line of display 
char location; //variable to hold location on the display

//Function Declaration/Prototypes
void hex2bcd(unsigned int);
void displayADC(char, char);


//-----------------------------------------------------------------
// Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor
//
//-----------------------------------------------------------------
void hex2bcd(unsigned int reading){
  //this function converts a hex value into a decimal value 
  
  int value = RESET_STATE; //reset value
  char i;
  for(i=BEGINNING; i < 4; i++){ //initialize value to ASCII 0000
    adc_char[i] = '0';
  }
  while (reading > 999){ //check the 1000's bit
    reading = reading - 1000; // subtract 1000
    value = value + 1; //add 1 to the value 
    adc_char[0] = HEX_ZERO + value; //1000s spot = value
  }
  value = RESET_STATE; //reset value
  while (reading > 99){ //check if reading is above 100
    reading = reading - 100; // subtract 100 from reading 
    value = value + 1; //increment value 
    adc_char[1] = HEX_ZERO + value; //put into hundreds spot in ADC char array
  }
  value = RESET_STATE; //reset value
  while (reading > 9){ //if value is great than 10
    reading = reading - 10; //keep subtracting 10 and incrementing value
    value = value + 1;
    adc_char[2] = HEX_ZERO + value; //put value into 10 place 
  }
  adc_char[3] = HEX_ZERO + reading; //put remainder into 1s place 
}

//-------------------------------------------------------------
// ADC Line insert
// Take the HEX to BCD value in the array adc_char and place it
// in the desired location on the desired line of the display.
// char line => Specifies the line 1 thru 4
// char location => Is the location 0 thru 9
//
//-------------------------------------------------------------
void displayADC(char line, char location){
  //displays adc_char into the display 
  int i;
  unsigned int real_line;
  real_line = line - 1; //0th line is first line 
  for(i=0; i < 4; i++) { //copy from adc char into display line
    display_line[real_line][i+location] = adc_char[i];
  }
   display_changed = TRUE; //update display process
}

  