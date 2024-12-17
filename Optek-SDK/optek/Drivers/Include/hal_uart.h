#ifndef __HAL_UART_H__
#define __HAL_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "hal_def.h"


/*
 ****************************************************************************************
 * CONSTANT DEFINITIONS
 ****************************************************************************************
 */
 /**
 * @brief UART transfer parity
 */
#define	UART_PARITY_NON			0
#define	UART_PARITY_EVEN		1
#define	UART_PARITY_ODD			2


/**
 * @enum enum_BAUD_RATE
 * @brief UART hardware transfer baudrate
 */
typedef enum
{
	BAUD_RATE_9600     = 9600,
	BAUD_RATE_19200    = 19200,
	BAUD_RATE_38400    = 38400,
	BAUD_RATE_115200   = 115200,
	BAUD_RATE_115200X2 = 115200*2,
	BAUD_RATE_115200X4 = 115200*4,
	BAUD_RATE_115200X8 = 115200*8
} enum_BAUD_RATE;


/**
 * @brief UART hardware transfer baudrate setting
 */
// #define UART0_BAUDRATE	BAUD_RATE_115200
// #define UART1_BAUDRATE	BAUD_RATE_115200
// #define UART2_BAUDRATE	BAUD_RATE_115200

// #define UART_FIFO_BAUDRATE	BAUD_RATE_115200
struct UART_HandleTypeDef
{
    // void* huart;
    // int tx_ena;
    // int rx_ena;
};

// HAL_StatusTypeDef hal_uart_init(UART_HandleTypeDef* huart, int tx_ena, int rx_ena);

int32_t hal_uart0_open(int32_t tx_ena, int32_t rx_ena, uint32_t baudrate);
void uart0_dma_tx_handler(void);
void uart0_dma_rx_handler(void);


#ifdef __cplusplus
}
#endif

#endif // __HAL_UART_H__