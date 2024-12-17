#include <stdint.h>

void mem_test(void) {

    uint32_t *ptr = 0x80000000;
    // uint32_t *ptr = 0x90000000;
    uint32_t size = 16 * 1024 / 4;

    for (uint32_t i = 0; i < size; i++) {
        ptr[i] = 0xaaaaaaaa;
    }

    // char str_error[] = "memory read back error\r\n";
    // for (uint32_t i = 0; i < size; i++) {
    //     // if (ptr[i] != 0xaa) {
    //     int length = xsprintf(strbuf, "memory read back at %x, value is 0x%x\r\n", ptr+i, ptr[i]);
    //     uart_write(UART0, strbuf, length);
    //     // }
    // }

}