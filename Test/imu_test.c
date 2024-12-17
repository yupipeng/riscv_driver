
I2C_StatusTypeDef SOFT_I2C_FUNC(SOFT_I2C_OP_TypeDef operate, uint8_t* param)
{
    switch (operate)
    {
        case SOFT_I2C_GPIO_INIT:
        {
            //i2c clk,it is set to open drain pin
            hal_gpio_mux_func_select(I2C0_SCL_PIN, MUX_SEL_GPIO_INPUT);
            hal_gpio_output_level_set(I2C0_SCL_PIN, GPIO_LOW);

            //i2c data,it is set to open drain pin
            hal_gpio_mux_func_select(I2C0_SDA_PIN, MUX_SEL_GPIO_INPUT);
            hal_gpio_output_level_set(I2C0_SDA_PIN, GPIO_LOW);

        } break;
        case SOFT_I2C_START:
        {

        } break;
        case SOFT_I2C_SDA_IN:
        {

        } break;
        case SOFT_I2C_SDA_OUT:
        {

        } break;
        case SOFT_I2C_SDA_READ:
        {

        } break;
        case SOFT_I2C_SDA_HIGH:
        {

        } break;
        case SOFT_I2C_SDA_LOW:
        {

        } break;
        case SOFT_I2C_SCL_HIGH:
        {

        } break;
        case SOFT_I2C_SCL_LOW:
        {

        } break;
        default:
            return I2C_OPERATE_ERROR;
            break;
    }
    return I2C_OK;
}
SOFT_I2C_HandleTypeDef si2c0 = {
    .iic = SOFT_I2C_FUNC,
    // .sda_pin = hal_gpio_get_pin(0, I2C0_SDA_PIN),
    .sda_pin = I2C0_SDA_PIN,
    .scl_pin = I2C0_SCL_PIN,
    .delay = 100
};

#include "imu_edmp_gaf.h"

/* Static variables for command interface */
static uint8_t print_si; /* Indicates if data should be printed in SI */
static uint8_t print_lsb; /* Indicates if data should be printed in LSB */
static uint8_t accel_en; /* Indicates accel state */
static uint8_t gyro_en; /* Indicates gyro state */
static uint8_t use_ln; /* Indicates which power mode is used (1: LN, 0: LP)*/
static uint8_t discard_accel_samples; /* Indicates how many accel samples should be discarded */
static uint8_t discard_gyro_samples; /* Indicates how many gyro samples should be discarded */

/* Static variables */
static inv_imu_device_t  imu_dev; /* Driver structure */
static volatile int      int1_flag; /* Flag set when INT1 is received */
static volatile uint64_t int1_timestamp; /* Store timestamp when int from IMU fires */
static uint64_t timestamp; /* Store int1_timestamp when IMU IRQ is being processed in main loop */
static inv_imu_edmp_gaf_outputs_t gaf_outputs; /* Outputs obtained from GAF running on EDMP */

static inv_imu_sensor_data_t d; /* Store raw data read from sensor registers */
static uint8_t               fifo_data[FIFO_MIRRORING_SIZE]; /* Memory where to store FIFO data */

/* Static variables for command interface */
static sensor_cfg_t sensor_cfg; /* Contains current configuration */
static uint8_t      accel_en; /* Indicates accel state */
static uint8_t      gyro_en; /* Indicates gyro state */

/* GAF biases */
static int32_t acc_bias_q16[3];
static int32_t gyr_bias_q16[3];
static int8_t  gyr_bias_accuracy;

/* Mounting matrix applied to raw data and to EDMP Calibration module */
static const int16_t mmatrix[9] = {
	/* Identity matrix in Q14 */
	(1 << 14), 0, 0, 0, (1 << 14), 0, 0, 0, (1 << 14),
};
int imu_iic_read_reg(uint8_t reg, uint8_t *buf, uint32_t len) {
    soft_i2c_read_reg(&si2c0, 0xd0, reg, buf, len);
    return 0;
}

int imu_iic_write_reg(uint8_t reg, const uint8_t *buf, uint32_t len) {
    soft_i2c_write_reg(&si2c0, 0xd0, reg, buf, len);
    return 0;
}

void sleep_us(uint32_t us) {
    for (int i = 0; i < us*3; i+=2) {
        __asm__ __volatile__("nop");
    }
}

int imu_setup(inv_imu_device_t *imu_dev) {
    int rc = 0;
	accel_en  = 1;
	gyro_en   = 1;
	use_ln    = 1;


	/* Set FSR */
	rc |= inv_imu_set_accel_fsr(imu_dev, ACCEL_CONFIG0_ACCEL_UI_FS_SEL_4_G);
	rc |= inv_imu_set_gyro_fsr(imu_dev, GYRO_CONFIG0_GYRO_UI_FS_SEL_2000_DPS);

    DBG_Assert(rc != 0);

	/* Set ODR */
	rc |= inv_imu_set_accel_frequency(imu_dev, ACCEL_CONFIG0_ACCEL_ODR_50_HZ);
	rc |= inv_imu_set_gyro_frequency(imu_dev, GYRO_CONFIG0_GYRO_ODR_50_HZ);

    DBG_Assert(rc != 0);

	/* Set BW = ODR/4 */
	rc |= inv_imu_set_accel_ln_bw(imu_dev, IPREG_SYS2_REG_131_ACCEL_UI_LPFBW_DIV_4);
	rc |= inv_imu_set_gyro_ln_bw(imu_dev, IPREG_SYS1_REG_172_GYRO_UI_LPFBW_DIV_4);

    DBG_Assert(rc != 0);

	/* Sensor registers are not available in ULP, so select RCOSC clock to use LP mode. */
	rc |= inv_imu_select_accel_lp_clk(imu_dev, SMC_CONTROL_0_ACCEL_LP_CLK_RCOSC);

    DBG_Assert(rc != 0);

	/* Set power modes */
	if (use_ln) {
		if (accel_en)
			rc |= inv_imu_set_accel_mode(imu_dev, PWR_MGMT0_ACCEL_MODE_LN);
		if (gyro_en)
			rc |= inv_imu_set_gyro_mode(imu_dev, PWR_MGMT0_GYRO_MODE_LN);
	} else {
		if (accel_en)
			rc |= inv_imu_set_accel_mode(imu_dev, PWR_MGMT0_ACCEL_MODE_LP);
		if (gyro_en)
			rc |= inv_imu_set_gyro_mode(imu_dev, PWR_MGMT0_GYRO_MODE_LP);
	}

	/* Discard N samples at 50Hz to ignore samples at sensor enabling time */
	if (accel_en)
		discard_accel_samples = (ACC_STARTUP_TIME_US / 20000) + 1;
	if (gyro_en)
		discard_gyro_samples = (GYR_STARTUP_TIME_US / 20000) + 1;

    DBG_Assert(rc != 0);

    return rc;
}

char str_buf[64];

void imu_test(void) {
    inv_imu_device_t imu_dev;
    inv_imu_sensor_data_t imu_data;
    float                 accel_g[3];
    float                 gyro_dps[3];
    float                 temp_degc;
    int rc = 0;
    uint8_t whoami = 0x00;
	inv_imu_adv_var_t *      e = (inv_imu_adv_var_t *)imu_dev.adv_var;

    imu_dev.transport.read_reg = imu_iic_read_reg;
    imu_dev.transport.write_reg = imu_iic_write_reg;
    imu_dev.transport.sleep_us = sleep_us;
    imu_dev.transport.serif_type = SERIF_TYPE;
	/* Init sensor event callback */
	e->sensor_event_cb = sensor_event_cb;

    // sleep for 3ms;
    soft_i2c_init(&si2c0);


    rc |= inv_imu_get_who_am_i(&imu_dev, &whoami);
    DBG_Assert(rc != 0);
    if (whoami != INV_IMU_WHOAMI) {
        printf("imu whoami error\r\n");
        printf("- read   whoami = %x\r\n", whoami);
        printf("- expect whoami = %x\r\n", INV_IMU_WHOAMI);
    } else {
        printf("imu whoami success\r\n");
        printf("- whoami = %x\r\n", whoami);
    }

    // char* string_buffer = malloc(128);
    char string_buffer[128];

    // soft-reset
    inv_imu_soft_reset(&imu_dev);
    DBG_Assert(rc != 0);


    rc |= imu_setup(&imu_dev);
    DBG_Assert(rc != 0);
    double gx = 0, gy = 0, gz = 0;


	/* Configure sensors */
	rc |= set_sensor_config(SENSOR_CFG_LN100);
	// rc |= configure_and_enable_edmp_gaf();
	inv_imu_sensor_event_t   event  = { 0 };
    event.sensor_mask = (1 << INV_SENSOR_ES0);

    sleep_us(100*1000);
    int i = 0;


	do {
		/* Poll device for data */
        inv_imu_int_state_t int_state;
			/* Read interrupt status */
        rc |= inv_imu_get_int_status(&imu_dev, INV_IMU_INT1, &int_state);
        DBG_Assert(rc != 0);

        /* DRDY is set, end-user requested to read sensor data from IMU registers */
        if (int_state.INV_UI_DRDY) {
            uint8_t input_mask = 0;

            rc |= inv_imu_get_register_data(&imu_dev, &imu_data);
            DBG_Assert(rc != 0);

            apply_mounting_matrix(mmatrix, d.accel_data);
            apply_mounting_matrix(mmatrix, d.gyro_data);
            if (accel_en || gyro_en) {
                if (accel_en && (d.accel_data[0] != INVALID_VALUE_FIFO) &&
                    (d.accel_data[1] != INVALID_VALUE_FIFO) &&
                    (d.accel_data[2] != INVALID_VALUE_FIFO))
                    input_mask |= MASK_NOTIFY_RAW_ACC_DATA;

                if (gyro_en && (d.gyro_data[0] != INVALID_VALUE_FIFO) &&
                    (d.gyro_data[1] != INVALID_VALUE_FIFO) &&
                    (d.gyro_data[2] != INVALID_VALUE_FIFO))
                    input_mask |= MASK_NOTIFY_RAW_GYR_DATA;

                notify_raw_data(timestamp, input_mask, d.accel_data, d.gyro_data, d.temp_data);
            }
        }

        if (int_state.INV_FIFO_THS) {
            uint16_t fifo_count;
            rc |= inv_imu_adv_get_data_from_fifo(&imu_dev, fifo_data, &fifo_count);
            DBG_Assert(rc != 0);
            rc |= inv_imu_adv_parse_fifo_data(&imu_dev, fifo_data, fifo_count);
            DBG_Assert(rc != 0);

        }
        sensor_event_cb(&event);

        printf("loop:%d\r\n", i++);
		// rc |= check_received_command();
	} while (rc == 0);


    while (1) {

        rc |= inv_imu_get_register_data(&imu_dev, &imu_data);
        DBG_Assert(rc != 0);

        /*
        * Accel and gyro data are coded as 16-bits signed (max_lsb = 2^(16-1) = 32768) with
        * the configured FSR (4 g and 2000 dps, see `setup_imu()` function).
        * Temperature is coded as 16-bits signed with a scale factor of 128 and an offset
        * of 25 °C.
        */
        accel_g[0]  = (float)(imu_data.accel_data[0] * 4 /* gee */) / 32768;
        accel_g[1]  = (float)(imu_data.accel_data[1] * 4 /* gee */) / 32768;
        accel_g[2]  = (float)(imu_data.accel_data[2] * 4 /* gee */) / 32768;
        gyro_dps[0] = (float)(imu_data.gyro_data[0] * 2000 /* dps */) / 32768;
        gyro_dps[1] = (float)(imu_data.gyro_data[1] * 2000 /* dps */) / 32768;
        gyro_dps[2] = (float)(imu_data.gyro_data[2] * 2000 /* dps */) / 32768;
        temp_degc   = (float)25 + ((float)imu_data.temp_data / 128);

        gx += gyro_dps[0]*0.1;
        gy += gyro_dps[1]*0.1;
        gz += gyro_dps[2]*0.1;
        // int gyro_x = (int)(gx * 180)*100;
        // int gyro_y = (int)(gy * 180)*100;
        // int gyro_z = (int)(gz * 180)*100;
        // int temp = (int)(temp_degc * 100);
        // printf ("gyro%d:%f,%f,%f\n", i++, gx, gy, gz);
        int len = snprintf(string_buffer, 128, "gyro%d:%f,%f,%f\n", i++, gx, gy, gz);
        uart_write(UART0, string_buffer, len);
        // printf("temp: %d\tpi:%f\n", temp, 3.1415926);
        // printf("gyro_x: %d\tgyro_y: %d\tgyro_z: %d\n", gyro_x, gyro_y, gyro_z);
        // sleep_us(100*1000);
    }

}