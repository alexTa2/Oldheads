//******************************************************************************
//
//  Description: This file contains the display update function.
//
//  Alex Tayyeb
//  Sept 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************
#include  "msp430.h"
#include  "functions.h"
#include <string.h>
#include  "macros.h"


extern volatile unsigned char display_changed; //Variable is manually changed everytime we want to change the display
extern volatile unsigned char update_display; //Update display is changed automatically by a 200ms timer B0 0



void Display_Process(void){ 
  //Display_Process() updates the display values in each line of the LCD, allowing
  //allowing for different characters to be displayed on the LCD screen
  if(update_display){ //checks if timer went off
    update_display = RESET_STATE; //toggles the update_display value so the timer can set it off again
    if(display_changed){//checks if the display was actually changed 
      display_changed = RESET_STATE; //if display changed toggle it off again, to prime the next change
      Display_Update(RESET_STATE,RESET_STATE,RESET_STATE,RESET_STATE); //clears the LCD to allow for new characters to be written in
    }
  }
}

