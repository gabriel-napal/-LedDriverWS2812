/*
 * pseudoRandom.h
 *
 *  Created on: Apr 15, 2020
 *      Author: Gabriel
 */

#ifndef PSEUDORANDOM_H_
#define PSEUDORANDOM_H_


#define PRNG_A 8121
#define PRNG_C 28411
#define PRNG_M 134456


unsigned int wyhash16() ;
unsigned int rand_range16(const unsigned int s);
unsigned int pseudoRandomLCG ( unsigned long int maxNumber);


#endif /* PSEUDORANDOM_H_ */
