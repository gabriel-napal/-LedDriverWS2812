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


/*
 * Implementations from Daniel Lemire.
 *
 * wyhash16() : random number of 16bit
 * unsigned int rand_range16(const unsigned int s) : random number from 0 to s
 */

unsigned int wyhash16() {

  static unsigned int wyhash16_x = 0;
  unsigned long int hash = wyhash16_x * 0x2ab;

  wyhash16_x += 0xfc15;

  return (((hash >> 16) ^ hash) & 0xFFFF);
}

unsigned int rand_range16(const unsigned int s) {
    unsigned int x = wyhash16();
    unsigned long int m = (unsigned long int)x * (unsigned long int)s;
    unsigned int l = (unsigned long int)m;
    if (l < s) {
        unsigned int t = -s % s;
        while (l < t) {
            x = wyhash16();
            m = (unsigned long int)x * (unsigned long int)s;
            l = (unsigned int)m;
        }
    }
    return m >> 16;
}


