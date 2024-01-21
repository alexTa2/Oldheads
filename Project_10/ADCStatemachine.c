#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"
extern unsigned int TMR;
unsigned volatile int gtg = 1;
extern volatile unsigned int IRflg;
unsigned int FLState;
unsigned int temp = 0;
unsigned int maxBlack = 0;
unsigned int maxWhite = 0;
extern volatile unsigned int CalDone;
extern volatile unsigned int StartADCTmr;
unsigned int CalState;
extern unsigned volatile int ADC_Channel;
extern unsigned volatile int ADC_Left_Detect;
extern unsigned volatile int ADC_Right_Detect;
extern unsigned volatile int ADC_Thumb;
extern volatile unsigned int CalTmr;
extern volatile unsigned int ADCTmr;
void ADCStateMachine(void){
  switch(gtg){
  case 1:
    LEFT_FORWARD_SPEED = WHEEL_PERIOD / 3;
    RIGHT_FORWARD_SPEED = WHEEL_PERIOD / 5;
    strcpy(display_line[3], "WHITESPACE");
    if(ADC_Right_Detect >= maxBlack-100 || ADC_Left_Detect >= maxBlack-100){
      gtg++;}
    break;
  case 2:
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    strcpy(display_line[3], "-BLCKLINE-");
    P6OUT ^= IR_LED;
    IRflg = 1;
    update_display = 1;
    display_changed = TRUE;
    if(ADCTmr >= 100){
    P6OUT ^= IR_LED;
    gtg++;}
    break;
  case 3:
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    if(ADCTmr >= 125){
    gtg++;
    ADCTmr = 0;
    }
    strcpy(display_line[3], "- WAITIN -");
    break;
  case 4:
    LEFT_REVERSE_SPEED = WHEEL_PERIOD / 2;
    RIGHT_FORWARD_SPEED = WHEEL_PERIOD / 2;
    if(ADC_Right_Detect >= maxBlack-50 || ADC_Left_Detect >= maxBlack-50){
          gtg++;
    }
    strcpy(display_line[3], "- FINDIN -");
    break;
  case 5:
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    gtg++;
    break;
  case 6:
    FollowLine();
    break;
  default:
    break;
  }
  return;
}

void FollowLine(void){
  switch(FLState){
  case 0:
    //Wait 5s then increment state and reset timer
    if(TMR >= 400){
      FLState++;
      TMR = 0;
    }
    strcpy(display_line[3], " WAIT-ING ");
    update_display = 1;
    break;
  case 1:
    //IS RIGHT DETECTOR OFF LINE?
    if(ADC_Right_Detect < maxBlack-100){
    LEFT_FORWARD_SPEED = WHEEL_OFF; //SLOW DOWN LEFT MOTOR
    RIGHT_FORWARD_SPEED = WHEEL_PERIOD / 3; //SPEED UP RIGHT MOTOR
    }
    //IS LEFT DETECTOR OFF LINE?
    if(ADC_Left_Detect < maxBlack-100){
    LEFT_FORWARD_SPEED = WHEEL_PERIOD / 3; //SPEED UP LEFT MOTOR
    RIGHT_FORWARD_SPEED = WHEEL_OFF; //SLOW DOWN RIGHT MOTOR
    }
    //Wait 200s then increment state and reset timer
    if(TMR >= 7500){
      FLState++;
      TMR = 0;}
    strcpy(display_line[3], "FOLLOWING");
    break;
  case 2:
    //STOP AFTER TWO TIMES AROUND THE CIRCLE
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    FLState++;
    strcpy(display_line[3], " STOPING ");
    break;
  case 3:
    //POINT SELF TO THE CENTER OF THE CIRCLE
    LEFT_REVERSE_SPEED = WHEEL_PERIOD;
    RIGHT_FORWARD_SPEED = WHEEL_PERIOD;
    if(TMR >= 80){
    FLState++;
    TMR = 0;
    }
    strcpy(display_line[3], " SPINING ");
    break;
  case 4:
    //WAIT STATE
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    if(TMR >= 1500){
      FLState++;
      TMR = 0;
    }
    strcpy(display_line[3], " WAITING ");
    break;
  case 5:
    //TRAVEL TO CENTER OF THE CIRCLE
    LEFT_FORWARD_SPEED = WHEEL_PERIOD / 2;
    RIGHT_FORWARD_SPEED = WHEEL_PERIOD / 2;
    if(TMR >= 200){
     FLState++;
     TMR = 0;
    }
    strcpy(display_line[3], "RETURNING");
    break;
  case 6:
    //STOP CAR IN CENTER OF CIRCLE
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    strcpy(display_line[3], " PARK-ING ");
    StartADCTmr = 0;
  default:
    break;
  }
  return;
}

void CalSM(void){
  switch(CalState){
  case 0:
    if(CalTmr >= 500){
      CalState++;
      CalTmr = 0;
    }
case 1:
    CalWhite();
    if(CalTmr >= 1500){
      CalState++;
      CalTmr = 0;
    }
  break;
case 2:
  CalBlack();
  if(CalTmr >= 1500){
      CalState++;
      CalTmr = 0;
  }
  break;
case 3:
  display_line[3][0] = 'C'; 
  display_line[3][1] = 'A';
  display_line[3][2] = 'L';
  display_line[3][3] = ' ';
  display_line[3][4] = 'D';
  display_line[3][5] = 'O';
  display_line[3][6] = 'N';
  display_line[3][7] = 'E';
  if(CalTmr >= 1000){
      StartADCTmr = 1;
  }
  break;
default:
  break;
}
}

void CalBlack(void){
if(ADC_Right_Detect > maxBlack){
      maxBlack = ADC_Right_Detect;
    }
    if (ADC_Left_Detect > maxBlack){
      maxBlack = ADC_Left_Detect;
    }
    
    strcpy(display_line[3], "Cal Black");
}

void CalWhite(void){
  if(ADC_Right_Detect > maxWhite){
      maxWhite = ADC_Right_Detect;
    }
    if (ADC_Left_Detect > maxWhite){
      maxWhite = ADC_Left_Detect;
    }  
    strcpy(display_line[3], "Cal White");
}

