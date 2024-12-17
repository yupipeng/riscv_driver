#ifndef __SOFT_SPI_H__
#define __SOFT_SPI_H__

#include <stdint.h>

typedef struct {
    uint8_t cpha : 1;  // write/read at (0: first 1: second) edge
    uint8_t cpol : 1;  // clk idle polarity (0: low 1: high)
    uint8_t cs_active : 1; // cs active level (0: low 1: high)
    uint8_t spi_line : 1;  // 3-wire mode (0: 4-wire 1: 3-wire)
    uint32_t cs;
    uint32_t sck;
    uint32_t sda;
    uint32_t miso;
} SOFT_SPI_HandleTypeDef;

#define SOFT_SPI_CS_ACTIVE_LEVEL 0
#define SOFT_SPI_CPOL 0
#define SOFT_SPI_CPHA 0
// #define SOFT_SPI_3WIRE

void soft_spi_cs_active(SOFT_SPI_HandleTypeDef *sspi);
void soft_spi_cs_idle(SOFT_SPI_HandleTypeDef *sspi);
void soft_spi_clk_idle(SOFT_SPI_HandleTypeDef *sspi);
void soft_spi_clk_active(SOFT_SPI_HandleTypeDef *sspi);
void soft_spi_gpio_init(SOFT_SPI_HandleTypeDef* sspi);
void soft_spi_write_byte(SOFT_SPI_HandleTypeDef *sspi, uint8_t byte);
void soft_spi_read_byte(SOFT_SPI_HandleTypeDef *sspi, uint8_t *byte);
void soft_spi_write_bytes(SOFT_SPI_HandleTypeDef *sspi, uint8_t *buf, uint32_t length);
void soft_spi_write_reg(SOFT_SPI_HandleTypeDef *sspi, uint8_t addr, const uint8_t *buf, uint32_t length);
void soft_spi_read_reg(SOFT_SPI_HandleTypeDef *sspi, uint8_t addr, uint8_t *buf, uint32_t length);
uint8_t soft_spi_readwrite_byte(SOFT_SPI_HandleTypeDef *sspi, uint8_t tx_data);

#endif // __SOFT_SPI_H__
