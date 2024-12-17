#include "regmap.h"
#include "hw_gpio.h"
#include "hw_uart.h"

#include "string.h"

#include "fifo1.h"


int hw_uart_write(UART_REGISTERS *huart, const char *data, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		while (huart->status.txBufFull);
		huart->txBuf = data[i];
	
	}
	return TRUE;
}



int hw_uart_read(UART_REGISTERS *huart, char *data, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		while (huart->status.rxBufEmpty);
		data[i] = huart->rxBuf;
	}
	return TRUE;
}

int hw_uart_init(UART_REGISTERS* huart, int tx_ena, int rx_ena)
{
	if (huart == UART0)
	{
		if (tx_ena)
		{
			REG_GPIO0_FUNC |= UART0_TX;
		}

		if (rx_ena)
		{
			REG_GPIO0_FUNC |= UART0_RX;
		}
	}
	else if (huart == UART1)
	{
		if (tx_ena)
		{
			REG_GPIO0_FUNC |= UART1_TX;
		}

		if (rx_ena)
		{
			REG_GPIO0_FUNC |= UART1_RX;
		}
	}
	else if (huart == UART2)
	{
		if (tx_ena)
		{
			REG_GPIO0_FUNC |= UART2_TX;
		}

		if (rx_ena)
		{
			REG_GPIO0_FUNC |= UART2_RX;
		}
	}
	else
	{
		DBG_Assert(FALSE);
	}

	return TRUE;
}

int hw_uart_open(UART_REGISTERS *huart, int tx_ena, int rx_ena)
{
	UART_CONTROLs val;
	// UART_CONTROLs *pUartCntl = &val;
	// DBG_assert ( num <)

	val = (huart->control);

	val.tx_ena = tx_ena;
	val.tx_fifo_ena = tx_ena;
	val.rx_ena = rx_ena;
	val.rx_fifo_ena = rx_ena;

	huart->control = val;

	return TRUE;

}

int hw_uart_setBaudRate(UART_REGISTERS *huart, U32 baudRate, U32 sysclk)
{
	u8 baudrate_div;

	baudrate_div = sysclk/baudRate;

	huart->baudrate_divisor = baudrate_div;

	return TRUE;
}

int hw_uart_setParity(UART_REGISTERS *huart, U32 parity)
{
	huart->control.parity = parity;

	return TRUE;
}

void hw_uart_setup(UART_REGISTERS *huart, uint32_t baudrate, uint32_t priority, int tx_ena, int rx_ena)
{
	volatile U32 val;
	MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *) &val;

	val = REG_MISC_CLKGATE;

	if (huart == UART0)
	{
		pGate->u_uart0_gate = 1;						//open clk gate
	}
	else if (huart == UART1)
	{
		pGate->u_uart1_gate = 1;						//open clk gate
	}
	else if (huart == UART2)
	{
		pGate->u_uart2_gate = 1;						//open clk gate
	}
	else
	{
		DBG_Assert(FALSE);
	}


	REG_MISC_CLKGATE = val;

	REG_MISC_STATUS = 0x00000000;

	hw_uart_init( huart, tx_ena,rx_ena );
	hw_uart_setParity(huart, priority);
	extern uint32_t SystemClock;
	hw_uart_setBaudRate(huart, baudrate, SystemClock/4);
	hw_uart_open( huart, tx_ena,rx_ena );
	// printf("%d %d", SystemClock, huart->baudrate_divisor);

}