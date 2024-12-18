#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// 串口接收缓冲区大小
#define RX_BUFFER_SIZE 256

// 模拟空闲超时时间 (毫秒)
#define UART_IDLE_TIMEOUT_MS 50

// 模拟串口硬件接收状态
volatile bool uart_has_data = false;  // 模拟串口是否有新数据
uint8_t uart_data;                   // 模拟串口接收到的数据

// 软件接收缓冲区
uint8_t rx_buffer[RX_BUFFER_SIZE];
uint16_t rx_index = 0; // 当前接收的数据位置

// 上次接收到数据的时间戳
clock_t last_data_time;

// 模拟函数：从串口读取数据（替代硬件寄存器读取）
bool UART_Read(uint8_t *data)
{
    if (uart_has_data)
    {
        *data = uart_data; // 模拟接收到的数据
        uart_has_data = false; // 模拟读取完毕
        return true;
    } 
    return false;
}

// 用户回调函数：处理接收到的数据
void UART_IdleCallback(uint8_t *data, uint16_t length)
{
    my_printf_dma("Idle Callback: Received %d bytes: ", length);
    for (int i = 0; i < length; i++)
    {
        my_printf_dma("%02X ", data[i]);
    }
    my_printf_dma("\n");
}

// 软件模拟空闲中断任务
void UART_SoftwareIdleTask(void)
{
    uint8_t data;
    clock_t current_time;

    // 初始化上次接收时间
    last_data_time = clock();

    while (1)
    {
        // 轮询接收串口数据
        if (UART_Read(&data))
        {
            // 将数据存入接收缓冲区
            if (rx_index < RX_BUFFER_SIZE)
            {
                rx_buffer[rx_index++] = data;
            }

            // 更新最后接收时间
            last_data_time = clock();
        }

        // 获取当前时间
        current_time = clock();

        // 判断是否超时
        if (rx_index > 0 && ((current_time - last_data_time) * 1000 / CLOCKS_PER_SEC) > UART_IDLE_TIMEOUT_MS)
        {
            // 超时处理：调用空闲回调
            UART_IdleCallback(rx_buffer, rx_index);

            // 重置接收缓冲区
            memset(rx_buffer, 0, RX_BUFFER_SIZE);
            rx_index = 0;
        }

        // 模拟任务延时，避免高频占用 CPU
        struct timespec delay = {0, 10 * 1000000}; // 10ms
        nanosleep(&delay, NULL);
    }
}


// 模拟发送数据到串口
void SimulateUARTReceive(uint8_t *data, uint16_t len, int delay_ms)
{
    for (int i = 0; i < len; i++)
    {
        uart_data = data[i];
        uart_has_data = true;

        // 模拟数据到达的间隔
        struct timespec delay = {delay_ms / 1000, (delay_ms % 1000) * 1000000};
        nanosleep(&delay, NULL);
    }
}

 