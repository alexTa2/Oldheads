#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

unsigned int delay_start = 0;
extern char state;
extern unsigned int cycle_time;
unsigned int right_motor_count = 0;
unsigned int left_motor_count = 0;
unsigned int segment_count = 0;
unsigned int turn_count = 0;
unsigned int fig_count = 0;
extern unsigned int Vibe;
volatile unsigned int stateTmr1;
volatile unsigned int stateTmr2;
extern unsigned int startTmr;
void wait_case(void){
  if(stateTmr2 >= 5){
    state = START;
    stateTmr2 = 0;
  }
}

void start_case(void){
 state = RUN;
}


void rightTurn(){
  RIGHT_FORWARD_SPEED = WHEEL_PERIOD / 4;
  LEFT_REVERSE_SPEED = WHEEL_PERIOD / 4;
}

void leftTurn(){
  LEFT_FORWARD_SPEED = WHEEL_PERIOD / 4;
  RIGHT_REVERSE_SPEED = WHEEL_PERIOD / 4;
}

void end_case(void){
 Forward_Off();
}

void Forward_Move(void){
    RIGHT_FORWARD_SPEED = WHEEL_PERIOD / 2;
    LEFT_FORWARD_SPEED = WHEEL_PERIOD / 2;
}

void Forward_Off(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}
void Reverse_Run(void){
  LEFT_REVERSE_SPEED = WHEEL_PERIOD / 3;
  RIGHT_REVERSE_SPEED = WHEEL_PERIOD / 3;
}

void Run_Straight(void){
 switch(state){
 case WAIT:
 wait_case();
 case START: // Begin
 start_case();
 break; //
 case RUN: // Run
 Forward_Move();
 break; //
 case END: // End
 end_case();
 startTmr = 0;   
 state = WAIT;
 break; //
 default: break;
 }
}

void Run_Straight_Back(void){
 switch(state){
 case WAIT:
 wait_case();
 case START: // Begin
 start_case();
 break; //
 case RUN: // Run
 Reverse_Run();
 break; //
 case END: // End
 end_case();
 startTmr = 0;
 break; //
 default: break;
 }
}

void Run_rightTurn(void){
 switch(state){
 case WAIT:
 wait_case();
 break;
 case START: // Begin
 start_case();
 break; //
 case RUN: // Run
 rightTurn();
 break; //
 case END: // End
 end_case();
 startTmr = 0;
 state = WAIT;
 break; //
 default: break;
 }
}

void Run_leftTurn(void){
 switch(state){
 case WAIT:
 wait_case();
 break;
 case START: // Begin
 start_case();
 break; //
 case RUN: // Run
 leftTurn();
 break; //
 case END: // End
 end_case();
 startTmr = 0;
 state = WAIT;
 break; //
 default: break;
 }
}
/*
void Run_Circle(void){
  switch(state){
 case WAIT: // Begin
 wait_case();
 break; //
 case START: // Begin
 start_case();
 break; //
 case END: // End
 end_case();
 case RUN:
   Turn_run_case4();
   break;
 break; //
 default: break;
 }
}

void Run_Fig(void){
  switch(state){
 case WAIT: // Begin
 wait_case();
 break; //
 case START: // Begin
 start_case();
 break; //
 case END: // End
 end_case();
 case RUN:
   Turn_run_case2();
   break;
  case CCLKWSE:
    Turn_run_case3();
    break; //
 default: break;
 }
}

void Carlson_StateMachine(void){
    switch(Vibe){
      case 9:                        //
        P3OUT |= LCD_BACKLITE;
        Vibe = 0;             //
        break;
      case 8:
        strcpy(display_line[0], "   WAIT   ");
        update_display = 1;
        display_changed = TRUE;
        Display_Process();
        P6OUT &= ~R_FORWARD;
        P6DIR &= ~R_FORWARD;
        P6OUT &= ~L_REVERSE_2433;
        P6DIR &= ~L_REVERSE_2433;
        break;
      case 7:
        strcpy(display_line[0], "   SPIN   ");
        update_display = 1;
        display_changed = TRUE;
        Display_Process();
        P6OUT |= R_FORWARD;
        P6DIR |= R_FORWARD;
        P6OUT |= L_REVERSE_2433;
        P6DIR |= L_REVERSE_2433;
        break;
      case 6:
        strcpy(display_line[0], "   WAIT   ");
        update_display = 1;
        display_changed = TRUE;
        Display_Process();
        P6OUT &= ~GRN_LED;
        P6OUT &= ~R_REVERSE;
        P6DIR &= ~R_REVERSE;
        P6OUT &= ~L_FORWARD;
        P6DIR &= ~L_FORWARD;
        break;
      case 5:
        strcpy(display_line[0], "-BEYBLADE-");
        update_display = 1;
        display_changed = TRUE;
        Display_Process();
        P6OUT &= ~GRN_LED;
        P6OUT |= R_REVERSE;
        P6DIR |= R_REVERSE;
        P6OUT |= L_FORWARD;
        P6DIR |= L_FORWARD;
        break;
      case 4:                        //
        strcpy(display_line[0], "   WAIT   ");
        update_display = 1;
        display_changed = TRUE;
        Display_Process();
        P6OUT &= ~GRN_LED;
        P6OUT &= ~R_REVERSE;
        P6DIR &= ~R_REVERSE;
        P6OUT &= ~L_REVERSE_2433;
        P6DIR &= ~L_REVERSE_2433;
        break;
      case 3:                        
          strcpy(display_line[0], " REVERSE ");
          update_display = 1;
          display_changed = TRUE;
          Display_Process();
          P6OUT |= GRN_LED;            // Change State of LED 5
          P1OUT |= RED_LED;
          P6OUT |= R_REVERSE;
          P6DIR |= R_REVERSE;
          P6OUT |= L_REVERSE_2433;
          P6DIR |= L_REVERSE_2433;
        break;
      case 2:                         //
        strcpy(display_line[0], "   WAIT   ");
        update_display = 1;
        display_changed = TRUE;
        Display_Process();
        P6OUT &= ~GRN_LED;
        P1OUT |= RED_LED;
        Forward_Off();
        break;
      case  1:                        //
          strcpy(display_line[0], "  FORWARD ");
          update_display = 1;
          display_changed = TRUE;
          Display_Process();
          P6OUT |= GRN_LED;            // Change State of LED 5
          Forward_On();
        break;                         //
      default: break;
    }
}
*/

