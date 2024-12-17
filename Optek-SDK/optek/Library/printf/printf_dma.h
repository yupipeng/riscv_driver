#ifndef __PRINTF_DMA_H_
#define  __PRINTF_DMA_H_
#define PRINTF_BUFFSIZE 256
#define UART_TX_BUFF_SIZE 6

#include <stdio.h>
#include "hal_dma.h"

// 定义缓冲区
typedef struct {
    char data[UART_TX_BUFF_SIZE][PRINTF_BUFFSIZE];
    uint8_t flag[UART_TX_BUFF_SIZE]; // 用于标记是否数据已准备好
} uart_tx_buff_t;

extern uart_tx_buff_t txbuff;
 
void my_printf_dma(const char *fmt, ...);

#endif