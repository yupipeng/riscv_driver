#ifndef __HAL_KEY_ADC_H__
#define __HAL_KEY_ADC_H__
#include "c_def.h"
#include "app_key.h"
#define HW_SARADC_ENABLE
void hal_key_adc_init(void);
U8 hal_key_adc_detect(APP_KEY *app_keys, U8 max_simult);
#endif