/*
 * timers.h
 *
 *  Created on: May 7, 2020
 *      Author: Gabriel
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include <msp430.h>

#define TIMERA0_1SEC 8420
void initTimerA0 (void);
void setTimerA0 (unsigned int counts);
void startTimerA0(void);


#endif /* TIMERS_H_ */
