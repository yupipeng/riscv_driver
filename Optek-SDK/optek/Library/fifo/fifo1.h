#ifndef _FIFO1_H
#define _FIFO1_H
#include "c_def.h"

typedef struct {
    U8 *fifo_start_addr;   // FIFO 缓冲区的起始地址
    U8 *fifo_end_addr;     // FIFO 缓冲区的结束地址
    unsigned long fifo_length; // FIFO 缓冲区的长度（以字节为单位）
    U8 *pRead;             // 指向当前读取位置的指针
    U8 *pWrite;            // 指向当前写入位置的指针
    unsigned long rLen;    // 已经读取的数据长度
    unsigned long wLen;    // 已经写入的数据长度
    U8 wlen_H;             // 写入数据的高字节（可能是分配的数据长度的高字节）
    
    unsigned long start_pos; // 文件中的读取和写入起始位置
    unsigned long stream_len; // 文件长度
    U16 entry;               // 文件索引或序号
    U16 stream_margin;       // 数据流的边缘位置（可能表示数据流的缓冲区或区域的边界）
    U8 valid;                // 数据是否有效（例如，1表示有效，0表示无效）
} FIFO;

#define STREAM_LENTH_UNKNOWN    0xFFFFFFFF

void fifo_init ( FIFO *pFifo, U8 *pBuf,unsigned long len, int empty);

#endif // !_FIFO1_Hfif