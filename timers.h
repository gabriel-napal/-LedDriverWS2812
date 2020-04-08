/*
 * timers.h
 *
 *  Created on: Apr 8, 2020
 *      Author: Gabriel
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include <msp430.h>

#define TIMERA0_1SEC 8420 // Timer will "tick" every second if the clock source is set to VLOCKL

void initTimerA0 (void);
void setTimerA0 (unsigned int counts);
void startTimerA0(void);
void stopTimerA0(void);
unsigned int readTimerA0(void);


#endif /* TIMERS_H_ */
