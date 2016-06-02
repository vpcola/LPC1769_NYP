/*
 * SSP1.c
 *
 *  Created on: May 29, 2016
 *      Author: vpcol
 */

#include "SSP1.h"
#include "lpc17xx_ssp.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_exti.h"

void Init_SSP1_Pins(void)
{
	 // Configure P0.7 SCK1, P0.8 MISO1, P0.9 MOSI1 to Alternate Func 2
    LPC_PINCON->PINSEL0 &= (~(1 << 14));
    LPC_PINCON->PINSEL0 |= (1 << 15);

    LPC_PINCON->PINSEL0 &= (~(1 << 16));
    LPC_PINCON->PINSEL0 |= (1 << 17);

    LPC_PINCON->PINSEL0 &= (~(1 << 18));
    LPC_PINCON->PINSEL0 |= (1 << 19);

    // Set Touch CS P0.6 to GPIO (Alternate func 00)
    LPC_PINCON->PINSEL0 &= (~(3 << 12));

    // Enable Pull Up resistor - mode 00
    LPC_PINCON->PINMODE0 &= (~(3 << 14));
    LPC_PINCON->PINMODE0 &= (~(3 << 16));
    LPC_PINCON->PINMODE0 &= (~(3 << 18));
    LPC_PINCON->PINMODE0 &= (~(3 << 12));

    // Configure P0.6 to output
    LPC_GPIO0->FIODIR |= (1 << 6);
}

void Init_SSP1(void)
{
    // Turn on peripheral SPI1
    LPC_SC->PCONP |= (1<<10);

	// Initialize SSP0 pins
	Init_SSP1_Pins();

	// Configure PCLK SSP1 to 10
	LPC_SC->PCLKSEL0 &= (~(1 << 20));
	LPC_SC->PCLKSEL0 |= ( 1 << 21);

	// Configure SCK to 2.5MHz
	LPC_SSP1->CPSR = 2;
	LPC_SSP1->CR0 |= (9 << 8);

	// Configure SSP1 : Data Size = 8, Frame Format = SPI, CPOL = 0, CPHA = 0
	LPC_SSP1->CR0 |= (7 << 0); // 8 bit
	LPC_SSP1->CR0 &= (~(3 << 4)); // SPI
	LPC_SSP1->CR0 &= (~(1 << 6)); // CPOL
	LPC_SSP1->CR0 &= (~(1 << 7)); // CPHA
	//LPC_SSP1->CR0 |= (1 << 7);
	// Configure as Master Mode and enable SSP1
	LPC_SSP1->CR1 &= (~(1 << 2));
	LPC_SSP1->CR1 |= (1 << 1);

}

// A quick way to switch between 8-bit and 16 bit mode
void SSP1_16(int s)
{
    if(s)
        LPC_SSP1->CR0 |= 0x08;  // switch to 16 bit Mode
    else
        LPC_SSP1->CR0 &= ~(0x08);  // switch to 8  bit Mode
}


void SSP1_Write(unsigned short data)
{
    // wait untill fifo is empty
    while(((LPC_SSP1->SR) & 0x02) == 0);
    LPC_SSP1->DR = data;
    while((LPC_SSP1->SR & (1<<2)) == 0)
    LPC_SSP1->DR;
}

char SSP1_Read(void)
{
	char byteread;

	while((LPC_SSP1->SR & (1 << 1)) == 0);
	// Send Data (0) to flush
	LPC_SSP1->DR = 0x00;
	while((LPC_SSP1->SR & (1 << 2)) == 0); // Wait until RFF is full
	//SSP1_WaitBusy();
	byteread = (LPC_SSP1->DR) & 0xFF ;

	return byteread;
}
int SSP1_Transfer(unsigned short data)
{
	while((LPC_SSP1->SR & (1 << 1)) == 0);
	// Send Data (0) to flush
	LPC_SSP1->DR = data;
	//while((LPC_SSP1->SR & (1 << 3)) == 0);
	SSP1_WaitBusy();
	return (LPC_SSP1->DR);
}
void SSP1_WaitBusy()
{
    while ((LPC_SSP1->SR & 0x10) == 0x10); // Wait while busy
}

void SSP1_CS(int enable)
{
    if (enable)
        LPC_GPIO0->FIOSET |= (1 << 6);
    else
        LPC_GPIO0->FIOCLR |= (1 << 6);
}
