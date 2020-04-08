/*
 * clock.c
 *
 *  Created on: Apr 3, 2020
 *      Author: Gabriel
 */

#include <clock.h>
#include <msp430.h>

void CS_init (void){

    CSCTL0_H = 0xA5;                            //Power Up Clear
    CSCTL1 |= DCORSEL;                          // Sets Clock sources to maximum range
    CSCTL1 |= DCOFSEL0 + DCOFSEL1;              // Set max. DCO setting (24Mhz)
    CSCTL2 = SELA__DCOCLK + SELS__VLOCLK + SELM_3;    // Set clock sources ACLK; SUBCLK = VLOCLK;  MCLK = DCO
    CSCTL3 = DIVA__4 + DIVS__1 + DIVM_3;         // set ACLK to 6 Mhz, SUBCLK to 8.42kHz and MCLK to 4Mhz

}
