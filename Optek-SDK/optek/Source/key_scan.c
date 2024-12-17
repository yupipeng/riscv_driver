#include "app_key.h"
#include "key_scan.h"


/*
 * STRUCT DEFINITION
 ****************************************************************************************
 */
struct {
	APP_KEY gpiokey[KEY_GPIO_NUMBER_MAX];
	APP_KEY adkey[KEY_ADC_NUMBER_MAX];

	struct {
		APP_KEY key_id;
		U32 filter_cnt;
	} key_filter, mix_key_release_filter;

	APP_KEY key_pressed;
	U16 num_of_key_pressed;
} keyScan_env;

void KeyScan_init(void)
{
    printf("%s\r\n",__func__);
	memset(keyScan_env.gpiokey, APP_KEY_INVALID, sizeof(keyScan_env.gpiokey));
	memset(keyScan_env.adkey, APP_KEY_INVALID, sizeof(keyScan_env.adkey));
	keyScan_env.key_filter.key_id = APP_KEY_INVALID;
	keyScan_env.key_filter.filter_cnt = 0;
	keyScan_env.key_pressed = APP_KEY_INVALID;

	keyScan_env.mix_key_release_filter.key_id = APP_KEY_INVALID;
	keyScan_env.mix_key_release_filter.filter_cnt = 0;

	#ifdef KEY_GPIO_ENABLE
	app_gpio_key_init();
	#endif

	#ifdef KEY_ADC_ENABLE
	hal_key_adc_init();
	#endif
}


