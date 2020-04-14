/*
 * gpio.c
 *
 *  Created on: Apr 10, 2020
 *      Author: Gabriel
 */

#include <gpio.h>

/*
 * initMSP_LEDS function
 *
 * Initializes PortJ from 0 to 3 to output for MSP Board LEDs (for debug purposes):
 *
 */

void initMSP_LEDS (void){
    PJDIR |= BIT0 + BIT1 + BIT2 + BIT3;                     // Set PJ.0 and PJ.1 to output direction
}


/*
 * initGPIO function
 *
 * Initializes INPUT ports :
 *
 * P4.0 --> S1
 * P4.1 --> S2
 * P1.0 --> S3
 * P1.1 --> S4
 *
 */

void initGPIO(void){

    P4IE = 0x00;
    P4DIR &= (BIT0 + BIT1);                            //Set P4.0 and P4.1 to input direction, pull up resistor
    P4REN |= BIT0 + BIT1;
    P4OUT |= BIT0 + BIT1;

    P1IE = 0x00;
    P1DIR &= (BIT0 + BIT1);                            //Set P1.0 and P1.1 to input direction, pull up resistor
    P1REN |= BIT0 + BIT1;
    P1OUT |= BIT0 + BIT1;
}

/*
 * readGPIO function
 *
 * Returns a byte that holds the input switches status
 *
 * Switch OFF   --> Bit = 0
 * Switch ON    --> Bit = 1
 *
 * Switches are mapped as follows
 *
 * GPIO_Status bit 0    --> P4.0
 * GPIO_Status bit 1    --> P4.1
 * GPIO_Status bit 2    --> P1.0
 * GPIO_Status bit 3    --> P1.1
 *
 *
 */

unsigned char readGPIO(void){

    static unsigned char GPIO_Status = 0;

    //GPIO_Status = (P4IN & BIT0) + ( P4IN & BIT1 ) + (P1IN & BIT0)<<2 +  (P1IN & BIT1)<<3;


    if (P4IN & BIT0)
        GPIO_Status &= ~BIT0;
    else
        GPIO_Status |= BIT0;

    if (P4IN & BIT1)
        GPIO_Status &= ~BIT1;
    else
        GPIO_Status |= BIT1;

    if (P1IN & BIT0)
        GPIO_Status &= ~BIT2;
    else
        GPIO_Status |= BIT2;

    if (P1IN & BIT1)
        GPIO_Status &= ~BIT3;
    else
        GPIO_Status |= BIT3;


    return (GPIO_Status & 0x0F);
}

unsigned char antiAliasGPIO(unsigned char oldOption, unsigned int refreshDelay){

    unsigned int i = 0;
    unsigned char userOption;

    while (i < refreshDelay) {
        __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
        userOption = readGPIO();
        if (oldOption == userOption)
            i ++;
        else
            break;
    }

    return (userOption);
}
