/*
 * COLORS.h
 *
 *  Created on: 17 avr. 2020
 *      Author: Laety
 */

#ifndef COLORS_H_
#define COLORS_H_

typedef struct color_s
{
unsigned char R ;
unsigned char G ;
unsigned char B ;
} color_t;

static const color_t color_off = {0x00,0x00,0x00};

static const color_t red_bright_1 = {0xFF,0x3b,0x3b};
static const color_t red_bright_2 = {0xc8,0x1d,0x1d};
static const color_t red_bright_3 = {0xe4,0x1a,0x1a};
static const color_t red_medium_1 = {0xb2,0x11,0x11};
static const color_t red_medium_2 = {0xa4,0x07,0x07};
static const color_t red_medium_3 = {0xa4,0x00,0x00};
static const color_t red_dark_1 = {0xFF,0x07,0x07};
static const color_t red_dark_2 = {0xFF,0x05,0x05};
static const color_t red_dark_3 = {0xFF,0x00,0x00};

static const color_t blue_bright_1 = {0x00,0xA6,0xA9};
static const color_t blue_bright_2 = {0x2c, 0x6f, 0x98};
//static const color_t blue_bright_3 = {};
static const color_t blue_medium_1 = {0x41,0x69,0xe1};
static const color_t blue_medium_2 = {0x00,0x00,0x70};
//static const color_t blue_medium_3 = {0};
static const color_t blue_dark_1 = {0x00,0x00,0x9B};
//static const color_t blue_dark_2 = {};
static const color_t blue_dark_3 = {0x00,0x00,0xFF};

static const color_t green_bright_1 = {0x7f, 0xfc, 0x00};
static const color_t green_medium_1 = {0x00, 0xB0, 0x00};
static const color_t green_dark_1 = {0x00, 0xFF, 0x00};

static const color_t purple_medium_1 = {0x2F,0x29,0xFF};

static const color_t orange_dark_1 = {0xff,0x8c,0x00};

static const color_t grey_medium_1 = {0x69,0x69,0x69};

static const color_t brow_dark_1 = {0xA0,0x52,0x2D};


#endif /* COLORS_H_ */
