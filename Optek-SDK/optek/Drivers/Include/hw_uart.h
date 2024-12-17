#ifndef	_HW_UART_H
#define	_HW_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "regmap.h"  // UART_REGISTERS
#include "sdk_com.h"
#include "hal_gpio.h"

#define UART0 ((UART_REGISTERS*)UART0_BASE_ADDR)
#define UART1 ((UART_REGISTERS*)UART1_BASE_ADDR)
#define UART2 ((UART_REGISTERS*)UART2_BASE_ADDR)

#define	UART0_TX        GPIO_INDEX2MASK(UART0_TX_GPIO_INDEX) //[1] 0
#define	UART0_RX        GPIO_INDEX2MASK(UART0_RX_GPIO_INDEX) //[1] 1

#define	UART1_TX        GPIO_INDEX2MASK(UART1_TX_GPIO_INDEX) //[0] 9
#define	UART1_RX        GPIO_INDEX2MASK(UART1_RX_GPIO_INDEX) //[0] 5

#define	UART2_TX        GPIO_INDEX2MASK(UART2_TX_GPIO_INDEX) //[0] 11
#define	UART2_RX        GPIO_INDEX2MASK(UART2_RX_GPIO_INDEX) //??? just for debug

typedef	enum {
	en_UART0	=	0,
	en_UART1,
	en_UART2,
	en_NUM_MAX
}	enum_UART_NUM;

#define DEFAULT_BAUD            460800

struct uart_txrxcallback
{
    // call back function pointer
    extif_callback callback;
    //parameter returned to callback when operation is over.
    uint32_t param;
};

typedef struct
{
	struct uart_txrxcallback tx;
	struct uart_txrxcallback rx;
	U32 dma_channel;

	U8 errordetect;
} hal_uart_stru;

/**
 * @brief setup uart hardware
 *
 * @param huart which hardware uart to use (eg: UART0 UART1 UART2)
 * @param baudrate baud rate
 * @param priority priority
 * @param tx_ena enable tx
 * @param rx_ena enable rx
 */
void hw_uart_setup(UART_REGISTERS *huart, uint32_t baudrate, uint32_t priority, int tx_ena, int rx_ena);

/**
 * @brief init uart hardware(actually set the gpio function)
 *
 * @param huart
 * @param tx_ena
 * @param rx_ena
 * @return int
 */
int hw_uart_init(UART_REGISTERS* huart, int tx_ena, int rx_ena);

/**
 * @brief init uart hardware, open tx and rx
 *
 * @param huart
 * @param tx_ena
 * @param rx_ena
 * @return int
 */
int hw_uart_open(UART_REGISTERS *huart, int tx_ena, int rx_ena);

/**
 * @brief set baud rate of huart
 *
 * @param huart
 * @param baudRate
 * @param sysclk
 * @return int
 */
int hw_uart_setBaudRate(UART_REGISTERS *huart, U32 baudRate, U32 sysclk);

/**
 * @brief
 *
 * @param huart
 * @param parity
 * @return int
 */
int hw_uart_setParity(UART_REGISTERS *huart, U32 parity);

/**
 * @brief transmit data through hardware uart
 *
 * @param huart which hardware uart to use (eg: UART0 UART1 UART2)
 * @param data data to be transmitted
 * @param len length of data
 * @return int
 * @retval TRUE success
 */
int hw_uart_write(UART_REGISTERS *huart, const char *data, int len);

/**
 * @brief receive data through hardware uart
 *
 * @param huart which hardware uart to use (eg: UART0 UART1 UART2)
 * @param data data buffer
 * @param len length of data
 * @return int
 */
int hw_uart_read(UART_REGISTERS *huart, char *data, int len);




#ifdef __cplusplus
}
#endif

#endif //_HW_UART_H
