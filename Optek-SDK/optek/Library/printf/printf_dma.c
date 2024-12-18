
#include "printf_dma.h"
#include <stdarg.h>
#include <stdio.h>
#include "hal_dma.h"
#include "fifo2.h"

uart_tx_buff_t txbuff;

extern void delay_ms(uint32_t ms);

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
    
   

    if(txbuff.flag[i] == 1){
        hal_dma_uart0_send_data(    UART0_TX_DMA_ID,
                                    (uint32_t*)&txbuff.data[i][0], 
                                    sizeof(txbuff.data[i])   );
        delay_ms(50);
        txbuff.flag[i] == 0;
    }
        
     // 循环使用缓冲区
    i++;
    if(i >= UART_TX_BUFF_SIZE) {
        i = 0;
    }

}

// DMA 完成回调函数
void uart0_dma_done_callback(void)
{
    return ;
}

#include <stdarg.h>
#include <stdio.h>

#define BUFFER_SIZE 256

char buffer[BUFFER_SIZE];

// int _write(int file, char *data, int len) {
//     hal_dma_uart0_send_data(    UART0_TX_DMA_ID,
//                                 (uint32_t*)&fifo_buffer, 
//                                 sizeof(fifo_buffer)   );
  
//     return len;
// }

// int printf(const char *format, ...) {
//     va_list args;
//     va_start(args, format);
//     vsnprintf(buffer, BUFFER_SIZE, format, args);
//     va_end(args);
//     _write(1, fifo_buffer, strlen(fifo_buffer));
//     return 0;
// }




