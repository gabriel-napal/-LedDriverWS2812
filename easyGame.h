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

void game1(color_t LEDS[PIXELS]);
void snake(color_t LEDS[PIXELS]);
void tetris(color_t LEDS[PIXELS]);
void moveTetrisObject(color_t LedTable[LENGTH][HEIGHT], objectTetris* block, signed int movX, signed int movY, color_t* background);
void RotateTetrisObject(objectTetris* block, int randomObject);


#endif /* EASYGAME_H_ */
