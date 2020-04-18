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

void xmasTinsel (color_t LEDS[PIXELS]){

    unsigned int j;
    color_t auxPixel;

    //Buffers the last pixel
    auxPixel = LEDS[PIXELS-1];

    for(j = PIXELS-1 ; j>0 ; j--){
        LEDS[j] = LEDS[j-1];
    }

    LEDS[0] = auxPixel;

}


void waterEffect (color_t LEDS[PIXELS]){

    unsigned int j;
    static unsigned char exit;


    //Initializes the LED Array
    for(j=0 ; j<PIXELS ; j++){
        LEDS[j] = blue_medium_2;
    }

    //Set some random blue pixels
    LEDS[5] = blue_dark_1;
    LEDS[14] = blue_dark_1;
    LEDS[24] = blue_dark_1;

    //Set some random blue pixels
    LEDS[9] = blue_bright_2;
    LEDS[22] = blue_bright_2;
    LEDS[29] = blue_bright_2;

    exit = 0;

    while (exit!=1){

        if(readGPIO() == USER_OPTION_S1){
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

void cozy (color_t LEDS[PIXELS]){

    unsigned int j;
    unsigned char exit;


    //Initializes the LED Array
    for(j=0 ; j<PIXELS ; j++){
        LEDS[j] = orange_dark_1;
    }

    //Set some random blue pixels
    LEDS[5] = blue_dark_1;
    LEDS[14]= blue_dark_1;
    LEDS[24]= blue_dark_1;

    exit = 0;

    while (exit!=1){

        if(readGPIO() == USER_OPTION_S1){
            //exit cozy
            exit = 1;

        }
        else{
            xmasTinsel(LEDS);
            sendFrame(LEDS);
        }
        __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
    }
}

/* TO BE UPDATED
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

            LedTable[x][0][0] = 0x00;
            LedTable[x][0][1] = 0xB0;
            LedTable[x][0][2] = 0xCD;

            LedTable[x][0][0] = 0xFF;
            LedTable[x][0][1] = 0x00;
            LedTable[x][0][2] = 0x00;
    }
    //Dark
    for (x = 0 ; x < LENGTH ; x++){

            LedTable[x][1][0] = 0x00;
            LedTable[x][1][1] = 0xB0;
            LedTable[x][1][2] = 0xFF;

            LedTable[x][1][0] = 0x00;
            LedTable[x][1][1] = 0xFF;
            LedTable[x][1][2] = 0x00;
    }
    // Medium dark
    for (x = 0 ; x < LENGTH ; x++){

            LedTable[x][2][0] = 0x00;
            LedTable[x][2][1] = 0xB0;
            LedTable[x][2][2] = 0xCD;

            LedTable[x][2][0] = 0xFF;
            LedTable[x][2][1] = 0x00;
            LedTable[x][2][2] = 0x00;
    }


    //Light
    for (x = 0 ; x < LENGTH ; x++){
        for (y = 3 ; y < HEIGHT ; y++){
            LedTable[x][y][0] = 0x00;
            LedTable[x][y][1] = 0x10;
            LedTable[x][y][2] = 0xCD;

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
*/

unsigned char waveInit(color_t LEDS[PIXELS], unsigned int theme){

    // Define a matrix that contains the RGB color code for each pair (x,y)
    unsigned int j;
    unsigned char userOption [KEYBOARD_BUFFER]  = {USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION};

    color_t optionTheme[3][3] =  {{red_bright_1, red_medium_1, red_dark_1}, {green_bright_1, green_medium_1, green_dark_1}, {blue_bright_1, blue_medium_1, blue_dark_1}};

        // Set color in function of the theme
       for(j=0 ; j<PIXELS ; j++){
           LEDS[j] = optionTheme[theme][2];
       }

       //Set some random pixels
       LEDS[5]  = optionTheme[theme][0];
       LEDS[14] = optionTheme[theme][0];
       LEDS[24] = optionTheme[theme][1];
       LEDS[9] = optionTheme[theme][1];

    while (1){

       antiAliasGPIO(userOption, 2);
       if(userOption[0] != USER_NO_OPTION){
           //exit waveInit
           return userOption[0];
           //exit = 1;
       }
       else{
           xmasTinsel(LEDS);
           sendFrame(LEDS);
       }

        __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
    }

}

