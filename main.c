/*
 * LedDriverWS2812
 * Version : 0.0.1
 *
 * Authors :
 *              L. Innocente
 *              G. Napal
 *
 *
 *  General configuration :
 *
 *  CPU : working at 12 Mhz
 *  SPI : CLK is set to 6 MHz
 *
 * * Connections:
 *                   MSP430F5529
//                 -----------------
//             /|\|                 |
//              | |                 |
//              --|RST              |
//                |                 |
//                |             P3.3|-> Data Out (UCA0SIMO)
//                |                 |
//                |             P1.2|<- P1_RED
//                |                 |
//                |             P1.3|<- P1_BLUE
//                |                 |
//                |             P1.4|<- P1_YELLOW
//                |                 |
//                |             P1.5|<- P1_GREEN
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

int main(void){

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
  setTimerA0(TIMERA0_1SEC/200);
  startTimerA0();

  //Begin State Machine

  while(1){

     userOption = displayText(LEDS, "BONJOUR  ", 9, 5, red_medium_1, yellow_dark_1);
     menu = INIT ;

     switch (userOption){
     case P1_BLUE:
         userOption = displayText(LEDS, "VISUAL EFFECTS  ", 16, 5, green_medium_1, orange_dark_1);
         menu = VISUALEFFECTS;
         break;
     case P1_YELLOW:
         userOption = displayText(LEDS, "GAMES  ", 7, 5, blue_dark_3, cyan_bright_1);
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
