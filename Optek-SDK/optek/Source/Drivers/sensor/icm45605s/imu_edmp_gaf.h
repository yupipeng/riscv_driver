#ifndef __EDMP_GAF_H__
#define __EDMP_GAF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "inv_imu.h"
#include "inv_imu_transport.h"
#include "inv_imu_driver.h"
#include "inv_imu_driver_advanced.h"
#include "inv_imu_edmp.h"
#include "inv_imu_edmp_gaf.h"
#include "imu_driver.h"

/*
 * This example showcases how to configure and use GAF in eDMP.
 * It enables accelerometer and gyroscope by default in Low-Noise mode at 100Hz.
 * It loads GAF RAM image and starts eDMP to output GAF values.
 * There is also capability to get and print raw data from sensor registers reading.
 */

/*
 * Define msg level
 */
#define MSG_LEVEL INV_MSG_LEVEL_INFO

#define MASK_NOTIFY_RAW_ACC_DATA 0x01 /** raw accelerometer data from IMU sensor register */
#define MASK_NOTIFY_RAW_GYR_DATA 0x02 /** raw gyroscope data from IMU sensor register */

// how message prints
#define INV_MSG(level, fmt...) printf(fmt);printf("\r\n");
#define SI_CHECK_RC(x)
// #define IMU_WHOAMI_CHECK
// #define IMU_NOTIFY_DATA
/*
 * Select communication link between SmartMotion and IMU.
 * SPI4: `UI_SPI4`
 * SPI3: `UI_SPI3`
 * I2C:  `UI_I2C`
 */
#define SERIF_TYPE UI_SPI3
#define IMU_INT_MODE INTX_CONFIG2_INTX_MODE_LATCH  // latch mode will keep the interrupt active until it is cleared by the host
// #define IMU_INT_MODE INTX_CONFIG2_INTX_MODE_PULSE  // pulse mode will generate a pulse when frame is ready
/*
 * Handy defines to describe power modes
 */
#define LOW_POWER_MODE 0
#define LOW_NOISE_MODE 1
/* Raw data conversion */
#define RAW_ACC_SCALE (8 /* gee */ * 2 /* / 32768 * (1<<16)) */)
#define RAW_GYR_SCALE (2000 /* dps */ * 2 /* / 32768 * (1<<16)) */)


/*
 * Enum for possible sensor config
 */
typedef enum {
	SENSOR_CFG_LN100,
	SENSOR_CFG_LN50,
	SENSOR_CFG_LP100,
	SENSOR_CFG_LP50
} sensor_cfg_options_t;

#define SENSOR_CFG_DEFINE SENSOR_CFG_LN100
#define IMU_ACCEL_EN 0
#define IMU_GYRO_EN 0
#define IMU_RAW_DATA_EN 0

/*
 * Sensor configuration details
 */
typedef struct {
	uint32_t odr_us; /* accel and gyro ODR in us */
	uint8_t  ln_bw; /* Low-Noise mode bandwidth which is ODR divider */
	uint8_t  lp_avg; /* Low-Power mode averaging value */
	uint8_t  acc_pm; /* accel power mode, one of LOW_NOISE_MODE or LOW_POWER_MODE */
	uint8_t  gyr_pm; /* gyro power mode, one of LOW_NOISE_MODE or LOW_POWER_MODE */
} sensor_cfg_t;

/*
 * Enum for possible commands from user
 */
typedef enum {
	SET_CFG_LN100,
	SET_CFG_LN50,
	SET_CFG_LP100,
	SET_CFG_LP50,
	RAW_ACC_TOGGLE,
	RAW_GYR_TOGGLE,
	SAVE_BIAS_NVM,
} inv_edmp_gaf_commands;

typedef struct {
	/** 6-axis (accel and gyro fusion) quaternion */
	int32_t grv_quat_q30[4];

	/** Calibrated accelerometer (1 g = 1<<16) */
	int32_t acc_cal_q16[3];

	/** Calibrated gyroscope (1 dps = 1<<16) */
	int32_t gyr_cal_q16[3];

	/** Gyro bias (1 dps = 1<<16)*/
	int32_t gyr_bias_q16[3];

	/** Gyro accuracy, from 0 (non calibrated) to 3 (well calibrated) */
	int8_t gyr_accuracy_flag;

	/** Stationary detection based on gyro data */
	int8_t stationary_flag;

	/** Temperature value (1 degree Celsius = 1<<16)*/
	int32_t temp_degC_q16;
} inv_edmp_gaf_outputs_t;


#ifdef __cplusplus
}
#endif

#endif // __EDMP_GAF_H__