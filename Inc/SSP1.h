/*
 * SSP1.h
 *
 *  Created on: May 29, 2016
 *      Author: vpcol
 */

#ifndef SSP1_H_
#define SSP1_H_

#include "lpc17xx.h"
#include "lpc17xx_pinsel.h"

#ifdef __cplusplus
extern "C" {
#endif


// Define for the SSP1 (SPI) Pins used
#define SSP1_CLK_PORT			PINSEL_PORT_0
#define SSP1_CLK_PIN			PINSEL_PIN_7
#define SSP1_MISO_PORT			PINSEL_PORT_0
#define SSP1_MISO_PIN			PINSEL_PIN_8
#define SSP1_MOSI_PORT			PINSEL_PORT_0
#define SSP1_MOSI_PIN			PINSEL_PIN_9

// SSP0 Configuration
#define SSP1_CLOCK_RATE			2000000 		// 10Mhz, can be faster
#define SSP1_POLARITY			SSP_CPOL_LO		// Clock Polarit Low
#define SSP1_PHASE				SSP_CPHA_FIRST	// Sample on first rising/low edge


void Init_SSP1(void);
void SSP1_SetConfig(uint32_t speed, uint32_t polarity, uint32_t phase, uint32_t databit);
void SSP1_16(int s);
void SSP1_Write(unsigned short data);
char SSP1_Read(void);
int SSP1_Transfer(unsigned short data);
void SSP1_WaitBusy();
void SSP1_CS(int);

#ifdef __cplusplus
}
#endif

#endif /* SSP1_H_ */
