/*
 * UART.h
 *
 *  Created on: 26 May, 2016
 *      Author: tkcov
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#ifdef __cpluplus
extern "C" {
#endif


#include "lpc17xx_pinsel.h"

extern const char XBEE_BROADCAST_ADDR[];

// Define the ports and pins used
#define UART3_TX_PORT	PINSEL_PORT_0
#define UART3_TX_PIN	PINSEL_PIN_0
#define UART3_RX_PORT	PINSEL_PORT_0
#define UART3_RX_PIN	PINSEL_PIN_1

// Determine if we are going to use
// non-blocking calls/circular buffer
//#define UART_USE_INTERRUPT

// Function prototypes
void Init_UART3(int baudrate);
// Functions to send/receive data from UART
unsigned int UART3_SendBytes(char *data, int len);
unsigned int UART3_ReceiveBytes(char * data, int len);
unsigned int UART3_XBEESend(char * dest, char * io, int onoff);

#ifdef __cplusplus
}
#endif

#endif /* INC_UART_H_ */
