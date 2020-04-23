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

#endif /* EASYGAME_H_ */
