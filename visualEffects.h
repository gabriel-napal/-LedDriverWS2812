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

// to be replace by displayText
void looser(color_t LEDS[PIXELS]);


unsigned char displayText(color_t LEDS[PIXELS], char* texte, unsigned char textLength, color_t frameColor, color_t textColor, unsigned int stringIndex);
unsigned char displayTextHorizontal(color_t LEDS[PIXELS], char* stringNumber, unsigned char textLength, color_t frameColor, color_t frameColor2, color_t textColor, unsigned char nbOfPlayers);

#define NUMBER_MAX_LENGTH   3

#endif /* VISUALEFFECTS_H_ */
