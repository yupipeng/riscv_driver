#ifndef __SOFT_I2C_H__
#define __SOFT_I2C_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "hw_gpio.h"
#include <stdint.h>

typedef enum {
    I2C_OK = 0,
    I2C_ERROR = 1,
    I2C_BUSY = 2,
    I2C_TIMEOUT = 3,

    I2C_OPERATE_ERROR
} I2C_StatusTypeDef;

typedef enum {
    I2C_CLK_50KHZ,
    I2C_CLK_100KHZ,
    I2C_CLK_150KHZ,
    I2C_CLK_200KHZ,
    I2C_CLK_400KHZ,
    I2C_CLK_1MHZ
} I2C_ClockSpeedTypeDef;

typedef enum {
    SOFT_I2C_GPIO_INIT,
    SOFT_I2C_START,
    SOFT_I2C_SDA_IN,
    SOFT_I2C_SDA_OUT,
    SOFT_I2C_SDA_READ,
    SOFT_I2C_SDA_HIGH,
    SOFT_I2C_SDA_LOW,
    SOFT_I2C_SCL_HIGH,
    SOFT_I2C_SCL_LOW
} SOFT_I2C_OP_TypeDef;

typedef void(*HandleFunTypeDef)(void);
typedef uint8_t(*SOFT_I2C_HandleFunTypeDef)(SOFT_I2C_OP_TypeDef, uint8_t*);

typedef struct {
    SOFT_I2C_HandleFunTypeDef iic;
    uint8_t iic_addr;
    uint32_t sda_pin;
    uint32_t scl_pin;
    uint32_t delay;
} SOFT_I2C_HandleTypeDef;

#define I2C_Retries 1


I2C_StatusTypeDef soft_i2c_init(SOFT_I2C_HandleTypeDef *hi2c);
I2C_StatusTypeDef soft_i2c_deinit(SOFT_I2C_HandleTypeDef *hi2c);

I2C_StatusTypeDef soft_i2c_start(SOFT_I2C_HandleTypeDef *si2c);
I2C_StatusTypeDef soft_i2c_write_byte(SOFT_I2C_HandleTypeDef *si2c, uint8_t data);
uint8_t soft_i2c_read_byte(SOFT_I2C_HandleTypeDef *si2c, BOOL con);
I2C_StatusTypeDef soft_i2c_write(SOFT_I2C_HandleTypeDef *hi2c, uint8_t dev_addr, const uint8_t *data, uint32_t len);
I2C_StatusTypeDef soft_i2c_read(SOFT_I2C_HandleTypeDef *hi2c, uint8_t dev_addr, uint8_t *data, uint32_t len);
I2C_StatusTypeDef soft_i2c_write_reg(SOFT_I2C_HandleTypeDef *hi2c, uint8_t dev_addr, uint8_t reg_addr, const uint8_t *data, uint32_t len);
I2C_StatusTypeDef soft_i2c_read_reg(SOFT_I2C_HandleTypeDef *hi2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint32_t len);

I2C_StatusTypeDef soft_i2c_scan(SOFT_I2C_HandleTypeDef *si2c);

#define I2C0_SCL_PIN GPIO0_10_INDEX
#define I2C0_SDA_PIN GPIO0_11_INDEX


#define I2C1_SCLK_PIN     GPIO0_06_INDEX//GPIO0_00_INDEX//GPIO0_19_INDEX
#define I2C1_SDATA_PIN    GPIO0_07_INDEX//GPIO0_19_INDEX//GPIO0_00_INDEX

#define I2C2_SCLK_PIN     GPIO0_04_INDEX//GPIO0_00_INDEX//GPIO0_19_INDEX
#define I2C2_SDATA_PIN    GPIO0_05_INDEX//GPIO0_19_INDEX//GPIO0_00_INDEX

#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
// template <class T>
// I2C_StatusTypeDef soft_i2c_write(I2C_HandleTypeDef *hi2c, uint8_t dev_addr, T data) {
//     return soft_i2c_write(hi2c, dev_addr, (uint8_t *)&data, sizeof(T));
// }
#endif

#endif // __SOFT_I2C_H__