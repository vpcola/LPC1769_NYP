/*
 * I2C.c
 *
 *  Created on: 26 May, 2016
 *      Author: tkcov
 */
#include "I2C.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_clkpwr.h"

I2C_M_SETUP_Type I2CConfigRxTx;

void Init_I2C2_Pins(void)
{
	// Configure I2C2 P0.10 & P0.11 to 10 - SDA2 & SCL2
	PINSEL_CFG_Type I2CPin;
	I2CPin.Portnum = I2C2_SDA_PORT;
	I2CPin.Pinnum = I2C2_SDA_PIN;	// SDA
	I2CPin.Pinmode = PINSEL_PINMODE_TRISTATE;
	I2CPin.Funcnum = PINSEL_FUNC_2;	// Alternate function 2 - I2C
	I2CPin.OpenDrain = PINSEL_PINMODE_OPENDRAIN;
	PINSEL_ConfigPin(&I2CPin);
	I2CPin.Portnum = I2C2_SCL_PORT;
	I2CPin.Pinnum = I2C2_SCL_PIN; 	// SCL
	PINSEL_ConfigPin(&I2CPin);
}

void Init_I2C2(void)
{
    // Turn on peripheral I2C2
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCI2C2, ENABLE);

    // Initialize the I2C Pins
	Init_I2C2_Pins();

	// Initialize I2C peripheral
	I2C_Init(LPC_I2C2, 400000); // 400KHz

	/* Enable I2C operation */
	I2C_Cmd(LPC_I2C2, I2C_MASTER_MODE, ENABLE);
}

unsigned int I2C2_SendBytes(int devaddr, char * data, int len)
{
	I2CConfigRxTx.sl_addr7bit = devaddr;
	I2CConfigRxTx.tx_data = (uint8_t *) data;
	I2CConfigRxTx.tx_length = len;
	// Leave RX options null
	I2CConfigRxTx.rx_data = NULL;
	I2CConfigRxTx.rx_length = 0;
	I2CConfigRxTx.retransmissions_max = 3;

	return (int) I2C_MasterTransferData(LPC_I2C2, &I2CConfigRxTx, I2C_TRANSFER_POLLING);

}

unsigned int I2C2_ReceiveBytes(int devaddr, int regaddr, char * data, int len)
{
	unsigned char registeraddress = (unsigned char) regaddr;

	// For receive, we need to send the device address first
	I2CConfigRxTx.sl_addr7bit = devaddr;
	I2CConfigRxTx.tx_data = (uint8_t *) &registeraddress;
	I2CConfigRxTx.tx_length = sizeof(registeraddress);
	// We set what we want to receive
	I2CConfigRxTx.tx_data = (uint8_t*) data;
	I2CConfigRxTx.tx_length = len;
	I2CConfigRxTx.retransmissions_max = 3;

	return (int) I2C_MasterTransferData(LPC_I2C2, &I2CConfigRxTx, I2C_TRANSFER_POLLING);
}

