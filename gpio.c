/*
 * gpio.c
 *
 *  Created on: Apr 10, 2020
 *      Author: Gabriel
 */

#include <gpio.h>
#include <constant.h>

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



    P1DIR &= ~(BIT2 + BIT3 + BIT4 + BIT5) ;             //Ports 1.2, 1.3, 1.4, 1.5 to OUTPUT
    P1SEL &= ~(BIT2 + BIT3 + BIT4 + BIT5) ;

    P1REN |= (BIT2 + BIT3 + BIT4 + BIT5) ;             //Configure pull up resitors
    P1OUT |= (BIT2 + BIT3 + BIT4 + BIT5) ;

/*
//    P1IE = 0x00;
    P1DIR &= (BIT0 + BIT1);                            //Set P1.0 and P1.1 to input direction, pull up resistor
    P1REN |= BIT0 + BIT1;
    P1OUT |= BIT0 + BIT1;
    */
}

/*
 * readGPIO function
 *
 * Returns a byte that holds the input Buttons status
 *
 * Button OFF   --> Bit = 0
 * Button ON    --> Bit = 1
 *
 * Buttons are mapped as follows
 *
 * GPIO_Status bit 0    --> P1.2
 * GPIO_Status bit 1    --> P1.3
 * GPIO_Status bit 2    --> P1.4
 * GPIO_Status bit 3    --> P1.5
 *
 *
 */

unsigned char readGPIO(void){

    static unsigned char GPIO_Status = 0;

    //GPIO_Status = (P4IN & BIT0) + ( P4IN & BIT1 ) + (P1IN & BIT0)<<2 +  (P1IN & BIT1)<<3;


    if (P1IN & BIT2)
        GPIO_Status &= ~BIT0;
    else
        GPIO_Status |= BIT0;

    if (P1IN & BIT3)
        GPIO_Status &= ~BIT1;
    else
        GPIO_Status |= BIT1;

    if (P1IN & BIT4)
        GPIO_Status &= ~BIT2;
    else
        GPIO_Status |= BIT2;

    if (P1IN & BIT5)
        GPIO_Status &= ~BIT3;
    else
        GPIO_Status |= BIT3;


    return (GPIO_Status & 0x0F);
}

/*
 * optionsBuffer[5] = {OPTION_S1, OPTION_S3, USER_NO_OPTION , USER_NO_OPTION , USER_NO_OPTION}
 */

void antiAliasGPIO(unsigned char optionsBuffer[KEYBOARD_BUFFER], unsigned int refreshDelay){

    unsigned int i;
    unsigned char userOption;
    unsigned char userOldOption = 0;
    unsigned int bufferIndex = 0;

    for (i = 0; i< refreshDelay; i++) {
        __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts

        while(bufferIndex < KEYBOARD_BUFFER){
            if(optionsBuffer[bufferIndex] == USER_NO_OPTION){
                userOption = readGPIO();
                if (userOption != userOldOption) {
                    optionsBuffer[bufferIndex] = userOption;
                    userOldOption = userOption;
                }
                break;
            }
            bufferIndex++;
        }
    }

}
