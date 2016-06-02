/*
 * UART.c
 *
 *  Created on: 26 May, 2016
 *      Author: tkcov
 */
#include "UART.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_clkpwr.h"
#include <string.h>
#include <ctype.h>

//#define UART_USE_INTERRUPT 1

const char XBEE_BROADCAST_ADDR[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF };

#ifdef UART_USE_INTERRUPT
/* buffer size definition */
#define UART_RING_BUFSIZE 256

/* Buf mask */
#define __BUF_MASK (UART_RING_BUFSIZE-1)
/* Check buf is full or not */
#define __BUF_IS_FULL(head, tail) ((tail&__BUF_MASK)==((head+1)&__BUF_MASK))
/* Check buf will be full in next receiving or not */
#define __BUF_WILL_FULL(head, tail) ((tail&__BUF_MASK)==((head+2)&__BUF_MASK))
/* Check buf is empty */
#define __BUF_IS_EMPTY(head, tail) ((head&__BUF_MASK)==(tail&__BUF_MASK))
/* Reset buf */
#define __BUF_RESET(bufidx)	(bufidx=0)
#define __BUF_INCR(bufidx)	(bufidx=(bufidx+1)&__BUF_MASK)


/** UART Ring buffer structure */
typedef struct
{
    __IO uint32_t tx_head;                /* UART Tx ring buffer head index */
    __IO uint32_t tx_tail;                /* UART Tx ring buffer tail index */
    __IO uint32_t rx_head;                /* UART Rx ring buffer head index */
    __IO uint32_t rx_tail;                /* UART Rx ring buffer tail index */
    __IO uint8_t  tx[UART_RING_BUFSIZE];  /* UART Tx data ring buffer */
    __IO uint8_t  rx[UART_RING_BUFSIZE];  /* UART Rx data ring buffer */
} UART_RING_BUFFER_T;


// UART Ring buffer
UART_RING_BUFFER_T rb_3;
UART_RING_BUFFER_T rb_2;

// Current Tx Interrupt enable state
__IO FlagStatus TxIntStat;

// Forward declerations
void UART_IntErr(uint8_t bLSErrType);
void UART_IntTransmit(LPC_UART_TypeDef * UARTPort);
void UART_IntReceive(LPC_UART_TypeDef * UARTPort);

// Forward declarations
uint32_t UARTReceive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen);
uint32_t UARTSend(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen);

UART_RING_BUFFER_T * getUartBuffer(LPC_UART_TypeDef * UARTPort)
{
	if (UARTPort == LPC_UART3)
		return &rb_3;
	if (UARTPort == LPC_UART2)
		return &rb_2;

	return NULL;
}

void resetUartBuffer(LPC_UART_TypeDef * UARTPort)
{
	UART_RING_BUFFER_T * rb = getUartBuffer(UARTPort);

	// Reset ring buf head and tail idx
	// RX
	__BUF_RESET(rb->rx_head);
	__BUF_RESET(rb->rx_tail);
	// TX
	__BUF_RESET(rb->tx_head);
	__BUF_RESET(rb->tx_tail);

}

// UART2 Interrupt handler
void UART2_IRQHandler(void)
{
	uint32_t intsrc, tmp, tmp1;

	/* Determine the interrupt source */
	intsrc = UART_GetIntId(LPC_UART2);
	tmp = intsrc & UART_IIR_INTID_MASK;

	// Receive Line Status
	if (tmp == UART_IIR_INTID_RLS){
		// Check line status
		tmp1 = UART_GetLineStatus(LPC_UART2);
		// Mask out the Receive Ready and Transmit Holding empty status
		tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
				| UART_LSR_BI | UART_LSR_RXFE);
		// If any error exist
		if (tmp1) {
				UART_IntErr(tmp1);
		}
	}

	// Receive Data Available or Character time-out
	if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)){
			UART_IntReceive(LPC_UART2);
	}

	// Transmit Holding Empty
	if (tmp == UART_IIR_INTID_THRE){
			UART_IntTransmit(LPC_UART2);
	}
}

// UART3 Interrupt handler
void UART3_IRQHandler(void)
{
	uint32_t intsrc, tmp, tmp1;

	/* Determine the interrupt source */
	intsrc = UART_GetIntId(LPC_UART3);
	tmp = intsrc & UART_IIR_INTID_MASK;

	// Receive Line Status
	if (tmp == UART_IIR_INTID_RLS){
		// Check line status
		tmp1 = UART_GetLineStatus(LPC_UART3);
		// Mask out the Receive Ready and Transmit Holding empty status
		tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
				| UART_LSR_BI | UART_LSR_RXFE);
		// If any error exist
		if (tmp1) {
				UART_IntErr(tmp1);
		}
	}

	// Receive Data Available or Character time-out
	if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)){
			UART_IntReceive(LPC_UART3);
	}

	// Transmit Holding Empty
	if (tmp == UART_IIR_INTID_THRE){
			UART_IntTransmit(LPC_UART3);
	}
}

/********************************************************************//**
 * @brief 		UART receive function (ring buffer used)
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void UART_IntReceive(LPC_UART_TypeDef * UARTPort)
{
	uint8_t tmpc;
	uint32_t rLen;
    UART_RING_BUFFER_T * rb = getUartBuffer(UARTPort);

	while(1){
		// Call UART read function in UART driver
		rLen = UART_Receive((LPC_UART_TypeDef *)UARTPort, &tmpc, 1, NONE_BLOCKING);
		// If data received
		if (rLen){
			/* Check if buffer is more space
			 * If no more space, remaining character will be trimmed out
			 */
			if (!__BUF_IS_FULL(rb->rx_head,rb->rx_tail)){
				rb->rx[rb->rx_head] = tmpc;
				__BUF_INCR(rb->rx_head);
			}
		}
		// no more data
		else {
			break;
		}
	}
}

/********************************************************************//**
 * @brief 		UART transmit function (ring buffer used)
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void UART_IntTransmit(LPC_UART_TypeDef * UARTPort)
{
    UART_RING_BUFFER_T * rb = getUartBuffer(UARTPort);

    // Disable THRE interrupt
    UART_IntConfig((LPC_UART_TypeDef *)UARTPort, UART_INTCFG_THRE, DISABLE);

	/* Wait for FIFO buffer empty, transfer UART_TX_FIFO_SIZE bytes
	 * of data or break whenever ring buffers are empty */
	/* Wait until THR empty */
    while (UART_CheckBusy((LPC_UART_TypeDef *)UARTPort) == SET);

	while (!__BUF_IS_EMPTY(rb->tx_head,rb->tx_tail))
    {
        /* Move a piece of data into the transmit FIFO */
    	if (UART_Send((LPC_UART_TypeDef *)UARTPort, (uint8_t *)&rb->tx[rb->tx_tail], 1, NONE_BLOCKING)){
        /* Update transmit ring FIFO tail pointer */
        __BUF_INCR(rb->tx_tail);
    	} else {
    		break;
    	}
    }

    /* If there is no more data to send, disable the transmit
       interrupt - else enable it or keep it enabled */
	if (__BUF_IS_EMPTY(rb->tx_head, rb->tx_tail)) {
    	UART_IntConfig((LPC_UART_TypeDef *)UARTPort, UART_INTCFG_THRE, DISABLE);
    	// Reset Tx Interrupt state
    	TxIntStat = RESET;
    }
    else{
      	// Set Tx Interrupt state
		TxIntStat = SET;
    	UART_IntConfig((LPC_UART_TypeDef *)UARTPort, UART_INTCFG_THRE, ENABLE);
    }
}


/*********************************************************************//**
 * @brief		UART Line Status Error
 * @param[in]	bLSErrType	UART Line Status Error Type
 * @return		None
 **********************************************************************/
void UART_IntErr(uint8_t bLSErrType)
{
	while(1);
}


/*********************************************************************//**
 * @brief		UART transmit function for interrupt mode (using ring buffers)
 * @param[in]	UARTPort	Selected UART peripheral used to send data,
 * 				should be UART0
 * @param[out]	txbuf Pointer to Transmit buffer
 * @param[in]	buflen Length of Transmit buffer
 * @return 		Number of bytes actually sent to the ring buffer
 **********************************************************************/
uint32_t UARTSend(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen)
{
    uint8_t *data = (uint8_t *) txbuf;
    uint32_t bytes = 0;
    UART_RING_BUFFER_T * rb = getUartBuffer(UARTPort);

	/* Temporarily lock out UART transmit interrupts during this
	   read so the UART transmit interrupt won't cause problems
	   with the index values */
    UART_IntConfig(UARTPort, UART_INTCFG_THRE, DISABLE);

	/* Loop until transmit run buffer is full or until n_bytes
	   expires */
	while ((buflen > 0) && (!__BUF_IS_FULL(rb->tx_head, rb->tx_tail)))
	{
		/* Write data from buffer into ring buffer */
		rb->tx[rb->tx_head] = *data;
		data++;

		/* Increment head pointer */
		__BUF_INCR(rb->tx_head);

		/* Increment data count and decrement buffer size count */
		bytes++;
		buflen--;
	}

	/*
	 * Check if current Tx interrupt enable is reset,
	 * that means the Tx interrupt must be re-enabled
	 * due to call UART_IntTransmit() function to trigger
	 * this interrupt type
	 */
	if (TxIntStat == RESET) {
		UART_IntTransmit(UARTPort);
	}
	/*
	 * Otherwise, re-enables Tx Interrupt
	 */
	else {
		UART_IntConfig(UARTPort, UART_INTCFG_THRE, ENABLE);
	}

    return bytes;
}


/*********************************************************************//**
 * @brief		UART read function for interrupt mode (using ring buffers)
 * @param[in]	UARTPort	Selected UART peripheral used to send data,
 * 				should be UART0
 * @param[out]	rxbuf Pointer to Received buffer
 * @param[in]	buflen Length of Received buffer
 * @return 		Number of bytes actually read from the ring buffer
 **********************************************************************/
uint32_t UARTReceive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen)
{
    uint8_t *data = (uint8_t *) rxbuf;
    uint32_t bytes = 0;
    UART_RING_BUFFER_T * rb = getUartBuffer(UARTPort);

	/* Temporarily lock out UART receive interrupts during this
	   read so the UART receive interrupt won't cause problems
	   with the index values */
	UART_IntConfig(UARTPort, UART_INTCFG_RBR, DISABLE);

	/* Loop until receive buffer ring is empty or
		until max_bytes expires */
	while ((buflen > 0) && (!(__BUF_IS_EMPTY(rb->rx_head, rb->rx_tail))))
	{
		/* Read data from ring buffer into user buffer */
		*data = rb->rx[rb->rx_tail];
		data++;

		/* Update tail pointer */
		__BUF_INCR(rb->rx_tail);

		/* Increment data count and decrement buffer size count */
		bytes++;
		buflen--;
	}

	/* Re-enable UART interrupts */
	UART_IntConfig(UARTPort, UART_INTCFG_RBR, ENABLE);

    return bytes;
}

#endif // UART_USE_INTERRUPT


void Init_UART3_Pins(void)
{
	// Configure UART Pins TXD3 and RXD3
	// Configure P0.0 (TXD3)
	PINSEL_CFG_Type UartPin;
	UartPin.Portnum = UART3_TX_PORT;
	UartPin.Pinnum = UART3_TX_PIN; 		// TX
	UartPin.Funcnum = PINSEL_FUNC_2; 	// TXD3
	UartPin.Pinmode = PINSEL_PINMODE_TRISTATE; // Neither pull-up or pull-down
	UartPin.OpenDrain = PINSEL_PINMODE_NORMAL;
	PINSEL_ConfigPin(&UartPin);

	// Configure P0.1 (RXD3)
	UartPin.Portnum = UART3_RX_PORT;
	UartPin.Pinnum = UART3_RX_PIN;		// RX
	UartPin.Funcnum = PINSEL_FUNC_2; 	// RXD3
	PINSEL_ConfigPin(&UartPin);
}

void Init_UART3(int baudrate)
{
	UART_CFG_Type UartCfg;
	UART_FIFO_CFG_Type FifoConfig;

    // Turn on peripheral UART3
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCUART3, ENABLE);

    // Initialize UART3 Pins
	Init_UART3_Pins();

	// Initialize Baudrate
	UART_ConfigStructInit(&UartCfg);
	// Only set the baudrate for now,
	// leave others untouched (8 bits, parity none, 1 stop bit)
	UartCfg.Baud_rate = baudrate;
	UART_Init(LPC_UART3, &UartCfg);

	// Setup UART FIFO, Enable Rx and Tx Fifo
	/* Initialize FIFOConfigStruct to default state:
	 * 				- FIFO_DMAMode = DISABLE
	 * 				- FIFO_Level = UART_FIFO_TRGLEV0
	 * 				- FIFO_ResetRxBuf = ENABLE
	 * 				- FIFO_ResetTxBuf = ENABLE
	 * 				- FIFO_State = ENABLE
	 */
	UART_FIFOConfigStructInit(&FifoConfig);
	UART_FIFOConfig(LPC_UART3, &FifoConfig);
	// Enable UART Transmit
	UART_TxCmd(LPC_UART3, ENABLE);

#ifdef UART_USE_INTERRUPT
    /* Enable UART Rx interrupt */
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART3, UART_INTCFG_RBR, ENABLE);
	/* Enable UART line status interrupt */
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART3, UART_INTCFG_RLS, ENABLE);
	/*
	 * Do not enable transmit interrupt here, since it is handled by
	 * UART_Send() function, just to reset Tx Interrupt state for the
	 * first time
	 */
	TxIntStat = RESET;

	// Reset ring buf head and tail idx
	resetUartBuffer(LPC_UART3);

	/* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(UART3_IRQn, ((0x01<<3)|0x01));
	/* Enable Interrupt for UART channel */
    NVIC_EnableIRQ(UART3_IRQn);
#endif // UART_USE_INTERRUPT
}

unsigned int UART3_SendBytes(char *data, int len)
{
#ifdef UART_USE_INTERRUPT
	return UARTSend(LPC_UART3, (uint8_t *)data, len);
#else
	return UART_Send(LPC_UART3, (uint8_t *)data, len, BLOCKING);
#endif
}

unsigned int UART3_ReceiveBytes(char * data, int len)
{
	if (data == NULL) return 0;

#ifdef UART_USE_INTERRUPT
	return UARTReceive(LPC_UART3, (uint8_t *) data, len);
#else
	return UART_Receive(LPC_UART3, (uint8_t *) data, len, BLOCKING);
#endif
}

unsigned int UART3_XBEESend(char * dest, char * io, int onoff)
{
	int sum, i;
	char * command, chk;
	char CommandON[20] = {0x7E, 0x00, 0x10, 0x17, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x40, 0x3B, 0xB3, 0x3A, 0xFF, 0xFE, 0x02, 0x44, 0x33, 0x04, 0xC2};
	char CommandOFF[20] = {0x7E, 0x00, 0x10, 0x17, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x40, 0x3B, 0xB3, 0x3A, 0xFF, 0xFE, 0x02, 0x44, 0x33, 0x05, 0xC3};

	if ((dest == NULL) || (io == NULL))
		return -1;
	if ((strlen(io) != 2) || (io[0] != 'D') || !isdigit(io[1]))
		return -1;

	if (onoff) // Turn on
	{
		memcpy(&CommandON[6], dest, 8); // Copy the 8 bytes dest addr
		memcpy(&CommandON[16],io, 2); //
		CommandON[18] = 0x04;
		command = &CommandON[0];
	}
	else
	{
		memcpy(&CommandOFF[6], dest, 8);
		memcpy(&CommandOFF[16],io, 2); //
		CommandOFF[18] = 0x05;
		command = &CommandOFF[0];
	}

	sum = 0;
	// Compute the checksum for all the bytes after the length byte (offset 2)
	for (i = 3; i < 18; i++ )
		sum += command[i];
	chk = 0xFF - ( sum & 0xFF );
	command[19] = chk;

	return UART3_SendBytes(command, 20);

}
