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
    unsigned int turnCommand = 0; // if turnCommand = 1, snake is turning left, if turnCommand = 2, snake is turning right, 0 = no change of direction requested)
    unsigned int direction = 0;   // Represent the direction of the snake in x or y axe (0 = x increasing ; 1 = y increasing ; 2 = x decreasing ; 3 = y decreasing)

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
            turnCommand = 1;
            break;
        case USER_OPTION_S4: // To go left
            turnCommand = 2;
            break;
        }

        // turn left
        if (turnCommand == 1) {
            switch (direction){
            case 0:
                direction = 3;
                break;
            case 1:
                direction = 0;
                break;
            case 2:
                direction = 1;
                break;
            case 3:
                direction = 2;
                break;
            }
        }
        // turn right
        else if (turnCommand == 2) {
            switch (direction){
            case 0:
                direction = 1;
                break;
            case 1:
                direction = 2;
                break;
            case 2:
                direction = 3;
                break;
            case 3:
                direction = 0;
                break;
            }
        }

        setPixel(LedTable, x , y , 0x00, 0xB0, 0x00);   //Erases the old pixel
        /*switch (direction){
            case 0:
                if( (x+1) < LENGTH)
                    x++;
                else
                    x = 0;
                break;
            case 1:
                if( (y+1) < HEIGHT)
                     y++;
                else
                    y = 0;
                break;
            case 2:
                if( (x-1) > 0)
                    x--;
                else
                    x = LENGTH;
                break;
            case 3:
                if( (y-1) > 0)
                    y--;
                else
                    y = HEIGHT;
                break;
        }*/

        if (direction == 0) {
            if( (x+1) < LENGTH)
                x++;
            else
                x = 0;
        }
        else if (direction == 1) {
            if( (y+1) < HEIGHT)
                 y++;
            else
                y = 0;
        }
        else if (direction == 2){
            if( x > 0)
                x--;
            else
                x = LENGTH;
        }
        else if (direction == 3){
            if( y > 0)
                y--;
            else
                y = HEIGHT;
        }


        setPixel(LedTable, x , y , 0xFF, 0x00, 0x00);   //Paints a new pixel
        array2Vector(LedTable,LEDS);
        sendFrame(LEDS);
        //__bis_SR_register(LPM0_bits + GIE);
        userOption = antiAliasGPIO(userOption, 5);
        turnCommand = 0;
    }

}




