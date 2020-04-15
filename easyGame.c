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

#define GREEN_APPLE      0
#define RED_APPLE    1

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

    //Represent x and y for the head of the snake
    unsigned int x;
    unsigned int y;
    unsigned int i;

    unsigned int snakeLength = 3;
    unsigned int nbApple = 1;
    unsigned int turnCommand = USER_NO_TURN;
    unsigned int direction = DIRECTION_X_INCREASING;
    unsigned int snake[30][2] = {{2,0},{1,0},{1,0}};
    unsigned int apple[3] = {(LENGTH-1),(HEIGHT-1),RED_APPLE};

    //Initializes array
    for (x = 0 ; x < LENGTH ; x++){
        for (y = 0 ; y < HEIGHT ; y++){
            LedTable[x][y][0] = 0x00;
            LedTable[x][y][1] = 0xA6;
            LedTable[x][y][2] = 0xA9;
        }
    }


    //Start game, place a pixel at (0,0)
    x = snake[0][0];
    y = snake[0][1];
    setPixel(LedTable, x , y , 0x2F, 0x29, 0xFF);
    setPixel(LedTable, snake[1][0] , snake[1][1] , 0x2F, 0x29, 0xFF);
    setPixel(LedTable, snake[2][0] , snake[2][1] , 0x2F, 0x29, 0xFF);

    //Start game, place the first apple
    setPixel(LedTable, apple[0] , apple[1] , 0xFF, 0x00, 0x00);

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

        //Check if the snake is eating an apple and check the color of the apple
        if ((x == apple[0]) && (y == apple[1])){
            if (apple [2] == RED_APPLE){
                snakeLength = snakeLength + 1;
            }
            else {
                setPixel(LedTable, snake[snakeLength-1][0] , snake[snakeLength-1][1] , 0x00, 0xA6, 0xA9);   //Erases the old pixel from the snake tail
                snakeLength = snakeLength - 1;
                setPixel(LedTable, snake[snakeLength-1][0] , snake[snakeLength-1][1] , 0x00, 0xA6, 0xA9);   //Make one pixel smaller the snake
            }
            // Run aleatory number function to set the new apple, for now it's just a fix position.
                if (nbApple%2 == 0){
                    apple[0] = 0;
                    apple[1] = 0;
                }
                else {
                    apple[0] = 4;
                    apple[1] = 4;
                }

                if (nbApple%4 == 0){
                    apple[2] = GREEN_APPLE ;
                    setPixel(LedTable, snake[snakeLength-1][0] , snake[snakeLength-1][1] , 0x00, 0xB0, 0x00);
                }
                else {
                    apple[2] = RED_APPLE ;
                    setPixel(LedTable, snake[snakeLength-1][0] , snake[snakeLength-1][1] , 0xFF, 0x00, 0x00);
                }
            nbApple ++;
        }
        else
            setPixel(LedTable, snake[snakeLength-1][0] , snake[snakeLength-1][1] , 0x00, 0xA6, 0xA9);   //Erases the old pixel from the snake tail

        for (i = (snakeLength - 1); i > 0; i--){
            snake[i][0] = snake[i-1][0];
            snake[i][1] = snake[i-1][1];
        }
        setPixel(LedTable, x , y , 0x2F, 0x29, 0xFF);   //Paints a new pixel

        snake[0][0] = x;
        snake[0][1] = y;


        array2Vector(LedTable,LEDS);
        sendFrame(LEDS);
        userOption = antiAliasGPIO(userOption, 5);
        turnCommand = USER_NO_TURN;
    }

}




