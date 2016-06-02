/*
 * I2C.h
 *
 *  Created on: 26 May, 2016
 *      Author: tkcov
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "lpc17xx_pinsel.h"

#define I2C2_SDA_PORT	PINSEL_PORT_0
#define I2C2_SDA_PIN	PINSEL_PIN_10
#define I2C2_SCL_PORT 	PINSEL_PORT_0
#define I2C2_SCL_PIN	PINSEL_PIN_11

void Init_I2C2(void);
// Wrapper functions to call I2C driver
unsigned int I2C2_SendBytes(int devaddr, char * data, int len);
unsigned int I2C2_ReceiveBytes(int devaddr, int regaddr, char * data, int len);

#ifdef __cplusplus
}
#endif

#endif /* INC_I2C_H_ */
