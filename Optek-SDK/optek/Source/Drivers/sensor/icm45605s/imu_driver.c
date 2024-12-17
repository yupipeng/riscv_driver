#include <stdint.h>

#include "imu_driver.h"
#include "inv_imu_defs.h"


// modify functions below or implement new functions in other files

/**
 * @brief set mcu hardware
 * @brief Initializes MCU peripherals.
 * @brief not necessary if initialized in other place
 */
__attribute__((weak)) int imu_setup_mcu(void)
{
	int rc = INV_IMU_OK;

    // hw_spi_init(SPI2, SPI_CS0_USED | SPI_MODE_0 | SPI_MISO_USED);
	// rc |= si_board_init();

	/* Configure GPIO to call `int_cb` when INT1 fires. */
	// rc |= si_init_gpio_int(SI_GPIO_INT1, int_cb);

	/* Init timer peripheral for sleep and get_time */
	// rc |= si_init_timers();

	/* Initialize serial interface between MCU and IMU */
	// rc |= si_io_imu_init(SERIF_TYPE);

	/* Initialize flash storage (NVM) to keep biases from one execution to the other */
	// rc |= si_flash_storage_init();

    return rc;
}
/**
 * @brief read imu register
 * @param reg register address, read address should be 0x80 | reg
 * @param buf data buffer to read
 * @param len data length
 * @return int INV_IMU_OK if success
 */
__attribute__((weak)) int imu_spi_read_reg(uint8_t reg, uint8_t *buf, uint32_t len) {
    reg |= 0x80;
    // soft_i2c_read_reg(&si2c0, 0xd0, reg, buf, len);
    // hw_spi_read_reg(SPI1, 0x80 | reg, buf, len);
	// soft_spi_read_reg(&sspi, 0x80 | reg, buf, len);
    return INV_IMU_ERROR;
}

/**
 * @brief write imu register
 * @param reg register address
 * @param buf data buffer to write
 * @param len data length
 * @return int INV_IMU_OK if success
 */
__attribute__((weak)) int imu_spi_write_reg(uint8_t reg, const uint8_t *buf, uint32_t len) {
    // soft_i2c_write_reg(&si2c0, 0xd0, reg, buf, len);
    // hw_spi_write_reg(SPI1, reg, buf, len);
	// soft_spi_write_reg(&sspi, reg, buf, len);
    return INV_IMU_OK;
}

/**
 * @brief every conversation complete will call this function and pass the data to it
 * @note algorithm in imu return the quaternions, calculate euler angle from quaternions
 * @note euler angle has some critical points, such as gimbal lock
 * @note it will cause some problem when euler angle is near to 90 degrees
 * @param euler_angle euler angle data
 */
__attribute__((weak)) int imu_get_euler_degree(const float euler_angle[3]) {
    // do something with euler angle
    return INV_IMU_OK;
}


/**
 * @brief delay function
 *
 * @param us delay time in us
 */
__attribute__((weak)) void sleep_us(uint32_t us) {
    for (int i = 0; i < us*3; i+=2) {
        __asm__ __volatile__("nop");
    }
}

/**
 * @brief store biases data to flash
 * @param sensor_bias[in] 84byte baises data
 * @return int
 * @retval 0 if success
 */
__attribute__((weak)) int imu_store_biases_data_to_flash(const uint8_t sensor_bias[84])
{
	// finish this function and call imu_store_biases_data to store biases

    return INV_IMU_ERROR;
}
/**
 * @brief read biases data from flash
 *
 * @param sensor_bias[out] 84byte baises data
 * @return int
 * @retval 0 if success
 */
__attribute__((weak)) int imu_read_biases_data_from_flash(uint8_t sensor_bias[84])
{
	// finish this function

    return INV_IMU_ERROR;
}