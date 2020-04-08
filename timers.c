/*
 * timers.c
 *
 *  Created on: Apr 8, 2020
 *      Author: Gabriel
 */

#include <timers.h>

//Initializes TA0
void initTimerA0 (void){
    TA0CCTL0 = CCIE;                // TACCR0 interrupt enabled
    TA0CTL = TASSEL_2 +ID__1;       // SMCLK, no clock divider
}

//Sets the timer TA0 by changing the CCR (number of counts)
void setTimerA0 (unsigned int counts){
    TA0CCR0 = counts;
}

//Starts timer TA0
void startTimerA0(void){
    TA0CTL |= MC_1;                 // Up Mode start
   // TA0CTL = TASSEL_2 +ID__1 +  MC_1;
}

//Stops timer TA0
void stopTimerA0(void){
    TA0CCR0 = 0;
}

// Reads the set parameter of CCR for Timer A0
unsigned int readTimerA0(void){
    return TA0CCR0;
}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
 // P1OUT ^= BIT0;
 __bic_SR_register_on_exit(CPUOFF);// Wake up
}

