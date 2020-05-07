/*
 * clock.c
 *
 *  Created on: May 7, 2020
 *      Author: Gabriel
 */

#include "clock.h"

void ucsInit(void){


    UCSCTL3 = SELREF__REFOCLK;                 //Set DCO FLL reference = REFO
    UCSCTL4 |= SELA_2 ;                        // Set ACLK = REFO, MCLK = SMCLK  = DCO
    UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx

    // Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
      do
      {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
                                                // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
      }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

      __bis_SR_register(SCG0);                  // Disable the FLL control loop
      UCSCTL1 = DCORSEL_5;                      // Select DCO range 16MHz operation
      UCSCTL2 |= 365  ;                           // Set DCO Multiplier for 12MHz
                                                // (N + 1) * FLLRef = Fdco
                                                // (365 + 1) * 32k = 12MHz

      UCSCTL5 |= DIVS__2  + DIVA__4;                      // Set SMCLK = fDCO /2, MCLK = DCO, ACLK = REFO/4 (8192 Hz)
      __bic_SR_register(SCG0);                  // Enable the FLL control loop

      // Worst-case settling time for the DCO when the DCO range bits have been
      // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
      // UG for optimization.
      // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
      __delay_cycles(250000);
}
