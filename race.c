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

    unsigned int i;


    unsigned int turnCommand = USER_NO_TURN;

    //Event Variables
    unsigned int timeCount = 0; //Counts the number of elapsed time
    volatile unsigned int roadRefreshRatio = MIN_ROAD_REFRESH;


    color_t roadLeftCorner;
    color_t roadRightCorner;

    //Initializes array.
    //Paints the road
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


    //Start game, place the car at  pixel at the center of the board (2,0)
    if( LENGTH % 2 == 0)                //For odd length boards
        xPlayerCar = LENGTH /2;
    else
        xPlayerCar = (LENGTH - 1)/2;    //If the board length is even

    LedTable[xPlayerCar][Y_CAR] = playerColor;

    //Start game, place the first opponent
    xOpponent1 = xPlayerCar;
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


        if(( timeCount % roadRefreshRatio ) == 0){

            //Car goes forward = the road goes downward
            //Back Up the road lower corners
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

            for( x = 1 ; x < LENGTH - 1 ; x++)
                LedTable[x][HEIGHT - 1] = grey_medium_1;

            //Random generator of opponents
            xOpponent1 = newOpponent();
            if(xOpponent1 != NO_CAR_CREATED)
                LedTable[xOpponent1][Y_OPPONENT] = red_dark_3;

        }

        //Check collision
        if( checkCollision(LedTable,xPlayerCar,Y_CAR,red_dark_3 ) == COLLISION)
            exit = 1;
        else
            LedTable[xPlayerCar][Y_CAR] = playerColor; //Paints the car in its new position

        array2Vector(LedTable,LEDS);
        sendFrame(LEDS);
        antiAliasGPIO(userOption, 3);
        turnCommand = USER_NO_TURN;
        timeCount++;

        if( ((timeCount % ( 60 * (MIN_ROAD_REFRESH -roadRefreshRatio+1))) == 0) && (roadRefreshRatio > 1) )
                roadRefreshRatio--;

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

unsigned int newOpponent(void){
    static unsigned char opponentClearance = 0;
    unsigned int position =  pseudoRandomLCG(3) + 1 ;
    unsigned int probability = pseudoRandomLCG(100);

    switch (opponentClearance)
    {
    case 0:
        opponentClearance++;        // No opponent creation if clearance is 0
        return NO_CAR_CREATED;
        break;
    case 1:
        if( probability > 80){
            opponentClearance = 0;
            return position;
        }
        else{
            opponentClearance++;
            return NO_CAR_CREATED;
        }
        break;
    case 2:
        if( probability > 70){
            opponentClearance = 0;
            return position;
        }
        else{
            opponentClearance++;
            return NO_CAR_CREATED;
        }
        break;
    case 3:
        if( probability > 50){
            opponentClearance = 0;
            return position;
        }
        else{
            opponentClearance++;
            return NO_CAR_CREATED;
        }
    case 4:
        if( probability > 50){
            opponentClearance = 0;
            return position;
        }
        else{
            opponentClearance++;
            return NO_CAR_CREATED;
        }
    default:
        opponentClearance = 0;      //Another car created, no clearance between the last one
        return 2;

    }



}



