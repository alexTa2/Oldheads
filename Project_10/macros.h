//===========================================================================
// File Name : macros.h
//
// Description: This file contains the macros used by all the other files
//
// Author: Solomon Ross
// Date: February, 2022
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (5.40.1)
//===========================================================================
extern char display_line[4][11];
extern char *display[4];

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
#define NONE ('N')
#define STRAIGHT ('L')
#define CIRCLE ('C')
#define WAIT ('W')
#define START ('S')
#define RUN ('R')
#define END ('E')
#define TURN ('T')
#define WHEEL_COUNT_TIME (10)
#define RIGHT_COUNT_TIME (9)
#define LEFT_COUNT_TIME (9)
#define LT_TURN_RIGHT_COUNT_TIME (0)
#define LT_TURN_LEFT_COUNT_TIME (10)
#define RT_TURN_RIGHT_COUNT_TIME (10)
#define RT_TURN_LEFT_COUNT_TIME (0)
#define CIRCLE_TRAVEL_DISTANCE (185)
#define FIG8_TRAVEL_DISTANCE (90)
#define TURN_TRAVEL_DISTANCE (40)
#define TRAVEL_DISTANCE (10)
#define WAITING2START (100)
#define PRESSED (0x01)
#define NOT_OKAY (0x00)
#define DEBOUNCE_RESTART (0)
#define DEBOUNCE_TIME (10000)
#define RELEASED (0x02)

#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED                 (0x01) // RED LED 0
#define GRN_LED                 (0x40) // GREEN LED 1
#define TEST_PROBE              (0x01) // 0 TEST PROBE
#define TRUE                    (0x01) //

//Port 1 Pins
#define A1_SPEED        (0x02)
#define V_DETECT_L      (0x04)
#define V_DETECT_R      (0x08)
#define SMCLK_2476      (0x10)
#define V_THUMB         (0x20)
#define UCA0RXD         (0x40)
#define UCA0TXD         (0x80)

// Port 2 Pins
#define RESET_LCD       (0x01) // 0 RESET LCD
#define L_REVERSE_2476  (0x02) // 1 L_REVERSE FOR MSP430FR2476
#define P2_2            (0x04) // 2
#define SW2             (0x08) // 3 SW2
#define IOT_RUN_CPU     (0x10) // 4 IOT_RUN_CPU
#define DAC_ENB         (0x20) // 5 DAC_ENB
#define LFXOUT          (0x40) // 6 XOUTR
#define LFXIN           (0x80) // 7 XINR

// Port 3 Pins
#define TEST_PROBE (0x01) // 0 TEST PROBE
#define DAC_CNTL1 (0x02) // 1 DAC_CNTL1
#define OA2N (0x04) // 2 Photodiode Circuit
#define LCD_BACKLITE (0x08) // 3 LCD_BACKLITE
#define SMCLK_2355 (0x10) // 4 SMCLK
#define DAC_CNTL (0x20) // 5 DAC signal from Processor
#define IOT_LINK_CPU (0x40) // 6 IOT_LINK_CPU
#define IOT_EN_CPU (0x80) // 7 IOT_EN_CPU

// Port 4 Pins
#define P4_0 (0x01) // 0 4_0
#define SW1 (0x02) // 1 SW1
#define UCA1RXD (0x04) // 2 Back Channel UCA1RXD
#define UCA1TXD (0x08) // 3 Back Channel UCA1TXD
#define UCB1_CS_LCD (0x10) // 4 Chip Select
#define UCB1CLK (0x20) // 5 SPI mode - clock output—UCB1CLK
#define UCB1SIMO (0x40) // 6 UCB1SIMO
#define UCB1SOMI (0x80) // 7 UCB1SOMI
#define P4PUD (P4OUT)

//Port 5 Pins
#define CHECK_BAT       (0x01)
#define V_BAT           (0x02)
#define V_DAC           (0x04)
#define V_3_3           (0x08)
#define IOT_BOOT_CPU    (0x10)

//Port 6 Pins
#define R_FORWARD       (0x01)
#define L_FORWARD       (0x02)
#define R_REVERSE       (0x04)
#define L_REVERSE_2433  (0x08)
#define IR_LED          (0x10)
#define P6_5            (0x20)
#define GRN_LED         (0x40)

// STATES ======================================================================
#define NONE ('N')
#define STRAIGHT ('L')
#define CIRCLE ('C')
#define WAIT ('W')
#define START ('S')
#define RUN ('R')
#define END ('E')
#define FIG8 ('F')
#define CCLKWSE ('U')
#define OKAY (1)

//PWM ==========================================================================
#define RIGHT_FORWARD_SPEED (TB3CCR1)
#define LEFT_FORWARD_SPEED (TB3CCR2)
#define RIGHT_REVERSE_SPEED (TB3CCR3)
#define LEFT_REVERSE_SPEED (TB3CCR4)
#define WHEEL_OFF (0)
#define WHEEL_PERIOD (50005)

// Timers ======================================================================
#define TB0CCR0_INTERVAL (2500)
#define TB0CCR1_INTERVAL (2500)
#define TB0CCR2_INTERVAL (2500)
#define TB1CCR0_INTERVAL (2500)
#define TB1CCR1_INTERVAL (2500)
#define TB1CCR2_INTERVAL (2500)

//Serial================================================================
// #defines
#define BEGINNING (0)
#define SMALL_RING_SIZE (16)
#define LARGE_RING_SIZE (32)