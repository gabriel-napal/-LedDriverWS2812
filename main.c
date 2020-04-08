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
 *
 *   Program Flow :
 *
 * 1) Initialization of clocks, SPI, LED Table and Timer
 * 2) Timer starts
 * 3) Sends First Frame
 * 4) Next LED composition is calculated (but not sent)
 * 5) CPU goes to LPM (Low Power Mode), nothing is executed.
 * 6) When the TimerA0 "ticks", CPU is woke up, the main execution loop proceeds (Back to step 3)
 *
 *
 *  General configuration :
 *
 *  CPU : working at 4 Mhz
 *  SPI : CLK is set to 6 MHz
 *  TimerA0 : uses a 8.42kHz clock. Its internal counter can be used to increase or reduce the main loop frequency
 *          Since we're using LPM and Timer interruptions, code is currently Non-Blocking --> cool :)
 *
 */

#include <msp430.h>
#include <clock.h>
#include <uscia0_spi.h>
#include <timers.h>

#define PIXELS 4


void lightsOff(void);                                                   //Turns all LEDS OFF
void sendPixel (unsigned char r, unsigned char g, unsigned char b);
void sendByte (unsigned char b);
void sendFrame (unsigned char LEDS[PIXELS][3]);

/* Visual function */
void xmasTinsel (unsigned char LEDS[PIXELS][3]);
void visual1(unsigned char LEDS[PIXELS][3]);
void visual2(unsigned char LEDS[PIXELS][3]);


unsigned char LEDS[4][3] = {{0xDC,0x14,0x3C},{0x00,0x64,0x00},{0x00,0xBF,0xFF},{0xFF,0xA5,0x00}};
//unsigned char LEDS[3][3] = {{0xFF,0x00,0x00},{0x00,0xFF,0x00},{0x00,0x00,0xFF}};

static unsigned char auxPixel[3];

int main(void)
{
  volatile unsigned int i;
  volatile unsigned int j;
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer

  CS_init();                                //Initializes all 3 clocks
  SPI_init();
  lightsOff();

  initTimerA0();
  setTimerA0(TIMERA0_1SEC/4);
  startTimerA0();


  while(1){

      sendFrame(LEDS);
      xmasTinsel(LEDS);

      __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
      //__no_operation();                       // Equivalent to an assembly NOP
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

    auxPixel[0] = LEDS[0][0];
    auxPixel[1] = LEDS[0][1];
    auxPixel[2] = LEDS[0][2];

    for(j=0 ; j<PIXELS-1 ; j++){

        LEDS[j][0] = LEDS[j+1][0];
        LEDS[j][1] = LEDS[j+1][1];
        LEDS[j][2] = LEDS[j+1][2];

    }

    LEDS[PIXELS-1][0] = auxPixel[0];
    LEDS[PIXELS-1][1] = auxPixel[1];
    LEDS[PIXELS-1][2] = auxPixel[2];

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

