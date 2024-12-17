#include <stdio.h>
#include <stdlib.h>

#include "soft_spi.h"
#include "hw_spi.h"
#include "hal_gpio.h"
// #include "ms520.h"
#include "NFC_TypeA.h"
// #include "NFC.h"
// #include "bsp_rs522.h"
// #include "mem_alloc.h"
#include <time.h>
#include "hw_timer.h"
#include "hw_uart.h"
#include "fifo2.h"
#include "hal_dma.h"
#include "printf_dma.h"
#include "printf.h"

#define IRQ_PIN          GPIO0_29_INDEX
#define RST_PIN          GPIO0_30_INDEX
#define GPIO_CS_PIN      GPIO0_14_INDEX
#define GPIO_SCK_PIN     GPIO0_19_INDEX
#define GPIO_MOSI_PIN    GPIO0_16_INDEX
#define GPIO_MISO_PIN    GPIO0_18_INDEX


SOFT_SPI_HandleTypeDef sspi = {
    .cpha = 1,
    .cpol = 1,
    .cs_active = 0,
    .spi_line = 0,
    .cs = GPIO_CS_PIN,
    .sck = GPIO_SCK_PIN,
    .sda = GPIO_MOSI_PIN,
    .miso = GPIO_MISO_PIN,
};

// #include "imu_driver.h"

void delay_us(uint32_t us) {
    for (int i = 0; i < us*3; i+=2) {
        __asm__ __volatile__("nop");
    }
}

void delay_ms(uint32_t ms)
{
    for (int i = 0; i < ms; i++) {
        delay_us(3999);
    }
}

FormatPICC PICC;


void system_task2(void)
{
	 static unsigned int nfc_number=0x00;

    // 3/0.00000075=4000000
    printf("system_task2\n");
//    MS520_Test();   //需要测试内部数据是否写正确时打开此函数  大约1秒打印一次数据。
    nfc_number++;
    if (nfc_number == 1) {
        printf("\r\n A_Start  ");
        TypeAInit();
    } else if (nfc_number == 2) {
        printf("reqa\n");
        if (REQA() == NFC_SUCCESS) {          // 检测是否有A卡
            printf("A card detected\n");
            AntiCollisionLoop();               // 防冲突循环
            if (PICC.SAK & 0x20) {            // 判断是否支持 1443-4(CPU卡)
                printf("CPU card detected\n");
                CLOSE_ANTENNA;                 // 关闭天线
            } else {
                printf("not a cpu card\n");
                // if(!MFAuthent(0x04,KEYA)) {     // 验证密码，切换区快需要重新验证密码
                //     ReadBlock(0x04,BlockData);   // 读块
                //     delay_ms(3);
                //     WriteBlock(0x04,data);       // 写块
                //     delay_ms(6);
                //     ReadBlock(0x04,BlockData);
                //     delay_ms(3);
                // }
            }
        }
        CLOSE_ANTENNA;                        // 关闭天线
    } else if (nfc_number == 5) {
        printf("reset\n");
        hal_gpio_output_level_set(RST_PIN, GPIO_LOW);    // 复位
    } else if (nfc_number == 6) {
        printf("reset end\n");
        hal_gpio_output_level_set(RST_PIN, GPIO_HIGH);   // 复位结束
    } else if (nfc_number == 7) {
        printf("next round\n");
        nfc_number = 0x00;
    }


}


void os_entry(void){
        
        init_uart_system();
        init_key_scan_task_system();
        #ifndef UART0_DMA_RX
            extern void init_uart_receive_task_system();
            init_uart_receive_task_system();
        #endif // !UART0_DMA_RX
    
        vTaskStartScheduler();  
}

#include "hw_spi.h"
#define RTOS
// #define Spi_dma_tx_test
#define UART_DMA_RX 
#define UART_DMA_TX

#ifdef UART_DMA_TX
#include "hal_uart.h"
void UART0_DMA_TX_TEST(){
    hal_uart0_open(1,0,115200);
    fifo_init(  &printf_fifo,
                fifo_buffer,
                FIFO_BUFFER_SIZE);
    char lpbuff[] = "Hello ,xxxxxxxxxxxxxxxxxxxxxxxxxx!\n";
    uint8_t data[] = "";
    printf("Welcome the riscv !\n");
    while (1)
    {  
        
        delay_ms(1000);
       
  
        for(uint8_t i = 0; i < sizeof(fifo_buffer) ;i++){
        if (fifo2_get(&printf_fifo, &data) == 0)
        {
            hal_dma_uart0_send_data(UART0_TX_DMA_ID,data,sizeof(data));
        }
        }
        delay_ms(1000);
        hal_dma_uart0_send_data(UART0_TX_DMA_ID,lpbuff,sizeof(lpbuff));
        
    }
}
#endif // UART_DMA_TX

#include "printf.h"

extern const extif_api_stru uart0_api;

extern void printf_init(const extif_api_stru* ext_if);
printf_test(){
    hal_uart0_open(1, 1, 115200);
    printf_init(&uart0_api);
    while (1)
    {
        printf_init(&uart0_api);
        // console_cmd_init(&uart0_api);
        // console_cmd_start();
        myprintf("hello word! %s\r\n" , "risc-v");

        myprintf ("SDK version:%s\n\r", "1.0.2.02");
        myprintf ("Buid time:%s %s\n\r", __TIME__, __DATE__);

        myprintf ("App version:%s\n\r", "1.0.2.02");
        myprintf ("Buid time:%s %s\n\r", __TIME__, __DATE__);
        delay_ms(1000);
    }
    
   
}

void app_main(void)
{

        fifo_init(  &printf_fifo,
                    fifo_buffer,
                    FIFO_BUFFER_SIZE);
 
        #ifdef Spi_dma_tx_test
            // spi_dma_open(1 , 0 ,NULL ,NULL);
            spi_dma_tx_test();
        #endif // spi_dma_tx_test

        #ifdef PRINTF_DMA
            my_printf_dma("Helo Word!!!\n");
            uart0_dma_init();
            DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(UART0_TX_DMA_ID)));
        #endif // DEBUG
        
        #ifdef RTOS
             os_entry();
        #endif 
   
    

}

char ReadReg(char Address) {
    uint8_t data = 0x00;

    soft_spi_read_reg(&sspi, 0x80 | (Address << 1), &data, 1);
    // printf("read reg: 0x%02x, value: 0x%02x\n", reg, data);
    return data;
}
void WriteReg(char Address,char Data) {
    soft_spi_write_reg(&sspi, Address, &Data, 1);
}
void ms520_reset_low(void){
    hal_gpio_output_level_set(RST_PIN, GPIO_LOW);
}
void ms520_reset_high(void){
    hal_gpio_output_level_set(RST_PIN, GPIO_HIGH);
}

void ms520_write_reg(uint8_t reg, const uint8_t value)
{
    // printf("write reg: 0x%02x, value: 0x%02x\n", reg, value);
    soft_spi_write_reg(&sspi, reg, &value, 1);
}
uint8_t ms520_read_reg(uint8_t reg)
{
    uint8_t data = 0x00;

    soft_spi_read_reg(&sspi, 0x80 | (reg << 1), &data, 1);
    // printf("read reg: 0x%02x, value: 0x%02x\n", reg, data);
    return data;
}

void ms520_write_regs(uint8_t reg, const uint8_t *buf, uint32_t len) {
    // soft_i2c_write_reg(&si2c0, 0xd0, reg, buf, len);
    // hw_spi_write_reg(SPI1, reg, buf, len);
	// soft_spi_write_reg(&sspi, reg, buf, len);

}
void ms520_read_regs(uint8_t reg, uint8_t *buf, uint32_t len) {
    // soft_i2c_read_reg(&si2c0, 0xd0, reg, buf, len);
    // hw_spi_read_reg(SPI1, 0x80 | reg, buf, len);
	// soft_spi_read_reg(&sspi, 0x80 | reg, buf, len);

}
