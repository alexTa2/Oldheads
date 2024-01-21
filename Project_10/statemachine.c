#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"
extern unsigned int time_change;
extern unsigned int delay_start;
extern char state;
extern char event;
extern unsigned int cycle_time;
extern unsigned int right_motor_count;
extern unsigned int left_motor_count;
extern unsigned int segment_count;


void wait_case(void){
 if(time_change){
 time_change = 0;
 if(delay_start++ >= WAITING2START){
 delay_start = 0;
 state = START;
 }
 }
}

void start_case(void){
 cycle_time = 0;
 right_motor_count = 0;
 left_motor_count = 0;
 Forward_On();
 segment_count = 0;
 state = RUN;
}

void run_case(void){
 if(time_change){
 time_change = 0;
 if(segment_count <= TRAVEL_DISTANCE){
 if(right_motor_count++ >= RIGHT_COUNT_TIME){
 P6OUT &= ~R_FORWARD;
 }
 if(left_motor_count++ >= LEFT_COUNT_TIME){
 P6OUT &= ~L_FORWARD;
 }
 if(cycle_time >= WHEEL_COUNT_TIME){
 cycle_time = 0;
 right_motor_count = 0;
 left_motor_count = 0;
 segment_count++;
 Forward_Move();
 }
 }else{
 state = END;
 }
 }
}

void end_case(void){
 Forward_Off();
 state = WAIT;
 event = NONE;
}