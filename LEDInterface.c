/*
 * LEDInterface.c
 *
 *  Created on: 14 avr. 2020
 *      Author: Laety
 */


#include <constant.h>
#include <msp430.h>
#include <gpio.h>
#include <uscia0_spi.h>
#include <LEDInterface.h>

/*
 * WS2812 DRIVER FUNCTION DEFINITIONS
 */

// send the full frame
void sendFrame (color_t LEDS[PIXELS]){
    unsigned int i;
    for ( i=PIXELS; i> 0 ;i--)              // Using a count down pixel to reduce the number of assembly instructions
        sendPixel(LEDS[PIXELS-i]);
}


// Send a single pixel worth of information.  Turn interrupts off while using.
void sendPixel (color_t LED){
    sendByte_SPIA (LED.G);        // NeoPixel wants colors in green-then-red-then-blue order
    sendByte_SPIA (LED.R);
    sendByte_SPIA (LED.B);
}


/*
 * VISUAL FUNCTION DEFINITIONS
 */

void lightsOff(color_t LEDS[PIXELS]){

    unsigned int i;

    for (i = 0 ; i < PIXELS ; i++){
        LEDS[i] = color_off;
    }

    sendFrame(LEDS);
}


void array2Vector (color_t inputArray[LENGTH][HEIGHT], color_t outputVector[PIXELS]){
    unsigned int x;
    unsigned int y;
    unsigned int l;

    // Translate odd columns
    for(x = 0 ; x < LENGTH ; x++ ){
        for( y = 0 ; y < HEIGHT ; y++ ){
            l = y + x * HEIGHT;
            outputVector[l] = inputArray[x][y];
        }
        x++;
    }

    // Translate even columns
    for(x = 1 ; x < LENGTH ; x++ ){
        for( y = 0 ; y < HEIGHT ; y++ ){
            l = HEIGHT - 1 - y + x * HEIGHT;
            outputVector[l] = inputArray[x][y];
        }
        x++;
    }


}

void setPixel(unsigned char LedTable[LENGTH][HEIGHT][3], unsigned char x, unsigned char y, unsigned char r, unsigned char g, unsigned char b){

    LedTable[x][y][0] = r;
    LedTable[x][y][1] = g;
    LedTable[x][y][2] = b;

}
/*
 * updateLedTable
 *
 * Updates each LED on the table.
 *
 * inputArray : 2-D input Array containing the RGB for each element
 * outputVector : 1-D output vector that will be sended via SPI
 */

void updateLedTable(color_t inputArray[LENGTH][HEIGHT],color_t outputVector[PIXELS]){
    unsigned int i;

    //Coordinates to loop over arrays
    unsigned int x;
    unsigned int y;
    unsigned int l;

    //Translate the 2-D array into an 1-D LED Strip vector
    // Translate odd columns
    for(x = 0 ; x < LENGTH ; x++ ){
        for( y = 0 ; y < HEIGHT ; y++ ){
            l = y + x * HEIGHT;
            outputVector[l] = inputArray[x][y];
        }
        x++;
    }

    // Then, translate even columns
    for(x = 1 ; x < LENGTH ; x++ ){
        for( y = 0 ; y < HEIGHT ; y++ ){
            l = HEIGHT - 1 - y + x * HEIGHT;
            outputVector[l] = inputArray[x][y];
        }
        x++;
    }

    for ( i=PIXELS; i> 0 ;i--)              // Using a count down pixel to reduce the number of assembly instructions
        sendPixel(outputVector[PIXELS-i]);
}



