#include <stdint.h>
#include "regmap.h"
#include "hw_uart.h"

#include "hal_dma.h"
#include "hw_cpu_clock.h"
#include "hw_uart.h"
#include "hal_dma.h"

#define HW_UART0_DMA_USE



/* hal uart structure */
static hal_uart_stru hal_uart0 = {0};
void uart0_dma_tx_handler(void);
void uart0_dma_rx_handler(void);
/*
 *****************************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Finish uart hardware setting function
 *
 * @param[in]  tx_ena    	Enable/disable uart tx function
 * @param[in]  rx_ena     	Enable/disable uart rx function
 * @param[in]  baudrate    	Transfer rate of data
 *
 * @return 0
 ****************************************************************************************
 */
int32_t hal_uart0_open(int32_t tx_ena, int32_t rx_ena, uint32_t baudrate)
{
#ifdef HW_UART0_DMA_USE
	MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *) &REG_MISC_CLKGATE;

	pGate->u_uart0_gate = 1; //open clk gate

	if (tx_ena)
	{
		hal_gpio_mux_func_select(	UART0_TX_GPIO_INDEX,	
									MUX_SEL_FUNCTION1	);
    	hal_dma_transfer_init(	UART0_TX_DMA_ID, 
								(U32 *)(&REG_UART0_TX_DATA), 
								NULL,
								SOURCE_DMA_UART_0_TX,
								DMA_TRANS_1BYTE, 
								DMA_BURST_SINGLE, 
								1, 0, 1, 
								uart0_dma_tx_handler	);
		DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(UART0_TX_DMA_ID)));

	}
	if (rx_ena)
	{
		hal_gpio_mux_func_select(	UART0_RX_GPIO_INDEX,
									MUX_SEL_FUNCTION1	);
		hal_dma_transfer_init(	UART0_RX_DMA_ID, 
								NULL, 
								(U32 *)(&REG_UART0_RX_DATA),
								SOURCE_DMA_UART_0_RX,
								DMA_TRANS_1BYTE,
								DMA_BURST_SINGLE,
								0, 1, 1, 
								uart0_dma_rx_handler	);
		DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(UART0_RX_DMA_ID)));
	}

	
    hw_uart_setBaudRate(en_UART0, baudrate, PERI_CLK_HZ);
    hw_uart_setParity(en_UART0, UART_PARITY_NON);
	hw_uart_open(en_UART0, tx_ena, rx_ena);//rx auto start if rx_ena = 1,no need start again

    // xt_ints_on(LEVEL3_INT9_MASK);

    return 0;
#else
	MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *) &REG_MISC_CLKGATE;

	pGate->u_uart0_gate = 1; //open clk gate

	if (tx_ena)
	{
		hal_gpio_mux_func_select(UART0_TX_GPIO_INDEX,MUX_SEL_FUNCTION1);
	}
	if (rx_ena)
	{
		hal_gpio_mux_func_select(UART0_RX_GPIO_INDEX,MUX_SEL_FUNCTION1);
	}
	
    hw_uart_setBaudRate(en_UART0, baudrate, PERI_CLK_HZ);
    hw_uart_setParity(en_UART0, UART_PARITY_NON);
	hw_uart_open(en_UART0, tx_ena, rx_ena);//rx auto start if rx_ena = 1,no need start again

    UART0_INT_ENABLE;
    // xt_ints_on(LEVEL1_INT1_MASK);

    return 0;
#endif
}

#ifdef HW_UART0_DMA_USE
void uart0_dma_tx_handler(void)
{
    extif_callback callback;
	uint32_t param;

	if(hal_uart0.tx.callback != NULL)
	{
		callback = hal_uart0.tx.callback;
		param = hal_uart0.tx.param;

		// Clear callback pointer
		hal_uart0.tx.callback = NULL;
		hal_uart0.tx.param    = NULL;	

		// Call handler
		callback(param, EXTIF_STATUS_OK);	
	}
}

void uart0_dma_rx_handler(void)
{
    extif_callback callback;
	uint32_t param;

	if(hal_uart0.rx.callback != NULL)
	{
		callback = hal_uart0.rx.callback;
		param = hal_uart0.rx.param;

		// Clear callback pointer
		hal_uart0.rx.callback = NULL;
		hal_uart0.rx.param    = NULL;	

		// Call handler
		callback(param, EXTIF_STATUS_OK);	
	}
}


/**
 *************************************************************************************
 * @brief Starts a data reception.
 *
 * @param[out] bufptr      Pointer to the RX buffer
 * @param[in]  size        Size of the expected reception
 * @param[in]  callback    Pointer to the function called back when transfer finished
 * @param[in]  param       parameter returned to callback when reception is finished
 *
 *************************************************************************************
 */
void hal_uart0_read(uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param)
{
#ifdef HW_UART0_DMA_USE
    //Sanity check
    DBG_Assert(bufptr != NULL);
    DBG_Assert(size != 0);

    hal_uart0.rx.callback = callback;
    hal_uart0.rx.param    = param;

    hal_dma_receive_data(UART0_RX_DMA_ID, bufptr, size);
#else
    // Sanity check
    DBG_Assert(bufptr != NULL);
    DBG_Assert(size != 0);

	puart0_rx_buffer = bufptr;
	uart0_rx_buffer_size = size;

    hal_uart0.rx.callback = callback;
    hal_uart0.rx.param    = param;

    UART_REGISTERS* pregs = (UART_REGISTERS*)&REG_UART0_CONTRL;

	if (!(pregs->intEnable.rx_buf_not_empty))
	{
		pregs->intEnable.rx_buf_not_empty = 1;
	}
#endif
}

/**
 *************************************************************************************
 * @brief Starts a data transmission.
 *
 * @param[in]  bufptr      Pointer to the TX buffer
 * @param[in]  size        Size of the transmission
 * @param[in]  callback    Pointer to the function called back when transfer finished
 * @param[in]  param       parameter returned to callback when transmission is finished
 *
 *************************************************************************************
 */
void hal_uart0_write(uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param)
{
#ifdef HW_UART0_DMA_USE
    //Sanity check
    DBG_Assert(bufptr != NULL);
    DBG_Assert(size != 0);

    hal_uart0.tx.callback = callback;
    hal_uart0.tx.param    = param;

    hal_dma_send_data(UART0_TX_DMA_ID, bufptr, size);
#else
    // Sanity check
    DBG_Assert(bufptr != NULL);
    DBG_Assert(size != 0);

    hal_uart0.tx.callback = callback;
    hal_uart0.tx.param    = param;

	puart0_tx_buffer = bufptr;
    uart0_tx_buffer_size = size;
    UART_REGISTERS* pregs = (UART_REGISTERS*)&REG_UART0_CONTRL;
	if (!(pregs->intEnable.tx_buf_half_empty))
	{
		pregs->intEnable.tx_buf_half_empty = 1;
	}
#endif
}

/**
 *************************************************************************************
 * @brief Enable Interface receive flow.
 *************************************************************************************
 */
void hal_uart0_rx_flow_on(void)
{
	//incomplete function
	DBG_Assert(FALSE);
}

/**
 *************************************************************************************
 * @brief Disable Interface receive flow.
 *************************************************************************************
 */
void hal_uart0_rx_flow_off(void)
{
	//incomplete function
	DBG_Assert(FALSE);
}


// Creation of uart0 external interface api
const extif_api_stru uart0_api =
{
    hal_uart0_read,
    hal_uart0_write,
    hal_uart0_rx_flow_on,
    hal_uart0_rx_flow_off,
};


#endif