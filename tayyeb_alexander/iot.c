//------------------------------------------------------------------------------
//
//  Description: This file contains iot processing buffer analyzer and tries to
//               find valid commands to send to FRAM
//
//  Alex Tayyeb
//  Nov 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h" //Created 9/9/22



volatile char IOT_Data[5][DATA_SIZE]; //double array of IOT process buffers
char lines; //variable denoting which line of the double array we are on
char character; //variable pointer of the buffer
char commandFlag; //flag to signify possible command
int copyPointer; //pointer to copy into commandArray
int temp_rd; //temp read pointer to read from IOT_Data
char commandValid; //flag to show possible command is a real command 

void IOT_Process(void){ // Process IOT messages
  //This is function that constantly checks if anything new was written in to the IOT receive buffer
  //from the serial interrupt
  //When there is something new in the iot receive buffer, the receive array is copied into an IOT_DATA
  //line where each character is checked within a switch case to see if it is a command or not
  unsigned int iot_rx_wr_temp; //temp write pointer 
  iot_rx_wr_temp = iot_rx_wr; //copy into temp pointer 
  
  if(iot_rx_wr_temp != iot_rx_rd ){ // Determine if IOT is available
    IOT_Data[lines][character] = IOT_Rx[iot_rx_rd]; //write from Rx array into IOT_Data buffer 
    IOT_Rx[iot_rx_rd++] = NULL; //clear Rx array to keep it clean
    if(iot_rx_rd >= sizeof(IOT_Rx)){ //ring buffer (If end of array go back to beginning)
      iot_rx_rd = BEGINNING; //read pointer back to beginning
    }

    
    switch(character){ //based on pointer in current IOT_Data Line
    case first: //first character in array 
      if(IOT_Data[lines][character] == '+'){ // Got "+" this is a command
        commandFlag = TRUE; //flag command
      }
      break;
    case third: //check third character coming in
      if(IOT_Data[lines][character] == 'O'){ // Got "CONNECTED"
        lcd_4line(); // lcd 4 line display 
        strcpy(display_line[3], "CONNECTED "); //last line says CONNECTED 
        display_changed = TRUE; //update display process 
      }
      if(IOT_Data[lines][character] == 'L'){ // Got "CLOSED"
        strcpy(display_line[2], "disconnect"); //3 line says disconnect
        display_changed = TRUE; //update display process
      }
      break;

    case tenth: //check tenth character coming in
      if(commandFlag){ //if there is a + in the first position of array 
        if(IOT_Data[lines][character] == '!'){ //check if there is an exclamation mark here
          commandValid = TRUE; //command is a valid command
          temp_rd = character + 1; //save where the command is in the IOT_Data buffer
        }
      }

      break;
    default:
      break;
    }
    
    if(IOT_Data[lines][character] == LN){ //if line feed (end of line)
      if(commandValid){// if there is a valid command 
        copyPointer = BEGINNING; //reset copy pointer 
        for(temp_rd = temp_rd; IOT_Data[lines][temp_rd] != CR; temp_rd++){ //copy IOT_DATA until there is a character return 
          commandArray[copyPointer] = IOT_Data[lines][temp_rd]; //copy into commandArray
          copyPointer++;
        }
        
        commandSet = TRUE; //flag a commandSet for the commandReceived function
        commandValid = FALSE; //reset command flags
        commandFlag = FALSE;
      }
      character = BEGINNING; //if end of the line, reset pointer back to beginning
      lines++; //go to another line on the double array 
      if(lines >= MAX_LINES){ //if the lines exceed the array size
        lines = FIRST_LINE; //go back to the first line 
      }
    }else{ //if not a LN
      character++; //continue to next character in IOT_Data buffer
    }
    
    
    
  }
}


      