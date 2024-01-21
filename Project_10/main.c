//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//
//  Jim Carlson
//  Jan 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"
//#include  "LCD.h"
//#include  "ports.h"


// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Carlson_StateMachine(void);

  // Global Variables
volatile unsigned int Time_Sequence = 0;
volatile char one_time;
extern unsigned int ugh;
volatile char ringBuffa1[LARGE_RING_SIZE];
volatile char ringBuffa0[LARGE_RING_SIZE];
volatile unsigned int usb_rx_ring_wr;
volatile unsigned int usb_rx_ring_rd;
volatile unsigned int usb_tx_ring_wr;
volatile unsigned int usb_tx_ring_rd;
volatile char USB_Char_Rx[SMALL_RING_SIZE];
volatile char USB_Char_Tx[SMALL_RING_SIZE];
volatile unsigned int fully_recieved = 0;
extern volatile unsigned int RnT;
unsigned volatile int clear_buffer = 0;
// Globals
// 
volatile unsigned char IOT_2_PC[64];
volatile unsigned int iot_rx_wr;
unsigned int iot_rx_rd;
unsigned int direct_iot;
volatile unsigned char PC_2_IOT[64];
volatile unsigned int iot_tx_wr;
unsigned int iot_tx_rd;
unsigned int direct_usb;
volatile unsigned int commandRecieved = 0;
volatile char command;
unsigned int commandReader = 0;
unsigned int IotInitSM = 0;
volatile unsigned char commands[64];
unsigned int i;
unsigned int commandCount = 0;
volatile unsigned int forward_go;
volatile unsigned int backward_go;
volatile unsigned int turnRight;
volatile unsigned int turnLeft;
unsigned int password;
char state = WAIT;
unsigned int readyForNext = 1;
extern volatile unsigned int stateTmr1;
extern volatile unsigned int stateTmr2;
extern unsigned int startTmr;
extern volatile unsigned int IOTTmr;
unsigned int gotIP;
volatile unsigned int IPRead;
unsigned char IPAddress[32];
extern volatile unsigned int IPTmr;
extern volatile unsigned int TMRtimer;
unsigned volatile int ADC_Channel;
unsigned volatile int ADC_Left_Detect;
unsigned volatile int ADC_Right_Detect;
unsigned volatile int ADC_Thumb;
extern volatile unsigned int StartADCTmr;

void main(void){
//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings

  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
  Init_Serial_UCA0();                  // Initialize Serial Comm.
  Init_Serial_UCA1();
  
  // Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[0], "   NCSU   ");
  //strcpy(display_line[1], " WOLFPACK ");
  strcpy(display_line[2], "  ECE306  ");
  strcpy(display_line[3], "          ");
  display_changed = TRUE;
//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
//  Carlson_StateMachine();            // Run a Time Based State Machine
    Display_Process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
    if(IOTTmr >= 800){
    Init_IOT();
    if(IotInitSM > 3){
      IotInitSM = 3;
    }
    }
    /* TODO: FIND ADEQUATE SPOT TO CLEAR PC_2_IOT AND IOT_2_PC
             START FROM STEP 57 ON THE PROJECT SPEC 
             MAKE FORWARD AND BACKWARD COMMANDS IN COMMAND STATE MACHINE
              ^(USE FLAGS THAT ARE LOOKED AT IN THE TIMER INTERRUPTS)
    */  
    //Message Received code should not be nested
    if(fully_recieved){
     message_received();
     fully_recieved = 0;
     clear_buffer = 1;
     UCA1IE |= UCRXIE;
    } else {
      HEXtoBCD(TMRtimer);
      DispTimer();
      update_display = 1;
      display_changed = TRUE;
    }
    
    //Command code should not be nested
    if(commandRecieved){
      for(commandReader; commandReader < sizeof(IOT_2_PC); commandReader++){
        if(IOT_2_PC[commandReader] == '#'){ //check for password
          if(IOT_2_PC[commandReader+1] == '7'){
            if(IOT_2_PC[commandReader+2] == '3'){
              if(IOT_2_PC[commandReader+3] == '0'){
                if(IOT_2_PC[commandReader+4] == '1'){
                  password = 1;
                } else {password = 0;}
              } else {password = 0;}
            } else {password = 0;}
          } else {password = 0;}
         }//check password
        
        if(password){
        if(IOT_2_PC[commandReader] == '^'){ //check for command
        commands[commandReader] = IOT_2_PC[commandReader+1];
        commandCount++;
        stateTmr1 = 0;
        stateTmr2 = 0;
        }
        }
      }
      
      
      for(int i = 0; i < sizeof(commands); i++){
       if(commands[i] != '\0'){
       state = WAIT;
       command = commands[i];
       commands[i] = '\0';
       break;
       }
      }
    commandRecieved = 0;
    }
      
      
       switch(command){
            case 'F':
                UCA0BRW = 4;
                UCA0MCTLW = 0x5551;
                readyForNext = 1;
                break;
            case 'S':
                UCA0BRW = 52;
                UCA0MCTLW = 0x4911;
                readyForNext = 1;
                break;
            case 'G':
                 startTmr = 1;
                 Run_Straight();
                 if(stateTmr1 >= 6){
                   state = END;
                   command = '\0';
                   readyForNext = 1;
                   stateTmr1 = 0;
                   stateTmr2 = 0;
                 }
                  
                  break;
            case 'B':
              startTmr = 1;
              Run_Straight_Back();
                if(stateTmr1 >= 6){
                   state = END;
                   command = '\0';
                   readyForNext = 1;
                   stateTmr1 = 0;
                   stateTmr2 = 0;
                 }
                
                break; 
            case 'R':
              startTmr = 1;
              Run_rightTurn();
                if(stateTmr1 >= 6){
                   command = '\0';
                   state = END;
                   readyForNext = 1;
                   stateTmr1 = 0;
                   stateTmr2 = 0;
                 }
                break;
            case 'L':
              startTmr = 1;
              Run_leftTurn();
                if(stateTmr1 >= 6){
                  command = '\0';
                   state = END;
                   readyForNext = 1;
                   stateTmr1 = 0;
                   stateTmr2 = 0;
                 }
                break;
       case 'I':
         StartADCTmr = 1;
          ADCStateMachine();
            case '\0':
                commandReader = 0;
                readyForNext = 1;
                password = 0;
                for(int i = 0; i < LARGE_RING_SIZE; i++){
                commands[i] = '\0';
                }
                break;
            default:break;
            }
       commandCount--;
  
    
    //clear buffer code should not be nested
    if(clear_buffer){
       for(int i = 0; i < LARGE_RING_SIZE; i++){
        ringBuffa0[i] = '\0';
        ringBuffa1[i] = '\0';
        IOT_2_PC[i] = '\0';
        if(PC_2_IOT[i] = '\n'){
          PC_2_IOT[i] = '\0';
          if(PC_2_IOT[i+2] != '\0'){
            UCA1IE |= UCRXIE;
            break;
          }
        }
        PC_2_IOT[i] = '\0';
      }
      clear_buffer = 0;
      commandCount = 0;
    }
//------------------------------------------------------------------------------
}
}

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
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
switch (ADC_Channel++){
case 0x00: // Channel A2 Interrupt
ADCMCTL0 &= ~ADCINCH_2; // Disable Last channel A2
ADCMCTL0 |= ADCINCH_3; // Enable Next channel A3
ADC_Left_Detect = ADCMEM0; // Move result into Global
ADC_Left_Detect = ADC_Left_Detect >> 2; // Divide the result by 4
HEXtoBCD(ADC_Left_Detect); // Convert result to String
adc_line4(0); // Place String in Display
ADCCTL0 |= ADCSC;
break;
case 0x01:
ADCMCTL0 &= ~ADCINCH_3; // Disable Last channel A2
ADCMCTL0 |= ADCINCH_5; // Enable Next channel A3
ADC_Right_Detect = ADCMEM0; // Move result into Global
ADC_Right_Detect = ADC_Right_Detect >> 2; // Divide the result by 4
HEXtoBCD(ADC_Right_Detect); // Convert result to String
adc_line4(1); // Place String in Display
ADCCTL0 |= ADCSC;
break;
case 0x02:
ADCMCTL0 &= ~ADCINCH_5; // Disable Last channel A2
ADCMCTL0 |= ADCINCH_2; // Enable Next channel A3
ADC_Thumb = ADCMEM0; // Move result into Global
ADC_Thumb = ADC_Thumb >> 2; // Divide the result by 4
HEXtoBCD(ADC_Thumb); // Convert result to String
adc_line4(2); // Place String in Display
ADC_Channel=0;
break;
default:
break;
}
ADCCTL0 |= ADCENC; // Enable Conversions
default:
break;
}
}