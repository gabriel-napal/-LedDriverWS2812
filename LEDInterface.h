/*
 * LEDInterface.h
 *
 *  Created on: 14 avr. 2020
 *      Author: Laety
 */

#ifndef LEDINTERFACE_H_
#define LEDINTERFACE_H_

#include <COLORS.h>

void sendPixel (color_t LED);
void sendFrame (color_t LEDS[PIXELS]);

/* Visual function */
void lightsOff(color_t LEDS[PIXELS]);                                                   //Turns all LEDS OFF
void setPixel(unsigned char LedTable[LENGTH][HEIGHT][3], unsigned char x, unsigned char y, unsigned char r, unsigned char g, unsigned char b);
void array2Vector (color_t inputArray[LENGTH][HEIGHT], color_t outputVector[PIXELS]);


#endif /* LEDINTERFACE_H_ */
