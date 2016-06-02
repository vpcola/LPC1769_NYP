/*
 * SSP0.c
 *
 *  Created on: May 29, 2016
 *      Author: vpcol
 */

#include "SSP0.h"
#include "lpc17xx_ssp.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_exti.h"

void Init_SSP0_Pins(void)
{
    // Configure P0.15 SCK0, P0.17 MISO0, P0.18 MOSI0 to Alternate Func 2
    PINSEL_CFG_Type SPIPin;
    SPIPin.Portnum = SSP0_CLK_PORT;
    SPIPin.Pinnum = SSP0_CLK_PIN;	// CLK
    SPIPin.Funcnum = PINSEL_FUNC_2; // Aternate Function 2 - SPI0_CLK
    SPIPin.OpenDrain = PINSEL_PINMODE_NORMAL;
    SPIPin.Pinmode = PINSEL_PINMODE_PULLUP; // Enable pull-up resistors
    PINSEL_ConfigPin(&SPIPin);
    SPIPin.Pinnum = SSP0_MISO_PIN;	// MISO
    PINSEL_ConfigPin(&SPIPin);
    SPIPin.Pinnum = SSP0_MOSI_PIN;	// MOSI
    PINSEL_ConfigPin(&SPIPin);

}

void SSP0_SetConfig(uint32_t speed, uint32_t polarity, uint32_t phase, uint32_t databit)
{
	SSP_CFG_Type SSP0Cfg;
	SSP_ConfigStructInit(&SSP0Cfg);
	SSP0Cfg.CPHA = phase;
	SSP0Cfg.Databit = databit;
	SSP0Cfg.ClockRate = speed; // 10Mhz
	SSP0Cfg.CPOL = polarity;
	SSP0Cfg.FrameFormat = SSP_FRAME_SPI; // MSB first
	SSP_Init(LPC_SSP0, &SSP0Cfg);
}

void Init_SSP0(void)
{
    // Turn on peripheral SPI0
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCSSP0, ENABLE);

	// Initialize SSP0 pins
	Init_SSP0_Pins();

	// Initialize SPI0 Clocks, Polarity, Phase, etc
	// CPOL = 0, CPHA = 1, Freq = 10MHz
	// Databit = 8,
	SSP0_SetConfig(SSP0_CLOCK_RATE, SSP0_POLARITY, SSP0_PHASE, SSP_DATABIT_8);

	// Enable SSP peripheral
	SSP_Cmd(LPC_SSP0, ENABLE);

    // setup DMA channel 0
    //LPC_SC->PCONP |= (1UL << 29);       // Power up the GPDMA.
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPDMA, ENABLE);
    LPC_GPDMA->DMACConfig = 1;          // enable DMA controller
    LPC_GPDMA->DMACIntTCClear = 0x1;    // Reset the Interrupt status
    LPC_GPDMA->DMACIntErrClr = 0x1;
    LPC_GPDMACH0->DMACCLLI   = 0;

}

// A quick way to switch between 8-bit and 16 bit mode
void SSP0_16(int s)
{
    if(s)
        LPC_SSP0->CR0 |= 0x08;  // switch to 16 bit Mode
    else
        LPC_SSP0->CR0 &= ~(0x08);  // switch to 8  bit Mode
}


void SSP0_Write(unsigned short data)
{
    // wait untill fifo is empty
    while(((LPC_SSP0->SR) & 0x02) == 0);
    LPC_SSP0->DR = data;
    // while((LPC_SSP0->SR & (1<<2)) == 0)
    // LPC_SSP0->DR;
}

int SSP0_Read(void)
{
	while((LPC_SSP0->SR & (1 << 1)) == 0);
	// Send Data (0) to flush
	LPC_SSP0->DR = 0x00;
	while((LPC_SSP0->SR & (1 << 2)) == 0);
	return (LPC_SSP0->DR);
}
int SSP0_Transfer(unsigned short data)
{
	while((LPC_SSP0->SR & (1 << 1)) == 0);
	// Send Data (0) to flush
	LPC_SSP0->DR = data;
	while((LPC_SSP0->SR & (1 << 2)) == 0);
	return (LPC_SSP0->DR);
}
void SSP0_WaitBusy()
{
    while ((LPC_SSP0->SR & 0x10) == 0x10); // Wait while busy
}

void SSP0_WriteLcdDMA(int color, int numbytes)
{
    unsigned int dma_count;

    LPC_GPDMACH0->DMACCSrcAddr = (uint32_t)&color;
    LPC_GPDMACH0->DMACCDestAddr = (uint32_t)&LPC_SSP0->DR; // we send to SSP0
    LPC_SSP0->DMACR = 0x2;

    // start DMA
    // Based on specs, DMA can transfer arnd 4k (4095) chunks
    // at a time, so we do it in batches
    do {
        if (numbytes > 4095) {
            dma_count = 4095;
            numbytes = numbytes - 4095;
        } else {
            dma_count = numbytes;
            numbytes = 0;
        }
        LPC_GPDMA->DMACIntTCClear = 0x1;
        LPC_GPDMA->DMACIntErrClr = 0x1;
        // Configure DMA to use 16bit transfers
        // no address increment, interrupt
        LPC_GPDMACH0->DMACCControl = dma_count | (1UL << 18) | (1UL << 21) | (1UL << 31) ;
        LPC_GPDMACH0->DMACCConfig  = DMA_CHANNEL_ENABLE | DMA_TRANSFER_TYPE_M2P | DMA_DEST_SSP0_TX;
        LPC_GPDMA->DMACSoftSReq = 0x1;   // DMA request
        // Wait until DMA is finshed
        do {
        } while ((LPC_GPDMA->DMACRawIntTCStat & 0x01) == 0); // DMA is running
    } while (numbytes > 0);

}
