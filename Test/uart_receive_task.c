#include <stdio.h>
#include "FreeRTOS.h"
#include "hal_gpio.h"
#include "hal_dma.h"

#define RX_TIMEOUT_MS 100     // 超时时间，单位：ms
#define DMA_BUFFER_SIZE 256   // DMA 缓冲区大小
#define DATA_BUFFER_SIZE 512  // 数据缓冲区大小

uint8_t dma_buffer[DMA_BUFFER_SIZE];  // DMA 接收缓冲区
uint8_t data_buffer[DATA_BUFFER_SIZE];  // 数据处理缓冲区
size_t data_length = 0;  // 当前缓冲区内有效数据长度

size_t last_position = 0;  // 上次处理的缓冲区位置
TickType_t last_activity_tick = 0;  // 上次活动时间戳

// 空闲回调函数
void uart_idle_callback(uint8_t *data, size_t length) {
    // 自定义处理接收完成或超时的数据
    printf("Data received: ");
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}


void uart_dma_receive_init() {
    // 初始化 UART，启用 DMA 接收
    hal_gpio_mux_func_select(UART0_RX_GPIO_INDEX,MUX_SEL_FUNCTION1);
    hal_dma_transfer_init(  UART0_RX_DMA_ID, 
                            NULL, (U32 *)(&REG_UART0_RX_DATA), 
                            SOURCE_DMA_UART_0_RX, 
                            DMA_TRANS_1BYTE, 
                            DMA_BURST_SINGLE, 
                            0, 1, 1, NULL);
    DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(UART0_RX_DMA_ID)));
}


void uart_receive_task(void *pvParameters) {
    while (1) {
        // 获取当前 DMA 缓冲区位置
        size_t current_position = DMA_BUFFER_SIZE - 
                                __HAL_DMA_GET_COUNTER(huart1.hdmarx);

        if (current_position != last_position) {
            // 数据更新，处理新接收的数据
            if (current_position > last_position) {
                // 数据未回绕
                memcpy( &data_buffer[data_length], 
                        &dma_buffer[last_position], 
                        current_position - last_position);
                data_length += current_position - last_position;
            } else {
                // 数据回绕
                memcpy( &data_buffer[data_length], 
                        &dma_buffer[last_position], 
                        DMA_BUFFER_SIZE - last_position);
                data_length += DMA_BUFFER_SIZE - last_position;
                memcpy( &data_buffer[data_length], 
                        dma_buffer, current_position);
                data_length += current_position;
            }

            last_position = current_position;  // 更新最后处理的位置
            last_activity_tick = xTaskGetTickCount();  // 更新最后活动时间
        }

        // 检查是否超时
        if ((xTaskGetTickCount() - last_activity_tick) 
            >= pdMS_TO_TICKS(RX_TIMEOUT_MS)) 
        {
            if (data_length > 0) {
                // 超时触发回调
                uart_idle_callback(data_buffer, data_length);
                data_length = 0;  // 清空数据缓冲区
            }
            last_activity_tick = xTaskGetTickCount();  // 重置超时
        }

        vTaskDelay(pdMS_TO_TICKS(10));  // 周期性轮询间隔
    }
}
