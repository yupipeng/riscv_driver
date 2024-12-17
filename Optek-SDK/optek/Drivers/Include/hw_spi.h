#ifndef __HW_SPI_H__
#define __HW_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "hw_gpio.h"
#include "regmap.h"
#include "hal_dma.h"

typedef struct {
    union {
        uint32_t control;
        SPI_CONTROLs ctrl;
    };
    union {
        uint32_t format;
        SPI_FORMATs fmt;
    };
    union {
        uint32_t int_mask;
        SPI_INT_MASKs int_msk;
    };
    union {
        uint32_t status;
        SPI_STATUSs stat;
    };
    uint32_t tx_data;
    uint32_t rx_data;
    uint32_t max_trans_num;
    uint32_t rxtx_num;
    uint32_t qspi_cmd;
    uint32_t qspi_cmd_2;
    uint32_t version;
} SPI_REGISTERS;

typedef enum {
    SPI_CS0_USED = 0x1,
    SPI_CS1_USED = 0x2,

    SPI_MODE_0 = 0x0,
    SPI_MODE_1 = 0x4,
    SPI_MODE_2 = 0x8,
    SPI_MODE_3 = 0xC,
    SPI_MODE_MASK = 0xC,

    SPI_MODE_CS_MANUAL = 0x10,
    SPI_MISO_USED = 0x20,
    SPI_QSPI_ENABLE = 0x40,
    SPI_MODE_SLAVE = 0x80,
    SPI_3WIRE = 0x100,
} SPI_MODE_MASK_t;

#define SPI1 ((SPI_REGISTERS *)BASE_ADDR_SPI)
#define SPI2 ((SPI_REGISTERS *)BASE_ADDR_SPI2)

// #define SPI_CS_AUTO      0
// #define SPI_CS_MANUAL    1

// #define SPI_MASTER       0
// #define SPI_SLAVE        1


enum {
    SPI_CS0 = 0,
    SPI_CS1 = 1,
    SPI_CS2 = 2,
    SPI_CS3 = 3
};

enum {
    SPI_WORD_8BITS  = 7,
    SPI_WORD_9BITS  = 8,
    SPI_WORD_10BITS = 9,
    SPI_WORD_11BITS = 10,
    SPI_WORD_12BITS = 11,
    SPI_WORD_13BITS = 12,
    SPI_WORD_14BITS = 13,
    SPI_WORD_15BITS = 14,
    SPI_WORD_16BITS = 15,
    SPI_WORD_32BITS = 31
};

#define SPI_CS_DELAY_HALF_CLK    0
#define SPI_CS_DELAY_1_CLK       1
#define SPI_CS_DELAY_2_CLK       2
#define SPI_CS_DELAY_3_CLK       3
#define SPI_CS_DELAY_4_CLK       4
#define SPI_CS_DELAY_5_CLK       5
#define SPI_CS_DELAY_6_CLK       6
#define SPI_CS_DELAY_7_CLK       7
#define set_spi_cs_delay(a)      bSPI_DELAY = a

//BUS CLK
enum {
    SPI_CLK_DIV_2    = 0,
    SPI_CLK_DIV_4    = 1,
    SPI_CLK_DIV_8    = 2,
    SPI_CLK_DIV_16   = 3,
    SPI_CLK_DIV_32   = 4,
    SPI_CLK_DIV_64   = 5,
    SPI_CLK_DIV_128  = 6,
    SPI_CLK_DIV_256  = 7,
    SPI_CLK_DIV_512  = 8,
    SPI_CLK_DIV_1024 = 9,
    SPI_CLK_DIV_2048 = 10,
    SPI_CLK_DIV_4096 = 11,
};

#define SPI_CLK_DEFAULT SPI_CLK_DIV_4



#include "hw_spi.h"



//#define SPI_INT_USE

//#define SPI_TX_INT_ENABLE

//#define SPI_RX_INT_ENABLE


#define SPI_DMA_ENABLE

#define SPI_DMA_TX_ENABLE

#define SPI_DMA_RX_ENABLE



/*********************************/
#define SPI_CS0_PIN_INDEX       GPIO0_14_INDEX
#define SPI_CLK_PIN_INDEX       GPIO0_15_INDEX
#define SPI_D0_PIN_INDEX        GPIO0_16_INDEX

#define SPI_D1_PIN_INDEX        GPIO0_28_INDEX
#define SPI_CS1_PIN_INDEX       GPIO0_29_INDEX


/*************spi2******************/
#define SPI_2_CS_PIN_INDEX       		GPIO0_22_INDEX
#define SPI_2_CLK_PIN_INDEX       		GPIO0_21_INDEX
#define SPI_2_MOSI_PIN_INDEX        	GPIO0_20_INDEX
#define SPI_2_MISO_PIN_INDEX        	GPIO0_23_INDEX



// for special spi interface

#define SPI_SPECIAL_INTERFACE


#ifdef SPI1_ENABLE

#define SPI_W_BUSY_PIN        	GPIO0_22_INDEX

#define SPI_R_IRQ_PIN         	GPIO0_24_INDEX

#else

#define SPI_W_BUSY_PIN        	GPIO_UNDEFINE_INDEX

#define SPI_R_IRQ_PIN         	GPIO_UNDEFINE_INDEX

#endif


#define SPI_WRITE_BYSY        	app_gpio_input_level_read(SPI_W_BUSY_PIN)

#define SPI_READ_IRQ          	app_gpio_input_level_read(SPI_R_IRQ_PIN)



void hw_spi_init(SPI_REGISTERS *hspi, uint32_t mode);
void hw_spi_write_byte(SPI_REGISTERS *hspi, uint8_t data);
void hw_spi_write_bytes(SPI_REGISTERS *hspi, uint8_t *buf, uint32_t byte_size);
void hw_spi_write_reg(SPI_REGISTERS *hspi, uint32_t addr, const uint8_t *data, uint32_t length);
void hw_spi_read(SPI_REGISTERS *hspi, uint8_t *buf, uint32_t byte_size);
void hw_spi_read_reg(SPI_REGISTERS *hspi, uint32_t addr, uint8_t *data, uint32_t length);

void hw_spi_close(void);

void hw_spi_mode_set(uint8_t mode);
void hw_spi_cs_active_set(uint8_t con);
void hw_spi_clk_freq_set(uint8_t div);
void hw_spi_bits_set(uint8_t bits);

void spi_transfer_size_set(uint8_t bits, uint8_t cs_select);

void Spi_Write_Bits(uint16_t data);
void Spi_Write_Bytes(uint8_t *buf, uint32_t byte_size);

void Spi_Write_32Bits(uint32_t data);

void Spi_Write(uint8_t *buf, uint32_t bytes_count);
// void Spi_Read(uint8_t *buf, uint32_t bytes_count);

void Spi_Page_Write(uint8_t addr, uint8_t *buf, uint32_t byte_size);

int32_t spi_dma_open(   int32_t tx_ena, 
                        int32_t rx_ena,
                        dma_done_callback *tx_cb, 
                        dma_done_callback * rx_cb);
void spi_dma_tx_test(void);
void spi_dma_tx_callback(void);

#ifdef __cplusplus
}
#endif

#endif //__HW_SPI_H__
