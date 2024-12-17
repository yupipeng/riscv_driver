#ifndef __IMU_DRIVER_H__
#define __IMU_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "inv_imu_defs.h"
/**
 * @brief init imu sensor by given parameters, call it before imu_loop at program start
 *
 */
void imu_init(void);
/**
 * @brief imu loop to get sensor data, call it every 20ms, 10ms or quicker
 *
 * @return int
 * @retval INV_IMU_OK if success
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

extern float imu_acc_euler_angle[3];

#ifdef __cplusplus
}
#endif

#endif //__IMU_DRIVER_H__