/*
 * XPT2046.c
 *
 *  Created on: 30 May, 2016
 *      Author: tkcov
 */

#include <stdio.h>

#include "XPT2046.h"
#include "SSP1.h"
#include "EXTI.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_ssp.h"


// Flag to indicate touch panel hit
// Interrupt sets this flag, user must reset
// after acknowledgement
__IO uint32_t HasTouched;

__IO uint16_t xraw, yraw, zraw;


uint16_t XPT2046_Read()
{
	unsigned int Xresult = 0;
	unsigned int Yresult = 0;
	char rdata[3];

	// Write control byte to touch
	// controller if TX Fifo is not
	// full and not busy
	SSP1_CS(0);
	SSP1_Write(0x80 | 0x50 );
	//Xresult = SSP1_Read() << 8;
	//Xresult |= SSP1_Read();
	//Xresult >>= 3;
	rdata[0] = SSP1_Read();
	rdata[1] = SSP1_Read();
	Xresult =  (( rdata[0] << 8 ) | rdata[1] ) >> 3;
	SSP1_WaitBusy();
	SSP1_CS(1);

	SSP1_CS(0);
	rdata[0] = SSP1_Transfer( 0x80 | 0x10 );
	rdata[1] = SSP1_Read();
	rdata[2] = SSP1_Read();
	Yresult = (( rdata[1] << 8) | rdata[2] ) >> 3;
	SSP1_WaitBusy();
	SSP1_CS(1);

	xraw = Xresult;
	yraw = Yresult;
	printf("X = %d, Y = %d\n", Xresult, Yresult);
	return 0;
}

void Init_XPT2046(void)
{
	// Initialize the value of the flag
	HasTouched = 0;

	// Get sample reading, leaving PENIRQ up
	SSP1_CS(0);
    while(((LPC_SSP1->SR) & 0x02) == 0);
    LPC_SSP1->DR = 0x80;
    SSP1_CS(1);

}

void XPT2046_Enable(void)
{
	// Enable interrupts to start getting
	// touch updates
	EXTI2_Enable();
}
void XPT2046_Disable(void)
{
	// Disable interrupts to stop receiving
	// updates
	EXTI2_Disable();
}

void XPT2046_Update(void)
{
	// Indicate that we have data to read
	HasTouched = 1;
}

// This function waits until the HasTouched flag
// is set and updates the touch coordinates.
int  XPT2046_Wait(int * x, int * y, int timeout)
{
	int temp = timeout;
	while((HasTouched == 0) && (temp > 0)) temp--;

	if (HasTouched)
	{
		XPT2046_Read(); // get coordinates
		*x = xraw;
		*y = yraw;
		// Reset the flag
		HasTouched = 0;
		return 1;
	}
	// Timeout
	return 0;
}
