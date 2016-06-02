/*
 * EXTI.h
 *
 *  Created on: May 29, 2016
 *      Author: vpcol
 */

#ifndef EXTI_H_
#define EXTI_H_


#include "lpc17xx.h"
#include "lpc17xx_pinsel.h"


#ifdef __cpluplus
extern "C" {
#endif


#define TOUCH_INT_PORT			PINSEL_PORT_2
#define TOUCH_INT_PIN			PINSEL_PIN_12
#define TOUCH_INT_INTERRUPT_PRIORITY	0x10

#define PUSHBUTTON_INT_PORT					PINSEL_PORT_2
#define PUSHBUTTON_INT_PIN					PINSEL_PIN_13
#define PUSHBUTTON_INT_INTERRUPT_PRIORITY	0x10

#define TOUCH_TIMEOUT 10000

void Init_EXTI();
int EXTI2_ReadIntPin();
int EXTI3_ReadIntPin();
void EXTI2_Enable();
void EXTI2_Disable();
void EXTI3_Enable();
void EXTI3_Disable();

#ifdef __cplusplus
}
#endif


#endif /* EXTI_H_ */
