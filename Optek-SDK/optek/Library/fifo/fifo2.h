#ifndef __FIFO2_H
#define __FIFO2_H

#include <stdint.h>


typedef struct {
    uint8_t *buffer;           // 缓冲区起始地址
    uint32_t size;             // 缓冲区大小
    uint32_t head;             // 写索引
    uint32_t tail;             // 读索引
} fifo_t;

// FIFO缓冲区定义大小
#define FIFO_BUFFER_SIZE 128

// 声明 FIFO 缓冲区
extern uint8_t fifo_buffer[FIFO_BUFFER_SIZE];
extern fifo_t printf_fifo;


void fifo2_init(fifo_t *fifo, uint8_t *buf, uint32_t size);

int fifo2_put(fifo_t *fifo, uint8_t data);


int fifo2_put1(fifo_t *fifo, const uint8_t *data, uint8_t len);

int fifo2_get(fifo_t *fifo, uint8_t *data);


#endif