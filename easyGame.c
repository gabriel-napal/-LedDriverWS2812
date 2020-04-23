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
#define USER_GO_LEFT    1
#define USER_GO_RIGHT   2
#define USER_TURN_OBJECT   3

#define GREEN_APPLE      0
#define RED_APPLE    1

void game1(color_t LEDS[PIXELS]){

    // Define a matrix that contains the RGB color code for each pair (x,y)
    color_t LedTable[LENGTH][HEIGHT];

    unsigned int x;
    unsigned int y;
    unsigned int i;

    unsigned char userOption[KEYBOARD_BUFFER] = {USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION};

    unsigned char exit = 0 ;

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
            turnCommand = USER_GO_RIGHT;
            break;
        case USER_OPTION_S4: // To go left
            turnCommand = USER_GO_LEFT;
            break;
        }

        // Erases the used option (shift left the buffer)
        for (i = 0; i < KEYBOARD_BUFFER-1 ; i++){
            userOption[i]=userOption[i+1];
        }
        userOption[KEYBOARD_BUFFER-1] = USER_NO_OPTION;

        if (turnCommand == USER_GO_RIGHT) {
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
        else if (turnCommand == USER_GO_LEFT) {
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
                apple[0] = rand_range16(LENGTH);
                apple[1] = rand_range16(HEIGHT);
                for (i = 0; i< snakeLength; i++){
                    if ((snake[i][0]== apple[0]) && (snake[i][1] == apple[1])){
                       testApple = 0;
                       break;
                    }
                }
            }

            if (rand_range16(100)>75){
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

/** KNOWN BUGS TO BE FIXED :
> Turn object function is partially available
 */

void tetris(color_t LEDS[PIXELS]){

    // templates of block
    objectTetris o_block = {{LENGTH/2, HEIGHT - 1},{LENGTH/2 + 1, HEIGHT - 1},{LENGTH/2, HEIGHT},{LENGTH/2 + 1, HEIGHT},yellow_dark_1};
    objectTetris i_block = {{LENGTH/2, HEIGHT - 1},{LENGTH/2, HEIGHT },{LENGTH/2, HEIGHT + 1},{LENGTH/2, HEIGHT + 2},cyan_bright_1};
    objectTetris s_block = {{LENGTH/2 + 1, HEIGHT - 1},{LENGTH/2, HEIGHT - 1},{LENGTH/2 + 1, HEIGHT},{LENGTH/2 + 2, HEIGHT},green_dark_1};
    objectTetris t_block = {{LENGTH/2, HEIGHT-1},{LENGTH/2 - 1, HEIGHT-1},{LENGTH/2, HEIGHT},{LENGTH/2 + 1, HEIGHT-1},purple_medium_1};
    objectTetris j_block = {{LENGTH/2, HEIGHT},{LENGTH/2, HEIGHT -1},{LENGTH/2 - 1, HEIGHT - 1},{LENGTH/2, HEIGHT + 1},blue_dark_2};
    objectTetris l_block = {{LENGTH/2, HEIGHT},{LENGTH/2 + 1, HEIGHT -1},{LENGTH/2, HEIGHT - 1},{LENGTH/2, HEIGHT + 1},orange_dark_1};
    objectTetris z_block = {{LENGTH/2 + 1, HEIGHT - 1},{LENGTH/2 + 2, HEIGHT - 1},{LENGTH/2, HEIGHT},{LENGTH/2 + 1, HEIGHT},red_dark_3};

    //setting the object that will be used
    objectTetris block = {{0,0},{0,0},{0,0},{0,0},color_off};

    // Define a matrix that contains the RGB color code for each pair (x,y)
    color_t LedTable[LENGTH][HEIGHT];
    unsigned char userOption[KEYBOARD_BUFFER] = {USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION};
    unsigned char exit = 0;
    coordinates *tempPoint;

    unsigned int x;
    unsigned int y;
    unsigned int y_delete;
    unsigned int i;

    // Boolean to be used for intern check
    unsigned int newObject = TRUE;
    unsigned int deleteLine = FALSE;
    unsigned int checkNextBlock = FALSE;

    // To be used for choosing the next object
    unsigned int randomObject = 0;

    unsigned int speed = 10;
    unsigned int time_count = 0;
    unsigned int nbObject = 0;
    unsigned int nbRotation = 0;

    color_t background = color_off;

    unsigned int turnCommand = USER_NO_TURN;

    //Initializes array
    for (x = 0 ; x < LENGTH ; x++){
        for (y = 0 ; y < HEIGHT ; y++){
            LedTable[x][y] = background;
        }
    }

    while (exit!=1){
        // Creates object
        if (newObject == TRUE) {
            nbRotation = 0;
            randomObject = rand_range16(7);
                   switch(randomObject){
                       case 0:
                           block = o_block;
                           break;
                       case 1:
                           block = i_block;
                           break;
                       case 2:
                           block = s_block;
                           break;
                       case 3:
                           block = t_block;
                           break;
                       case 4:
                           block = j_block;
                           break;
                       case 5:
                           block = l_block;
                           break;
                       case 6:
                           block = z_block;
                           break;
                   }

                   // Before creating the new object, check if the space is available, if not, the player loses
                   tempPoint = &block.point1;
                   for (i = 0; i < 4; i++){
                       if (checkCollision(LedTable, (*tempPoint).x, (*tempPoint).y, &background, &block) == FALSE){
                           looser(LEDS);
                           exit = 1;
                           break;
                       }
                       tempPoint = (++tempPoint);
                   }
                   // set the new object in the top of the game and check for each point, if it's included in the display
                   if (block.point1.y < HEIGHT)
                       LedTable[block.point1.x][block.point1.y] = block.color;
                   if (block.point2.y < HEIGHT)
                       LedTable[block.point2.x][block.point2.y] = block.color;
                   if (block.point3.y < HEIGHT)
                       LedTable[block.point3.x][block.point3.y] = block.color;
                   if (block.point4.y < HEIGHT)
                       LedTable[block.point4.x][block.point4.y] = block.color;

                   nbObject ++;
                   // Increase speed after few blocks
                   if (nbObject%10 == 0){
                       if (speed > 3)
                           speed--;
                   }
                   newObject = FALSE;
        }
        else if (newObject == FALSE){
                switch (userOption[0]){
                case USER_OPTION_S1:
                    //To turn the position of the object
                    turnCommand = USER_TURN_OBJECT;
                    break;
                case USER_OPTION_S2:
                    //exit effect / game
                    exit = 1;
                    break;
                case USER_OPTION_S3: // To go right
                    turnCommand = USER_GO_RIGHT;
                    break;
                case USER_OPTION_S4: // To go left
                    turnCommand = USER_GO_LEFT;
                    break;
                }
                // Erases the used option (shift left the buffer)
                for (i = 0; i < KEYBOARD_BUFFER-1 ; i++){
                    userOption[i]=userOption[i+1];
                }
                userOption[KEYBOARD_BUFFER-1] = USER_NO_OPTION;

               if (turnCommand != USER_NO_OPTION){
                   switch (turnCommand){

                   case USER_GO_LEFT :
                       checkNextBlock = TRUE;       //By default, the block movement is doable
                       tempPoint = &block.point1;   //Pointer created to sweep al four points
                       for (i = 0; i < 4; i++){
                           if (checkCollision(LedTable,(*tempPoint).x - 1, (*tempPoint).y, &background, &block) == FALSE){
                               checkNextBlock = FALSE;  // I a collision occurs, movement is no valid.
                               break;
                           }
                           tempPoint = (++tempPoint);
                       }
                       if (checkNextBlock==TRUE)        //Movement is valid, then erase the old block position
                           moveTetrisObject(LedTable, &block, -1, 0, &background);
                       break;

                   case USER_GO_RIGHT :
                       checkNextBlock = TRUE;
                       tempPoint = &block.point1;
                       for (i = 0; i < 4; i++){
                           if (checkCollision(LedTable,(*tempPoint).x + 1, (*tempPoint).y, &background, &block) == FALSE){
                               checkNextBlock = FALSE;
                               break;
                           }
                           tempPoint = (++tempPoint);
                       }
                       if (checkNextBlock==TRUE) //Movement is valid, then erase the old block position
                           moveTetrisObject(LedTable, &block, 1, 0, &background);
                       break;

                   case USER_TURN_OBJECT :
                       nbRotation = rotateTetrisObject(LedTable, &block, randomObject, &background, nbRotation);
                       break;
                   }
               }

               if (time_count == speed){
                   checkNextBlock = TRUE ;
                   tempPoint = &block.point1;
                   for (i = 0; i < 4; i++){
                       if (checkCollision(LedTable,(*tempPoint).x, (*tempPoint).y - 1, &background, &block) == FALSE){
                           checkNextBlock = FALSE;
                           break;
                        }
                       tempPoint = (++tempPoint);
                       }

                   if (checkNextBlock==TRUE){
                       //Movement is valid, then erase the old block position
                       moveTetrisObject(LedTable, &block, 0, -1, &background);
                   }
                   else {
                       // Check if it can delete lines
                       for (y=0; y<HEIGHT; y++){
                           deleteLine = TRUE;
                           for (x=0; x<LENGTH; x++){
                               if ((LedTable[x][y].R==background.R) && (LedTable[x][y].G==background.G) && (LedTable[x][y].B==background.B)){
                                   deleteLine = FALSE;
                                   break;
                               }
                           }
                           if (deleteLine == TRUE){
                               for (y_delete = y; y_delete<HEIGHT-1; y_delete++){
                                   for (x=0; x<LENGTH; x++){
                                       LedTable[x][y_delete].R = LedTable[x][y_delete+1].R;
                                       LedTable[x][y_delete].G = LedTable[x][y_delete+1].G;
                                       LedTable[x][y_delete].B = LedTable[x][y_delete+1].B;
                                   }
                               }
                               for (x=0; x<LENGTH; x++){
                                   LedTable[x][HEIGHT-1] = background;
                               }
                               y--;
                           }
                       }
                       // Check if the player loses
                       if ((block.point1.y == HEIGHT) || (block.point2.y == HEIGHT) ||(block.point3.y == HEIGHT) ||(block.point4.y == HEIGHT)){
                           looser(LEDS);
                           exit = 1;
                       }
                       // if not create a new object
                       else
                           newObject = TRUE;
                   }
                   time_count = 0;
               }
        }
        if (exit != 1){
            array2Vector(LedTable,LEDS);
            sendFrame(LEDS);
            antiAliasGPIO(userOption, 2);
            time_count++;
            turnCommand = USER_NO_TURN;
        }
    }

}

/*
 * moveTetrisObject:
 * Moves a tetris block (down, right or left)
 *
 * INPUTS:
 *
 * color_t LedTable     : Led Table RGB array
 * objectTetris* block  : tetris block to be moved
 * signed int movX      : x-displacement (could be negative)
 * signed int movY      : x-displacement (could be negative)
 * color_t* background  : background color
 *
 */

void moveTetrisObject(color_t LedTable[LENGTH][HEIGHT], objectTetris* block, signed int movX, signed int movY, color_t* background){

    //Erases the old block
    if ((*block).point1.y < HEIGHT)
        LedTable[(*block).point1.x][(*block).point1.y] = *background;
    if ((*block).point2.y < HEIGHT)
        LedTable[(*block).point2.x][(*block).point2.y] = *background;
    if ((*block).point3.y < HEIGHT)
        LedTable[(*block).point3.x][(*block).point3.y] = *background;
    if ((*block).point4.y < HEIGHT)
        LedTable[(*block).point4.x][(*block).point4.y] = *background;

    // Recalculate each point
    (*block).point1.x = (*block).point1.x + movX;
    (*block).point2.x = (*block).point2.x + movX;
    (*block).point3.x = (*block).point3.x + movX;
    (*block).point4.x = (*block).point4.x + movX;
    (*block).point1.y = (*block).point1.y + movY;
    (*block).point2.y = (*block).point2.y + movY;
    (*block).point3.y = (*block).point3.y + movY;
    (*block).point4.y = (*block).point4.y + movY;

    //RE draw the block in the Led Table array
    if ((*block).point1.y < HEIGHT)
        LedTable[(*block).point1.x][(*block).point1.y] = (*block).color;
    if ((*block).point2.y < HEIGHT)
        LedTable[(*block).point2.x][(*block).point2.y] = (*block).color;
    if ((*block).point3.y < HEIGHT)
        LedTable[(*block).point3.x][(*block).point3.y] = (*block).color;
    if ((*block).point4.y < HEIGHT)
        LedTable[(*block).point4.x][(*block).point4.y] = (*block).color;
}

/*
 * Performs a simple 2-D Matrix rotation:
 *
 * 1) Sets a first pivot-point
 * 2) then calculates the coordinates relative to the pivot (tempX and tempY)
 *
 * 3) Rotates and sets the absolute coordinate in the same step
 *      newX = tempY + pivot_x
 *      newY = -tempX + pivot_y
 *
 */
unsigned int rotateTetrisObject(color_t LedTable[LENGTH][HEIGHT], objectTetris* block, int randomObject, color_t* background, unsigned int nbOfRotation) {

    unsigned int i;
    unsigned int j;
    unsigned int canRotate;
    int tempX;
    int tempY;
    int tempXforCheck;
    int tempYforCheck;
    coordinates *tempPoint;
    objectTetris rotatedBlock;
    int matriceRotationi [4][2] = {{-1,2},{0,1},{1,0},{2,-1}};

    // o_block
    if  (randomObject == 0){
        //Do nothing
        return 0;
    }
    // i_block
    else if (randomObject == 1){
        // All points of the object shall rotate, first step is to know which rotation to be used.
        for (i=0; i<nbOfRotation; i++){
            for (j=0; j<4; j++){
                matriceRotationi [j][0] = tempX;
                matriceRotationi [j][0] = matriceRotationi [j][1];
                matriceRotationi [j][1] = -tempX;
            }
        }

        tempPoint = &((*block).point1);
        canRotate = TRUE;
        for (i = 0; i<4 ; i++) {
            tempXforCheck = (*tempPoint).x + matriceRotationi[i][0];
            tempYforCheck = (*tempPoint).y + matriceRotationi[i][0];
            if (checkCollision(LedTable,tempXforCheck,tempYforCheck, background, block) == FALSE){
                canRotate = FALSE;
                break;
            }
            tempPoint = (++tempPoint);
         }

        // if it's ok, then makes the block rotate
        if (canRotate==TRUE){
            tempPoint = &((*block).point1);
            if ((*tempPoint).y < HEIGHT)
                LedTable[(*tempPoint).x][(*tempPoint).y] = *background;
            for (i = 0; i<4 ; i++) {
                (*tempPoint).x = (*tempPoint).x + matriceRotationi[i][0];
                (*tempPoint).y = (*tempPoint).y + matriceRotationi[i][0];
                // Displays the new pixel if it's on the display
                if ((*tempPoint).y < HEIGHT)
                    LedTable[(*tempPoint).x][(*tempPoint).y] = (*block).color;
                tempPoint = (++tempPoint);
            }
            return nbOfRotation++;
        }
        else
            return nbOfRotation;
    }

    /*
    *       All other blocks rotates with :
    *      newX = tempY + pivot_x
    *      newY = -tempX + pivot_y
    */
    else
    {

        // First, only check if it can rotate
        //Assign the block to the rotated block, then set the pointer to the rotated block
        rotatedBlock = *block;
        tempPoint = &(rotatedBlock.point2);

        canRotate = TRUE;
        for (i = 0; i < 3; i++){
            tempX = (*tempPoint).x - block->point1.x;   //Obtains the relative coordinates
            tempY = (*tempPoint).y - block->point1.y;

            (*tempPoint).x = tempY + block->point1.x;   //Calculate the rotated point, then check if the rotation is possible
            (*tempPoint).y = -tempX + block->point1.y;

            if (checkCollision(LedTable, (*tempPoint).x, (*tempPoint).y, background, block) == FALSE){
                canRotate = FALSE;
                break;
            }
            tempPoint = (++tempPoint);
        }

        // If the rotation is possible, then the rotated coordinates are already calculated at rotatedBlock
        if (canRotate == TRUE)
        {

            tempPoint = &((*block).point1);
            //Erase the old block
            for (i = 0; i < 4; i++){
                //First deletes the last pixel
                if ((*tempPoint).y < HEIGHT)
                    LedTable[(*tempPoint).x][(*tempPoint).y] = *background;
                tempPoint = (++tempPoint);
            }
            //Assign  the rotated block to the current block
            (*block) = rotatedBlock;

            tempPoint = &((*block).point1);
            //Draw the new block, rotated
            for (i = 0; i < 4; i++){
                // Draws the new block if it lays on the display
                if ((*tempPoint).y < HEIGHT)
                    LedTable[(*tempPoint).x][(*tempPoint).y] = (*block).color;
                tempPoint = (++tempPoint);
            }
        }
    }
    return 0;
}

/*
 * checkCollision
 *
 * INPUTS:
 * color_t LedTable : 2-D Matrix of the led table
 * int xCoordinate : one of the coordinates of the point to be checked.
 * int yCoordinate : one of the coordinates of the point to be checked
 * color_t background : RGB of the board background
 * objectTetris *block : address containing the information of the origin block being rotated.
 *
 * OUTPUTS:
 *
 * FALSE : rotation cannot be performed
 * TRUE : rotation can be done
 *
 */

unsigned int checkCollision(color_t LedTable[LENGTH][HEIGHT], int xCoordinate, int yCoordinate, color_t *background,objectTetris *block ){

    //First check, the coordinates must be inside the table, but it could be higher than HEIGHT
if ((yCoordinate >= 0) && (xCoordinate >= 0) && (xCoordinate < LENGTH )) {
    if (yCoordinate < HEIGHT){
        //Second check, see if the desired is not empty
        if ((LedTable[xCoordinate][yCoordinate].R != (*background).R) ||
            (LedTable[xCoordinate][yCoordinate].B != (*background).B) ||
            (LedTable[xCoordinate][yCoordinate].G != (*background).G)) {

            // If the coordinate is not empty, check if the coordinate is occupied by the block itself.
            if ( (( xCoordinate == (*block).point1.x) && ( yCoordinate == (*block).point1.y)) ||
                 (( xCoordinate == (*block).point2.x) && ( yCoordinate == (*block).point2.y)) ||
                 (( xCoordinate == (*block).point3.x) && ( yCoordinate == (*block).point3.y)) ||
                 (( xCoordinate == (*block).point4.x) && ( yCoordinate == (*block).point4.y) ))
               return TRUE; //If it matches with a coordinate of the block, the rotation can be done
            else
                return FALSE;
        }
        else
            return TRUE; // If its empty, the rotation can be done
    }
    else
        return TRUE; // If its empty, the rotation can be done
 }
else // Rotation lays outside the x-Space, or below the floor. It cannot be done.
    return FALSE;
}
