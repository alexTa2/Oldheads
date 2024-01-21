#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"
char Jimmothy[11];
//------------------------------------------------------------------------------
//******************************************************************************
// Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor
//
//------------------------------------------------------------------------------
void HEXtoBCD(int hex_value){
int value;
Jimmothy[0] = '0';
Jimmothy[1] = '0';
Jimmothy[2] = '0';
Jimmothy[3] = '0';
Jimmothy[4] = ' ';
Jimmothy[5] = ' ';
Jimmothy[6] = ' ';
Jimmothy[7] = ' ';
Jimmothy[8] = ' ';
Jimmothy[9] = ' ';
Jimmothy[10] = ' ';
Jimmothy[11] = '\0';


while (hex_value > 999){
hex_value = hex_value - 1000;
value = value + 1;
Jimmothy[0] = 0x30 + value;
}
value = 0;
while (hex_value > 99){
hex_value = hex_value - 100;
value = value + 1;
Jimmothy[1] = 0x30 + value;
}
value = 0;
while (hex_value > 9){
hex_value = hex_value - 10;
value = value + 1;
Jimmothy[2] = 0x30 + value;
}
Jimmothy[3] = 0x30 + hex_value;
}
//******************************************************************************
//------------------------------------------------------------------------------

void adc_line4(int line){
  switch(line){
  case 0:
    display_line[0][0] = Jimmothy[0];
    display_line[0][1] = Jimmothy[1];
    display_line[0][2] = Jimmothy[2];
    display_line[0][3] = Jimmothy[3];
    display_line[0][4] = ' ';
    break;
  case 1:
    display_line[0][5] = Jimmothy[0];
    display_line[0][6] = Jimmothy[1];
    display_line[0][7] = Jimmothy[2];
    display_line[0][8] = Jimmothy[3];
    break;
  case 2:
    display_line[1][0] = Jimmothy[0];
    display_line[1][1] = Jimmothy[1];
    display_line[1][2] = Jimmothy[2];
    display_line[1][3] = Jimmothy[3];
    break;
  default:
    break; }
  update_display = 1;
  display_changed = TRUE;
}

void DispTimer(void){
    display_line[3][0] = 'T';
    display_line[3][1] = 'I';
    display_line[3][2] = 'M';
    display_line[3][3] = 'E';
    display_line[3][4] = ' ';
    if(Jimmothy[0] > 0x39){
      Jimmothy[0] = '9';
    } else {
    display_line[3][5] = Jimmothy[0];
    }
    display_line[3][6] = Jimmothy[1];
    display_line[3][7] = '.';
    display_line[3][8] = Jimmothy[2];
}

void Init_ADC(void){
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
ADCMCTL0 |= ADCINCH_5; // V_THUMB (0x20) Pin 5 A5
ADCIE |= ADCIE0; // Enable ADC conv complete interrupt
ADCCTL0 |= ADCENC; // ADC enable conversion.
ADCCTL0 |= ADCSC; // ADC start conversion.
}
