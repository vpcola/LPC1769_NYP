/*
 * GPIO.h
 *
 *  Created on: 26 May, 2016
 *      Author: tkcov
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"

#ifdef __cplusplus
extern "C" {
#endif

// GPIO Pins

// RED Led
#define RED_LED_PORT 	PINSEL_PORT_0
#define RED_LED_PIN		PINSEL_PIN_22

// BLUE Led
#define BLUE_LED_PORT 	PINSEL_PORT_3
#define BLUE_LED_PIN	PINSEL_PIN_26

// GREEN Led
#define GREEN_LED_PORT	PINSEL_PORT_3
#define GREEN_LED_PIN	PINSEL_PIN_25


void Init_GPIO(void);
void GPIO_RedLed(int value);
void GPIO_BlueLed(int value);
void GPIO_GreenLed(int value);

#ifdef __cpluplus
}
#endif

#endif /* INC_GPIO_H_ */
