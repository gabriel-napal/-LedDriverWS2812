# -LedDriverWS2812

This CCS project uses the MSP430FR5739 to drive a string of WS2812 RGB LEDs.

Microcontroller : MSP430FR5739

Current CPU clock : 1 MHz (could be boosted up to 24 MHz)

Communications:

The microcontroller uses a 3-wire SPI interface with the WS2812 LEDS. This LEDs are daisy chained to each other. 