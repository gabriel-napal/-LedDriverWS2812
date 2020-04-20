/*
 * race.h
 *
 *  Created on: Apr 18, 2020
 *      Author: Gabriel
 */

#ifndef RACE_H_
#define RACE_H_


/*
 * INCLUDED LIBRARIES
 */

#include <COLORS.h>
#include <constant.h>
#include <msp430.h>
#include <gpio.h>
#include <LEDInterface.h>
#include <pseudoRandom.h>

/*
 * CONSTANTS
 */
// USER COMMAND
#define USER_NO_TURN        0
#define USER_TURN_LEFT      1
#define USER_TURN_RIGHT     2

//COORDINATE CONSTANTS
#define Y_CAR               0
#define Y_OPPONENT          HEIGHT-1

//EVENT CONSTANTS
#define COLLISION           1
#define NO_COLLISION        0

//FUNCTION DECLARATIONS
void race(color_t LEDS[PIXELS]);
unsigned char checkCollision( color_t LedTable[LENGTH][HEIGHT], unsigned char xCar, unsigned char yCar, color_t opponentColor);

#endif /* RACE_H_ */
