/*
 * ADC.h
 *
 *  Created on: Jun 2, 2016
 *      Author: Vergil
 */

#ifndef ADC_H_
#define ADC_H_

#include "lpc17xx.h"
#include "lpc17xx_pinsel.h"

#ifdef __cplusplus
extern "C" {
#endif


#define ADC_CHANX_PORT 	PINSEL_PORT_0
#define ADC_CHANX_PIN	PINSEL_PIN_23

#define ADC_CHANY_PORT 	PINSEL_PORT_0
#define ADC_CHANY_PIN	PINSEL_PIN_24


void Init_ADC(void);

uint32_t ADC_ReadX(void);
uint32_t ADC_ReadY(void);

#ifdef __cplusplus
}
#endif

#endif /* ADC_H_ */
