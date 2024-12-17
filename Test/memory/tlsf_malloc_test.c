#include "mem_alloc.h"
#include <stdlib.h>
#include <stdio.h>

void tlsf_malloc_test(void) {
    int *list = malloc(10*sizeof(int));

    if (list == NULL) {
        printf("malloc failed\r\n");
        while (1);
    }

    for (int i = 0; i < 10; i++) {
        list[i] = i;
    }

    for (int i = 0; i < 10; i++) {
        printf("list[%d] = %d@%x\r\n", i, list[i], list+i);
    }

    free(list);
    // uint8_t *pBuf = malloc(200);
    // fifo_init(&fifo, pBuf, 10, 1);
}