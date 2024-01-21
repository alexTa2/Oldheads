// ------------------------------------------------------------------------------
//
//  Description: This file contains the tests functions for project 5
//
//  Alex Tayyeb
//  Sept 2022
//  Built with IAR Embedded Workbench Version: V7.12.1
// ------------------------------------------------------------------------------
//
//#include  "functions.h"
//#include  "msp430.h"
//#include "macros.h"
//
//extern char time_change;
//extern unsigned int cycle_time;
//extern unsigned int right_motor_count;
//extern unsigned int left_motor_count;
//extern unsigned int delay_start;
//extern char segment_count;
//extern char dirToggle; 
//
//void rightForwardTest(void){
//  if(time_change){
//    time_change = 0;
//    if(dirToggle > 0){
//      
//      driveRight();
//      P6OUT |= GRN_LED; //INITIAL VALUE = ON
//      
//      strcpy(display_line[0], "          ");
//      strcpy(display_line[1], "          ");
//      strcpy(display_line[2], "  Right   ");
//      strcpy(display_line[3], "Forward   ");
//      
//      display_changed = TRUE;
//  
//    }else{
//      
//      stopRight();
//      P6OUT &= ~GRN_LED;
//      
//      
//    }
//  }
//}
//
//void rightReverseTest(void){
//  if(time_change){
//    time_change = 0;
//    if(dirToggle > 0){
//      
//      rightReverse();
//      P6OUT |= GRN_LED; //INITIAL VALUE = ON
//      
//    }else{
//      
//      stopRight();
//      P6OUT &= ~GRN_LED;
//      
//    }
//  }
//}
//
//void leftForwardTest(void){
//  if(time_change){
//    time_change = 0;
//    if(dirToggle > 0){
//      
//      driveLeft();
//      P6OUT |= GRN_LED; //INITIAL VALUE = ON
//      
//    }else{
//      
//      stopLeft();
//      P6OUT &= ~GRN_LED;
//      
//    }
//  }
//}
//
//void leftReverseTest(void){
//  if(time_change){
//    time_change = 0;
//    if(dirToggle > 0){
//      
//      leftReverse();
//      P6OUT |= GRN_LED; //INITIAL VALUE = ON
//      
//    }else{
//      
//      stopLeft();
//      P6OUT &= ~GRN_LED;
//      
//    }
//  }
//}
