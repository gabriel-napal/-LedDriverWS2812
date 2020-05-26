/*
 * gpio.h
 *
 *  Created on: Apr 10, 2020
 *      Author: Gabriel
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <msp430.h>

unsigned char readGPIO(void);
void antiAliasGPIO(unsigned char optionsBuffer[5], unsigned int refreshDelay);
void initGPIO(void);

#define GPIO_POLL_TH        20  // Polls every (8420 / 100)/8420 * 20 = 200ms

unsigned char userOptionMain;

#endif /* GPIO_H_ */
