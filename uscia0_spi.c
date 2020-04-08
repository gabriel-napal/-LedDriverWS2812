/*
 * uscia0_spi.c
 *
 *  Created on: Apr 3, 2020
 *      Author: Gabriel
 */

#include <msp430.h>
#include <uscia0_spi.h>

void SPI_init(void){

    P1SEL1 |= BIT5;                             //Selects P1.5 to tertiary function
    P2SEL1 |= BIT0 + BIT1;                      //Selects P2.0 and P2.1 to tertiary function
    UCA0CTLW0 |= UCSWRST;                       // **Put state machine in reset**
    UCA0CTLW0 |= UCMST+UCSYNC+UCCKPL+UCMSB;     // 3-pin, 8-bit SPI master
                                                // Clock polarity high, MSB
    UCA0CTLW0 |= UCSSEL_1;                      // BRCLK set to ACLK
    UCA0BR0 = 0;                                // No frequency divider
    UCA0BR1 = 0;
    UCA0MCTLW = 0;                            // No modulation
    UCA0CTLW0 &= ~UCSWRST;                    // **Initialize USCI state machine**
    UCA0IE &= ~UCRXIE;                         // Disable USCI_A0 RX interrupt
}

// Sends one byte to the LED strip by SPI.
void sendByte_SPIA (unsigned char b){

    unsigned char bit;
    for (bit = 0; bit < 8; bit++){
      if (b & 0x80){                                // For each bit, we will send the encoded data

          while (!(UCA0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
          UCA0TXBUF = WS2812_ONE;                   // If the color bit = 1 --> sends a WS2812 ONE

      }
      else
      {
          while (!(UCA0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
          UCA0TXBUF = WS2812_ZERO;                  // If the color bit = 0 --> sends a WS2812 ZERO
      }
      b <<= 1;                      // shift next bit into high-order position
    }
}

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

  switch(__even_in_range(UCA0IV,0x04))
  {
    case 0: break;                          // Vector 0 - no interrupt
    case 2:
           //RXData = UCA0RXBUF;            // Hold the data somewhere (define variable)
           UCA0IFG &= ~UCRXIFG;
           __bic_SR_register_on_exit(CPUOFF);// Wake up to setup next TX
           break;
    case 4:
          //UCA0TXBUF = TXData;             // get data from somewhere
          UCA0IE &= ~UCTXIE;
          break;
    default: break;
  }
}
