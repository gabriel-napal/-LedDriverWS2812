/*
 * easyGame.c
 *
 *  Created on: 14 avr. 2020
 *      Author: Laety
 */

#include <constant.h>
#include <msp430.h>
#include <gpio.h>
#include <LEDInterface.h>
#include <easyGame.h>

#define DIRECTION_X_INCREASING      0
#define DIRECTION_Y_INCREASING      1
#define DIRECTION_X_DECREASING      2
#define DIRECTION_Y_DECREASING      3

#define USER_NO_TURN      0
#define USER_TURN_LEFT    1
#define USER_TURN_RIGHT   2

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

//Main objectif of this game : make move a small snake (do not integrate the apple eating process)

void preSnake(unsigned char LEDS[PIXELS][3]){

    // Define a matrix that contains the RGB color code for each pair (x,y)
    unsigned char LedTable[LENGTH][HEIGHT][3];
    unsigned char userOption = 0;
    unsigned char exit = 0;

    unsigned int x;
    unsigned int y;
    unsigned int turnCommand = USER_NO_TURN;
    unsigned int direction = DIRECTION_X_INCREASING;

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
    sendFrame(LEDS);

    while (exit!=1){

        switch (userOption){
        case USER_OPTION_S1:
        //exit effect / game
            exit = 1;
            break;
        case USER_OPTION_S2:
            //exit effect / game
            exit = 1;
            break;
        case USER_OPTION_S3: // To go right
            turnCommand = USER_TURN_RIGHT;
            break;
        case USER_OPTION_S4: // To go left
            turnCommand = USER_TURN_LEFT;
            break;
        }

        if (turnCommand == USER_TURN_RIGHT) {
            switch (direction){
            case DIRECTION_X_INCREASING:
                direction = DIRECTION_Y_DECREASING;
                break;
            case DIRECTION_Y_INCREASING:
                direction = DIRECTION_X_INCREASING;
                break;
            case DIRECTION_X_DECREASING:
                direction = DIRECTION_Y_INCREASING;
                break;
            case DIRECTION_Y_DECREASING:
                direction = DIRECTION_X_DECREASING;
                break;
            }
        }
        else if (turnCommand == USER_TURN_LEFT) {
            switch (direction){
            case DIRECTION_X_INCREASING:
                direction = DIRECTION_Y_INCREASING;
                break;
            case DIRECTION_Y_INCREASING:
                direction = DIRECTION_X_DECREASING;
                break;
            case DIRECTION_X_DECREASING:
                direction = DIRECTION_Y_DECREASING;
                break;
            case DIRECTION_Y_DECREASING:
                direction = DIRECTION_X_INCREASING;
                break;
            }
        }

        setPixel(LedTable, x , y , 0x00, 0xB0, 0x00);   //Erases the old pixel
        switch (direction){
            case DIRECTION_X_INCREASING:
                if( (x+1) < LENGTH)
                    x++;
                else
                    x = 0;
                break;
            case DIRECTION_Y_INCREASING:
                if( (y+1) < HEIGHT)
                     y++;
                else
                    y = 0;
                break;
            case DIRECTION_X_DECREASING:
                if( x > 0)
                    x--;
                else
                    x = LENGTH - 1;
                break;
            case DIRECTION_Y_DECREASING:
                if( y > 0)
                    y--;
                else
                    y = HEIGHT - 1;
                break;
        }


        setPixel(LedTable, x , y , 0xFF, 0x00, 0x00);   //Paints a new pixel
        array2Vector(LedTable,LEDS);
        sendFrame(LEDS);
        //__bis_SR_register(LPM0_bits + GIE);
        userOption = antiAliasGPIO(userOption, 5);
        turnCommand = USER_NO_TURN;
    }

}




