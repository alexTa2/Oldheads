//******************************************************************************
//
//  Description: This file contains the thumbwheel and ADC functions
//
//  Alex Tayyeb
//  Sept 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************
#include  "msp430.h"
#include  "functions.h"
#include <string.h>
#include  "macros.h"

// globals
char thousands;
char hundreds;
char tens;
char ones;
char emitting = RESET_STATE;
char detecting = RESET_STATE;
unsigned int reading;

char line;
char location;

//Function Declaration/Prototypes
void hex2bcd(unsigned int);

void displayADC(char, char);


//-----------------------------------------------------------------
// Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor
//
//-----------------------------------------------------------------
void hex2bcd(unsigned int reading){
  
  char bcdArray[SIZE];
  char i = 0;
  
  while(reading){
    
    if(reading >= 1000){
      reading = reading - 1000;
      thousands = 1;
    }else{
      thousands = 0;
    }
    
    for(i = RESET_STATE; i < SIZE; i++){
      bcdArray[i] = reading % 10;
      reading /= 10;
    }
  }
  hundreds = bcdArray[2];
  tens = bcdArray[1];
  ones = bcdArray[0];
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
  char adc_char[SIZE + 1] = {thousands, hundreds, tens, ones};
  int i;
  unsigned int real_line;
  
  real_line = line - 1;
  for(i=0; i < 4; i++) {
    display_line[real_line][i+location] = adc_char[i];
    
  }
}

  