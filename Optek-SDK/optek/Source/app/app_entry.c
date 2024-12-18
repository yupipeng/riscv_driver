#include "os_config.h"

#include "hw_timer.h"
#include "hw_audio_adc_dac.h"

#include "hal_uart.h"
#include "hal_dma.h"

#include "sdk_config.h"
#include "sdk_ver.h"
#include "system_timer.h"
#include "codec.h"
#include "app_source.h"
#include "app_window.h"
#include "console_task.h"
#include "key_scan.h"

#ifdef DISPLAY_MODULE_ENABLE
#include "DisplayModule.h"
#endif

#ifdef I2C0_ENABLE
#include "app_i2c0.h"
#endif

#ifdef I2C1_ENABLE
#include "app_i2c1.h"
#endif

#ifdef I2C2_ENABLE
#include "app_i2c2.h"
#endif

#ifdef I2C3_ENABLE
#include "app_i2c3.h"
#endif

#ifdef SPI1_ENABLE
#include "hw_spi.h"
#endif

#ifdef SPI2_ENABLE
#include "hw_spi2.h"
#endif

#ifdef KEY_ENABLE
#include "key_scan.h"
#endif

#ifdef AUDIO_PROCESSOR
#include "audio_device.h"
#endif

#ifdef SAVE_MODULE_ENABLE
#include "app_save.h"
#endif

#ifdef OPTEK_HW_ARC_CEC_ENABLE
#include "arc_detect.h"
#include "arc_process.h"
#elif defined OPTEK_GPIO_ARC_CEC_ENABLE
#include "arc_detect_gpio.h"
#include "arc_process_gpio.h"
#endif

#ifdef IT66322_M031_ENABLE
#include "IT66322_TRM_M031_Sys_iTE.h"
#endif

#ifdef DISPLAY_MODULE_ENABLE
#include "LCDdevice.h"
#endif

const char app_ver_date[] = __DATE__;

const char app_time_date[] = __TIME__;

/*
const char app_version[] = "0.0.1";
*/
const char app_version[] __attribute__((section(".rom_version_rodata"))) = "0.0.1";


const char *app_version_get(void)
{
    return app_version;
}

const char *app_date_get(void)
{
    return app_ver_date;
}

const char *app_time_get(void)
{
    return app_time_date;
}


/**
 ****************************************************************************************
 * @brief Initialize dma module and printf module before starting schedule
 *
 ****************************************************************************************
 */
void app_PreTaskSch_init(void)
{
    hal_dma_module_init();

#ifdef UART0_USED_FOR_DEBUG
	hal_uart0_open(1, 1, UART0_BAUDRATE);
	printf_init(&uart0_api);
	console_cmd_init(&uart0_api);
	console_cmd_start();
#elif defined UART1_USED_FOR_DEBUG
	hal_uart1_open(1, 1, UART1_BAUDRATE);
	printf_init(&uart1_api);
	console_cmd_init(&uart1_api);
	console_cmd_start();
#elif defined UART2_USED_FOR_DEBUG
	hal_uart2_open(1, 1, UART2_BAUDRATE);
	printf_init(&uart2_api);
	console_cmd_init(&uart2_api);
	console_cmd_start();
#endif

	printf_info ("SDK version:%s\n\r", sdk_version_get());
	printf_info ("Buid time:%s %s\n\r", sdk_time_get(), sdk_date_get());

	printf_info ("App version:%s\n\r", app_version_get());
	printf_info ("Buid time:%s %s\n\r", app_time_get(), app_date_get());
}


extern const u8 optek_link_group[3];
/**
 ****************************************************************************************
 * @brief Initialize some function modules and enter default source
 * 
 * @return will not return
 *
 ****************************************************************************************
 */
void app_PostTaskSch_init(void)
{
	printf_info("%s\n\r", __func__);

	#ifdef DEBUG_GPIO_ENABLE
    debug_pin_init();
    #endif

	system_timer_data_init();

	#ifdef I2C0_ENABLE
	app_i2c0_init();
	#endif

	#ifdef I2C1_ENABLE
	app_i2c1_init();
	#endif

	#ifdef I2C2_ENABLE
	app_i2c2_init();
	#endif

	#ifdef I2C3_ENABLE
	app_i2c3_init();
	#endif

	#ifdef SPI1_ENABLE
	hw_spi_init();
	#endif

	#ifdef SPI2_ENABLE
	hw_spi_2_init();
	#endif

	#if (defined AUX_INPUT_FROM_ADC || defined AUDIO_OUTPUT_FROM_DAC)
	hw_audio_adc_dac_clk_init();
	hw_audio_adc_dac_enable(TRUE);
    #ifdef AUX_INPUT_FROM_ADC
	hw_audio_adc_init(CLOSE_ALL_ADC_MARK);
    #endif
    #ifdef AUDIO_OUTPUT_FROM_DAC
	hw_audio_dac_init();
	#endif
	#endif

	#ifdef HDMI_ENABLE
	hdmi_process_init();
	#endif
	
	upgrade_status_get();

	#ifdef REMOTE_ENABLE
	Remote_Init();
	#endif

	#ifdef KEY_ENABLE
	KeyScan_init();
	#endif

	#ifdef ROTARY_ENABLE
	#ifdef ROTARY_DET_USED_GPIO
	rotary_used_gpio_init();
	#endif //ROTARY_DET_USED_GPIO
	#endif //ROTARY_ENABLE    
	
	#ifdef HDMI_ENABLE
	#ifdef IT66322_ENABLE
	hdmi_start_set(TRUE);
	#endif

	#ifdef HDMI_MODULE_IT66320_RESET
	ITE66320_hw_reset();
	hdmi_start_set(TRUE);
	#endif
	#endif

    app_window_init();

	system_timer_init();

	#ifdef PROMPT_TONE
	prompt_tone_init();
	#endif
	
	#ifdef BTDM52_ENABLE
	#ifndef BT_RF_TEST_ENABLE
	app_bt_init();
	#else 
	app_btdut_init();
	#endif
	#endif

	#ifdef PROJECT_UI_GLOBAL_VALUE_INIT
	app_audio_device_init();
	app_global_vai_init();
	#endif

	#ifdef IT66322_M031_ENABLE
	IT66321_Cfg(0,DEFAULT_APP_SOURCE,audio_vari.volume,VOLUME_MAX);
	#endif//IT66322_M031_ENABLE
	
	#ifdef DISPLAY_MODULE_ENABLE
	app_display_module.init();
	app_display_module.open();
	#endif

	#ifdef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
	app_save_system_data_init();
	#endif

	#ifdef AUDIO_PROCESSOR
	#ifndef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
	audio_vari.volume = VOLUME_DEFAULT;
	#endif
	
	#if ((defined AUDIO_OPTEK_DAPP) || (defined SONA_DSP_DECODE_INIT))
	AUDIOdevice.init();
	AUDIOdevice.open();
	#endif

	#ifdef AUDIO_AMP_DEV_ENABLE
	int i;
	for (i=0;i<AMP_DEV_COUNT_MAX;i++)
	{
		AudioAmpDev[i].init();
		AudioAmpDev[i].open();
	}
	#endif
	#endif

	#if defined OPTEK_HW_ARC_CEC_ENABLE 
	arc_cec_standby_status = FALSE;
	hw_cec_init();
	arc_dectect_pin_init();
	arc_dectect_status_init();
	optek_cec_init();
	optek_cec_open();
	#elif defined OPTEK_GPIO_ARC_CEC_ENABLE
    DBG_Printf("CEC Ver:%s\n\r", optek_cec_version_get());
    DBG_Printf("Build:%s %s\n\r", optek_cec_data_get(), optek_cec_time_get());
	arc_cec_standby_status = FALSE;
	cec_detect_hw_init(ARC_DETECT_PIN_GPIO_GROUP, ARC_DETECT_PIN);
	cec_line_hw_init(CEC_LINE_PIN_GPIO_GROUP, CEC_LINE_PIN);
    arc_cec_process_init();
	cec_init();
	cec_open();
	#endif
		
	#ifndef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU	
	global_vari.audio_source = DEFAULT_APP_SOURCE;
	#endif

	APP_SOURCE_STRU* pSrcFunction;

	if (global_vari.optek_link_role == OPL_ROLE_SLAVE)
    {
	    pSrcFunction = get_source_entry_by_source_id(SOURCE_OPTEKLINK_SLAVE);
    }
	else
    {
	    pSrcFunction = get_source_entry_by_source_id(global_vari.audio_source);
    }
	#ifdef RUNTIME_DEBUG
	app_timer_check_step(__func__,1);
	#endif
	#ifdef SYSTEM_POWER_ON_TO_STANDBY_MODE
	global_vari.audio_source = pSrcFunction->id;
	printf_info("audio_source : %d\n\r", global_vari.audio_source);
	app_source_convert_to_standby_enter();
	#else
    global_vari.audio_source = SOURCE_UNKNOWN;
	if (pSrcFunction != NULL)
	{
        printf_info("pSrcFunction:%s;%d\n", pSrcFunction->string, global_vari.audio_source);
		pSrcFunction->switch_to_source();
	}
	else
	{
        printf_warn("pSrcFunction:%s;%d\n", pSrcFunction->string, global_vari.audio_source);
		printf_warn("default source entry not found\n");
	}
	#endif
}

