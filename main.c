/*
 * LedDriverWS2812
 * Version : 0.0.1
 *
 * Authors :
 *              L. Innocente
 *              G. Napal
 *
 * Connections:
 *
 *   LaunchPad      LED Strip
 *   ---------      ---------
 *   3V3            5VDC (it works with 3V3)s
 *   P2.0(MOSI)     DIN
 *   GND            GND
 */

#include <msp430.h>
#include <clock.h>
#include <uscia0_spi.h>
#include <timers.h>

#define PIXELS 3


void lightsOff(void);                                                   //Turns all LEDS OFF
void sendPixel (unsigned char r, unsigned char g, unsigned char b);
void sendByte (unsigned char b);
void sendFrame (unsigned char LEDS[PIXELS][3]);

/* Visual function */
void xmasTinsel (unsigned char LEDS[PIXELS][3]);
void visual1(unsigned char LEDS[PIXELS][3]);
void visual2(unsigned char LEDS[PIXELS][3]);

unsigned char RXData =0;
unsigned char TXData;

//unsigned char LEDS[4][3] = {{0xFF,0x00,0x00},{0x00,0xFF,0x00},{0x00,0x00,0xFF},{0xFF,0xA5,0x00}};
unsigned char LEDS[3][3] = {{0xFF,0x00,0x00},{0x00,0xFF,0x00},{0x00,0x00,0xFF}};

static unsigned char auxPixel[3];

int main(void)
{
  volatile unsigned int i;
  volatile unsigned int j;
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer

  CS_init();                                //Initializes all 3 clocks
  SPI_init();
  lightsOff();

 /* initTimerA0();
  setTimerA0(TIMERA0_1SEC);
  startTimerA0();
*/
  while(1){

      sendFrame(LEDS);

 /*     for(j=0 ; j<5;j++)
          __delay_cycles(65535);
      lightsOff();*/
      for(j=0 ; j<15;j++)
          __delay_cycles(65535);

      xmasTinsel(LEDS);
     // __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
  }
/*
  while(1)
  {
    UCA0TXBUF = TXData;
    __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
    __no_operation();                       // Remain in LPM0
    __delay_cycles(20000);                   // Delay before next transmission
  }
*/
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
           RXData = UCA0RXBUF;
           UCA0IFG &= ~UCRXIFG;
           __bic_SR_register_on_exit(CPUOFF);// Wake up to setup next TX
           break;
    case 4:
          UCA0TXBUF = TXData;             // Transmit characters
          UCA0IE &= ~UCTXIE;
          break;
    default: break;
  }
}

/*
 * WS2812 DRIVER FUNCTION DEFINITIONS
 */

// send the full frame
void sendFrame (unsigned char LEDS[PIXELS][3]){
    int i;
    for ( i=0; i<PIXELS;i++)
        sendPixel(LEDS[i][0], LEDS[i][1],LEDS[i][2]);
}


// Send a single pixel worth of information.  Turn interrupts off while using.
void sendPixel (unsigned char r, unsigned char g, unsigned char b){
    sendByte_SPIA (g);        // NeoPixel wants colors in green-then-red-then-blue order
    sendByte_SPIA (r);
    sendByte_SPIA (b);
}


/*
 * VISUAL FUNCTION DEFINITIONS
 */

void lightsOff(void){

    unsigned int i;

    for (i = 0 ; i < PIXELS ; i++)
        sendPixel(0x00,0x00,0x00);
}

void xmasTinsel (unsigned char LEDS[PIXELS][3]){


    int j;
    unsigned char foo1;
    unsigned char foo2;
    unsigned char foo3;
    /*
    auxPixel[0] = LEDS[0][0];
    auxPixel[1] = LEDS[0][1];
    auxPixel[2] = LEDS[0][2];
*/
    foo1 = LEDS[0][0];
    foo2 = LEDS[0][1];
    foo3 = LEDS[0][2];

    for(j=0 ; j<PIXELS-1 ; j++){

        LEDS[j][0] = LEDS[j+1][0];
        LEDS[j][1] = LEDS[j+1][1];
        LEDS[j][2] = LEDS[j+1][2];

    }

  /*  LEDS[PIXELS][0] = auxPixel[0];
    LEDS[PIXELS][1] = auxPixel[1];
    LEDS[PIXELS][2] = auxPixel[2];
    */
    LEDS[PIXELS-1][0] = foo1;
    LEDS[PIXELS-1][1] = foo2;
    LEDS[PIXELS-1][2] = foo3;


}


void visual1(unsigned char LEDS[PIXELS][3]){
    int j;
    for(j=0 ; j<PIXELS ; j++){
        LEDS[j][0] = LEDS[j][0] + 5;
        LEDS[j][1] = LEDS[j][1] + 5;
        LEDS[j][2] = LEDS[j][2] + 5;
    }
}

void visual2(unsigned char LEDS[PIXELS][3]){
    int j;

    for(j=0 ; j<PIXELS ; j++){
        if ((j % 2) == 0) {
            LEDS[j][0] = LEDS[j][0] - 5;
            LEDS[j][2] = LEDS[j][2] + 5;
        }
        else {
            LEDS[j][0] = LEDS[j][0] + 5;
            LEDS[j][2] = LEDS[j][2] - 5;
        }
    }
}

