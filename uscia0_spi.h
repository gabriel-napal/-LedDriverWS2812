/*
 * uscia0_spi.h
 *
 *  Created on: May 7, 2020
 *      Author: Gabriel
 */

#ifndef USCIA0_SPI_H_
#define USCIA0_SPI_H_

#include <msp430.h>

#define WS2812_ZERO 0x60
#define WS2812_ONE  0x7C

void spiInit(void);
void sendByte_SPIA (unsigned char b);



#endif /* USCIA0_SPI_H_ */
