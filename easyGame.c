/*
 * easyGame.c
 *
 *  Created on: 14 avr. 2020
 *      Author: Laety
 */

#include <constant.h>
#include <msp430.h>
#include <gpio.h>
#include <easyGame.h>

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
    userOption = readGPIO();

    while (exit!=1){

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
                else
                    x = 0;
                setPixel(LedTable, x , y , 0xFF, 0x00, 0x00);   //Paints a new pixel
                array2Vector(LedTable,LEDS);
                sendFrame(LEDS);
                break;
            case USER_OPTION_S4:
                setPixel(LedTable, x , y , 0x00, 0xB0, 0x00);   //Erases the old pixel
                if( (y+1) < HEIGHT)
                    y++;
                else
                    y = 0;
                setPixel(LedTable, x , y , 0xFF, 0x00, 0x00);   //Paints a new pixel
                array2Vector(LedTable,LEDS);
                sendFrame(LEDS);

                break;
            default:
                sendFrame(LEDS);
                break;
        }

        userOption = antiAliasGPIO(userOption, 5);

    }

}



