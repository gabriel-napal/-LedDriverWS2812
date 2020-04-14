/*
 * LEDInterface.h
 *
 *  Created on: 14 avr. 2020
 *      Author: Laety
 */

#ifndef LEDINTERFACE_H_
#define LEDINTERFACE_H_


void sendPixel (unsigned char r, unsigned char g, unsigned char b);
void sendFrame (unsigned char LEDS[PIXELS][3]);

/* Visual function */
void lightsOff(unsigned char LEDS[PIXELS][3]);                                                   //Turns all LEDS OFF
void setPixel(unsigned char LedTable[LENGTH][HEIGHT][3], unsigned char x, unsigned char y, unsigned char r, unsigned char g, unsigned char b);
void array2Vector (unsigned char inputArray[LENGTH][HEIGHT][3], unsigned char outputVector[PIXELS][3]);


#endif /* LEDINTERFACE_H_ */
