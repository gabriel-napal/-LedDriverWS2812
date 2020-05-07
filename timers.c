/*
 * timers.c
 *
 *  Created on: May 7, 2020
 *      Author: Gabriel
 */


#include "timers.h"

// Timer0 A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TIMER0_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    __bic_SR_register_on_exit(CPUOFF);// Wake up
}


void initTimerA0 (void){

    TA0CCTL0 = CCIE;                          // CCR0 interrupt enabled
    TA0CTL = TASSEL_1 + TACLR;         // SMCLK, upmode, clear TAR

}

void setTimerA0 (unsigned int counts){
    TA0CCR0 = counts;
}

void startTimerA0(void){
    TA0CTL |= MC_1;
}
