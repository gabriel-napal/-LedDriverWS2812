/*
 * visualEffects.c
 *
 *  Created on: 14 avr. 2020
 *      Author: Laety
 */

#include <constant.h>
#include <msp430.h>
#include <gpio.h>
#include <LEDInterface.h>
#include <visualEffects.h>

void xmasTinsel (unsigned char LEDS[PIXELS][3]){


    unsigned int j;
    unsigned char auxPixel[3];

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
    unsigned char exit;


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

void wave1(unsigned char LEDS[PIXELS][3]){

    // Define a matrix that contains the RGB color code for each pair (x,y)
    unsigned char LedTable[LENGTH][HEIGHT][3];

    unsigned int x;
    unsigned int y;

    unsigned char userOption;

    unsigned char exit = 0;

    //Initializes array
    // Medium dark
    for (x = 0 ; x < LENGTH ; x++){

         /*   LedTable[x][0][0] = 0x00;
            LedTable[x][0][1] = 0xB0;
            LedTable[x][0][2] = 0xCD;*/

            LedTable[x][0][0] = 0xFF;
            LedTable[x][0][1] = 0x00;
            LedTable[x][0][2] = 0x00;
    }
    //Dark
    for (x = 0 ; x < LENGTH ; x++){

           /* LedTable[x][1][0] = 0x00;
            LedTable[x][1][1] = 0xB0;
            LedTable[x][1][2] = 0xFF;*/

            LedTable[x][1][0] = 0x00;
            LedTable[x][1][1] = 0xFF;
            LedTable[x][1][2] = 0x00;
    }
    // Medium dark
    for (x = 0 ; x < LENGTH ; x++){

          /*  LedTable[x][2][0] = 0x00;
            LedTable[x][2][1] = 0xB0;
            LedTable[x][2][2] = 0xCD;*/

            LedTable[x][2][0] = 0xFF;
            LedTable[x][2][1] = 0x00;
            LedTable[x][2][2] = 0x00;
    }


    //Light
    for (x = 0 ; x < LENGTH ; x++){
        for (y = 3 ; y < HEIGHT ; y++){
           /* LedTable[x][y][0] = 0x00;
            LedTable[x][y][1] = 0x10;
            LedTable[x][y][2] = 0xCD;*/

            LedTable[x][y][0] = 0x00;
            LedTable[x][y][1] = 0x00;
            LedTable[x][y][2] = 0xFF;
        }

    }

    array2Vector(LedTable,LEDS);

    while (exit!=1){

        userOption = readGPIO();

       if(userOption == USER_OPTION_S4)
           exit = 1;
       else
           sendFrame(LEDS);

        __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
    }

}



