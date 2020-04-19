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
#include <visualEffects.h>
#include <pseudoRandom.h>

#define DIRECTION_X_INCREASING      0
#define DIRECTION_Y_INCREASING      1
#define DIRECTION_X_DECREASING      2
#define DIRECTION_Y_DECREASING      3

#define USER_NO_TURN      0
#define USER_TURN_LEFT    1
#define USER_TURN_RIGHT   2

#define GREEN_APPLE      0
#define RED_APPLE    1

void game1(color_t LEDS[PIXELS]){

    // Define a matrix that contains the RGB color code for each pair (x,y)
    color_t LedTable[LENGTH][HEIGHT];

    unsigned int x;
    unsigned int y;
    unsigned int i;

    unsigned char userOption[KEYBOARD_BUFFER] = {USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION};

    unsigned char exit = 0;

    //Initializes array
    for (x = 0 ; x < LENGTH ; x++){
        for (y = 0 ; y < HEIGHT ; y++){
            LedTable[x][y] = green_medium_1;
        }

    }

    //Start game, place a pixel at (0,0)
    x = 0;
    y = 0;
    LedTable[x][y] = red_dark_3;

    //Translates the 3D Array into a 2D Array
    array2Vector(LedTable,LEDS);
    userOption[0] = readGPIO();

    while (exit!=1){

        switch (userOption[0]){
            case USER_OPTION_S1:
                //exit effect / game
                exit = 1;
                break;
            case USER_OPTION_S2:
                LedTable[x][y] = green_medium_1;   //Erases the old pixel
                x = 0;
                y = 0;
                LedTable[x][y] = red_dark_3;  //Paints a new pixel
                array2Vector(LedTable,LEDS);
                sendFrame(LEDS);
                break;
            case USER_OPTION_S3:
                LedTable[x][y] = green_medium_1;   //Erases the old pixel
                if( (x+1) < LENGTH)
                    x++;
                else
                    x = 0;
                LedTable[x][y] = red_dark_3;   //Paints a new pixel
                array2Vector(LedTable,LEDS);
                sendFrame(LEDS);
                break;
            case USER_OPTION_S4:
                LedTable[x][y] = green_medium_1;   //Erases the old pixel
                if( (y+1) < HEIGHT)
                    y++;
                else
                    y = 0;
                LedTable[x][y] = red_dark_3;   //Paints a new pixel
                array2Vector(LedTable,LEDS);
                sendFrame(LEDS);

                break;
            default:
                sendFrame(LEDS);
                break;
        }

        // Erases the used option (shift left the buffer)
        for (i = 0; i < KEYBOARD_BUFFER-1 ; i++)
            userOption[i]=userOption[i+1];
        userOption[KEYBOARD_BUFFER-1] = USER_NO_OPTION;
        antiAliasGPIO(userOption, 2);

    }

}

//Main objectif of this game : make move a small snake and eat apples

void snake(color_t LEDS[PIXELS]){

    // Define a matrix that contains the RGB color code for each pair (x,y)
    color_t LedTable[LENGTH][HEIGHT];
    unsigned char userOption[KEYBOARD_BUFFER] = {USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION};
    unsigned char exit = 0;

    //Represent x and y for the head of the snake
    unsigned int x;
    unsigned int y;
    unsigned int i;
    unsigned int newApple = 0;
    unsigned int testApple = 0;

    unsigned int snakeLength = 3;
    unsigned int turnCommand = USER_NO_TURN;
    unsigned int direction = DIRECTION_X_INCREASING;
    unsigned int snake[30][2] = {{2,0},{1,0},{1,0}};
    unsigned int apple[3] = {(LENGTH-1),(HEIGHT-1),RED_APPLE};

    //Initializes array
    for (x = 0 ; x < LENGTH ; x++){
        for (y = 0 ; y < HEIGHT ; y++){
            LedTable[x][y] = cyan_bright_1;
        }
    }


    //Start game, place a pixel at (0,0)
    x = snake[0][0];
    y = snake[0][1];
    LedTable[x][y] = purple_medium_1;
    LedTable[snake[1][0]][snake[1][1]] = purple_medium_1;
    LedTable[snake[2][0]][snake[2][1]] = purple_medium_1;

    //Start game, place the first apple
    LedTable[apple[0]][apple[1]] = red_dark_3;

    //Translates the 3D Array into a 2D Array
    array2Vector(LedTable,LEDS);
    sendFrame(LEDS);

    while (exit!=1){

        switch (userOption[0]){
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

        // Erases the used option (shift left the buffer)
        for (i = 0; i < KEYBOARD_BUFFER-1 ; i++){
            userOption[i]=userOption[i+1];
        }
        userOption[KEYBOARD_BUFFER-1] = USER_NO_OPTION;

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

        // change the position of the snake head
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

        // Check if the snake is eating itself
        for (i = 0; i< snakeLength; i++){
            if ((snake[i][0]== x) && (snake[i][1] == y)){
                looser(LEDS);
                exit = 1;
            }
        }

        //Check if the snake is eating an apple and check the color of the apple
        if ((x == apple[0]) && (y == apple[1])){
            if (apple [2] == RED_APPLE){
                snakeLength = snakeLength + 1;
            }
            else {
                LedTable [snake[snakeLength-1][0]][snake[snakeLength-1][1]] = cyan_bright_1;   //Erases the old pixel from the snake tail
                snakeLength = snakeLength - 1;
                LedTable [snake[snakeLength-1][0]][snake[snakeLength-1][1]] = cyan_bright_1;   //Make one pixel smaller the snake
            }
            newApple = 1;
        }
        else
            LedTable[snake[snakeLength-1][0]][snake[snakeLength-1][1]] = cyan_bright_1;   //Erases the old pixel from the snake tail

        for (i = (snakeLength - 1); i > 0; i--){
            snake[i][0] = snake[i-1][0];
            snake[i][1] = snake[i-1][1];
        }
        LedTable[x][y] = purple_medium_1;   //Paints a new pixel

        snake[0][0] = x;
        snake[0][1] = y;

        // Run aleatory number function to set the new apple.
        if (newApple == 1){
            testApple = 0;
            while (testApple == 0){
                testApple = 1;
                apple[0] = pseudoRandom(LENGTH);
                apple[1] = pseudoRandom(HEIGHT);
                for (i = 0; i< snakeLength; i++){
                    if ((snake[i][0]== apple[0]) && (snake[i][1] == apple[1])){
                       testApple = 0;
                       break;
                    }
                }
            }

            if (pseudoRandom(100)>75){
                apple[2] = GREEN_APPLE;
                LedTable [apple[0]][apple[1]] =  green_medium_1;
            }
            else {
                apple[2] = RED_APPLE;
                LedTable[apple[0]][apple[1]] =  red_dark_3;
            }
            newApple = 0 ;
        }
        if (exit != 1){
            array2Vector(LedTable,LEDS);
            sendFrame(LEDS);
            antiAliasGPIO(userOption, 5);
            turnCommand = USER_NO_TURN;
        }
    }

}

