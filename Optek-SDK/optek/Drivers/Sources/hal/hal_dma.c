#include "hal_dma.h"
#include "fifo2.h"
#include "printf_dma.h"
#include <string.h>
dma_done_callback dma_callbacks[DMA_COUNT];

void hal_dma_module_init(void)
{
	memset(dma_callbacks, 0, sizeof(dma_callbacks));
}

void hal_dma_transfer_init( enumDMA_ID dma_id, U32 *dst, U32 *src, 
                            enumDMA_TRIG dma_trig, 
                            enumDMA_TRANS_BYTEs size, 
                            enumDMA_TRANS_BURST_SIZE b_size, 
                            BOOL src_inc, BOOL dst_inc, 
                            BOOL int_enb, dma_done_callback callback)
{
	DMA_CNTRs *dma_cntr = (DMA_CNTRs *) (DMA_CONFIG_BASE_ADDR 
                                    + (int)dma_id*DMA_CNTRs_SIZE);

	//sanity
	DBG_Assert((int)dma_id < DMA_ID_MAX);
	DBG_Assert(dma_trig < SOURCE_DMA_MAX);
	DBG_Assert(size < DMA_TRANS_BYTEs_MAX);
	DBG_Assert(b_size <= DMA_TRANS_BURST_MAX);	

	if ((int)dma_id < DMA_ID_MAX)
	{
		if (dma_callbacks[dma_id] != callback)
		{
			if (dma_callbacks[dma_id] != NULL)
			{
				//permit DMA to reuse
				__asm__("nop");
			}

			dma_callbacks[dma_id] = callback;
		}

		dma_cntr->sub_cntr.channel_enb = 0;

		dma_cntr->count = 0;
		dma_cntr->source = (U32)src;
		dma_cntr->dest = (U32)dst;
		dma_cntr->sub_cntr.size = size;
		dma_cntr->sub_cntr.burst_size = b_size;
		dma_cntr->sub_cntr.saddr_inc = src_inc;
		dma_cntr->sub_cntr.daddr_inc = dst_inc;
		dma_cntr->sub_cntr.source = dma_trig;
		dma_cntr->sub_cntr.start = 0;
		dma_cntr->sub_cntr.int_enb = int_enb;
		dma_cntr->sub_cntr.channel_enb = 1;
	}
}

void uart0_dma_init() {
    printf("%s\r\n",__func__);
    // 假设 UART0 的 DMA 通道号
    enumDMA_ID dma_id = DMA_ID_4;

    // 假设 UART0 数据寄存器地址
    U32 *uart0_data_reg = (U32 *)REG_UART0_TX_DATA;

    // 发送缓冲区地址（可以是您的缓冲区）
    U32 *send_buffer = &txbuff;

    // 触发源：假设 UART0_TX_DMA_TRIG 是 UART0 的 DMA 触发信号
    enumDMA_TRIG dma_trig = SOURCE_DMA_UART_0_TX;

    // 设置传输字节大小和突发大小
    enumDMA_TRANS_BYTEs trans_size = DMA_TRANS_1BYTE; // 每次传输 1 字节
    enumDMA_TRANS_BURST_SIZE burst_size = DMA_BURST_SINGLE; // 突发大小为 1

    // 设置地址自增：UART 数据寄存器固定，缓冲区地址递增
    BOOL src_inc = TRUE;  // 发送缓冲区地址自增
    BOOL dst_inc = FALSE; // UART 数据寄存器地址不变

    // 中断使能
    BOOL int_enb = FALSE;

    // 初始化 DMA 传输
    hal_dma_transfer_init(
        dma_id,           // DMA 通道号
        uart0_data_reg,   // 目标地址（UART 数据寄存器）
        send_buffer,      // 源地址（发送缓冲区）
        dma_trig,         // DMA 触发源
        trans_size,       // 每次传输的字节大小
        burst_size,       // 突发大小
        src_inc,          // 源地址是否递增
        dst_inc,          // 目标地址是否递增
        int_enb,          // 是否使能中断
        uart0_dma_done_callback // DMA 完成回调函数
    );

    printf("UART0 DMA initialized.\n");
}

void hal_dma_uart0_send_data( enumDMA_ID dma_id ,
                              U32 *buf, U32 size)
{
     
     DBG_Assert(dma_id < DMA_ID_MAX);
     DBG_Assert(buf != NULL);
     DMA_CNTRs *dma_cntr = (DMA_CNTRs *)(DMA_CONFIG_BASE_ADDR + 
                                         dma_id*DMA_CNTRs_SIZE);
     dma_cntr->source = buf;
     dma_cntr->count = size;
}



void uart0_dma_done_callback(void)
{
    printf("%s\n" ,__func__);
    // DMA 完成后，执行的回调函数
    // 比如准备下一批数据
    // 此处假设我们可以继续从 txbuff 中发送数据
    // 检查缓冲区 flag 是否为 1，表示数据已准备好
    for (int i = 0; i < UART_TX_BUFF_SIZE; i++) {
        if (txbuff.flag[i] == 1) {
            // 数据已经准备好，继续传输
            hal_dma_uart0_send_data( UART0_TX_DMA_ID,&txbuff,
                                     strlen(txbuff.data));
            
            txbuff.flag[i] = 0;  // 传输完成后，重置标志
        }
    }
}



