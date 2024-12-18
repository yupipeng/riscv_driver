// See LICENSE for license details.
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
// #include "nuclei_sdk_soc.h"

// #include "demosoc.h"
// #include "system_demosoc.h"
#include "regmap.h"
#include "hal_dma.h"

#include "hw_uart.h"

#include "hw_cpu_clock.h"



// #include "xprintf.h"

#define TEST_SW_VERSION		0x23072401 //yy/mm/dd/s. no

#ifndef CFG_SIMULATION
#define RUN_LOOPS   20
#else
#define RUN_LOOPS   5
#endif

#define U32 unsigned int


volatile uint32_t *pwdata = (uint32_t*)(0x90000000 + 8*1024);	//pointer to iRam start address
const uint32_t sampledata[]  = {	0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
									0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
									0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
									0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F};

volatile uint32_t *prflashdata = (uint32_t*)(0x6A000400);		//read flash data


// #define FIFO_BUFFER_SIZE 128
// uint8_t fifo_buffer[FIFO_BUFFER_SIZE];
// fifo_t printf_fifo;

void delaynop(uint32_t cnt)
{
    while(cnt--)
    {
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");

        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
    }
}

uint32_t gpio_int_cnt = 0;

void gpio_int_handler(void)
{
    REG_GPIO0_INTST |= (0x1<<3);

    REG_GPIO0_DOUT = (1<<2);
    //delaynop(1);
    REG_GPIO0_DOUT = 0;
    gpio_int_cnt++;
}

void dma_int_handler(void)
{
    int val = DMA_GCNTR;;

    REG_MISC_STATUS = val;

    if ((val & (0x1<<6)) == (0x1<<6))
    {
        DMA_GCNTR = (0x1000 | (0x1<<6));
        REG_GPIO0_DOUT = (1<<2);
        delaynop(10);
        REG_GPIO0_DOUT = 0;
        gpio_int_cnt++;
    }

    if ((val & (0x1<<7)) == (0x1<<7))
    {
        DMA_GCNTR = (0x1000 | (0x1<<7));
        REG_GPIO0_DOUT = (1<<4);
        delaynop(10);
        REG_GPIO0_DOUT = 0;
        gpio_int_cnt++;
    }

    val = DMA_GCNTR;
    REG_MISC_STATUS = val;

}


void ctest_delay(uint32_t ms)
{
    uint64_t cur_cycle = __get_rv_cycle();
    uint64_t end_cycle = cur_cycle + (12000000  / 1000) * ms;

    do {
        cur_cycle = __get_rv_cycle();
    } while (cur_cycle < end_cycle);
}

const U32 number [16 * 1024];

char strbuf[64];

uint8_t mybuff[128];

U16 saradc_buf[256];

#include "printf_dma.h"
#include "hw_spi.h"
int main(void)
{      
        // printf("are you ok?\n");
     
        app_main();
        return 0;
}
    

