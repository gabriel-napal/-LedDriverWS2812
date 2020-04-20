/*
 * race.c
 *
 *  Created on: Apr 18, 2020
 *      Author: Gabriel
 */



//Main objectif of this game : make move a small snake
#include "race.h"







void race(color_t LEDS[PIXELS]){

    // Define a matrix that contains the RGB color code for each pair (x,y)
    color_t LedTable[LENGTH][HEIGHT];

    unsigned char userOption[KEYBOARD_BUFFER] = {USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION};

    //Exit flag used to leave game
    unsigned char exit = 0;

    //Drawing coordinates
    unsigned int x;
    unsigned int y;

    //Players car properties
    unsigned int xPlayerCar;
    color_t playerColor = green_dark_1;

    //First opponent position
    unsigned int xOpponent1;

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
    xPlayerCar = 2;
    LedTable[xPlayerCar][Y_CAR] = playerColor;

    //Start game, place the first opponent
    xOpponent1 = 2;
    LedTable[xOpponent1][Y_OPPONENT] = red_dark_3;

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
                if( (xPlayerCar + 1) < LENGTH-1){
                    LedTable[xPlayerCar][Y_CAR] = grey_medium_1;  //Erases the car
                    xPlayerCar++;
                }
                break;
            case USER_TURN_LEFT:
                if( xPlayerCar > 1 ){
                    LedTable[xPlayerCar][Y_CAR] = grey_medium_1;  //Erases the car
                    xPlayerCar--;
                }
                break;
        }


        //Car goes forward = the road goes downward
        //Back Up the road corner
        roadLeftCorner = LedTable[0][0];
        roadRightCorner = LedTable[LENGTH-1][0];

        //Paint the new road lower corners
        LedTable[0][0]= LedTable[0][1];
        LedTable[LENGTH-1][0] = LedTable[LENGTH-1][1];

        // Let the rest of the road go "down"
        for ( y = 1; y < HEIGHT ; y++){
            for(x = 0 ; x < LENGTH ;x++)
                LedTable[x][y-1] = LedTable[x][y];
        }

        //Paint the upper corners of the road
        LedTable[0][HEIGHT-1]        = roadLeftCorner;
        LedTable[LENGTH-1][HEIGHT-1] = roadRightCorner;

        for( x = 1 ; x < LENGTH - 2 ; x++)
            LedTable[x][HEIGHT - 1] = grey_medium_1;


        if( checkCollision(LedTable,xPlayerCar,Y_CAR,red_dark_3 ) == COLLISION)
            exit = 1;
        else
            LedTable[xPlayerCar][Y_CAR] = playerColor; //Paints the car in its new position


        array2Vector(LedTable,LEDS);
        sendFrame(LEDS);
        antiAliasGPIO(userOption, 2);
        turnCommand = USER_NO_TURN;
    }

}

/*
 * checkCollision
 *
 * Inputs : 2-D led table color array, the current coordinates of the car
 * Output
 *
 * NO_COLLISION : no collision occurred
 * COLLISION: car crash occurred
 *
 *
 */
unsigned char checkCollision( color_t LedTable[LENGTH][HEIGHT], unsigned char xCar, unsigned char yCar, color_t opponentColor){

   // __no_operation();
    if( (LedTable[xCar][yCar].R == opponentColor.R ) &&  (LedTable[xCar][yCar].G == opponentColor.G ) &&  (LedTable[xCar][yCar].B == opponentColor.B ) )
        return COLLISION;
    else
        return NO_COLLISION;

}



