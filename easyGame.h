/*
 * easyGame.h
 *
 *  Created on: 14 avr. 2020
 *      Author: Laety
 */

#ifndef EASYGAME_H_
#define EASYGAME_H_

#include <COLORS.h>

typedef struct coordinates {
        unsigned int x;
        unsigned int y;
    } coordinates ;

    typedef struct objectTetris{
        coordinates point1;
        coordinates point2;
        coordinates point3;
        coordinates point4;
        color_t color;
    } objectTetris;

void snake(color_t LEDS[PIXELS]);
void tetris(color_t LEDS[PIXELS]);
void moveTetrisObject(color_t LedTable[LENGTH][HEIGHT], objectTetris* block, signed int movX, signed int movY, color_t* background);
unsigned int rotateTetrisObject(color_t LedTable[LENGTH][HEIGHT], objectTetris* block, int randomObject, color_t* background, unsigned int nbOfRotation);
unsigned int checkCollision(color_t LedTable[LENGTH][HEIGHT], int xCoordinate, int yCoordinate, color_t *background,objectTetris *block);

#define DIRECTION_X_INCREASING      0
#define DIRECTION_Y_INCREASING      1
#define DIRECTION_X_DECREASING      2
#define DIRECTION_Y_DECREASING      3

#define USER_NO_TURN                0
#define USER_GO_LEFT                1
#define USER_GO_RIGHT               2
#define USER_TURN_OBJECT            3
#define USER_GO_DOWN                4

#define NEXT_LEVEL_TH               5


#define GREEN_APPLE                 0
#define RED_APPLE                   1
#define SNAKE_INITIAL_SPEED         30
#define SNAKE_MAX_SPEED            5
#define NEXT_LEVEL_TH               5

#endif /* EASYGAME_H_ */
