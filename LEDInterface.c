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



