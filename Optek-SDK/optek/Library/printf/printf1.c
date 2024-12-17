#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#define TEMP_BUFFER_SIZE 1024
static char tempBuffer[TEMP_BUFFER_SIZE];
static int tempWriteIndex = 0;

int AsyncPrint(const char *format, ...) {
    va_list args;
    char *buffer;
    int length;

    buffer = pvPortMalloc(128);
    if (buffer == NULL) {
        return -1;
    }

    va_start(args, format);
    length = vsnprintf(buffer, 128, format, args);
    va_end(args);

    if (xQueueSend(printQueue, &buffer, 0) != pdPASS) {
        // 队列满，缓存到临时缓冲区
        if ((tempWriteIndex + length) < TEMP_BUFFER_SIZE) {
            memcpy(&tempBuffer[tempWriteIndex], buffer, length);
            tempWriteIndex += length;
        }
        vPortFree(buffer);
        return -1;
    }

    return length;
}
