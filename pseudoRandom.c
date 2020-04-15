/*
 * pseudoRandom.c
 *
 *  Created on: Apr 14, 2020
 *      Author: Gabriel
 */
#include <pseudoRandom.h>
/*
 *pseudoRandom :
 *
 * Generates a random number using a Linear Congruential Generator.
 * Formula is Xn+1 = (a*Xn + c )mod m
 * As a raw output, the values are between 0 and m-1. The function takes an input to re-scale its random output.
 *
 * Inputs :
 *
 * unsigned int maxNumber : used to re-scale the otupt
 *
 * Output:
 *
 * unsigned int random number from 0 to maxNumber
 *
 *
 */
unsigned int pseudoRandom ( unsigned int maxNumber){

    static unsigned int Xn = 1; // Seed is set to 1, then Xn is refreshed every time the function is called.

    Xn = ( PRNG_A * Xn + PRNG_C ) % PRNG_M;

    return (maxNumber * Xn)/(PRNG_M-2);

}

