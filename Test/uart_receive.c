#include "hw_uart.h"
#include "FreeRTOS.h"
#include "fifo2.h"
#include <string.h>
#include <stdio.h>
uint8_t receivedata[128];
// #define UART_DMA_RX
#include "hw_uart.h"
#include "hal_dma.h"
uint8_t txbuff1[128];
void uart_receive_task(){
    while (1)
    {
        printf("%d");
        #ifdef UART_DMA_RX
        hal_dma_transfer_init(  UART0_RX_DMA_ID, txbuff1 ,
                            (U32 *)(&REG_UART0_RX_DATA), 
                            SOURCE_DMA_UART_0_RX, 
                            DMA_TRANS_1BYTE, 
                            DMA_BURST_SINGLE, 
                            0, 1, 1, NULL);
        DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(UART0_RX_DMA_ID)));
        #endif // UART_DMA_RX

        hw_uart_read(  UART0,receivedata ,
                    8);
        printf("%s" , receivedata);
              
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    
}

void init_uart_receive_task_system(){
    xTaskCreate(uart_receive_task, "UartReceive", 
                512, NULL, 4, NULL);   
}

