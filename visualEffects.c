/*
 * visualEffects.c
 *
 *  Created on: 14 avr. 2020
 *      Author: Laety
 */

#include <constant.h>
#include <msp430.h>
#include <gpio.h>
#include <LEDInterface.h>
#include <pseudoRandom.h>
#include <visualEffects.h>

void xmasTinsel (color_t LEDS[PIXELS]){

    unsigned int j;
    color_t auxPixel;

    //Buffers the last pixel
    auxPixel = LEDS[PIXELS-1];

    for(j = PIXELS-1 ; j>0 ; j--){
        LEDS[j] = LEDS[j-1];
    }

    LEDS[0] = auxPixel;

}


void waterEffect (color_t LEDS[PIXELS]){

    unsigned int j;
    unsigned int index = 0;
    static unsigned char exit;

    exit = 0;

    while (exit!=1){

        if(readGPIO() == P1_RED){
            //exit waterEffect
            exit = 1;
        }
        else{
            for(j=0 ; j<PIXELS ; j++){
                index = rand_range16(100);
                if (index < 2)
                    LEDS[j]=blue_bright_1;
                else if ((index >= 2) && (index <7))
                    LEDS[j]=blue_bright_2;
                else if ((index >= 7) && (index <15))
                    LEDS[j]=blue_bright_3;
                else if ((index >= 15) && (index <30))
                    LEDS[j]=blue_medium_1;
                else if ((index >= 30) && (index <70))
                    LEDS[j]=blue_medium_2;
                else if ((index >= 70) && (index <85))
                    LEDS[j]=blue_medium_3;
                else if ((index >= 85) && (index <92))
                    LEDS[j]=blue_dark_1;
                else if ((index >= 92) && (index <98))
                    LEDS[j]=blue_dark_2;
                else if (index >= 98)
                    LEDS[j]=blue_dark_3;
            }
            sendFrame(LEDS);
        }
        __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
    }
}

void cozy (color_t LEDS[PIXELS]){

    unsigned int j;
    unsigned char exit;


    //Initializes the LED Array
    for(j=0 ; j<PIXELS ; j++){
        LEDS[j] = orange_dark_1;
    }

    //Set some random blue pixels
    LEDS[5] = blue_dark_1;
    LEDS[14]= blue_dark_1;
    LEDS[24]= blue_dark_1;

    exit = 0;

    while (exit!=1){

        if(readGPIO() == P1_RED){
            //exit cozy
            exit = 1;

        }
        else{
            xmasTinsel(LEDS);
            sendFrame(LEDS);
        }
        __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
    }
}

/* TO BE UPDATED
void wave1(unsigned char LEDS[PIXELS][3]){

    // Define a matrix that contains the RGB color code for each pair (x,y)
    unsigned char LedTable[LENGTH][HEIGHT][3];

    unsigned int x;
    unsigned int y;

    unsigned char userOption;

    unsigned char exit = 0;

    //Initializes array
    // Medium dark
    for (x = 0 ; x < LENGTH ; x++){

            LedTable[x][0][0] = 0x00;
            LedTable[x][0][1] = 0xB0;
            LedTable[x][0][2] = 0xCD;

            LedTable[x][0][0] = 0xFF;
            LedTable[x][0][1] = 0x00;
            LedTable[x][0][2] = 0x00;
    }
    //Dark
    for (x = 0 ; x < LENGTH ; x++){

            LedTable[x][1][0] = 0x00;
            LedTable[x][1][1] = 0xB0;
            LedTable[x][1][2] = 0xFF;

            LedTable[x][1][0] = 0x00;
            LedTable[x][1][1] = 0xFF;
            LedTable[x][1][2] = 0x00;
    }
    // Medium dark
    for (x = 0 ; x < LENGTH ; x++){

            LedTable[x][2][0] = 0x00;
            LedTable[x][2][1] = 0xB0;
            LedTable[x][2][2] = 0xCD;

            LedTable[x][2][0] = 0xFF;
            LedTable[x][2][1] = 0x00;
            LedTable[x][2][2] = 0x00;
    }


    //Light
    for (x = 0 ; x < LENGTH ; x++){
        for (y = 3 ; y < HEIGHT ; y++){
            LedTable[x][y][0] = 0x00;
            LedTable[x][y][1] = 0x10;
            LedTable[x][y][2] = 0xCD;

            LedTable[x][y][0] = 0x00;
            LedTable[x][y][1] = 0x00;
            LedTable[x][y][2] = 0xFF;
        }

    }

    array2Vector(LedTable,LEDS);

    while (exit!=1){

        userOption = readGPIO();

       if(userOption == USER_OPTION_S4)
           exit = 1;
       else
           sendFrame(LEDS);

        __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
    }

}
*/

unsigned char waveInit(color_t LEDS[PIXELS], unsigned int theme){

    // Define a matrix that contains the RGB color code for each pair (x,y)
    unsigned int j;
    unsigned char userOption [KEYBOARD_BUFFER]  = {USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION};

    color_t optionTheme[3][3] =  {{red_bright_1, red_medium_1, red_dark_1}, {green_bright_1, green_medium_1, green_dark_1}, {blue_bright_1, blue_medium_1, blue_dark_1}};

        // Set color in function of the theme
       for(j=0 ; j<PIXELS ; j++){
           LEDS[j] = optionTheme[theme][2];
       }

       //Set some random pixels
       LEDS[5]  = optionTheme[theme][0];
       LEDS[14] = optionTheme[theme][0];
       LEDS[24] = optionTheme[theme][1];
       LEDS[9] = optionTheme[theme][1];

    while (1){

       antiAliasGPIO(userOption, ANTI_BOUNCE);
       if(userOption[0] != USER_NO_OPTION){
           //exit waveInit
           return userOption[0];
           //exit = 1;
       }
       else{
           xmasTinsel(LEDS);
           sendFrame(LEDS);
       }

        __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
    }

}

void looser(color_t LEDS[PIXELS]){

    unsigned int j;
    unsigned int i;

    for (i = 0; i<3; i++){
        for(j=0 ; j<PIXELS ; j++){
            LEDS[j] = red_dark_1;
        }

        sendFrame(LEDS);
        __bis_SR_register(LPM0_bits + GIE);
        __bis_SR_register(LPM0_bits + GIE);
        __bis_SR_register(LPM0_bits + GIE);

        for(j=0 ; j<PIXELS ; j++){
            LEDS[j] = color_off;
        }

        sendFrame(LEDS);
        __bis_SR_register(LPM0_bits + GIE);
        __bis_SR_register(LPM0_bits + GIE);
        __bis_SR_register(LPM0_bits + GIE);
    }
}


unsigned char displayText(color_t LEDS[PIXELS], char* texte, unsigned char textLength, unsigned char speed, color_t frameColor, color_t textColor){

    unsigned int i;
    unsigned int j;
    unsigned int x;
    unsigned int y;
    unsigned int l;

    unsigned char userOption [KEYBOARD_BUFFER]  = {USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION, USER_NO_OPTION};

    color_t LedTable[LENGTH][HEIGHT];

    unsigned char letterA [22][2]= {{4,12},{5,12},{3,11},{6,11},{3,10},{6,10},{3,9},{6,9},{2,8},{7,8},{2,7},{7,7},{2,6},{7,6},{1,5},{8,5},{1,4},{8,4},{3,8},{4,8},{5,8},{6,8}};
    unsigned char letterB [21][2]= {{3,12},{4,12},{5,12},{3,11},{3,10},{3,9},{3,8},{3,7},{3,6},{3,5},{3,4},{4,8},{5,8},{4,4},{5,4},{6,11},{6,10},{6,9},{6,7},{6,6},{6,5}};
    unsigned char letterC [15][2]= {{3,12},{4,12},{5,12},{6,12},{3,11},{3,10},{3,9},{3,8},{3,7},{3,6},{3,5},{3,4},{4,4},{5,4},{6,4}};
    unsigned char letterD [20][2]= {{3,12},{4,12},{5,12},{3,11},{3,10},{3,9},{3,8},{3,7},{3,6},{3,5},{3,4},{4,4},{5,4},{6,11},{6,10},{6,9},{6,8},{6,7},{6,6},{6,5}};
    unsigned char letterE [17][2]= {{3,12},{4,12},{5,12},{6,12},{3,11},{3,10},{3,9},{3,8},{3,7},{3,6},{3,5},{3,4},{4,8},{5,8},{4,4},{5,4},{6,4}};
    unsigned char letterF [14][2]= {{3,12},{4,12},{5,12},{6,12},{3,11},{3,10},{3,9},{3,8},{3,7},{3,6},{3,5},{3,4},{4,8},{5,8}};
    unsigned char letterG [20][2]= {{3,12},{4,12},{5,12},{6,12},{3,11},{3,10},{3,9},{3,8},{3,7},{3,6},{3,5},{3,4},{5,8},{6,8},{4,4},{5,4},{6,4},{6,5},{6,6},{6,7}};
    unsigned char letterH [20][2]= {{3,12},{3,11},{3,10},{3,9},{3,8},{3,7},{3,6},{3,5},{3,4},{6,12},{6,11},{6,10},{6,9},{6,8},{6,7},{6,6},{6,5},{6,4},{4,8},{5,8}};
    unsigned char letterI [7][2]= {{4,12},{4,9},{4,8},{4,7},{4,6},{4,5},{4,4}};
    unsigned char letterJ [13][2]= {{6,12},{6,11},{6,10},{6,9},{6,8},{6,7},{6,6},{6,5},{6,4},{5,4},{4,4},{3,4},{5,12}};
    unsigned char letterK [18][2]= {{3,12},{3,11},{3,10},{3,9},{3,8},{3,7},{3,6},{3,5},{3,4},{4,8},{5,9},{5,7},{6,10},{6,6},{7,11},{7,12},{7,5},{7,4}};
    unsigned char letterL [12][2]= {{3,12},{3,11},{3,10},{3,9},{3,8},{3,7},{3,6},{3,5},{3,4},{4,4},{5,4},{6,4}};
    unsigned char letterM [21][2]= {{2,11},{2,10},{2,9},{2,8},{2,7},{2,6},{2,5},{2,4},{6,11},{6,10},{6,9},{6,8},{6,7},{6,6},{6,5},{6,4},{3,12},{5,12},{4,11},{4,10},{4,9}};
    unsigned char letterN [22][2]= {{2,11},{2,10},{2,9},{2,8},{2,7},{2,6},{2,5},{2,4},{7,11},{7,10},{7,9},{7,8},{7,7},{7,6},{7,5},{7,4},{3,10},{4,9},{4,8},{5,7},{5,6},{6,5}};
    unsigned char letterO [22][2]= {{3,12},{4,12},{5,12},{6,12},{3,11},{3,10},{3,9},{3,8},{3,7},{3,6},{3,5},{3,4},{4,4},{5,4},{6,11},{6,10},{6,9},{6,8},{6,7},{6,6},{6,5},{6,4}};
    unsigned char letterP [18][2]= {{3,12},{4,12},{5,12},{6,12},{3,11},{3,10},{3,9},{3,8},{3,7},{3,6},{3,5},{3,4},{4,8},{5,8},{6,8},{6,9},{6,10},{6,11}};
    unsigned char letterQ [24][2]= {{3,12},{4,12},{5,12},{6,12},{3,11},{3,10},{3,9},{3,8},{3,7},{3,6},{3,5},{3,4},{4,4},{5,4},{6,11},{6,10},{6,9},{6,8},{6,7},{6,6},{6,5},{6,4},{7,3},{8,2}};
    unsigned char letterR [22][2]= {{3,12},{4,12},{5,12},{6,12},{3,11},{3,10},{3,9},{3,8},{3,7},{3,6},{3,5},{3,4},{4,8},{5,8},{6,8},{6,9},{6,10},{6,11},{5,7},{6,6},{7,5},{7,4}};
    unsigned char letterS [14][2]= {{4,12},{5,12},{6,12},{3,11},{3,10},{3,9},{6,7},{6,6},{6,5},{4,4},{5,4},{3,4},{4,8},{5,8}};
    unsigned char letterT [13][2]= {{3,12},{4,12},{5,12},{6,12},{7,12},{5,11},{5,10},{5,9},{5,8},{5,7},{5,6},{5,5},{5,4}};
    unsigned char letterU [20][2]= {{3,12},{3,11},{3,10},{3,9},{3,8},{3,7},{3,6},{3,5},{3,4},{4,4},{5,4},{6,12},{6,11},{6,10},{6,9},{6,8},{6,7},{6,6},{6,5},{6,4}};
    unsigned char letterV [18][2]= {{2,12},{2,11},{2,10},{3,9},{3,8},{3,7},{3,6},{4,5},{4,4},{5,4},{5,5},{6,6},{6,7},{6,8},{6,9},{7,10},{7,11},{7,12}};
    unsigned char letterW [20][2]= {{2,11},{2,10},{2,9},{2,8},{2,7},{2,6},{2,5},{2,12},{6,11},{6,10},{6,9},{6,8},{6,7},{6,6},{6,12},{3,4},{5,4},{4,5},{4,6},{4,7}};;
    unsigned char letterX [17][2]= {{2,12},{2,11},{3,10},{3,9},{4,8},{5,9},{5,10},{6,11},{6,12},{3,7},{3,6},{2,5},{2,4},{5,7},{5,6},{6,5},{6,4}};
    unsigned char letterY [13][2]= {{2,12},{2,11},{3,10},{3,9},{4,8},{4,7},{4,6},{4,5},{4,4},{5,9},{5,10},{6,11},{6,12}};
    unsigned char letterZ [16][2]= {{3,12},{4,12},{5,12},{6,12},{3,4},{4,4},{5,4},{6,4},{6,11},{6,10},{5,10},{5,9},{4,8},{4,7},{3,6},{3,5}};
    unsigned char exclamation [7][2]= {{4,12},{4,11},{4,10},{4,9},{4,8},{4,7},{4,4}};


    for (i = 0; i<textLength; i++){
        // sets the screen frame
        for(x=0 ; x<LENGTH ; x++){
            for(y=0 ; y<HEIGHT ; y++){
                LedTable[x][y] = frameColor;
            }
        }

        switch (texte[i]){
        case 'A' :
            for (l = 0; l < 22; l++){
                LedTable[letterA[l][0]][letterA[l][1]] = textColor;
            }
            break;
        case 'B' :
            for (l = 0; l < 21; l++){
                LedTable[letterB[l][0]][letterB[l][1]] = textColor;
            }
            break;
        case 'C' :
            for (l = 0; l < 15; l++){
                LedTable[letterC[l][0]][letterC[l][1]] = textColor;
            }
            break;
        case 'D' :
            for (l = 0; l < 20; l++){
                LedTable[letterD[l][0]][letterD[l][1]] = textColor;
            }
            break;
        case 'E' :
            for (l = 0; l < 17; l++){
                LedTable[letterE[l][0]][letterE[l][1]] = textColor;
            }
            break;
        case 'F' :
            for (l = 0; l < 14; l++){
                LedTable[letterF[l][0]][letterF[l][1]] = textColor;
            }
            break;
        case 'G' :
            for (l = 0; l < 20; l++){
                LedTable[letterG[l][0]][letterG[l][1]] = textColor;
            }
            break;
        case 'H' :
            for (l = 0; l < 20; l++){
                LedTable[letterH[l][0]][letterH[l][1]] = textColor;
            }
            break;
        case 'I' :
            for (l = 0; l < 7; l++){
                LedTable[letterI[l][0]][letterI[l][1]] = textColor;
            }
            break;
        case 'J' :
            for (l = 0; l < 13; l++){
                LedTable[letterJ[l][0]][letterJ[l][1]] = textColor;
            }
            break;
        case 'K' :
            for (l = 0; l < 18; l++){
                LedTable[letterK[l][0]][letterK[l][1]] = textColor;
            }
            break;
        case 'L' :
            for (l = 0; l < 12; l++){
                LedTable[letterL[l][0]][letterL[l][1]] = textColor;
            }
            break;
        case 'M' :
            for (l = 0; l < 21; l++){
                LedTable[letterM[l][0]][letterM[l][1]] = textColor;
            }
            break;
        case 'N' :
            for (l = 0; l < 22; l++){
                LedTable[letterN[l][0]][letterN[l][1]] = textColor;
            }
            break;
        case 'O' :
            for (l = 0; l < 22; l++){
                LedTable[letterO[l][0]][letterO[l][1]] = textColor;
            }
            break;
        case 'P' :
            for (l = 0; l < 18; l++){
                LedTable[letterP[l][0]][letterP[l][1]] = textColor;
            }
            break;
        case 'Q' :
            for (l = 0; l < 24; l++){
                LedTable[letterQ[l][0]][letterQ[l][1]] = textColor;
            }
            break;
        case 'R' :
            for (l = 0; l < 22; l++){
                LedTable[letterR[l][0]][letterR[l][1]] = textColor;
            }
            break;
        case 'S' :
            for (l = 0; l < 14; l++){
                LedTable[letterS[l][0]][letterS[l][1]] = textColor;
            }
            break;
        case 'T' :
            for (l = 0; l < 13; l++){
                LedTable[letterT[l][0]][letterT[l][1]] = textColor;
            }
            break;
        case 'U' :
            for (l = 0; l < 20; l++){
                LedTable[letterU[l][0]][letterU[l][1]] = textColor;
            }
            break;
        case 'V' :
            for (l = 0; l < 18; l++){
                LedTable[letterV[l][0]][letterV[l][1]] = textColor;
            }
            break;
        case 'W' :
            for (l = 0; l < 20; l++){
                LedTable[letterW[l][0]][letterW[l][1]] = textColor;
            }
            break;
        case 'X' :
            for (l = 0; l < 17; l++){
                LedTable[letterX[l][0]][letterX[l][1]] = textColor;
            }
            break;
        case 'Y' :
            for (l = 0; l < 13; l++){
                LedTable[letterY[l][0]][letterY[l][1]] = textColor;
            }
            break;
        case 'Z' :
            for (l = 0; l < 16; l++){
                LedTable[letterZ[l][0]][letterZ[l][1]] = textColor;
            }
            break;
        case '!' :
            for (l = 0; l < 7; l++){
                LedTable[exclamation[l][0]][exclamation[l][1]] = textColor;
            }
            break;
        default :
            break;
        }

        array2Vector(LedTable,LEDS);
        sendFrame(LEDS);
        for(j=0 ; j<speed ; j++){
            __bis_SR_register(LPM0_bits + GIE);
            antiAliasGPIO(userOption, 1);
            if(userOption[0] != USER_NO_OPTION){
                return userOption[0];
            }
        }
    }
   return 0;
}

