/*
 * timers.h
 *
 *  Created on: May 7, 2020
 *      Author: Gabriel
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include <msp430.h>
#include "constant.h"

#define GPIO_POLL_TH        30  // Polls every (8420 / 100)/8420 * 20 = 200ms

#define TIMERA0_1SEC 8420
void initTimerA0 (void);
void setTimerA0 (unsigned int counts);
void startTimerA0(void);

extern unsigned char readGPIO_Flag;

#endif /* TIMERS_H_ */
