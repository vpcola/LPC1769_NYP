/*
 * GPIO.c
 *
 *  Created on: 26 May, 2016
 *      Author: tkcov
 */

#include "GPIO.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_clkpwr.h"

void Init_GPIO_Pins(void)
{
	// Configure GPIO Pins for LED control (RED Led and Blue Led)
	PINSEL_CFG_Type LedPin;
	LedPin.Portnum = RED_LED_PORT;
	LedPin.Pinnum = RED_LED_PIN;
	LedPin.Funcnum = PINSEL_FUNC_0; // Default GPIO
	LedPin.Pinmode = PINSEL_PINMODE_PULLUP;
	LedPin.OpenDrain = PINSEL_PINMODE_NORMAL;
	// Configure P0.22 (RED Led) for output
	PINSEL_ConfigPin(&LedPin);
	// Configure P3.26 (BLUE Led) for output
	LedPin.Portnum = BLUE_LED_PORT;
	LedPin.Pinnum = BLUE_LED_PIN;
	PINSEL_ConfigPin(&LedPin);
	LedPin.Portnum = GREEN_LED_PORT;
	LedPin.Pinnum = GREEN_LED_PIN;
	PINSEL_ConfigPin(&LedPin);

	// Set IO Direction
	GPIO_SetDir(RED_LED_PORT, (1 << RED_LED_PIN), GPIO_DIR_OUTPUT); // Red LED
	GPIO_SetDir(BLUE_LED_PORT, (1 << BLUE_LED_PIN), GPIO_DIR_OUTPUT); // Blue LED
	GPIO_SetDir(GREEN_LED_PORT, (1 << GREEN_LED_PIN), GPIO_DIR_OUTPUT); // Green LED
}

void Init_GPIO(void)
{
    // Turn on peripheral GPIO0
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPIO, ENABLE);

    // Initialize the GPIO Pins
    Init_GPIO_Pins();

    // Set default value for the LEDs
    GPIO_RedLed(1);
    GPIO_GreenLed(1);
    GPIO_BlueLed(1);
}

// The leds actually have reverse logic, you need to send
// zero (0) to turn them on, and 1 to turn them off.
void GPIO_RedLed(int value)
{
	if (value)
		GPIO_ClearValue(RED_LED_PORT, (1 << RED_LED_PIN));
	else
		GPIO_SetValue(RED_LED_PORT, (1 << RED_LED_PIN));

}

void GPIO_BlueLed(int value)
{
	if (value)
		GPIO_ClearValue(BLUE_LED_PORT, (1 << BLUE_LED_PIN));
	else
		GPIO_SetValue(BLUE_LED_PORT, (1 << BLUE_LED_PIN));

}

void GPIO_GreenLed(int value)
{
	if (value)
		GPIO_ClearValue(GREEN_LED_PORT, (1 << GREEN_LED_PIN));
	else
		GPIO_SetValue(GREEN_LED_PORT, (1 << GREEN_LED_PIN));

}
