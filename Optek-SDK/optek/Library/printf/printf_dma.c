
#include "printf_dma.h"
#include <stdarg.h>
#include "hal_dma.h"

uart_tx_buff_t txbuff;

void my_printf_dma(const char *fmt, ...)
{
    static uint8_t i = 0;
    va_list args;
    va_start(args, fmt);
    
    // 清空当前缓冲区
    memset(&txbuff.data[i][0], 0, PRINTF_BUFFSIZE);
    
    // 使用 vsnprintf 格式化字符串并存储到 txbuff 中
    vsnprintf((char*)&txbuff.data[i][0], PRINTF_BUFFSIZE, fmt, args);
    
    // 设置标志，表示数据已准备好
    txbuff.flag[i] = 1;

    // 结束变参列表
    va_end(args);
    
    // 循环使用缓冲区
    i++;
    if(i >= UART_TX_BUFF_SIZE) {
        i = 0;
    }


    // hal_dma_uart0_send_data(SOURCE_DMA_UART_0_TX,
    //                         txbuff.data[i][0],1);   
    
}




