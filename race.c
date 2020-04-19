/*
 * race.c
 *
 *  Created on: Apr 18, 2020
 *      Author: Gabriel
 */



//Main objectif of this game : make move a small snake
#include "race.h"



#define USER_NO_TURN      0
#define USER_TURN_LEFT    1
#define USER_TURN_RIGHT   2


void race(color_t LEDS[PIXELS]){

    // Define a matrix that contains the RGB color code for each pair (x,y)
    color_t LedTable[LENGTH][HEIGHT];

    unsigned char userOption[KEYBOARD_BUFFER] = {USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION};

    //Exit flag used to leave game
    unsigned char exit = 0;

    //Drawing coordinates
    unsigned int x;
    unsigned int y;

    //Represent the player's car position
    unsigned int xCar;

    //First opponent position
    unsigned int xOpponent1;
    unsigned int yOpponent1;
    //unsigned int yCar;
    unsigned int i;

    //unsigned int snakeLength = 3;
    //unsigned int nbApple = 1;
    unsigned int turnCommand = USER_NO_TURN;
    //unsigned int direction = DIRECTION_X_INCREASING;
   // unsigned int snake[30][2] = {{2,0},{1,0},{1,0}};
    //unsigned int apple[3] = {(LENGTH-1),(HEIGHT-1),RED_APPLE};

    color_t roadLeftCorner;
    color_t roadRightCorner;

    //Initializes array. Paints the road
    for (x = 0 ; x < LENGTH ; x++){
        for (y = 0 ; y < HEIGHT ; y++){
            LedTable[x][y] = grey_medium_1;
        }
    }

    //Paint sides
    LedTable[0][0] = LedTable[LENGTH-1][0] = brow_dark_1;
    LedTable[0][2] = LedTable[LENGTH-1][2] = brow_dark_1;
    LedTable[0][3] = LedTable[LENGTH-1][3] = brow_dark_1;
    LedTable[0][5] = LedTable[LENGTH-1][5] = brow_dark_1;
    //LedTable[LENGTH-1][0] = brow_dark_1;


    //Start game, place the car at  pixel at (2,0)
    xCar = 2;
    LedTable[xCar][Y_CAR] = blue_medium_1;

    //Start game, place the first opponent
    xOpponent1 = 2;
    yOpponent1 = HEIGHT-1;
    LedTable[xOpponent1][yOpponent1] = red_dark_3;

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

        //Places the car to is new position
        switch (turnCommand)
        {
            case USER_TURN_RIGHT:
                if( (xCar + 1) < LENGTH-1){
                    LedTable[xCar][Y_CAR] = grey_medium_1;  //Erases the car
                    xCar++;
                }
                break;
            case USER_TURN_LEFT:
                if( xCar > 1 ){
                    LedTable[xCar][Y_CAR] = grey_medium_1;  //Erases the car
                    xCar--;
                }
                break;
        }
        LedTable[xCar][Y_CAR] = blue_medium_1; //Paints the car in its new position

        //Car goes forward = the road goes downward
        //Back Up the road corner
        roadLeftCorner = LedTable[0][0];
        roadRightCorner = LedTable[LENGTH-1][0];

        //Paint the new road corners
        LedTable[0][0]= LedTable[0][1];
        LedTable[LENGTH-1][0] = LedTable[LENGTH-1][1];

        for ( y = 2; y < HEIGHT ; y++){
            for(x = 0 ; x < LENGTH ;x++)
                LedTable[x][y-1] = LedTable[x][y];
        }

        //Paint the end of the road
        LedTable[0][HEIGHT-1]        = roadLeftCorner;
        LedTable[LENGTH-1][HEIGHT-1] = roadRightCorner;

        for( x = 1 ; x < LENGTH - 2 ; x++)
            LedTable[x][HEIGHT - 1] = grey_medium_1;

/*
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

        //Check if the snake is eating an apple and check the color of the apple
        if ((x == apple[0]) && (y == apple[1])){
            if (apple [2] == RED_APPLE){
                snakeLength = snakeLength + 1;
            }
            else {
                LedTable [snake[snakeLength-1][0]][snake[snakeLength-1][1]] = blue_bright_1;   //Erases the old pixel from the snake tail
                snakeLength = snakeLength - 1;
                LedTable [snake[snakeLength-1][0]][snake[snakeLength-1][1]] = blue_bright_1;   //Make one pixel smaller the snake
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
                    LedTable [apple[0]][apple[1]] =  green_medium_1;
                }
                else {
                    apple[2] = RED_APPLE ;
                    LedTable[apple[0]][apple[1]] =  red_dark_3;
                }
            nbApple ++;
        }
        else
            LedTable[snake[snakeLength-1][0]][snake[snakeLength-1][1]] = blue_bright_1;   //Erases the old pixel from the snake tail

        for (i = (snakeLength - 1); i > 0; i--){
            snake[i][0] = snake[i-1][0];
            snake[i][1] = snake[i-1][1];
        }
        LedTable[x][y] = purple_medium_1;   //Paints a new pixel

        snake[0][0] = x;
        snake[0][1] = y;
*/

        array2Vector(LedTable,LEDS);
        sendFrame(LEDS);
        antiAliasGPIO(userOption, 2);
        turnCommand = USER_NO_TURN;
    }

}




