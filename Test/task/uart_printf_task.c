#include "FreeRTOS.h"
#include "hw_uart.h"
#include "fifo2.h"


void uart_printf(void *arg) {
    // fifo_t *fifo = (fifo_t *)arg;
    uint8_t data;

    while (1) {
        if (fifo2_get(&printf_fifo, &data) == 0) { // 从FIFO读取数据
            hw_uart_write(UART0, &data, 1); // 使用串口发送数据
        } else {
            vTaskDelay(pdMS_TO_TICKS(10)); // 如果FIFO为空，延迟一段时间
        }
    }
}


 //创建uart_send_task任务并启动
void init_uart_system() {

    xTaskCreate(uart_printf, "uart_printf", 256, &printf_fifo, 3, NULL);
   
}
