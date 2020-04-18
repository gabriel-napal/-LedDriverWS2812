/*
 * visualEffects.h
 *
 *  Created on: 14 avr. 2020
 *      Author: Laety
 */

#ifndef VISUALEFFECTS_H_
#define VISUALEFFECTS_H_

#include <COLORS.h>

void xmasTinsel (color_t LEDS[PIXELS]);
void waterEffect (color_t LEDS[PIXELS]);
void cozy (color_t LEDS[PIXELS]);
//void wave1(unsigned char LEDS[PIXELS][3]);
unsigned char waveInit(color_t LEDS[PIXELS], unsigned int theme);


#endif /* VISUALEFFECTS_H_ */
