/*
 *
 * Copyright (c) [2016] by InvenSense, Inc.
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

#ifndef _DYN_PROTOCOL_H_
#define _DYN_PROTOCOL_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DYN_PROTOCOL_GROUP_ID 3
#define DYN_PROTOCOL_VERSION  "0.0.1-dev1"

/** @brief Sensor status definition
 */
enum DynProSensorStatus {
	DYN_PRO_SENSOR_STATUS_DATA_UPDATED   = 0, /**< new sensor data */
	DYN_PRO_SENSOR_STATUS_STATE_CHANGED  = 1, /**< indicates a change in sensor state */
	DYN_PRO_SENSOR_STATUS_FLUSH_COMPLETE = 2, /**< indicates the flush is completed */
	DYN_PRO_SENSOR_STATUS_POLLED_DATA    = 3, /**< sensor data value after manual request */
};

/** @brief Sensor type identifier definition
 */
enum DynSensorType {
	DYN_PRO_SENSOR_TYPE_RESERVED               = 0, /**< Reserved ID: do not use */
	DYN_PRO_SENSOR_TYPE_ACCELEROMETER          = 1, /**< Accelerometer */
	DYN_PRO_SENSOR_TYPE_MAGNETOMETER           = 2, /**< Magnetic field */
	DYN_PRO_SENSOR_TYPE_ORIENTATION            = 3, /**< Deprecated orientation */
	DYN_PRO_SENSOR_TYPE_GYROSCOPE              = 4, /**< Gyroscope */
	DYN_PRO_SENSOR_TYPE_LIGHT                  = 5, /**< Ambient light sensor */
	DYN_PRO_SENSOR_TYPE_PRESSURE               = 6, /**< Barometer */
	DYN_PRO_SENSOR_TYPE_TEMPERATURE            = 7, /**< Temperature */
	DYN_PRO_SENSOR_TYPE_PROXIMITY              = 8, /**< Proximity */
	DYN_PRO_SENSOR_TYPE_GRAVITY                = 9, /**< Gravity */
	DYN_PRO_SENSOR_TYPE_LINEAR_ACCELERATION    = 10, /**< Linear acceleration */
	DYN_PRO_SENSOR_TYPE_ROTATION_VECTOR        = 11, /**< Rotation vector */
	DYN_PRO_SENSOR_TYPE_HUMIDITY               = 12, /**< Relative humidity */
	DYN_PRO_SENSOR_TYPE_AMBIENT_TEMPERATURE    = 13, /**< Ambient temperature */
	DYN_PRO_SENSOR_TYPE_UNCAL_MAGNETOMETER     = 14, /**< Uncalibrated magnetic field */
	DYN_PRO_SENSOR_TYPE_GAME_ROTATION_VECTOR   = 15, /**< Game rotation vector */
	DYN_PRO_SENSOR_TYPE_UNCAL_GYROSCOPE        = 16, /**< Uncalibrated gyroscope */
	DYN_PRO_SENSOR_TYPE_SMD                    = 17, /**< Significant motion detection */
	DYN_PRO_SENSOR_TYPE_STEP_DETECTOR          = 18, /**< Step detector */
	DYN_PRO_SENSOR_TYPE_STEP_COUNTER           = 19, /**< Step counter */
	DYN_PRO_SENSOR_TYPE_GEOMAG_ROTATION_VECTOR = 20, /**< Geomagnetic rotation vector */
	DYN_PRO_SENSOR_TYPE_HEART_RATE             = 21, /**< Heart rate */
	DYN_PRO_SENSOR_TYPE_TILT_DETECTOR          = 22, /**< Tilt detector */
	DYN_PRO_SENSOR_TYPE_WAKE_GESTURE           = 23, /**< Wake-up gesture  */
	DYN_PRO_SENSOR_TYPE_GLANCE_GESTURE         = 24, /**< Glance gesture  */
	DYN_PRO_SENSOR_TYPE_PICK_UP_GESTURE        = 25, /**< Pick-up gesture */
	DYN_PRO_SENSOR_TYPE_BAC                    = 26, /**< Basic Activity Classifier */
	DYN_PRO_SENSOR_TYPE_PDR                    = 27, /**< Pedestrian Dead Reckoning */
	DYN_PRO_SENSOR_TYPE_B2S                    = 28, /**< Bring to see */
	DYN_PRO_SENSOR_TYPE_3AXIS                  = 29, /**< 3 Axis sensor */
	DYN_PRO_SENSOR_TYPE_EIS                    = 30, /**< Electronic Image Stabilization */
	DYN_PRO_SENSOR_TYPE_OIS                    = 31, /**< Optical Image Stabilization (gyro only) */
	DYN_PRO_SENSOR_TYPE_RAW_ACCELEROMETER      = 32, /**< Raw accelerometer */
	DYN_PRO_SENSOR_TYPE_RAW_GYROSCOPE          = 33, /**< Raw gyroscope */
	DYN_PRO_SENSOR_TYPE_RAW_MAGNETOMETER       = 34, /**< Raw magnetometer */
	DYN_PRO_SENSOR_TYPE_RAW_TEMPERATURE        = 35, /**< Raw temperature */
	DYN_PRO_SENSOR_TYPE_CUSTOM_PRESSURE        = 36, /**< Custom Pressure Sensor */
	DYN_PRO_SENSOR_TYPE_MIC                    = 37, /**< Stream audio from microphone */
	DYN_PRO_SENSOR_TYPE_TSIMU                  = 38, /**< TS-IMU */
	DYN_PRO_SENSOR_TYPE_RAW_PPG                = 39, /**< Raw Photoplethysmogram */
	DYN_PRO_SENSOR_TYPE_HRV                    = 40, /**< Heart rate variability */
	DYN_PRO_SENSOR_TYPE_SLEEP_ANALYSIS         = 41, /**< Sleep analysis */
	DYN_PRO_SENSOR_TYPE_BAC_EXTENDED           = 42, /**< Basic Activity Classifier Extended */
	DYN_PRO_SENSOR_TYPE_BAC_STATISTICS         = 43, /**< Basic Activity Classifier Statistics */
	DYN_PRO_SENSOR_TYPE_FLOOR_CLIMB_COUNTER    = 44, /**< Floor Climbed Counter */
	DYN_PRO_SENSOR_TYPE_ENERGY_EXPENDITURE     = 45, /**< Energy Expenditure */
	DYN_PRO_SENSOR_TYPE_DISTANCE               = 46, /**< Distance */
	DYN_PRO_SENSOR_TYPE_SHAKE                  = 47, /**< Shake Gesture */
	DYN_PRO_SENSOR_TYPE_DOUBLE_TAP             = 48, /**< Double Tap */
	DYN_PRO_SENSOR_TYPE_CUSTOM0, /**< Custom sensor ID 0 */
	DYN_PRO_SENSOR_TYPE_CUSTOM1, /**< Custom sensor ID 1 */
	DYN_PRO_SENSOR_TYPE_CUSTOM2, /**< Custom sensor ID 2 */
	DYN_PRO_SENSOR_TYPE_CUSTOM3, /**< Custom sensor ID 3 */
	DYN_PRO_SENSOR_TYPE_CUSTOM4, /**< Custom sensor ID 4 */
	DYN_PRO_SENSOR_TYPE_CUSTOM5, /**< Custom sensor ID 5 */
	DYN_PRO_SENSOR_TYPE_CUSTOM6, /**< Custom sensor ID 6 */
	DYN_PRO_SENSOR_TYPE_CUSTOM7, /**< Custom sensor ID 7 */
	DYN_PRO_SENSOR_TYPE_WOM, /**< Wake-up on motion */
	DYN_PRO_SENSOR_TYPE_SEDENTARY_REMIND, /**< Sedentary Remind */
	DYN_PRO_SENSOR_TYPE_DATA_ENCRYPTION = 59, /**< Data Encryption */
	DYN_PRO_SENSOR_TYPE_FSYNC_EVENT, /**< FSYNC event */
	DYN_PRO_SENSOR_TYPE_HIGH_RATE_GYRO, /**< High Rate Gyro */
	DYN_PRO_SENSOR_TYPE_CUSTOM_BSCD, /**< Custom BAC StepCounter Calorie counter and Distance counter */
	DYN_PRO_SENSOR_TYPE_HRM_LOGGER, /**< HRM Logger */
	DYN_PRO_SENSOR_TYPE_PRED_QUAT_0, /**< Predictive Quaternion instance 0 */
	DYN_PRO_SENSOR_TYPE_PRED_QUAT_1, /**< Predictive Quaternion instance 1 */
	DYN_PRO_SENSOR_TYPE_OIS_1, /**< Optical Image Stabilization with gyroscope only data instance 1 */
	DYN_PRO_SENSOR_TYPE_LOW_G, /**< Low G detection */
	DYN_PRO_SENSOR_TYPE_HIGH_G, /**< High G detection */
	DYN_PRO_SENSOR_TYPE_TAP, /**< Tap detection */
	DYN_PRO_SENSOR_TYPE_R2W, /**< Raise to Wake detection */
	DYN_PRO_SENSOR_TYPE_OIS_AG_0, /**< Optical Image Stabilization with accelerometer and gyroscope data instance 0 */
	DYN_PRO_SENSOR_TYPE_OIS_AG_1, /**< Optical Image Stabilization with accelerometer and gyroscope data instance 1 */
	DYN_PRO_SENSOR_TYPE_FF, /**< Free Fall detection */

	DYN_PRO_SENSOR_TYPE_MAX /**< sentinel value for sensor type */
};

/** @brief Event identifier definition
 */
/* Commented label are currently not implemented */
enum DynProtocolEid {
	/* Protocol */
	DYN_PROTOCOL_EID_PROTOCOLVERSION = 0x00,

	/* IDD methods */
	DYN_PROTOCOL_EID_WHO_AM_I = 0x10,
	DYN_PROTOCOL_EID_RESET    = 0x11,
	DYN_PROTOCOL_EID_SETUP    = 0x12,
	DYN_PROTOCOL_EID_CLEANUP  = 0x13,
	// DYN_PROTOCOL_EID_LOAD               = 0x14,
	DYN_PROTOCOL_EID_SELF_TEST   = 0x15,
	DYN_PROTOCOL_EID_GET_FW_INFO = 0x16,
	DYN_PROTOCOL_EID_PING_SENSOR = 0x17,
	// DYN_PROTOCOL_EID_SET_RUNNING_STATE  = 0x18,
	DYN_PROTOCOL_EID_START_SENSOR       = 0x19,
	DYN_PROTOCOL_EID_STOP_SENSOR        = 0x1A,
	DYN_PROTOCOL_EID_SET_SENSOR_PERIOD  = 0x1B,
	DYN_PROTOCOL_EID_SET_SENSOR_TIMEOUT = 0x1C,
	DYN_PROTOCOL_EID_FLUSH_SENSOR       = 0x1D,
	DYN_PROTOCOL_EID_SET_SENSOR_BIAS    = 0x1E,
	DYN_PROTOCOL_EID_GET_SENSOR_BIAS    = 0x1F,
	DYN_PROTOCOL_EID_SET_SENSOR_MMATRIX = 0x20,
	DYN_PROTOCOL_EID_GET_SENSOR_DATA    = 0x21,
	DYN_PROTOCOL_EID_GET_SW_REG         = 0x22,
	DYN_PROTOCOL_EID_SET_SENSOR_CFG     = 0x23,
	DYN_PROTOCOL_EID_GET_SENSOR_CFG     = 0x24,
	DYN_PROTOCOL_EID_WRITE_MEMS_REG     = 0x25,
	DYN_PROTOCOL_EID_READ_MEMS_REG      = 0x26,
	DYN_PROTOCOL_EID_TRANSFER_BUFFER    = 0x27,

	/* Events */
	DYN_PROTOCOL_EID_NEW_SENSOR_DATA = 0x30,
};

/** @brief Event type definition
 */
enum DynProtocolEtype {
	DYN_PROTOCOL_ETYPE_CMD   = 0,
	DYN_PROTOCOL_ETYPE_RESP  = 1,
	DYN_PROTOCOL_ETYPE_ASYNC = 2
};

enum dynamic_protocol_ereg {
	DYN_PROTOCOL_EREG_MAJOR_VERSION = 0,
	DYN_PROTOCOL_EREG_MINOR_VERSION,
	DYN_PROTOCOL_EREG_REV_VERSION,
	DYN_PROTOCOL_EREG_SUFFIX_VERSION,
	DYN_PROTOCOL_EREG_HANDSHAKE_SUPPORT,
	DYN_PROTOCOL_EREG_FIFO_SIZE_LSB,
	DYN_PROTOCOL_EREG_FIFO_SIZE_MSB
};

enum DynProtocolConfigType {
	DYN_PROTOCOL_CONFIG_TYPE_RESERVED        = 0, /**< reserved - do not use */
	DYN_PROTOCOL_CONFIG_TYPE_REFERENCE_FRAME = 1, /**< sensor reference frame (mouting matrix) */
	DYN_PROTOCOL_CONFIG_TYPE_GAIN            = 2, /**< sensor gain to be applied on sensor data */
	DYN_PROTOCOL_CONFIG_TYPE_OFFSET          = 3, /**< sensor offset to be applied on sensor data */
	DYN_PROTOCOL_CONFIG_TYPE_CONTEXT         = 4, /**< arbitray context buffer */
	DYN_PROTOCOL_CONFIG_TYPE_FSR             = 5, /**< sensor's full scale range */
	DYN_PROTOCOL_CONFIG_TYPE_BW              = 6, /**< sensor's bandwidth */
	DYN_PROTOCOL_CONFIG_TYPE_AVG             = 7, /**< sensor's average */
	DYN_PROTOCOL_CONFIG_TYPE_RESET           = 8, /**< Reset the specified service */
	DYN_PROTOCOL_CONFIG_TYPE_POWER_MODE      = 9, /**< Change the power mode of the sensor */

	DYN_PROTOCOL_CONFIG_TYPE_CUSTOM = 32, /**< base value to indicate custom config */

	DYN_PROTOCOL_CONFIG_TYPE_MAX = 64, /**< absolute maximum value for config type */
};

/** @brief Mask to retrieve accuracy value from sensor event meta-data
 */
#define DYN_PROTOCOL_DATA_ACCURACY_MASK ((uint32_t)0x7)

/** @brief Value for UNKNOWN accuracy in sensor event meta-data
 */
#define DYN_PROTOCOL_DATA_ACCURACY_UNKNOWN 0

/** @brief Value for LOW accuracy in sensor event meta-data
 */
#define DYN_PROTOCOL_DATA_ACCURACY_LOW 1

/** @brief Value for MEDIUM accuracy in sensor event meta-data
 */
#define DYN_PROTOCOL_DATA_ACCURACY_MEDIUM 2

/** @brief Value for HIGH accuracy in sensor event meta-data
 */
#define DYN_PROTOCOL_DATA_ACCURACY_HIGH 3

/** @brief Base dynamic protocol sensor data structure
 *
 *  Upon NEW_DATA event, event data are expected to point to a DynProtocolSensorDataBase_t.
 *
 *  Any data structure should inherit from it.
 */
typedef struct {
	uint32_t timestamp; /**< data timestamp */
	uint32_t
	    meta_data; /**< additionnal status/info regarding the sensor event: (bit0-2: accuracy flag, bit3-31: reserved (must be ignored) */
} DynProtocolSensorDataBase_t;

/** @brief Structure to hold any dynamic protocol sensor data
 */
typedef struct {
	DynProtocolSensorDataBase_t base; /**< base dynamic protocol sensor data object */
	union {
		uint8_t  u8[64]; /**< 64 bytes of data */
		uint16_t u16[32]; /**< 32 halfwords */
		uint32_t u32[16]; /**< 16 words */
	} data; /**< dynamic protocol sensor data (use a union to ensure correct pointer alignement) */
} DynProtocolSensorData_t;

/*
 * Configuration DATA defintion
 */

/** @brief Base dynamic protocol sensor configuration data structure
 *
 *  Any configuration data structure should inherit from it
 */
typedef struct {
	uint32_t type; /**< configuration type identifier */
	uint32_t size; /**< configuration data size */
} DynProtocolConfig_t;

/** @brief Configuration data for CONTEXT identifier
 */
typedef struct DynProtocolConfigContext {
	DynProtocolConfig_t base; /**< base */
	uint8_t             buffer[64]; /**< buffer data */
} DynProtocolConfigContext_t;

/** @brief Configuration data for OFFSET identifier
 */
typedef struct {
	DynProtocolConfig_t base; /**< base */
	int32_t             vect[3]; /**< 3x1 offset data coded as Q16 (1:2^16)*/
} DynProtocolConfigOffset_t;

/** @brief Configuration data for GAIN identifier
 */
typedef struct {
	DynProtocolConfig_t base; /**< base */
	int32_t             matrix[9]; /**< 3x3 matrix gain data coded as Q30 (1:2^30) */
} DynProtocolConfigGain_t;

/** @brief Configuration data for REFERENCE_FRAME identifier
 */
typedef struct {
	DynProtocolConfig_t base; /**< base */
	int32_t             matrix[9]; /**< 3x3 mountig matrix coded as Q30 (1:2^30) */
} DynProtocolConfigReferenceFrame_t;

/** @brief Configuration data for FSR identifier
 */
typedef struct {
	DynProtocolConfig_t base; /**< base */
	int32_t             fsr; /**< fsr value */
} DynProtocolConfigFullScaleRange_t;

/** @brief Configuration data for BW identifier
 */
typedef struct {
	DynProtocolConfig_t base; /**< base */
	uint32_t            bw; /**< bw value */
} DynProtocolConfigBandwidth_t;

/** @brief Configuration data for AVG identifier
 */
typedef struct {
	DynProtocolConfig_t base; /**< base */
	uint16_t            avg; /**< avg value */
	uint16_t            avg_rate; /**< rate value */
} DynProtocolConfigAverage_t;

/** @brief Configuration data for Reset
 */
typedef DynProtocolConfig_t DynProtocolConfigReset_t;

/** @brief Configuration data for Power Mode 
 */
typedef struct {
	DynProtocolConfig_t base; /**< base */
	uint8_t             powermode; /**< Sensor's power mode: 0 for LP, 1 for LN */
} DynProtocolConfigPowerMode_t;

/** @brief Event data structure definition
 */
typedef struct DynProtocolEdata {
	int sensor_id; /** 0 if not applicable */
	union {
		union {
			uint32_t period; /** for EID_SET_SENSOR_PERIOD */
			uint32_t timeout; /** for EID_SET_SENSOR_TIMEOUT */
			uint8_t  regAddr; /** for EID GET_SW_REG */
			struct { /** for EID_WRITE_MEMS_REG / EID_READ_MEMS_REG */
				uint8_t regAddr;
				uint8_t regValue;
			} memsReg;
			DynProtocolConfigContext_t cfg; /** for EID_SET_SENSOR_CFG */
			struct {
				uint16_t size;
				uint8_t *address;
			} buffer;
		} command;
		union {
			char version[16]; /** for EID_PROTOCOLVERSION */
			int  rc; /** return code */
			struct { /** for EID_READ_MEMS_REG */
				int     rc;
				uint8_t regValue;
			} memsReg;
			struct { /** for EID GET_SENSOR_DATA */
				int                     rc;
				uint32_t                status; /** sensor status (see enum DynSensorStatus) */
				DynProtocolSensorData_t vdata;
			} sensorData;
			struct { /** for EID_GET_SENSOR_CFG */
				int                        rc;
				DynProtocolConfigContext_t cfg;
			} sensorcfg;
		} response; /** returned data if applicable */
		union {
			struct {
				uint32_t                status; /** sensor status (see enum DynSensorStatus) */
				DynProtocolSensorData_t vdata;
			} sensorEvent; /** for EID_NEW_SENSOR_DATA */
			uint8_t buffer[128];
		} async;
	} d;
} DynProtocolEdata_t;

/** @brief Protocol Event callback prototype definition
 */
typedef void (*DynProtocolEvent_cb)(enum DynProtocolEtype etype, enum DynProtocolEid eid,
                                    const DynProtocolEdata_t *edata, void *cookie);

/** @brief Protocol handler definition
 */
typedef struct DynProtocol {
	DynProtocolEvent_cb event_cb;
	void *              event_cb_cookie;
	struct {
		enum sm {
			PROTOCOL_STATE_IDLE = 0,
			PROTOCOL_STATE_GID  = PROTOCOL_STATE_IDLE,
			PROTOCOL_STATE_CID,
			PROTOCOL_STATE_PAYLOAD,
		} state;
		uint16_t current_frame_size;
		uint8_t  event_type;
		uint8_t  group_id;
		uint8_t  cmd_id;
		uint16_t expected_size;
		uint16_t received_size;
		uint8_t  tmp_buffer[640];
	} decode_state_machine;
	struct {
		uint8_t acc;
		uint8_t gyro;
	} precision;
} DynProtocol_t;

void DynProtocol_init(DynProtocol_t *self, DynProtocolEvent_cb event_cb, void *event_cb_cookie);

void DynProtocol_processReset(DynProtocol_t *self);

void DynProtocol_setCurrentFrameSize(DynProtocol_t *self, uint16_t frameSizeB);

int DynProtocol_setPrecision(DynProtocol_t *self, int sensor, uint8_t precision);

int DynProtocol_processPktByte(DynProtocol_t *self, uint8_t rcv_byte);

int DynProtocol_encodeAsync(DynProtocol_t *self, enum DynProtocolEid eid,
                            const DynProtocolEdata_t *edata, uint8_t *outBuffer,
                            uint16_t maxBufferSize, uint16_t *outBufferSize);

int DynProtocol_encodeResponse(DynProtocol_t *self, enum DynProtocolEid eid,
                               const DynProtocolEdata_t *edata, uint8_t *outBuffer,
                               uint16_t maxBufferSize, uint16_t *outBufferSize);

int DynProtocol_encodeCommand(DynProtocol_t *self, enum DynProtocolEid eid,
                              const DynProtocolEdata_t *edata, uint8_t *outBuffer,
                              uint16_t maxBufferSize, uint16_t *outBufferSize);

/** @brief Utility function that returns a string from a sensor type
 *  Empty string is returned if sensor is invalid
 */
const char *DynProtocol_sensorTypeToStr(int type);

#ifdef __cplusplus
}
#endif

#endif /* _GSH_PROTOCOL_H_ */