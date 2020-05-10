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
 *
 *                   MSP430FR5739
//                 -----------------
//             /|\|                 |
//              | |                 |
//              --|RST              |
//                |                 |
//                |             P2.0|-> Data Out (UCA0SIMO)
//                |                 |
//                |             P2.1|<- Data In (UCA0SOMI)  NOT USED
//                |                 |
//                |             P1.5|-> Serial Clock Out (UCA0CLK)  NOT USED
 *
 *
 *
 *
 *
 *  TimerA0 : uses a 8.42kHz clock. Its internal counter can be used to increase or reduce the main loop frequency
 *          Since we're using LPM and Timer interruptions, code is currently Non-Blocking --> cool :)
 *
 */

#include <msp430.h>
#include <clock.h>
#include <uscia0_spi.h>
#include <timers.h>
#include <gpio.h>
#include <constant.h>
#include <visualEffects.h>
#include <easyGame.h>
#include <LEDInterface.h>

/*
 * GLOBAL VARIABLES AND ARRAYS
 */

#define INIT      0
#define VISUALEFFECTS    1
#define GAMES   2

int main(void)
{

  color_t LEDS[PIXELS] ;
  volatile unsigned int i;
  unsigned int menu = INIT;
  volatile unsigned int j;
  unsigned char userOption = 0;
  volatile unsigned int GPIO_Status;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer. Its mandatory!

  //Peripherals initiaization
  ucsInit();
  spiInit();
  lightsOff(LEDS);
  initTimerA0();
  initGPIO();

  //Set and start Timer
  setTimerA0(TIMERA0_1SEC/10);
  startTimerA0();

  //Begin State Machine

  while(1){

     userOption = displayText(LEDS, "BONJOUR  ", 9, 5, red_medium_1, yellow_dark_1);
     menu = INIT ;
     //userOption = waveInit(LEDS, INIT); // waveInit() shows and animation screen and gets the current keyboard status.
     switch (userOption){
     case P1_BLUE:
         userOption = displayText(LEDS, "VISUAL EFFECTS  ", 16, 5, green_medium_1, orange_dark_1);
         //userOption = waveInit(LEDS, VISUALEFFECTS);
         menu = VISUALEFFECTS;
         break;
     case P1_YELLOW:
         userOption = displayText(LEDS, "GAMES  ", 7, 5, blue_dark_3, cyan_bright_1);
         //userOption = waveInit(LEDS, GAMES);
         menu = GAMES;
         break;
     default:
         menu = INIT ;
         break;
     }

     if (menu==VISUALEFFECTS){
        switch (userOption){
        case P1_RED:
            menu = INIT;
            break;
        case P1_BLUE:
            menu = INIT;
            cozy(LEDS);
            break;
        case P1_YELLOW:
            menu = INIT;
            waterEffect(LEDS);
            break;
        case P1_GREEN:
            menu = INIT;
            break;
        }
     }
     else if (menu==GAMES){
        switch (userOption){
        case P1_RED:
            menu = INIT;
            break;
        case P1_BLUE:
            menu = INIT;
            tetris(LEDS);
            break;
        case P1_YELLOW:
            snake(LEDS);
            break;
        case P1_GREEN:
            menu = INIT;
            break;
        }
     }

      __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
  }

}
