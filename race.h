/*
 * race.h
 *
 *  Created on: Apr 18, 2020
 *      Author: Gabriel
 */

#ifndef RACE_H_
#define RACE_H_


#include <COLORS.h>
#include <constant.h>
#include <msp430.h>
#include <gpio.h>
#include <LEDInterface.h>
#include <easyGame.h>

#define Y_CAR 0

void race(color_t LEDS[PIXELS]);

#endif /* RACE_H_ */
