#include <stdint.h>
#include <stdio.h>
#include "fifo2.h"

#define FIFO_BUFFER_SIZE 128
uint8_t fifo_buffer[FIFO_BUFFER_SIZE];
fifo_t printf_fifo;

void fifo2_init(fifo_t *fifo, uint8_t *buf, uint32_t size) 
{
    fifo->buffer = buf;
    fifo->size = size;
    fifo->head = 0;
    fifo->tail = 0;
}

int fifo2_put(fifo_t *fifo, uint8_t data) 
{
    uint32_t next_head = (fifo->tail + 1) % fifo->size; // 计算下一个写索引
    if (next_head == fifo->head) {
        // 缓冲区已满
        return -1;
    }
 
    fifo->buffer[fifo->tail] = data;
    fifo->tail = next_head; // 更新写索引
    return 0;
}

int fifo2_put1(fifo_t *fifo, const uint8_t *data, uint8_t len) 
{
    if (!fifo || !data || len == 0) {
        return -1; // 参数无效
    }

    // 检查是否有足够的空间
    uint32_t free_space =    (fifo->head <= fifo->tail) 
                           ? (fifo->tail - fifo->head - 1)
                           : (fifo->size - (fifo->head - fifo->tail) - 1);


    if (len > free_space) {

        len = free_space;
        if(len >= 2){
            len -= 2;
        }else{
            len = 0;
        }
        for (size_t i = 0; i < len; i++)
        {
            fifo->buffer[fifo->tail] = data[i];
            fifo->tail = (fifo->tail + 1) % fifo->size;
        }
        for (size_t i = 0; i < 2; i++)
        {
            fifo->buffer[fifo->tail] = "*\n";
            fifo->tail = (fifo->tail + 1) % fifo->size;
        }
        
        return -1; // 空间不足
        }
        else{

            for (size_t i = 0; i < len; i++) {
                fifo->buffer[fifo->tail] = data[i];
                fifo->tail = (fifo->tail + 1) % fifo->size; // 更新写索引
            }

            return 0; // 成功
        }
}


int fifo2_get(fifo_t *fifo, uint8_t *data) {
    

    if (fifo->head == fifo->tail) {
    // 缓冲区为空
    return -1;
    }

    *data = fifo->buffer[fifo->head];

    // fifo->buffer[fifo->head] = '\0';
    // memset(&fifo->buffer[fifo->head], 0, 1);

    fifo->head = (fifo->head + 1) % fifo->size; // 更新读索引


    return 0;
  
}
