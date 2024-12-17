# 如何导入这个IMU驱动到项目中

## 移植
`imu_driver.c` 中定义了六个函数，前面三个必须定义，后面三个根据需求选择性定义
```c
int imu_spi_read_reg(uint8_t reg, uint8_t *buf, uint32_t len) {
    // 寄存器地址的第一个bit为1，表示为读
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

## 使用
下面四个函数在`imu_driver.h`中被声明，用户需要在恰当的位置进行调用
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
带有返回值的函数，没有错误返回0，有错误返回非0

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
在`app_entry`函数中或者在`app_task`函数的`while`之前，初始化3-wire spi和int1对应的gpio，然后调用`imu_init`初始化陀螺仪。初始化过程比较费时，比如，spi速率2M时需要耗费700ms

2. 设置 int1 的检测
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
使用gpio扫描或者gpio中断取决于你如何配置项目，imu的中断可以被设置成latch或者pulse模式，latch模式会一直使int1激活，直到用户读取一帧数据，而pulse模式会在一帧数据准备好后就立刻触发。latch模式可以使用gpio扫描进行读取，而pulse模式必须通过硬件中断才能够正确响应。
一旦你的程序检测到了一个中断，就需要调用`imu_int1_triggered`去设置标志，它会让`imu_loop`执行接受数据和处理数据的代码

```c
extern float imu_acc_euler_angle[3];
```
使用上面的变量获取欧拉角，在中断触发时，它会立刻更新，所以线程是不安全的，你需要在`imu_loop`函数调用结束后立刻拷贝这个数值用于计算


3. bias storage

程序支持存储偏移量到flash或者其他非易失性存储设备中，通过调用`imu_store_biases_data`函数保存偏移量，偏移量会在`imu_init`中被读取使用

## misc

```c
#define INV_MSG(level, fmt...) printf(fmt);printf("\r\n");
#define SI_CHECK_RC(x)
```

当使用`INV_MES`来打印消息到uart的时候，为空关闭输出
使用`SI_CHECK_RC`处理返回的错误，为空不对异常值进行处理


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