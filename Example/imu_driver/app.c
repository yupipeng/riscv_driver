#include <stdio.h>
#include <stdlib.h>

#include "soft_spi.h"
#include "hw_spi.h"
#include "hal_gpio.h"
#include "hw_uart.h"



#define INT1_PIN         GPIO0_29_INDEX

#define GPIO_CS_PIN      GPIO0_14_INDEX
#define GPIO_SCK_PIN     GPIO0_15_INDEX
#define GPIO_MOSI_PIN    GPIO0_16_INDEX
#define GPIO_MISO_PIN    GPIO0_18_INDEX

SOFT_SPI_HandleTypeDef sspi = {
    .cpha = 1,
    .cpol = 1,
    .cs_active = 0,
    .spi_line = 1,
    .cs = GPIO_CS_PIN,
    .sck = GPIO_SCK_PIN,
    .sda = GPIO_MOSI_PIN,
    .miso = GPIO_MISO_PIN,
};

#include "imu_driver.h"

void print_euler_angle(const float euler_angle[3]) {
	int d0 = (int)((-euler_angle[0]) * 100);
	int d1 = (int)((-euler_angle[1]) * 100);
	int d2 = (int)((-euler_angle[2]) * 100);
	int d01 = d0 / 100;
	int d02 = d0 % 100;
	int d11 = d1 / 100;
	int d12 = d1 % 100;
	int d21 = d2 / 100;
	int d22 = d2 % 100;
	if (d02 < 0){
		d02 = -d02;
	}
	if (d12 < 0){
		d12 = -d12;
	}
	if (d22 < 0){
		d22 = -d22;
	}
    printf("%d.%d,%d.%d,%d.%d\n", d01, d02, d11, d12, d21, d22);
}

void app_main(void)
{

    printf("%s", __TIME__);

	// soft_spi_gpio_init(&sspi);
    // hw_spi_init2();
    // init spi
    hw_spi_init(SPI1, SPI_CS0_USED | SPI_MODE_0 | SPI_3WIRE);
    // init gpio int
	hal_gpio_mux_func_select(INT1_PIN, MUX_SEL_GPIO_INPUT);
	hal_gpio_output_level_set(INT1_PIN, GPIO_LOW);

    imu_init();

    while (1)
    {
        // in latch mode, if int1 pin is high, call imu_int1_triggered
        if (GPIO_READ(INT1_PIN)) {
            imu_int1_triggered();
            // printf("INT1 triggered\n");
        }
        // better to use hardware interrupt
        // must use hardware interrupt in pulse mode

        imu_loop();
        // printf("Hello, World!\n");
    }

}
int imu_get_euler_degree(const float euler_angle[3]) {
    // hal_gpio_output_level_set(GPIO_TEST_PIN, GPIO_HIGH);
    print_euler_angle(euler_angle);
    // hal_gpio_output_level_set(GPIO_TEST_PIN, GPIO_LOW);
    return INV_IMU_OK;
}

int imu_spi_read_reg(uint8_t reg, uint8_t *buf, uint32_t len) {
    // soft_i2c_read_reg(&si2c0, 0xd0, reg, buf, len);
    hw_spi_read_reg(SPI1, 0x80 | reg, buf, len);
    // Spi_Page_Read(0x80 | reg, buf, len);
	// soft_spi_read_reg(&sspi, 0x80 | reg, buf, len);
    return INV_IMU_OK;
}

int imu_spi_write_reg(uint8_t reg, const uint8_t *buf, uint32_t len) {
    // soft_i2c_write_reg(&si2c0, 0xd0, reg, buf, len);
    hw_spi_write_reg(SPI1, reg, buf, len);
    // Spi_Page_Write(reg, buf, len);
	// soft_spi_write_reg(&sspi, reg, buf, len);
    return INV_IMU_OK;
}

void sleep_us(uint32_t us) {
    for (int i = 0; i < us*11; i+=2) {
        __asm__ __volatile__("nop");
    }
}

/**
 * @brief store biases data to flash
 *
 * @param sensor_bias 84byte baises data
 * @return int
 * @retval INV_IMU_OK if success
 */
int imu_store_biases_data_to_flash(uint8_t sensor_bias[84])
{
	// finish this function and call imu_store_biases_data to store biases

    return INV_IMU_OK;
}
/**
 * @brief read biases data from flash
 *
 * @param sensor_bias 84byte baises data
 * @return int
 * @retval INV_IMU_OK if success
 */
int imu_read_biases_data_from_flash(uint8_t sensor_bias[84])
{
	// finish this function

    return INV_IMU_OK;
}
