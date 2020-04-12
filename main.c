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

/*
 * CONSTANTS
 */
#define HEIGHT  6
#define LENGTH  5
#define PIXELS  HEIGHT * LENGTH

#define USER_NO_OPTION      0
#define USER_OPTION_S1      0x01
#define USER_OPTION_S2      0x02
#define USER_OPTION_S3      0x04
#define USER_OPTION_S4      0x08

/*
 * FUNCTIONS DECLARATIONS
 */

void sendPixel (unsigned char r, unsigned char g, unsigned char b);
void sendFrame (unsigned char LEDS[PIXELS][3]);

/* Visual function */
void lightsOff(unsigned char LEDS[PIXELS][3]);                                                   //Turns all LEDS OFF
void xmasTinsel (unsigned char LEDS[PIXELS][3]);
void visual1(unsigned char LEDS[PIXELS][3]);
void visual2(unsigned char LEDS[PIXELS][3]);
void waterEffect (unsigned char LEDS[PIXELS][3]);
void setPixel(unsigned char LedTable[LENGTH][HEIGHT][3], unsigned char x, unsigned char y, unsigned char r, unsigned char g, unsigned char b);
void game1(unsigned char LEDS[PIXELS][3]);
void cozy (unsigned char LEDS[PIXELS][3]);
void array2Vector (unsigned char inputArray[LENGTH][HEIGHT][3], unsigned char outputVector[PIXELS][3]);

/*
 * GLOBAL VARIABLES AND ARRAYS
 */


unsigned char LEDS[PIXELS][3] = {{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00}};

static unsigned char auxPixel[3];




int main(void)
{
  volatile unsigned int i;
  volatile unsigned int j;
  unsigned char userOption = 0;
 // volatile unsigned int GPIO_Status;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer

  //Peripherals initiaization
  CS_init();
  SPI_init();
  lightsOff(LEDS);
  initTimerA0();
  initGPIO();

  //Set and start Timer
  setTimerA0(TIMERA0_1SEC/4);
  startTimerA0();

  //Begin State Machine

  while(1){
     userOption = readGPIO();

          switch (userOption)
          {
              case USER_OPTION_S3:
                 cozy(LEDS);
                  break;
              case USER_OPTION_S4:
                  game1(LEDS);
                  break;
              default:
                  lightsOff(LEDS);
          }

      __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
      //__no_operation();                       // Equivalent to an assembly NOP
  }

}

/*
 * WS2812 DRIVER FUNCTION DEFINITIONS
 */

// send the full frame
void sendFrame (unsigned char LEDS[PIXELS][3]){
    unsigned int i;
    for ( i=PIXELS; i> 0 ;i--)              // Using a count down pixel to reduce the number of assembly instructions
        sendPixel(LEDS[PIXELS-i][0], LEDS[PIXELS-i][1],LEDS[PIXELS-i][2]);
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

void lightsOff(unsigned char LEDS[PIXELS][3]){

    unsigned int i;

    for (i = 0 ; i < PIXELS ; i++){
        LEDS[i][0] = 0x00;
        LEDS[i][1] = 0x00;
        LEDS[i][2] = 0x00;
    }

    sendFrame(LEDS);
}

void xmasTinsel (unsigned char LEDS[PIXELS][3]){


    unsigned int j;

    //Buffers the last pixel
    auxPixel[0] = LEDS[PIXELS-1][0];
    auxPixel[1] = LEDS[PIXELS-1][1];
    auxPixel[2] = LEDS[PIXELS-1][2];

    for(j = PIXELS-1 ; j>0 ; j--){

        LEDS[j][0] = LEDS[j-1][0];
        LEDS[j][1] = LEDS[j-1][1];
        LEDS[j][2] = LEDS[j-1][2];

    }

    LEDS[0][0] = auxPixel[0];
    LEDS[0][1] = auxPixel[1];
    LEDS[0][2] = auxPixel[2];

}



void waterEffect (unsigned char LEDS[PIXELS][3]){

    unsigned int j;
    static unsigned char exit;


    //Initializes the LED Array
    for(j=0 ; j<PIXELS ; j++){

        LEDS[j][0] = 0x00;
        LEDS[j][1] = 0x00;
        LEDS[j][2] = 0x80;

    }
    //Set some random blue pixels
    LEDS[5][0] = 0x00;
    LEDS[5][1] = 0x00;
    LEDS[5][2] = 0x8B;

    LEDS[14][0] = 0x00;
    LEDS[14][1] = 0x00;
    LEDS[14][2] = 0x8B;

    LEDS[24][0] = 0x00;
    LEDS[24][1] = 0x00;
    LEDS[24][2] = 0x8B;

    exit = 0;

    while (exit!=1){

        //userOptionWater = readGPIO();

        if(readGPIO() == USER_OPTION_S4){
            //exit waterEffect
            exit = 1;

        }
        else{
            xmasTinsel(LEDS);
            sendFrame(LEDS);
        }
        __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
    }
}

void cozy (unsigned char LEDS[PIXELS][3]){

    unsigned int j;
    static unsigned char exit;


    //Initializes the LED Array
    for(j=0 ; j<PIXELS ; j++){

        LEDS[j][0] = 0xFF;
        LEDS[j][1] = 0x8C;
        LEDS[j][2] = 0x00;

    }
    //Set some random blue pixels
    LEDS[5][0] = 0x00;
    LEDS[5][1] = 0x00;
    LEDS[5][2] = 0x8B;

    LEDS[14][0] = 0x00;
    LEDS[14][1] = 0x00;
    LEDS[14][2] = 0x8B;

    LEDS[24][0] = 0x00;
    LEDS[24][1] = 0x00;
    LEDS[24][2] = 0x8B;

    exit = 0;

    while (exit!=1){

        //userOptionWater = readGPIO();

        if(readGPIO() == USER_OPTION_S4){
            //exit waterEffect
            exit = 1;

        }
        else{
            xmasTinsel(LEDS);
            sendFrame(LEDS);
        }
        __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
    }
}

void game1(unsigned char LEDS[PIXELS][3]){

    // Define a matrix that contains the RGB color code for each pair (x,y)
    unsigned char LedTable[LENGTH][HEIGHT][3];

    unsigned int x;
    unsigned int y;

    unsigned char userOption;

    unsigned char exit = 0;

    //Initializes array
    for (x = 0 ; x < LENGTH ; x++){
        for (y = 0 ; y < HEIGHT ; y++){
            LedTable[x][y][0] = 0x00;
            LedTable[x][y][1] = 0xB0;
            LedTable[x][y][2] = 0x00;
        }

    }

    //Start game, place a pixel at (0,0)
    x = 0;
    y = 0;
    setPixel(LedTable, x , y , 0xFF, 0x00, 0x00);

    //Translates the 3D Array into a 2D Array
    array2Vector(LedTable,LEDS);

    while (exit!=1){

        userOption = readGPIO();

        switch (userOption){
            case USER_OPTION_S1:
                //exit effect / game
                exit = 1;
                break;
            case USER_OPTION_S2:
                setPixel(LedTable, x , y , 0x00, 0xB0, 0x00);   //Erases the old pixel
                x = 0;
                y = 0;
                setPixel(LedTable, x , y , 0xFF, 0x00, 0x00);   //Paints a new pixel
                array2Vector(LedTable,LEDS);
                sendFrame(LEDS);
                break;
            case USER_OPTION_S3:
                setPixel(LedTable, x , y , 0x00, 0xB0, 0x00);   //Erases the old pixel
                if( (x+1) < LENGTH)
                    x++;
                setPixel(LedTable, x , y , 0xFF, 0x00, 0x00);   //Paints a new pixel
                array2Vector(LedTable,LEDS);
                sendFrame(LEDS);
                break;
            case USER_OPTION_S4:
                setPixel(LedTable, x , y , 0x00, 0xB0, 0x00);   //Erases the old pixel
                if( (y+1) < HEIGHT)
                    y++;
                setPixel(LedTable, x , y , 0xFF, 0x00, 0x00);   //Paints a new pixel
                array2Vector(LedTable,LEDS);
                sendFrame(LEDS);

                break;
            default:
                sendFrame(LEDS);
                break;
        }

        __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
    }

}

void array2Vector (unsigned char inputArray[LENGTH][HEIGHT][3], unsigned char outputVector[PIXELS][3]){
    unsigned int x;
    unsigned int y;
    unsigned int l;

    // Translate odd columns
    for(x = 0 ; x < LENGTH ; x++ ){
        for( y = 0 ; y < HEIGHT ; y++ ){
            l = y + x * HEIGHT;
            outputVector[l][0] = inputArray[x][y][0];
            outputVector[l][1] = inputArray[x][y][1];
            outputVector[l][2] = inputArray[x][y][2];
        }
        x++;
    }

    // Translate even columns
    for(x = 1 ; x < LENGTH ; x++ ){
        for( y = 0 ; y < HEIGHT ; y++ ){
            l = HEIGHT - 1 - y + x * HEIGHT;
            outputVector[l][0] = inputArray[x][y][0];
            outputVector[l][1] = inputArray[x][y][1];
            outputVector[l][2] = inputArray[x][y][2];
        }
        x++;
    }


}

void setPixel(unsigned char LedTable[LENGTH][HEIGHT][3], unsigned char x, unsigned char y, unsigned char r, unsigned char g, unsigned char b){

    LedTable[x][y][0] = r;
    LedTable[x][y][1] = g;
    LedTable[x][y][2] = b;

}
