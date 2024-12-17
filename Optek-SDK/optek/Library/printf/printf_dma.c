
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
    
   

    if(txbuff.flag[i] == 1){
        hal_dma_uart0_send_data(    UART0_TX_DMA_ID,
                                    (uint32_t*)&txbuff.data[i][0], 
                                    sizeof(txbuff.data[i])   );
  
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
    // 清除当前缓冲区的标志位，表示数据已发送
    // txbuff.flag[0] = 0;

    // // 如果还有其他数据需要发送，继续进行 DMA 传输
    // if (txbuff.flag[1] == 1) {
    //     hal_dma_transfer_init(  UART0_TX_DMA_ID, 
    //                             (U32 *)(&REG_UART0_TX_DATA),
    //                             (uint32_t*)&txbuff.data[1][0], 
    //                             SOURCE_DMA_UART_0_TX,
    //                             DMA_TRANS_1BYTE, 
    //                             DMA_BURST_SINGLE, 
    //                             1, 0, 1, 
    //                             uart0_dma_done_callback     );
    // }
    return ;
}



