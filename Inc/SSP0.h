/*
 * SSP0.h
 *
 *  Created on: May 29, 2016
 *      Author: vpcol
 */

#ifndef SSP0_H_
#define SSP0_H_

#include "lpc17xx.h"
#include "lpc17xx_pinsel.h"

#ifdef __cplusplus
extern "C" {
#endif


// Define for the SSP0 (SPI) Pins used
#define SSP0_CLK_PORT			PINSEL_PORT_0
#define SSP0_CLK_PIN			PINSEL_PIN_15
#define SSP0_MISO_PORT			PINSEL_PORT_0
#define SSP0_MISO_PIN			PINSEL_PIN_17
#define SSP0_MOSI_PORT			PINSEL_PORT_0
#define SSP0_MOSI_PIN			PINSEL_PIN_18

// SSP0 Configuration
#define SSP0_CLOCK_RATE			20000000		// 20Mhz, can be faster
#define SSP0_POLARITY			SSP_CPOL_LO		// Clock Polarit Low
#define SSP0_PHASE				SSP_CPHA_FIRST	// Sample on first rising/low edge

// some defines for the SSP0 DMA use
#define DMA_CHANNEL_ENABLE      1
#define DMA_TRANSFER_TYPE_M2P   (1UL << 11)
#define DMA_CHANNEL_TCIE        (1UL << 31)
#define DMA_CHANNEL_SRC_INC     (1UL << 26)
#define DMA_MASK_IE             (1UL << 14)
#define DMA_MASK_ITC            (1UL << 15)
#define DMA_SSP1_TX             (1UL << 2)
#define DMA_SSP0_TX             (0)
#define DMA_DEST_SSP1_TX        (2UL << 6)
#define DMA_DEST_SSP0_TX        (0UL << 6)

void Init_SSP0(void);
void SSP0_SetConfig(uint32_t speed, uint32_t polarity, uint32_t phase, uint32_t databit);
void SSP0_16(int s);
void SSP0_Write(unsigned short data);
int SSP0_Read(void);
int SSP0_Transfer(unsigned short data);
void SSP0_WriteLcdDMA(int color, int numbytes);
void SSP0_WaitBusy();

#ifdef __cplusplus
}
#endif

#endif /* SSP0_H_ */
