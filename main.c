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
//                |                 |
//                |             P2.0|<- P2_RED
//                |                 |
//                |             P2.2|<- P2_BLUE
//                |                 |
//                |             P2.4|<- P2_YELLOW
//                |                 |
//                |             P2.5|<- P2_GREEN
 *
 *
 *  TimerA0 : uses a 8.192kHz clock. Its internal counter can be used to increase or reduce the main loop frequency
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
#define VISUAL_EFFECTS    1
#define GAMES   2



unsigned char readGPIO_Flag = FALSE; // Flag that indicates wheater to read or not the GPIOs.
unsigned int stringIndex = 0;

int main(void){

  color_t LEDS[PIXELS] ;
  volatile unsigned int i;
  volatile unsigned int j;

  volatile unsigned int GPIO_Status;

  unsigned char zeroCross = FALSE;
  unsigned char stateMachine = INIT;
  unsigned char speedCounter = MAIN_SCREEN_REFRESH;
  unsigned char userOption = 0;


  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer. It's good for you!

  //Peripherals initialization
  ucsInit();
  spiInit();
  lightsOff(LEDS);
  initTimerA0();
  initGPIO();

  //Set and start Timer
  setTimerA0(TIMERA0_1SEC/200);         //Timer interrupts every 1second/NUMBER (if number = 200 --> interrupts every 5ms)
  startTimerA0();

  //Program starts
  while(1){

      //Check if the its time to poll the buttons user interface
     if (readGPIO_Flag == TRUE){
         readGPIO_Flag = FALSE;
         userOption = readGPIO();

         if(userOption == USER_NO_OPTION)
             zeroCross = TRUE;
     }

     //Begin State Machine
     switch(stateMachine){
     case INIT:

         if(speedCounter == 0){

                if (displayText(LEDS, "BONJOUR  ", 9, 5, red_medium_1,
                                yellow_dark_1,
                                stringIndex) == READ_OVERFLOW_TRUE)
                    stringIndex = 0;
                else
                    stringIndex++;


                speedCounter = MAIN_SCREEN_REFRESH;
         }

         //Check if the user selected an valid option
         switch(userOption){
         case  P1_BLUE:
             stateMachine = VISUAL_EFFECTS;
             stringIndex = 0;
             speedCounter = MAIN_SCREEN_REFRESH;
             zeroCross = FALSE;                     //Avoids passing through options if the user keeps pushing the button

             break;
         case P1_YELLOW:
             stateMachine = GAMES;
             stringIndex = 0;
             speedCounter = MAIN_SCREEN_REFRESH;
             zeroCross = FALSE;
             break;
         }

         break;
        case VISUAL_EFFECTS:

            //Update the screen with animation
            if (speedCounter == 0)
            {
                if (displayText(LEDS, "VISUAL EFFECTS  ", 16, 5, green_medium_1,
                                orange_dark_1,
                                stringIndex) == READ_OVERFLOW_TRUE)
                    stringIndex = 0;
                else
                    stringIndex++;
                speedCounter = MAIN_SCREEN_REFRESH;
            }
            //Check if the user selected an valid option
            switch(userOption){
            case  P1_RED:
                stateMachine = INIT;
                stringIndex = 0;
                speedCounter = MAIN_SCREEN_REFRESH;

                break;
            case P1_BLUE:
                if(zeroCross == TRUE){
                    cozy(LEDS);
                    stringIndex = 0;
                    speedCounter = MAIN_SCREEN_REFRESH;
                }

                break;
            case P1_YELLOW:
                if(zeroCross == TRUE){
                    waterEffect(LEDS);
                    stringIndex = 0;
                    speedCounter = MAIN_SCREEN_REFRESH;
                }
                break;
            }

            break;

        case GAMES:

            //Update the screen with animation
            if (speedCounter == 0)
            {
                if (displayText(LEDS, "GAMES  ", 7, 5, blue_dark_3,
                                             cyan_bright_1,
                                             stringIndex) == READ_OVERFLOW_TRUE)
                    stringIndex = 0;
                else
                    stringIndex++;
                speedCounter = MAIN_SCREEN_REFRESH;
            }

            switch(userOption){
            case  P1_RED:
                stateMachine = INIT;
                stringIndex = 0;
                speedCounter = MAIN_SCREEN_REFRESH;

                break;
            case P1_BLUE:
                if(zeroCross == TRUE){
                    tetris(LEDS);
                    stringIndex = 0;
                    speedCounter = MAIN_SCREEN_REFRESH;
                }


                break;
            case P1_YELLOW:
                if(zeroCross == TRUE){
                    snake(LEDS);
                    stringIndex = 0;
                    speedCounter = MAIN_SCREEN_REFRESH;
                }
                break;
            }

            break;

     }
     --speedCounter;
      __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts

  }

}
