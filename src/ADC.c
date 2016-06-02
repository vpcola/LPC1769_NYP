/*
 * ADC.c
 *
 *  Created on: Jun 2, 2016
 *      Author: Vergil
 */

#include "lpc17xx.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_clkpwr.h"
#include "ADC.h"


void Init_ADC_Pins(void)
{
	PINSEL_CFG_Type ADCPin;
	// Configure X Channel
	ADCPin.Portnum = ADC_CHANX_PORT;
	ADCPin.Pinnum = ADC_CHANX_PIN;
	ADCPin.Funcnum = PINSEL_FUNC_1; // ADC
	ADCPin.Pinmode = PINSEL_PINMODE_PULLUP;
	ADCPin.OpenDrain = PINSEL_PINMODE_NORMAL;
	PINSEL_ConfigPin(&ADCPin);
	// Configure Y Channel
	ADCPin.Funcnum = PINSEL_FUNC_1;
	ADCPin.Pinnum = ADC_CHANY_PIN;
	ADCPin.Portnum = ADC_CHANY_PORT;
	PINSEL_ConfigPin(&ADCPin);

}

void Init_ADC(void)
{
	// Power on ADC peripheral
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCAD, ENABLE);

    // Initialize ADC pins
    Init_ADC_Pins();

	// Configuration for ADC :
	// 	Frequency at 1Mhz
	//  ADC channel 0 (X), no Interrupt
	//
	ADC_Init(LPC_ADC, 100000); // 100KHz
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN0,DISABLE); // Disable interrupts on Channel 0 (X)
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN1,DISABLE); // Disable interrupts on Channel 2 (Y)

	// Select Channel
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,ENABLE); // Enable Channel 0 (X)
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_1,ENABLE); // Enable Channel 2 (Y)

	ADC_StartCmd(LPC_ADC, ADC_START_CONTINUOUS);
	ADC_BurstCmd(LPC_ADC, ENABLE);
}

uint32_t ADC_ReadX(void)
{
	//Wait conversion complete
	//while (!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_0,ADC_DATA_DONE)));
	return ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0);
}

uint32_t ADC_ReadY(void)
{
	//Wait conversion complete
	//while (!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_2,ADC_DATA_DONE)));
	return ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_1);
}



