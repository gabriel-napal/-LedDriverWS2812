/*
 * visualEffects.h
 *
 *  Created on: 14 avr. 2020
 *      Author: Laety
 */

#ifndef VISUALEFFECTS_H_
#define VISUALEFFECTS_H_

void xmasTinsel (unsigned char LEDS[PIXELS][3]);
void waterEffect (unsigned char LEDS[PIXELS][3]);
void cozy (unsigned char LEDS[PIXELS][3]);
void wave1(unsigned char LEDS[PIXELS][3]);
unsigned int waveInit(unsigned char LEDS[PIXELS][3], unsigned int theme);


#endif /* VISUALEFFECTS_H_ */
