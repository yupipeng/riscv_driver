/*
 *
 * Copyright (c) [2020] by InvenSense, Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

/* Frontend */
// #include "frontend.h"


#include "imu_edmp_gaf.h"
/*
 * This example showcases how to configure and use GAF in eDMP.
 * It enables accelerometer and gyroscope by default in Low-Noise mode at 100Hz.
 * It loads GAF RAM image and starts eDMP to output GAF values.
 * There is also capability to get and print raw data from sensor registers reading.
 */

/* Static variables */
static inv_imu_device_t  imu_dev; /* Driver structure */
static volatile int      int1_flag; /* Flag set when INT1 is received */
static volatile uint64_t int1_timestamp; /* Store timestamp when int from IMU fires */
static uint64_t timestamp; /* Store int1_timestamp when IMU IRQ is being processed in main loop */
static inv_imu_edmp_gaf_outputs_t gaf_outputs; /* Outputs obtained from GAF running on EDMP */

static inv_imu_sensor_data_t d; /* Store raw data read from sensor registers */
static uint8_t      fifo_data[FIFO_MIRRORING_SIZE]; /* Memory where to store FIFO data */

/* Static variables for command interface */
static sensor_cfg_t sensor_cfg; /* Contains current configuration */
static uint8_t      accel_en; /* Indicates accel state */
static uint8_t      gyro_en; /* Indicates gyro state */
static uint8_t      raw_data_enable;

/* GAF biases */
static int32_t acc_bias_q16[3];
static int32_t gyr_bias_q16[3];
static int8_t  gyr_bias_accuracy;

/* Mounting matrix applied to raw data and to EDMP Calibration module */
static const int16_t mmatrix[9] = {
	/* Identity matrix in Q14 */
	(1 << 14), 0, 0, 0, (1 << 14), 0, 0, 0, (1 << 14),
};

/* Static functions definition */
static int  setup_imu();
void sensor_event_cb(inv_imu_sensor_event_t *event);
static int configure_and_enable_edmp_gaf();
void int_cb(void *context, unsigned int int_num);
static int set_sensor_config(sensor_cfg_options_t opt);
int  on_command_received_cb(inv_edmp_gaf_commands cmd);
static void apply_mounting_matrix(const int16_t matrix[9], int16_t raw[3]);
static int init_imu_biases(void);
int store_imu_biases(inv_imu_edmp_gaf_outputs_t *gaf_outputs);
static void fixedpoint_to_float(const int32_t *in, float *out, const uint8_t fxp_shift,
                                const uint8_t dim);
static void quaternions_to_angles(const float quat[4], float angles[3]);

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*****************external user defined functions******************/
extern int imu_spi_read_reg(uint8_t reg, uint8_t *buf, uint32_t len);
extern int imu_spi_write_reg(uint8_t reg, const uint8_t *buf, uint32_t len);
extern int imu_store_biases_data_to_flash(uint8_t sensor_bias[84]);
extern int imu_read_biases_data_from_flash(uint8_t sensor_bias[84]);
extern int imu_setup_mcu(void);
extern void sleep_us(uint32_t us);

// this variable is used to keep update when multiple frame data is received
// process one frame data then ignore the rest frame data
uint8_t imu_single_trans_compelete = 0;

void notify_data(uint64_t time, const inv_edmp_gaf_outputs_t *gaf_outputs)
{
	float grv_quat[4], angles_deg_grv[3];
	float acc_g[3];
	float gyr_dps[3], gyr_bias[3];
	float temp;

	fixedpoint_to_float(gaf_outputs->grv_quat_q30, grv_quat, 30, 4);
	quaternions_to_angles(grv_quat, angles_deg_grv);
	INV_MSG(INV_MSG_LEVEL_INFO, "%lld: OUTPUT 6Axis=[%f, %f, %f, %f]", time, grv_quat[0],
	        grv_quat[1], grv_quat[2], grv_quat[3]);
	INV_MSG(INV_MSG_LEVEL_INFO, "%lld: OUTPUT Euler6Axis=[%.2f, %.2f, %.2f]deg", time,
	        angles_deg_grv[0], angles_deg_grv[1], angles_deg_grv[2]);

	fixedpoint_to_float(gaf_outputs->acc_cal_q16, acc_g, 16, 3);
	INV_MSG(INV_MSG_LEVEL_INFO, "%lld: OUTPUT Acc=[%.3f, %.3f, %.3f]g", time, acc_g[0], acc_g[1],
	        acc_g[2]);

	fixedpoint_to_float(gaf_outputs->gyr_cal_q16, gyr_dps, 16, 3);
	fixedpoint_to_float(gaf_outputs->gyr_bias_q16, gyr_bias, 16, 3);
	INV_MSG(INV_MSG_LEVEL_INFO,
	        "%lld: OUTPUT Gyr=[%.3f, %.3f, %.3f]dps GyrBias=[%.3f, %.3f, %.3f]dps Accuracy=%d",
	        time, gyr_dps[0], gyr_dps[1], gyr_dps[2], gyr_bias[0], gyr_bias[1], gyr_bias[2],
	        (int32_t)gaf_outputs->gyr_accuracy_flag);

	fixedpoint_to_float(&gaf_outputs->temp_degC_q16, &temp, 16, 1);
	INV_MSG(INV_MSG_LEVEL_INFO, "%lld: OUTPUT Temp=%.2f C", time, temp);

}

void notify_raw_data(uint64_t time, uint8_t input_mask, const int16_t accel_data[3],
					const int16_t gyro_data[3], const int16_t temp_data)
{
	if (input_mask & MASK_NOTIFY_RAW_ACC_DATA){
		/* IMU data */
		INV_MSG(INV_MSG_LEVEL_INFO, "%lld: OUTPUT RAcc=[%d, %d, %d]", time, (uint32_t)accel_data[0],
				(uint32_t)accel_data[1], (uint32_t)accel_data[2]);
	}
	if (input_mask & MASK_NOTIFY_RAW_GYR_DATA){
		/* IMU data */
		INV_MSG(INV_MSG_LEVEL_INFO, "%lld: OUTPUT RGyr=[%d, %d, %d]", time, (uint32_t)gyro_data[0],
				(uint32_t)gyro_data[1], (uint32_t)gyro_data[2]);
	}
	INV_MSG(INV_MSG_LEVEL_INFO, "%lld: OUTPUT RTemp=%d", time, (uint32_t)temp_data);
}


void imu_init(void)
{

	int rc = 0;

	accel_en = IMU_ACCEL_EN;
	gyro_en  = IMU_GYRO_EN;
	raw_data_enable = IMU_RAW_DATA_EN;

	rc |= imu_setup_mcu();
	SI_CHECK_RC(rc);

	INV_MSG(INV_MSG_LEVEL_INFO, "###");
	INV_MSG(INV_MSG_LEVEL_INFO, "### EDMP GAF START");
	INV_MSG(INV_MSG_LEVEL_INFO, "###");

	/* Retrieve from NVM backend */
	rc |= init_imu_biases();
	SI_CHECK_RC(rc);

	rc |= setup_imu();
	SI_CHECK_RC(rc);

#if defined(IMU_WHOAMI_CHECK)
	uint8_t whoami[10];
	inv_imu_get_who_am_i(&imu_dev, whoami);

	if (whoami[0] != INV_IMU_WHOAMI) {
		printf("imu whoami error\r\n");
		printf("- read   whoami = %x\r\n", whoami[0]);
		printf("- expect whoami = %x\r\n", INV_IMU_WHOAMI);
	} else {
		printf("imu whoami success\r\n");
		printf("- whoami = %x\r\n", whoami[0]);
	}

	while (1);
#endif

	/* Configure sensors */
	rc |= set_sensor_config(SENSOR_CFG_DEFINE);
	rc |= configure_and_enable_edmp_gaf();
	SI_CHECK_RC(rc);

	/* Reset timestamp and interrupt flag */
	int1_flag      = 0;
	int1_timestamp = 0;
}

void imu_int1_triggered(void)
{
	int1_flag = 1;
}

/* Main function implementation */
int imu_loop(void)
{
	int rc = 0;
	/* Poll device for data */

	if (int1_flag) {
		inv_imu_int_state_t int_state;
		intx_statusx_t	  int_statusx;
		// si_disable_irq();
		/* Clear interrupt flag */
		int1_flag = 0;
		/* Retrieve timestamp */
		// timestamp = int1_timestamp;
		// si_enable_irq();

		/* Read interrupt status */
#if IMU_RAW_DATA_EN == 1
		rc |= inv_imu_get_int_status(&imu_dev, INV_IMU_INT1, &int_state);
		SI_CHECK_RC(rc);

		/* DRDY is set, end-user requested to read sensor data from IMU registers */
		if (raw_data_enable && int_state.INV_UI_DRDY) {
			uint8_t input_mask = 0;

			rc |= inv_imu_get_register_data(&imu_dev, &d);
			SI_CHECK_RC(rc);

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
#else
		rc |= inv_imu_read_reg(&imu_dev, INT1_STATUS0, 2, (uint8_t *)&int_statusx);
		SI_CHECK_RC(rc);
		if (int_statusx.int1_status0.int1_status_fifo_ths) {
#endif
			uint16_t fifo_count;
			rc |= inv_imu_adv_get_data_from_fifo(&imu_dev, fifo_data, &fifo_count);
			// printf("fifo_count: %d\n", fifo_count);
			imu_single_trans_compelete = 0;
			SI_CHECK_RC(rc);
			rc |= inv_imu_adv_parse_fifo_data(&imu_dev, fifo_data, fifo_count);
			SI_CHECK_RC(rc);
		}
	}
	return rc;
}

/* Initializes IMU device and apply configuration. */
static int setup_imu()
{
	int                      rc = 0;
	inv_imu_int_pin_config_t int_pin_config;
	inv_imu_adv_var_t *      e = (inv_imu_adv_var_t *)imu_dev.adv_var;

	/* Init transport layer */
	imu_dev.transport.read_reg   = imu_spi_read_reg;
	imu_dev.transport.write_reg  = imu_spi_write_reg;
	imu_dev.transport.serif_type = SERIF_TYPE;
	imu_dev.transport.sleep_us   = sleep_us;

	/* Init sensor event callback */
	e->sensor_event_cb = sensor_event_cb;

	/* Wait 3 ms to ensure device is properly supplied  */
	sleep_us(3000);

	/* In SPI, configure slew-rate to prevent bus corruption on DK-SMARTMOTION-REVG */
	if (imu_dev.transport.serif_type == UI_SPI3 || imu_dev.transport.serif_type == UI_SPI4) {
		drive_config0_t drive_config0;
		drive_config0.pads_spi_slew = DRIVE_CONFIG0_PADS_SPI_SLEW_TYP_10NS;
		rc |= inv_imu_write_reg(&imu_dev, DRIVE_CONFIG0, 1, (uint8_t *)&drive_config0);
		SI_CHECK_RC(rc);
		sleep_us(2); /* Takes effect 1.5 us after the register is programmed */
	}

	rc |= inv_imu_adv_init(&imu_dev);
	SI_CHECK_RC(rc);

	/*
	 * Configure interrupts pins
	 * - Polarity High
	 * - Pulse mode
	 * - Push-Pull drive
	 */
	int_pin_config.int_polarity = INTX_CONFIG2_INTX_POLARITY_HIGH;
	int_pin_config.int_mode     = IMU_INT_MODE;
	int_pin_config.int_drive    = INTX_CONFIG2_INTX_DRIVE_PP;
	rc |= inv_imu_set_pin_config_int(&imu_dev, INV_IMU_INT1, &int_pin_config);
	SI_CHECK_RC(rc);

	return rc;
}

/* FIFO sensor event callback */
void sensor_event_cb(inv_imu_sensor_event_t *event)
{
	if (event->sensor_mask & (1 << INV_SENSOR_ES0)) {
		int rc;

		rc = inv_imu_edmp_gaf_build_outputs(&imu_dev, (const uint8_t *)event->es0, &gaf_outputs);

		if (rc == -1) {
			INV_MSG(INV_MSG_LEVEL_ERROR,
			        "Error when rebuilding GAF output, unknown FIFO frame received");
			sleep_us(10 * 1000 * 1000);
		} else if (rc == 1) {
			inv_edmp_gaf_outputs_t edmp_outputs;

#if IMU_RAW_DATA_EN == 1
			/* Apply accel biases that were given as input to EDMP since it won't recompute any and use them directly */
			edmp_outputs.acc_cal_q16[0] =
			    ((int32_t)d.accel_data[0] * RAW_ACC_SCALE) - acc_bias_q16[0];
			edmp_outputs.acc_cal_q16[1] =
			    ((int32_t)d.accel_data[1] * RAW_ACC_SCALE) - acc_bias_q16[1];
			edmp_outputs.acc_cal_q16[2] =
			    ((int32_t)d.accel_data[2] * RAW_ACC_SCALE) - acc_bias_q16[2];
#endif
			memcpy(edmp_outputs.grv_quat_q30, gaf_outputs.grv_quat_q30,
			       sizeof(gaf_outputs.grv_quat_q30));

#if IMU_RAW_DATA_EN == 1
			edmp_outputs.gyr_cal_q16[0] =
			    (int32_t)d.gyro_data[0] * RAW_GYR_SCALE - gaf_outputs.gyr_bias_q16[0];
			edmp_outputs.gyr_cal_q16[1] =
			    (int32_t)d.gyro_data[1] * RAW_GYR_SCALE - gaf_outputs.gyr_bias_q16[1];
			edmp_outputs.gyr_cal_q16[2] =
			    (int32_t)d.gyro_data[2] * RAW_GYR_SCALE - gaf_outputs.gyr_bias_q16[2];
			memcpy(edmp_outputs.gyr_bias_q16, gaf_outputs.gyr_bias_q16,
			       sizeof(gaf_outputs.gyr_bias_q16));
#endif
			edmp_outputs.gyr_accuracy_flag = gaf_outputs.gyr_accuracy_flag;
			edmp_outputs.stationary_flag   = gaf_outputs.stationary_flag;
			edmp_outputs.temp_degC_q16 =
			    (25 * (1UL << 16)) +
			    ((int32_t)d.temp_data * 512); /* 25 + (raw_temp / 128) converted to q16 */

#if defined(IMU_NOTIFY_DATA)
			notify_data(timestamp, &edmp_outputs);
#else
			float grv_quat[4], angles_deg_grv[3];
			fixedpoint_to_float(edmp_outputs.grv_quat_q30, grv_quat, 30, 4);
			quaternions_to_angles(grv_quat, angles_deg_grv);
			extern int imu_get_euler_degree(const float euler_angle[3]);
			imu_get_euler_degree(angles_deg_grv);
#endif
			imu_single_trans_compelete = 1;
		}
	} else {
		INV_MSG(INV_MSG_LEVEL_ERROR, "Unknown FIFO event %d", event->sensor_mask);
		sleep_us(10 * 1000 * 1000);
	}
}

static int configure_and_enable_edmp_gaf(void)
{
	int                           rc = 0;
	inv_imu_edmp_gaf_parameters_t gaf_params;
	inv_imu_int_state_t           int_config;
	inv_imu_adv_fifo_config_t     fifo_config;

	/* Only support 100 Hz and 50Hz ODR */
	if ((sensor_cfg.odr_us != 10 * 1000) && (sensor_cfg.odr_us != 20 * 1000))
		return -1;

	/* Check `acc_pm` and `gyr_pm` are valid */
	if ((sensor_cfg.acc_pm != LOW_NOISE_MODE) && (sensor_cfg.acc_pm != LOW_POWER_MODE))
		return -1;
	if ((sensor_cfg.gyr_pm != LOW_NOISE_MODE) && (sensor_cfg.gyr_pm != LOW_POWER_MODE))
		return -1;

	/* Stop all sensors in case they were previously ON */
	rc |= inv_imu_edmp_disable(&imu_dev);
	rc |= inv_imu_set_accel_mode(&imu_dev, PWR_MGMT0_ACCEL_MODE_OFF);
	rc |= inv_imu_set_gyro_mode(&imu_dev, PWR_MGMT0_GYRO_MODE_OFF);
	SI_CHECK_RC(rc);

	/* Set ODR */
	if (sensor_cfg.odr_us == 10 * 1000) {
		rc |= inv_imu_edmp_set_frequency(&imu_dev, DMP_EXT_SEN_ODR_CFG_APEX_ODR_100_HZ);
		rc |= inv_imu_set_accel_frequency(&imu_dev, ACCEL_CONFIG0_ACCEL_ODR_100_HZ);
		rc |= inv_imu_set_gyro_frequency(&imu_dev, GYRO_CONFIG0_GYRO_ODR_100_HZ);
	} else {
		rc |= inv_imu_edmp_set_frequency(&imu_dev, DMP_EXT_SEN_ODR_CFG_APEX_ODR_50_HZ);
		rc |= inv_imu_set_accel_frequency(&imu_dev, ACCEL_CONFIG0_ACCEL_ODR_50_HZ);
		rc |= inv_imu_set_gyro_frequency(&imu_dev, GYRO_CONFIG0_GYRO_ODR_50_HZ);
	}
	SI_CHECK_RC(rc);

	/* Set sensor FSR */
	rc |= inv_imu_set_accel_fsr(&imu_dev, ACCEL_CONFIG0_ACCEL_UI_FS_SEL_8_G);
	rc |= inv_imu_set_gyro_fsr(&imu_dev, GYRO_CONFIG0_GYRO_UI_FS_SEL_2000_DPS);
	SI_CHECK_RC(rc);

	/* Initialize APEX for GAF */
	rc |= inv_imu_edmp_gaf_init(&imu_dev);
	SI_CHECK_RC(rc);

	/* Configure GAF parameters */
	rc |= inv_imu_edmp_gaf_init_parameters(&imu_dev, &gaf_params);
	gaf_params.pdr_us = sensor_cfg.odr_us;
	if (sensor_cfg.gyr_pm == LOW_POWER_MODE) {
		/* If gyro is in LP mode, gyr_cal_threshold_metricX shall be updated */
		gaf_params.gyr_cal_threshold_metric1 = 800;
		gaf_params.gyr_cal_threshold_metric2 = 25000;
	}
	/* Set GAF mounting matrix. In case matrix is identity, assignment is useless. */
	memcpy(gaf_params.mmatrix, mmatrix, sizeof(mmatrix));
	gaf_params.gyr_bias_accuracy = gyr_bias_accuracy;
	rc |= inv_imu_edmp_gaf_set_parameters(&imu_dev, &gaf_params);
	SI_CHECK_RC(rc);
	INV_MSG(INV_MSG_LEVEL_INFO,
	        "eDMP parameter clock_variation is 0x%hhx corresponding to %+f%% error",
	        gaf_params.clock_variation, ((float)gaf_params.clock_variation / 25.40f));

	/* Set accel in low-noise mode */
	if (sensor_cfg.acc_pm == LOW_NOISE_MODE) {
		switch (sensor_cfg.ln_bw) {
		case 4:
			rc |= inv_imu_set_accel_ln_bw(&imu_dev, IPREG_SYS2_REG_131_ACCEL_UI_LPFBW_DIV_4);
			break;
		case 8:
			rc |= inv_imu_set_accel_ln_bw(&imu_dev, IPREG_SYS2_REG_131_ACCEL_UI_LPFBW_DIV_8);
			break;
		case 16:
			rc |= inv_imu_set_accel_ln_bw(&imu_dev, IPREG_SYS2_REG_131_ACCEL_UI_LPFBW_DIV_16);
			break;
		default:
			return -1;
		}
		rc |= inv_imu_set_accel_mode(&imu_dev, PWR_MGMT0_ACCEL_MODE_LN);
	} else {
		switch (sensor_cfg.lp_avg) {
		case 1:
			rc |= inv_imu_set_accel_lp_avg(&imu_dev, IPREG_SYS2_REG_129_ACCEL_LP_AVG_1);
			break;
		case 2:
			rc |= inv_imu_set_accel_lp_avg(&imu_dev, IPREG_SYS2_REG_129_ACCEL_LP_AVG_2);
			break;
		case 4:
			rc |= inv_imu_set_accel_lp_avg(&imu_dev, IPREG_SYS2_REG_129_ACCEL_LP_AVG_4);
			break;
		case 5:
			rc |= inv_imu_set_accel_lp_avg(&imu_dev, IPREG_SYS2_REG_129_ACCEL_LP_AVG_5);
			break;
		case 7:
			rc |= inv_imu_set_accel_lp_avg(&imu_dev, IPREG_SYS2_REG_129_ACCEL_LP_AVG_7);
			break;
		case 8:
			rc |= inv_imu_set_accel_lp_avg(&imu_dev, IPREG_SYS2_REG_129_ACCEL_LP_AVG_8);
			break;
		default:
			return -1;
		}
		rc |= inv_imu_set_accel_mode(&imu_dev, PWR_MGMT0_ACCEL_MODE_LP);
	}
	if (sensor_cfg.gyr_pm == LOW_NOISE_MODE) {
		switch (sensor_cfg.ln_bw) {
		case 4:
			rc |= inv_imu_set_gyro_ln_bw(&imu_dev, IPREG_SYS1_REG_172_GYRO_UI_LPFBW_DIV_4);
			break;
		case 8:
			rc |= inv_imu_set_gyro_ln_bw(&imu_dev, IPREG_SYS1_REG_172_GYRO_UI_LPFBW_DIV_8);
			break;
		case 16:
			rc |= inv_imu_set_gyro_ln_bw(&imu_dev, IPREG_SYS1_REG_172_GYRO_UI_LPFBW_DIV_16);
			break;
		default:
			return -1;
		}
		rc |= inv_imu_set_gyro_mode(&imu_dev, PWR_MGMT0_GYRO_MODE_LN);
	} else {
		switch (sensor_cfg.lp_avg) {
		case 1:
			rc |= inv_imu_set_gyro_lp_avg(&imu_dev, IPREG_SYS1_REG_170_GYRO_LP_AVG_1);
			break;
		case 2:
			rc |= inv_imu_set_gyro_lp_avg(&imu_dev, IPREG_SYS1_REG_170_GYRO_LP_AVG_2);
			break;
		case 4:
			rc |= inv_imu_set_gyro_lp_avg(&imu_dev, IPREG_SYS1_REG_170_GYRO_LP_AVG_4);
			break;
		case 5:
			rc |= inv_imu_set_gyro_lp_avg(&imu_dev, IPREG_SYS1_REG_170_GYRO_LP_AVG_5);
			break;
		case 7:
			rc |= inv_imu_set_gyro_lp_avg(&imu_dev, IPREG_SYS1_REG_170_GYRO_LP_AVG_7);
			break;
		case 8:
			rc |= inv_imu_set_gyro_lp_avg(&imu_dev, IPREG_SYS1_REG_170_GYRO_LP_AVG_8);
			break;
		default:
			return -1;
		}
		rc |= inv_imu_set_gyro_mode(&imu_dev, PWR_MGMT0_GYRO_MODE_LP);
	}
	SI_CHECK_RC(rc);

	/* Wait for gyro startup time */
	sleep_us(GYR_STARTUP_TIME_US);

	/* Interrupts configuration:
	 * - Enable FIFO interrupt for GRV
	 * - Enable DRDY interrupt for raw data (raw data could also be retrieved from FIFO) */
	memset(&int_config, INV_IMU_DISABLE, sizeof(int_config));
	int_config.INV_FIFO_THS = INV_IMU_ENABLE;

#if IMU_RAW_DATA_EN == 1
	int_config.INV_UI_DRDY  = INV_IMU_ENABLE;
#endif
	rc |= inv_imu_set_config_int(&imu_dev, INV_IMU_INT1, &int_config);
	SI_CHECK_RC(rc);

	/*
	 * Configure FIFO so that eDMP can push GAF outputs in ES FIFO frame
	 * Set ES0 format to 9bytes
	 * Configure FIFO WM to 4 (number of FIFO frame per GAF run, can be less)
	 */
	rc |= inv_imu_adv_get_fifo_config(&imu_dev, &fifo_config);
	fifo_config.base_conf.fifo_mode  = FIFO_CONFIG0_FIFO_MODE_SNAPSHOT;
	fifo_config.base_conf.fifo_depth = FIFO_CONFIG0_FIFO_DEPTH_GAF;
	fifo_config.base_conf.fifo_wm_th = 4; //*
	fifo_config.base_conf.hires_en   = INV_IMU_DISABLE;
	fifo_config.base_conf.gyro_en    = INV_IMU_DISABLE; // raw data are read from sensor registers
	fifo_config.base_conf.accel_en   = INV_IMU_DISABLE; // raw data are read from sensor registers
	fifo_config.fifo_wr_wm_gt_th     = FIFO_CONFIG2_FIFO_WR_WM_EQ_OR_GT_TH;
	fifo_config.es1_en               = INV_IMU_DISABLE;
	fifo_config.es0_en               = INV_IMU_ENABLE;
	fifo_config.comp_en              = INV_IMU_DISABLE;
	fifo_config.tmst_fsync_en        = INV_IMU_DISABLE;
	fifo_config.es0_6b_9b            = FIFO_CONFIG4_FIFO_ES0_9B;
	rc |= inv_imu_adv_set_fifo_config(&imu_dev, &fifo_config);
	SI_CHECK_RC(rc);

	/* Enable GAF */
	rc |= inv_imu_edmp_gaf_enable(&imu_dev);
	SI_CHECK_RC(rc);

	/* Enable EDMP */
	rc |= inv_imu_edmp_enable(&imu_dev);
	SI_CHECK_RC(rc);

	/* Apply initial offsets */
	/* To give an accelerometer bias, it would need to happen before
	 * values are fed into `inv_imu_edmp_gaf_set_biases` call:
	 * acc_bias_q16[0] = <accel_bias_x>;
	 * acc_bias_q16[1] = <accel_bias_y>;
	 * acc_bias_q16[2] = <accel_bias_z>;
	 * By default, it is set to {0, 0, 0}
	 */
	acc_bias_q16[0] = 0;
	acc_bias_q16[1] = 0;
	acc_bias_q16[2] = 0;
	rc |= inv_imu_edmp_gaf_set_biases(&imu_dev, acc_bias_q16, gyr_bias_q16);
	SI_CHECK_RC(rc);

	return rc;
}


static int set_sensor_config(sensor_cfg_options_t opt)
{
	switch (opt) {
	case SENSOR_CFG_LN100:
		sensor_cfg.odr_us = 10 * 1000;
		sensor_cfg.ln_bw  = 4; /* BW is ODR div 4 = 25Hz in LN mode */
		sensor_cfg.acc_pm = LOW_NOISE_MODE;
		sensor_cfg.gyr_pm = LOW_NOISE_MODE;
		break;

	case SENSOR_CFG_LN50:
		sensor_cfg.odr_us = 20 * 1000;
		sensor_cfg.ln_bw  = 4; /* BW is ODR div 4 = 12.5Hz in LN mode */
		sensor_cfg.acc_pm = LOW_NOISE_MODE;
		sensor_cfg.gyr_pm = LOW_NOISE_MODE;
		break;

	case SENSOR_CFG_LP100:
		sensor_cfg.odr_us = 10 * 1000;
		sensor_cfg.ln_bw  = 4; /* BW is ODR div 4 = 25Hz in LN mode */
		sensor_cfg.lp_avg = 4; /* 4x averaging in LP mode */
		sensor_cfg.acc_pm = LOW_NOISE_MODE;
		sensor_cfg.gyr_pm = LOW_POWER_MODE;
		break;

	case SENSOR_CFG_LP50:
		sensor_cfg.odr_us = 20 * 1000;
		sensor_cfg.ln_bw  = 4; /* BW is ODR div 4 = 12.5Hz in LN mode */
		sensor_cfg.lp_avg = 4; /* 4x averaging in LP mode */
		sensor_cfg.acc_pm = LOW_NOISE_MODE;
		sensor_cfg.gyr_pm = LOW_POWER_MODE;
		break;

	default:
		return -1;
	}

	return 0;
}


static void apply_mounting_matrix(const int16_t matrix[9], int16_t raw[3])
{
	int32_t data_q14[3];

	for (unsigned i = 0; i < 3; i++) {
		data_q14[i] = ((int32_t)matrix[3 * i + 0] * raw[0]);
		data_q14[i] += ((int32_t)matrix[3 * i + 1] * raw[1]);
		data_q14[i] += ((int32_t)matrix[3 * i + 2] * raw[2]);
	}
	raw[0] = (int16_t)(data_q14[0] >> 14);
	raw[1] = (int16_t)(data_q14[1] >> 14);
	raw[2] = (int16_t)(data_q14[2] >> 14);
}

static int init_imu_biases(void)
{
	int rc;
	/* NVM allows for up to 84B to be stored */
	uint8_t sensor_bias[84] = { 0 };
	rc = imu_read_biases_data_from_flash(sensor_bias);
	if (0 == rc) {
		/* But we only need 12B for gyroscope biases */
		memcpy(gyr_bias_q16, sensor_bias, sizeof(gyr_bias_q16));
		INV_MSG(INV_MSG_LEVEL_INFO, "Loading calibration from flash:");
		INV_MSG(INV_MSG_LEVEL_INFO, "   - Gyro:  [%f %f %f]dps", (float)gyr_bias_q16[0] / (1 << 16),
		        (float)gyr_bias_q16[1] / (1 << 16), (float)gyr_bias_q16[2] / (1 << 16));
		/* Having previously saved biases gives a better confidence */
		gyr_bias_accuracy = 2;
	} else {
		/* rc is FLASH_HEADER_MISSING_RC (or 1) if data couldn't be found */
		memset(gyr_bias_q16, 0, sizeof(gyr_bias_q16));
		rc = INV_IMU_OK;
	}
	return rc;
}

int store_imu_biases(inv_imu_edmp_gaf_outputs_t *gaf_outputs)
{
	int rc = INV_IMU_OK;
	/* NVM allows for up to 84B to be stored */
	uint8_t sensor_bias[84] = { 0 };

	if (3 == gaf_outputs->gyr_accuracy_flag) {
		/* Only save gyroscope biases that are correctly calibrated */
		gyr_bias_q16[0] = gaf_outputs->gyr_bias_q16[0];
		gyr_bias_q16[1] = gaf_outputs->gyr_bias_q16[1];
		gyr_bias_q16[2] = gaf_outputs->gyr_bias_q16[2];
		/* Update the biases' accuracy since they are about to be saved */
		gyr_bias_accuracy = gaf_outputs->gyr_accuracy_flag;
		memcpy(sensor_bias, gyr_bias_q16, sizeof(gyr_bias_q16));
		INV_MSG(INV_MSG_LEVEL_INFO, "Saving calibration in flash:");
		INV_MSG(INV_MSG_LEVEL_INFO, "   - Gyro:  [%f %f %f]dps", (float)gyr_bias_q16[0] / (1 << 16),
		        (float)gyr_bias_q16[1] / (1 << 16), (float)gyr_bias_q16[2] / (1 << 16));
		rc |= imu_store_biases_data_to_flash(sensor_bias);
	} else {
		INV_MSG(INV_MSG_LEVEL_WARNING,
		        "Biases were not saved due to insufficient accuracy level (was %d, must be 3)",
		        (int32_t)gaf_outputs->gyr_accuracy_flag);
	}

	return rc;
}

int imu_store_biases_data(void)
{
	store_imu_biases(&gaf_outputs);
	return 0;
}


static void fixedpoint_to_float(const int32_t *in, float *out, const uint8_t fxp_shift,
                                const uint8_t dim)
{
	int   i;
	float scale = 1.f / (1 << fxp_shift);

	for (i = 0; i < dim; i++)
		out[i] = scale * in[i];
}

static void quaternions_to_angles(const float quat[4], float angles[3])
{
	const float RAD_2_DEG = (180.f / 3.14159265358979f);
	float       rot_matrix[9];

	// quaternion to rotation matrix
	const float dTx  = 2.0f * quat[1];
	const float dTy  = 2.0f * quat[2];
	const float dTz  = 2.0f * quat[3];
	const float dTwx = dTx * quat[0];
	const float dTwy = dTy * quat[0];
	const float dTwz = dTz * quat[0];
	const float dTxx = dTx * quat[1];
	const float dTxy = dTy * quat[1];
	const float dTxz = dTz * quat[1];
	const float dTyy = dTy * quat[2];
	const float dTyz = dTz * quat[2];
	const float dTzz = dTz * quat[3];

	rot_matrix[0] = 1.0f - (dTyy + dTzz);
	rot_matrix[1] = dTxy - dTwz;
	rot_matrix[2] = dTxz + dTwy;
	rot_matrix[3] = dTxy + dTwz;
	rot_matrix[4] = 1.0f - (dTxx + dTzz);
	rot_matrix[5] = dTyz - dTwx;
	rot_matrix[6] = dTxz - dTwy;
	rot_matrix[7] = dTyz + dTwx;
	rot_matrix[8] = 1.0f - (dTxx + dTyy);

	angles[0] = atan2f(-rot_matrix[3], rot_matrix[0]) * RAD_2_DEG;
	angles[1] = atan2f(-rot_matrix[7], rot_matrix[8]) * RAD_2_DEG;
	angles[2] = asinf(-rot_matrix[6]) * RAD_2_DEG;
}

