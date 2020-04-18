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



#endif /* GPIO_H_ */
