/*
 * uscia0_spi.c
 *
 *  Created on: May 7, 2020
 *      Author: Gabriel
 */
#include "uscia0_spi.h"

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    volatile unsigned int i;

  switch(__even_in_range(UCA0IV,4))
  {
    case 0: break;                          // Vector 0 - no interrupt
    case 2:                                 // Vector 2 - RXIFG         --> receive interruption
      /*while (!(UCA0IFG&UCTXIFG));           // USCI_A0 TX buffer ready?

      if (UCA0RXBUF==SLV_Data)              // Test for correct character RX'd
        P1OUT |= 0x01;                      // If correct, light LED
      else
        P1OUT &= ~0x01;                     // If incorrect, clear LED

      MST_Data++;                           // Increment data
      SLV_Data++;
      UCA0TXBUF = MST_Data;                 // Send next value

      for(i = 20; i>0; i--);                // Add time between transmissions to
                                            // make sure slave can process information

      */
      break;
    case 4: break;                          // Vector 4 - TXIFG
    default: break;
  }
}

void spiInit(void){
    volatile unsigned int i;
    // Set P1.1 for slave reset
   // P1DIR |= 0x03;                            // Set P1.0-2 to output direction
    P3SEL |= BIT3 + BIT4;                       // P3.3,4 option select
    P2SEL |= BIT7;                            // P2.7 option select

    UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA0CTL0 |= UCMST + UCSYNC + UCCKPL + UCMSB;    // 3-pin, 8-bit SPI master
    // Clock polarity high, MSB
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 0x01;                           // No divider
    UCA0BR1 = 0;                              //
    UCA0MCTL = 0;                             // No modulation
    UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
    UCA0IE &= ~UCRXIE;                         // Enable USCI_A0 RX interrupt

 //   P1OUT &= ~0x02;                         // Now with SPI signals initialized,
   // P1OUT |= 0x02;                            // reset slave

    for(i=50;i>0;i--);                        // Wait for slave to initialize
}

// Sends one byte to the LED strip by SPI.
void sendByte_SPIA (unsigned char b){

    unsigned char bit;
    for (bit = 0; bit < 8; bit++){
      if (b & 0x80){                                // For each bit, we will send the encoded data

          while (!(UCA0IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
          UCA0TXBUF = WS2812_ONE;                   // If the color bit = 1 --> sends a WS2812 ONE

      }
      else
      {
          while (!(UCA0IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
          UCA0TXBUF = WS2812_ZERO;                  // If the color bit = 0 --> sends a WS2812 ZERO
      }
      b <<= 1;                      // shift next bit into high-order position
    }
}


