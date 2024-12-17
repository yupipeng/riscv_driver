# how to import this imu driver

## porting
five function defined in `imu_driver.c`, first three functions have to be defined, and other only when you need
```c
int imu_spi_read_reg(uint8_t reg, uint8_t *buf, uint32_t len) {
    // first bit of register address as 1 is read
    Spi_Page_Read(0x80 | reg, buf, len);
    return 0;
}

int imu_spi_write_reg(uint8_t reg, const uint8_t *buf, uint32_t len) {
    Spi_Page_Write(reg, buf, len);
    return 0;
}

void sleep_us(uint32_t us) {

}

/**
 * @brief every conversation complete will call this function and pass the data to it
 * @param euler_angle euler angle data
 */
int imu_get_euler_degree(const float euler_angle[3]) {
    // do something with euler angle
    print_euler_angle(euler_angle);
    return INV_IMU_OK;
}

/**
 * @brief store biases data to flash
 *
 * @param sensor_bias 84byte baises data
 * @return int
 * @retval 0 if success
 */
int imu_store_biases_data_to_flash(uint8_t sensor_bias[84])
{
	// finish this function and call imu_store_biases_data to store biases

    return 0;
}
/**
 * @brief read biases data from flash
 *
 * @param sensor_bias 84byte baises data
 * @return int
 * @retval 0 if success
 */
int imu_read_biases_data_from_flash(uint8_t sensor_bias[84])
{
	// finish this function

    return 0;
}
```

## usage
four functions below, declared in `imu_driver.h`, need to be called when user needed
```c
/**
 * @brief init imu sensor by given parameters, call it before imu_loop at program start
 *
 */
void imu_init(void);
/**
 * @brief imu loop to get sensor data, call it every 20ms, 10ms or quicker
 *
 * @return int
 * @retval 0 if success
 */
int imu_loop(void);
/**
 * @brief call it when saving biases data to flash
 *
 * @return int
 */
int imu_store_biases_data(void);
/**
 * @brief call it when imu int1 triggered
 *
 */
void imu_int1_triggered(void);
```

function with return value need to return 0 when nothing error, return none-zero when error happened

1. startup
```c
#ifdef I2C3_ENABLE
app_i2c3_init();
#endif

#ifdef SPI1_ENABLE
hw_spi_init();
#endif

#ifdef SPI2_ENABLE
hw_spi_2_init();
#endif

#if defined(ICM45605S_ENABLE) && defined(SPI1_ENABLE)

hal_gpio_mux_func_select(GPIO0_29_INDEX, MUX_SEL_GPIO_INPUT);
hal_gpio_output_level_set(GPIO0_29_INDEX, GPIO_LOW);
imu_init();

#endif
```
at `app_entry` or before `while` of `app_task`, initialize your 3-wire spi, gpio(int1) and call imu_init to initialize imu
the startup will cost some time, for example, 700ms@2M

2. set up gpio detect(or interrupt)
```c
#ifdef KEY_ENABLE
KeyScan(tick);
#endif
#ifdef ICM45605S_ENABLE
extern void imu_int1_triggered(void);
if (hal_gpio_input_level_read(GPIO0_29_INDEX)) {
    imu_int1_triggered();
}
imu_loop();
#endif
```
use gpio scan or gpio interrupt depend on your choice, imu interrupt can be set as latch or pulse mode. latch mode will keep int pin as active until user read out frame. pulse mode will pulse when a frame was prepared. latch mode can be read by normal gpio scan, pulse mode may only be detected by hardware.

once your program detected a interrupt, call `imu_int1_triggered` function to set flag, it make `imu_loop` to receive and compute data.
```c
extern float imu_acc_euler_angle[3];
```

use the variable above to get euler angle, it will be update when interrupt happed, so it not thread-safe. when `imu_loop` is done, just copy these three data.


3. bias storage

program support to storage bias to the flash or other non-volatile storage device, call `imu_store_biases_data` to storage data, bias data will be loaded at startup

it now configure as 3-wire spi, your project need to config a 3-wire spi communication

## misc

```c
#define INV_MSG(level, fmt...) printf(fmt);printf("\r\n");
#define SI_CHECK_RC(x)
#define SERIF_TYPE UI_SPI3
#define IMU_INT_MODE INTX_CONFIG2_INTX_MODE_LATCH  // latch mode will keep the interrupt active until it is cleared by the host
// #define IMU_INT_MODE INTX_CONFIG2_INTX_MODE_PULSE  // pulse mode will generate a pulse when frame is ready

#define SENSOR_CFG_DEFINE SENSOR_CFG_LN100

```

driver use `INV_MSG` to print message to uart, just to `#define` it as empty to close output

driver use `SI_CHECK_RC` to check the return value of operations, you can define this to use your own error checks

`SENSOR_CFG_DEFINE` decide the rate of angle report from imu, `SENSOR_CFG_LN100` means 100 times a second and `SENSOR_CFG_LN50` means 50 times a second, `LN` for low noise and `LP` for low power
`SERIF_TYPE` decide how to communicate with imu, `UI_SPI3` means 3-wire spi
`IMU_INT_MODE` decide how `INT1` show interrupt event, difference show in `##USAGE.2`

## Exmaple

```c

void main(void)
{
    // printf("Hello, World!\n");

    // init spi and int1 pin
    hw_spi_init(SPI1, SPI_CS0_USED | SPI_MODE_0 | SPI_3WIRE);
	hal_gpio_mux_func_select(INT1_PIN, MUX_SEL_GPIO_INPUT);
	hal_gpio_output_level_set(INT1_PIN, GPIO_LOW);

    // init imu
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

        // when imu_int1_triggered is called, imu_loop will do it job
        imu_loop();

        // once it get a full frame of data, it will call
        // imu_get_euler_degree and pass the angle data to it
    }

}

```