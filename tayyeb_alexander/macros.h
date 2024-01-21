//******************************************************************************
//
//  Description: This file contains the macros and #defines used for the program
//               This file is also used to avoid magic numbers in the program
//  Alex Tayyeb
//  Sept 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************

//Macros of the password for the IOT
#define firstPass ('2')
#define secondPass ('3')
#define thirdPass ('6')
#define fourthPass ('7')

//magic numbers for iot process 
#define MAX_LINES (4)
#define FIRST_LINE (0)

//parameters of switch case in IOT_Setup
#define eleventh (11)
#define fourth (4) 

//state parameters for IOT_Process state machine 
#define first (0)
#define third (3)
#define tenth (10)

//Specific command letters that the FRAM uses to perform different commands
//Time commands
#define F2 ('F')
#define R1 ('B')
#define RIGHT_90 ('R')
#define LEFT_45 ('L')

//Specific command letters that the FRAM uses to perform different commands
//Instant commands
#define FORWARD ('F')
#define REVERSE ('B')
#define RIGHT_TURN ('R')
#define LEFT_TURN ('L')
#define STOP ('S')
#define BEGIN ('A')
#define EXIT_CIRC ('X')


//Time macros used to gauge how long to turn the wheels to perform the time commands 
#define twoSeconds (20)
#define oneSecond (10)
#define ninetyDegrees (7)
#define fortyfiveDegrees (4)

//Macros used in the endCommandChar() switch statement that deducts whether a command is a multi command/nor
#define PARANTH (')')
#define EXCLAM ('!')

//externs from commandStructure to be used in other files 
extern char readyForNext;
extern char foundIP;
extern char foundSSID;
extern char setUpFlag;
extern char setting;
extern char settingPointer;
extern char stopFlag;
extern char arriveNum;


extern volatile char adcReady; //variable flag to determine if adc is ready T/F

//Final project macros
extern char courseEvent; //current event
extern char nextEvent; //next event 

//State parameter definitions for main.c state machine
#define START (0)
#define IOT_MOVING (1)
#define FIND_WHITE (2)
#define FIND_BLACK (3)
#define LINE_UP (4)
#define FOLLOW_LINE (5)
#define FIRST_CIRCLE (6)
#define CONTINUE_CIRCLE (10)
#define EXIT_CIRCLE (7)
#define AHHH (8)


//externs for the display function, used to update and configure display
extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

//interrupt_switch macros
extern char interrCount; //interrupt count used in switches for debouncing

//adc interrupt externs 
extern char channelCounter;
extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;
extern volatile unsigned int ADC_Batt_Detect;
extern volatile unsigned int ADC_Thumb_Detect;

//Command structure externs
#define DATA_SIZE (50)
extern char continueFlag;
extern int timerVal;
extern unsigned char commandArray[DATA_SIZE];
extern int commandPointer;
extern char commandSet;
extern volatile char IOT_Data[5][DATA_SIZE];
extern char lines;
extern int temp_rd;
extern char character;
extern int copyPointer;


#define ALWAYS                  (1)
#define HI                      (1)
#define RESET_STATE             (0)
#define LO                      (0)
#define TRUE                 (1) //
#define FALSE                (0) //
#define TEST_PROBE           (0x01) // 3.0 TEST PROBE
#define DISPLAY_SIZE          (10)

#define HEX_ZERO (0x30)
#define P4PUD (P4OUT)

//Timer Intervals
#define TB0CCR0_INTERVAL (7000) //32768 / 1 / 1 / 5 (200 ms)UPDATE DISPLAY
#define TB0CCR1_INTERVAL (17000) //32768 / 1 / 1 / 2 (500 ms) //ping timer
#define TB0CCR2_INTERVAL (17000) //17000 / 1 / 1 / 10 (200 ms) IOT Delay

#define TB1CCR0_INTERVAL (12500) // 8,000,000 / 8 / 8 / (1 / 200msec)
#define TB1CCR1_INTERVAL (12500)  //8,000,000 / 8 / 8 / (1 / 200msec)(200 msecond) //DEBOUNCE 1
#define TB1CCR2_INTERVAL (12500)  //8,000,000 / 8 / 8 / (1 / 200msec) (200 msecond) //DEBOUNCE 2

#define TB2CCR0_INTERVAL (3270)  //32768 / 1 / 1 / 20 (100 ms)//ADC Sample
#define TB2CCR1_INTERVAL (17000)  //32768 / 1 / 1 / 10 (500 ms) //5 second timer
#define TB2CCR2_INTERVAL (17000)  //32768 / 1 / 1 / 2 (500 ms) //Revolution Timer

#define DEBOUNCE_1_SECOND       (10)
#define TIMER_2_SECONDS         (20) 
#define TIMER_1_SECOND          (5)
#define REVOLUTION              (40)
#define FIVE_SECOND_COUNT       (10) //10 * .5 seconds = 5 seconds
#define TEN_SECOND_COUNT         (23) //10 * .5 seconds = 5 seconds
#define COMPLETE_REVOLUTIONS     (28) // 5 seconds


////// Timer 0
#define TIMER_B0_0_VECTOR               (TIMER0_B0_VECTOR)
#define TIMER_B0_1_OVFL_VECTOR          (TIMER0_B1_VECTOR)
//Timer 1
#define TIMER_B1_0_VECTOR               (TIMER1_B0_VECTOR)
#define TIMER_B1_1_OVFL_VECTOR          (TIMER1_B1_VECTOR)
//Timer 2
#define TIMER_B2_0_VECTOR               (TIMER2_B0_VECTOR)
#define TIMER_B2_1_OVFL_VECTOR          (TIMER2_B1_VECTOR)
////////

// THUMBWHEEL.C MACROS===============================================================
#define SIZE (3)
#define FIVE (5)
// PWM MACROS====================================================================
#define RIGHT_FORWARD_SPEED (TB3CCR1)
#define RIGHT_REVERSE_SPEED (TB3CCR2)
#define LEFT_FORWARD_SPEED (TB3CCR3)
#define LEFT_REVERSE_SPEED (TB3CCR4)
#define WHEEL_OFF           (0)
#define WHEEL_PERIOD         (50000)
#define FAST                 (40000)
#define FULL_SPEED           (50000)
#define MID                  (22600)
#define WHITE_RIGHT          (20000)
#define WHITE_LEFT           (14000)
#define MID_2                (18000)
#define VIBE                 (12000)
#define CRUISE               (12000)
#define CRUISE_BLACK         (10000)
#define PID                   (5500)
#define SLOW                 (9400)
#define SLOW_ADJUST                 (9400)
#define DEAD_ADJUST                 (9100)
#define SLOWER               (7000)
#define DEAD                  (7000)

//detectionStates macros 
#define IDLE            (0)
#define SEARCH          (0)
#define SEARCH_WHITE    (1)
#define FOUND           (2)
#define ADJUST          (3)
#define FINISH          (4)



//Idle State Macros
#define BLACK_LINE_DETECT (1)
#define BLACK_LINE_FOLLOW (2)

// Serial #defines 
#define BEGINNING (0)
#define SMALL_RING_SIZE (16)
#define LARGE_RING_SIZE (30)
// global variables
extern volatile char USB_Rx[LARGE_RING_SIZE] ;
extern volatile char USB_Tx[LARGE_RING_SIZE] ;
extern volatile unsigned int usb_rx_wr;
extern unsigned int usb_rx_ring_rd;
extern volatile unsigned int usb_tx_wr;
extern unsigned int usb_tx_rd;
extern char transmitArray[LARGE_RING_SIZE];
extern char transmitToPC;
extern volatile unsigned char IOT_Tx_buf[LARGE_RING_SIZE];
extern volatile char usb_tx;
extern char pingFlag;
extern volatile char iot_tx;

//New Macros for Project 8--------------------------------------------------
extern volatile unsigned char IOT_Rx[LARGE_RING_SIZE];
extern volatile unsigned int iot_rx_wr;
extern unsigned int iot_rx_rd;
extern unsigned int usb_rx_rd;

//Serial Defines
#define CR (0x0D)
#define LN (0x0A)

/////--------------------------------------------------------------------------


//Sensor Switch states
#define LEFT (0)
#define RIGHT (1)

// CLOCK.C MACROS===============================================================
#define MCLK_FREQ_MHZ           (8) // MCLK = 8MHz
#define CLEAR_REGISTER     (0X0000)

// Port 1 Pins
#define RED_LED (0x01) // 0 RED LED 0
#define V_A1_SEEED (0x02) // 1 A1_SEEED
#define V_DETECT_L (0x04) // 2 V_DETECT_L
#define V_DETECT_R (0x08) // 3 V_DETECT_R
#define V_A4_SEEED (0x10) // 4 A4_SEEED
#define V_THUMB (0x20) // 5 V_THUMB
#define UCA0RXD (0x40) // 6 Back Channel UCA0RXD
#define UCA0TXD (0x80) // 7 Back Channel UCA0TXD

// Port 2 Pins
#define RESET_LCD (0x01) // 0 RESET LCD
#define Check_Bat (0x02) // 1 Check ADC Voltages
#define IR_LED (0x04) // 2 IR_LED
#define SW2 (0x08) // 3 SW2
#define IOT_RUN_CPU (0x10) // 4 IOT_RUN_CPU
#define DAC_ENB (0x20) // 5 DAC_ENB
#define LFXOUT (0x40) // 6 XOUTR
#define LFXIN (0x80) // 7 XINR

// Port 3 Pins
#define TEST_PROBE (0x01) // 0 TEST PROBE
#define OA2O (0x02) // 1 OA2O
#define OA2N (0x04) // 2 OA2N
#define OA2P (0x08) // 3 OA2P
#define SMCLK (0x10) // 4 SMCLK
#define DAC_CNTL (0x20) // 5 DAC signal from Processor
#define IOT_LINK_CPU (0x40) // 6 IOT_LINK_CPU
#define IOT_EN_CPU (0x80) // 7 IOT_EN_CPU

// Port 4 Pins
#define DAC_CNTL1 (0x01) // 0 DAC_CNTR1
#define SW1 (0x02) // 1 SW1
#define UCA1RXD (0x04) // 2 Back Channel UCA1RXD
#define UCA1TXD (0x08) // 3 Back Channel UCA1TXD
#define UCB1_CS_LCD (0x10) // 4 Chip Select
#define UCB1CLK (0x20) // 5 SPI mode - clock output—UCB1CLK
#define UCB1SIMO (0x40) // 6 UCB1SIMO
#define UCB1SOMI (0x80) // 7 UCB1SOMI

//Port 5 Pins
#define V_BAT (0x01)
#define V_5_0 (0x02)
#define V_DAC (0x04)
#define V_3_3 (0x08)
#define IOT_BOOT (0x10)

//Port 6 Pins
#define R_FORWARD (0x01)
#define R_REVERSE (0x02)
#define L_FORWARD (0x04)
#define L_REVERSE (0x08)

#define LCD_BACKLITE (0x10)
#define P6_5 (0x20)
#define GRN_LED (0x40)
