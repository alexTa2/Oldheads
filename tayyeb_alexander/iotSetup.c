//******************************************************************************
//
//  Description: This file contains the iot to fram communication functions and scheduler
//
//  Alex Tayyeb
//  Nov 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************

#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h" //Created 9/9/22

#define QUOTES ('"') //define quotes to add into strings 
#define FINAL_SETTING (9) //last setting needed to initalize the IOT


//Pointer offsets to read correct numbers
#define IP_OFFSET (3)
#define SSID_OFFSET (4)

char iotPing[26] = "AT+PING= www.google.com   "; //message to ping to server 

char settings[9][DATA_SIZE] = {
  //contains a double array of all the initialization functions of the IOT before they have been fully prepped
  // to enter the IOT module
  
  "AT+SYSSTORE=1  ", //0 SAVE
  "AT+CWJAP= ncsu ,    ", // 1 CONNECT
  "AT+CWAUTOCONN=1  ", //2 AUTO
  "AT+CWRECONNCFG=5,100  ", //3 RECONN
  "AT+CIPMUX=1  ",      //4 MULT
  "AT+SYSSTORE=0  ", //5 STOPSAVE
  "AT+CIPSERVER=1,1337  ", // 6 SERVER
  "AT+CIFSR  ", // 7 iotIP
  "AT+CWJAP?  " // 8 iotSSID
};


char readyForNext; //flag for next command 
char foundIP; //flag for foundIP
char foundSSID; //flag for found SSID
char setUpFlag; //flag for starting the function
char setting; //pointer for double array in settings 
char settingPointer; //pointer for which character of the setting we are on
char firstIP[10] = "          "; //empty array to copy into display when full
char secondIP[10] = "          "; //empty array to copy into display when full
char dots; //variable to count the number of dots in the IP address
char ssid[10] = "          "; //empty array to copy into display when full

void Init_IOT(void){
//Init_IOT() is called to fill in the rest of the settings so that they can be fully ready
  //to enter the IOT
  //This is needed because adding in linefeeds and character returns and double quotes is difficult to
  //do in the C language 
  settings[0][13] = CR;
  settings[0][14] = LN;
  settings[1][9] = QUOTES;
  settings[1][14] = QUOTES;
  settings[1][16] = QUOTES;
  settings[1][17] = QUOTES;
  settings[1][18] = CR;
  settings[1][19] = LN;
  
  settings[2][15] = CR;
  settings[2][16] = LN;
  
  settings[3][20] = CR;
  settings[3][21] = LN;
  
  settings[5][13] = CR;
  settings[5][14] = LN;
  
  settings[4][11] = CR;
  settings[4][12] = LN;
  
  settings[6][19] = CR;
  settings[6][20] = LN;
  
  settings[7][8] = CR;
  settings[7][9] = LN;  
  
  settings[8][9] = CR;
  settings[8][10] = LN;
  
  iotPing[8] = QUOTES;
  iotPing[23] = QUOTES;
  iotPing[24] = CR;
  iotPing[25] = LN;
  
}

void nextCommand(void){
  //nextCommand() primes the next command in the settings array to be sent to the IOT
  //It preps the command and enables the transmit function to the IOT when the readyForNext flag is HIGH
  //If it is finished with the settings it turns off the setUpFlag which stops the IOT_Setup function
  
  readyForNext = FALSE; //reset readyForNext flag 
  settingPointer = BEGINNING; //reset settingPointer 
  
  for(int i = RESET_STATE; i < sizeof(IOT_Tx_buf); i++){ //copy into the transmit buffer the current setting
    IOT_Tx_buf[i] = settings[setting][settingPointer++]; //copy into the transmit buffer
  }
  setting++; //move to next setting
  USCI_A0_transmit(); //enable transmit interrupt
  if(setting > FINAL_SETTING){ //if we are done initializing
    setting = RESET_STATE; //reset the settings pointer
    setUpFlag = FALSE; //turn of the setupFlag
    courseEvent = nextEvent; //move the main courseEvent to the nextEvent, which is IOT_MOVING
  }
  
}

void pingServer(void){
  //pingServer() is needed to ping the network so the ncsu IT does not kick off
  //our EV IOT Module from the network
  //The function transmits www.google.com to the internet to keep the network satisfied
  //the pingFlag is enabled every 5 seconds with a timer in interrupt_timer.c
  readyForNext = FALSE; //reset readyforNext flag
  pingFlag = FALSE; //reset ping flag
 
  for(int i = RESET_STATE; i < sizeof(IOT_Tx_buf); i++){ //copy the ping message into IOT transmit buffer
    IOT_Tx_buf[i] = iotPing[i];
  }
  USCI_A0_transmit(); //enable the transmission of the IOT
}

void IOT_Setup(void){ // Process IOT messages
  //This function is very similar to IOT_Process except that it is only checking 
  //if it is not getting an error message, of that its getting an IP/SSID
  //It uses the IOT_Data double array again and sorts through each character to check what the message is 
  
  
  unsigned int iot_rx_wr_temp; //create a temp write pointer to compare with
  iot_rx_wr_temp = iot_rx_wr; //copy into the temp var
  
  if(iot_rx_wr_temp != iot_rx_rd ){ // Determine if IOT is available
    IOT_Data[lines][character] = IOT_Rx[iot_rx_rd]; //copy from Rx into an IOT_Date line
    IOT_Rx[iot_rx_rd++] = NULL; // clear the Rx array after copying
    if(iot_rx_rd >= sizeof(IOT_Rx)){ //ring buffer
      iot_rx_rd = BEGINNING;
    }
    
    switch(character){ //check based on character 
    case first: //check first character coming in
      if(IOT_Data[lines][character] == 'O'){ // Got "O" for OK
        readyForNext = TRUE; //OK means it is ready to send next command
      }
      break;
    case eleventh: //check the eleventh character coming in
        if(IOT_Data[lines][character] == 'P'){ //Got "xxxxxxxxxIP"
          foundIP = TRUE; //found IP
          temp_rd = character + IP_OFFSET; //
        }

      break;
    case fourth:
      if(IOT_Data[lines][character] == 'A'){ // Got +CWJ"A"P
        foundSSID = TRUE;
        temp_rd = character + SSID_OFFSET;
      }
      break;
      
    default: break;
    }//------------------------------------------------------------------
    
    //If end of line
    if(IOT_Data[lines][character] == LN){ //if line feed
      //Check if we got an IP
      if(foundIP){
        copyPointer = RESET_STATE; //reset copy pointer
        dots = RESET_STATE; //reset amount of dots
        foundIP = FALSE; //reset foundIP flag
        for(temp_rd = temp_rd; dots < 2 ; temp_rd++){ //copy into firstIP array as long as there is less than 2 dots seen
          firstIP[copyPointer++] = IOT_Data[lines][temp_rd]; //copy into first array
          if(IOT_Data[lines][temp_rd] == '.'){ //if there is a dot
            dots++; //add to dots var
          }
        }
        copyPointer = RESET_STATE; //reset copy pointer for second array 
        for(temp_rd = temp_rd; IOT_Data[lines][temp_rd] != QUOTES ; temp_rd++){ //copy into second array until there is a QUOTE
          secondIP[copyPointer++] = IOT_Data[lines][temp_rd]; //copy into secondIP
        }
        
        strncpy(display_line[1], firstIP, DISPLAY_SIZE); //display firstIP string on second line of display
        strncpy(display_line[2], secondIP, DISPLAY_SIZE); //display secondIP on third line of display
        display_changed = TRUE; //update display process
        
      }//---------if foundIP--------------------
      
      //Check if we got an SSID
      if(foundSSID){ //if ssid flag is HIGH
        foundSSID = FALSE; //toggle to false
        copyPointer = RESET_STATE; //reset copy pointer 
        for(temp_rd = temp_rd; IOT_Data[lines][temp_rd] != QUOTES ; temp_rd++){ //copy SSID until QUOTE
          ssid[copyPointer++] = IOT_Data[lines][temp_rd]; //copy into ssid array 
        }
        strncpy(display_line[0], ssid, DISPLAY_SIZE); //copy into first line of display the ssid string
        display_changed = TRUE;    //update display function
        
        courseEvent = nextEvent; //when ssid is found IOT is done initializing, move on to next state in main.c
        TB0CCTL1 |= CCIE; //enable ping to server
        
      }//----If found SSID we are Done// start receiving new commands and ping
        
        
      
      character = BEGINNING; //move pointer back to beginning
      lines++; //move to next line 
      if(lines >= MAX_LINES){ //if greater than max lines go back to first line 
        lines = FIRST_LINE;
      }
    }else{
      character++; //move to next character in IOT_Rx
    }
    
    
    
  }
}

//


























