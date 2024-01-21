#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"
unsigned int ugh = 0;
volatile unsigned int RnT = 0;
extern volatile unsigned int stateTmr1;
extern volatile unsigned int stateTmr2;
extern volatile unsigned int forward_go;
extern volatile unsigned int backward_go;
extern volatile unsigned int turnRight;
extern volatile unsigned int password;
unsigned int TMR1Seqeunce;
unsigned int startTmr;
volatile unsigned int IPTmr;
extern unsigned int ipTmrStart;
volatile unsigned int IOTTmr;
volatile unsigned int IOTSMTmr;
extern volatile unsigned int IPRead;
unsigned int CalTmr;
volatile unsigned int StartADCTmr;
volatile unsigned int ADCTmr;
volatile unsigned int TMRtimer;
unsigned int TMRTimeSequence;
unsigned int ADCTimeSequence;
unsigned int IRTMR;
volatile unsigned int IRflg;
unsigned int TMR;
void Init_Timers(void){
  Init_Timer_B0();
  Init_Timer_B1();
  Init_Timer_B3();
}
//------------------------------------------------------------------------------
// Timer B0 initialization sets up both B0_0, B0_1-B0_2 and overflow
void Init_Timer_B0(void) {
  TB0CTL = TBSSEL__SMCLK; // SMCLK source
  TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
  TB0CTL |= MC__CONTINOUS; // Continuous up
  TB0CTL |= ID__2; // Divide clock by 2
  TB0EX0 = TBIDEX__8; // Divide clock by an additional 8
  TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
  TB0CCTL0 |= CCIE; // CCR0 enable interrupt
  TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
  TB0CCTL1 &= ~CCIE; // CCR1 enable interrupt
  TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
  TB0CCTL2 &= ~CCIE; // CCR2 enable interrupt
  TB0CTL &= ~TBIE; // Disable Overflow Interrupt
  TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}
void Init_Timer_B1(void) {
  TB1CTL = TBSSEL__SMCLK; // SMCLK source
  TB1CTL |= TBCLR; // Resets TB0R, clock divider, count direction
  TB1CTL |= MC__CONTINOUS; // Continuous up
  TB1CTL |= ID__8; // Divide clock by 8
  
  TB1EX0 = TBIDEX__8; // Divide clock by an additional 8
  
  TB1CCR0 = TB1CCR0_INTERVAL; // CCR0
  TB1CCTL0 |= CCIE; // CCR0 enable interrupt
  
  TB1CCR1 = TB1CCR1_INTERVAL; // CCR1
  TB1CCTL1 |= CCIE; // CCR1 enable interrupt
  
  TB1CCR2 = TB1CCR2_INTERVAL; // CCR2
  TB1CCTL2 |= CCIE; // CCR2 enable interrupt
  
  TB1CTL |= TBIE; // ENABLE Overflow Interrupt
  TB1CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}

//------------------------------------------------------------------------------
void Init_Timer_B3(void) {
  //------------------------------------------------------------------------------
  // SMCLK source, up count mode, PWM Right Side
  // TB3.1 P6.0 R_FORWARD
  // TB3.2 P6.1 L_FORWARD
  // TB3.3 P6.2 R_REVERSE
  // TB3.4 P6.3 L_REVERSE
  //------------------------------------------------------------------------------
  TB3CTL = TBSSEL__SMCLK; // SMCLK
  TB3CTL |= MC__UP; // Up Mode
  TB3CTL |= TBCLR; // Clear TAR
  TB3CCR0 = WHEEL_PERIOD; // PWM Period
  TB3CCTL1 = OUTMOD_7; // CCR1 reset/set
  RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM duty cycle
  TB3CCTL2 = OUTMOD_7; // CCR2 reset/set
  LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Left Forward PWM duty cycle
  TB3CCTL3 = OUTMOD_7; // CCR3 reset/set
  RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM duty cycle
  TB3CCTL4 = OUTMOD_7; // CCR4 reset/set
  LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM duty cycle
  //------------------------------------------------------------------------------
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
//------------------------------------------------------------------------------
// TimerB0 0 Interrupt handler
//----------------------------------------------------------------------------  
    Time_Sequence++;
    IOTTmr++;
    if(IOTTmr >= 800){
      IOTSMTmr++;
    }
    if(Time_Sequence == 50){
      ugh = 1;
      Time_Sequence = 0;
      P3OUT |= IOT_EN_CPU;
      if(P3OUT & IOT_EN_CPU){
        P3OUT ^= IOT_LINK_CPU;
      }
      if(IPRead){
      IPTmr++;
      }
      /*if(ipTmrStart){
      IPTmr++;
    }*/
    }
    
  TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0
//----------------------------------------------------------------------------
}
 #pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
//----------------------------------------------------------------------------
// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
switch(__even_in_range(TB0IV,14)){
case 0: break; // No interrupt
case 2: // CCR1 not used
P4IFG &= ~SW1;
P4IE |= SW1;
P6IFG |= SW1;
TB0CCR1 += TB0CCR1_INTERVAL; // Add Offset to TBCCR1
ugh++;
CalTmr++;
CalSM();
if (ugh >= 4000){
TB0CCTL1 &= ~CCIE;
TB0CCTL1 &= ~CCIFG;
TB0CCTL0 |= CCIE;
TB0CCR0 = TB0R + TB0CCR0_INTERVAL;
P3OUT ^= LCD_BACKLITE;
ugh = 0;
}
break;
case 4: // CCR2 not used
P2IFG &= ~SW2;
P2IE |= SW2;
TB0CCR2 += TB0CCR2_INTERVAL;// Add Offset to TBCCR2
ugh++;
if (ugh == 25){
TB0CCTL2 &= ~CCIE;
TB0CCTL2 &= ~CCIFG;
TB0CCTL0 |= CCIE;
TB0CCR0 = TB0R + TB0CCR0_INTERVAL;
P3OUT &= ~LCD_BACKLITE;
UCA0MCTLW = 0x4911;
UCA0BRW = 52;
ugh = 0;
}
break;
case 14: // overflow
//...... Add What you need happen in the interrupt ......
break;
default: break;
}
//----------------------------------------------------------------------------
}

#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void){
  if(startTmr){
  TMR1Seqeunce++;
    if(TMR1Seqeunce >= 50){
      stateTmr1++;
      stateTmr2++;
      TMR1Seqeunce = 0;
    }
  }
  ADCTimeSequence++;
  if(ADCTimeSequence >= 20){
    ADCCTL0 |= ADCSC;
  }
  if(StartADCTmr){
    TMRTimeSequence++;
  if(TMRTimeSequence >= 80){
    TMRtimer++;
    TMR++;
    ADCTmr++;
  }
  }
  IRTMR++;
  if(IRflg){
    if(IRTMR >= 25){
    P6OUT ^= GRN_LED;
    IRTMR = 0;
    }
  }
  TB1CCR0 += TB1CCR0_INTERVAL; // Add Offset to TBCCR0
}

#pragma vector=TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void){
//----------------------------------------------------------------------------
// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
switch(__even_in_range(TB1IV,14)){
case 0: break; // No interrupt
case 2: // CCR1 not used

TB1CCR1 += TB1CCR1_INTERVAL; // Add Offset to TBCCR1

break;
case 4: // CCR2 not used

TB1CCR2 += TB1CCR2_INTERVAL;
break;
case 14: // overflow
//...... Add What you need happen in the interrupt ......
break;
default: break;
}
//----------------------------------------------------------------------------
}


