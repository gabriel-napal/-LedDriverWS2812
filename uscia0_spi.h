/*
 * uscia0_spi.h
 *
 *  Created on: Apr 3, 2020
 *      Author: Gabriel
 */


#ifndef USCIA0_SPI_H_
#define USCIA0_SPI_H_

#define WS2812_ZERO 0x60
#define WS2812_ONE  0x7C


void SPI_init(void);
void sendByte_SPIA (unsigned char b);



#endif /* USCIA0_SPI_H_ */
