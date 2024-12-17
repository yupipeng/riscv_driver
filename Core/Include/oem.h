#ifndef __OEM_H__
#define __OEM_H__

#define SPI_FLASH_4BIT_ENABLE

//#define AUDIO_MIPS_PRINTF_ENABLE


/*
OTK626x with pSram
OTK6267B
OTK6267LDV
OTK6267DV
OTK6268D
OTK6269
*/
// #define OTK626X_WITH_PSRAM

/***********************************************
*******             USER SET           ********
***********************************************/
//
#define PROJECT_UI_GLOBAL_VALUE_INIT

// #define PROJECT_UI_HIDDEN_FUNCTION
// #define PROJECT_UI_FACTORY_RESET_FUNCTION
// #define PROJECT_UI_FACTORY_TEST_FUNCTION
// #define PROJECT_UI_WINDOW_MENU_FUNCTION

// #define PROJECT_UI_WOOFER_FUNCTION


// #define TCA9534A_ENABLE
#ifdef TCA9534A_ENABLE
/*
 * #define I2C0_ENABLE
 * #define I2C1_ENABLE
 * #define I2C2_ENABLE
*/
#define I2C2_ENABLE
#endif

//#define RUNTIME_DEBUG

#define PROMPT_TONE

/***********************************************
*******             CODEC SET           ********
***********************************************/
// #define DOLBY_AC3_DEC_ENABLE
#ifdef DOLBY_AC3_DEC_ENABLE
#define DOLBY_AC3_DEC_ONLY
#endif

// #define DOLBY_DDPLUS71_DEC_ENABLE

//#define DOLBY_DDPLUS71_DEC_TEST
//#define DOLBY_EC3_TEST_BY_USB
//#define DOLBY_DDPLUS71_DEC_CHECK_OUT

//#define ATMOS_DDP_OBJECT_AUDIO_TEST
//#define ATMOS_DDP_OBJECT_AUDIO_CCHECK_OUT
//#define ATMOS_CODEC_ENABLE

// #define DTS_DMP_DEC_EANBLE
// #define DTS_DMP_DEC_ONLY
// #define DTS_CS_TO_X_CH_ENABLE

// #define DTS_VIRTUAL_X_ENABLE

#ifdef DTS_VIRTUAL_X_ENABLE
#define CLIB_RELOCATION_ENABLE
#define DTS_VIRTUAL_X_APP_LIB_USED
#define DTS_VIRTUAL_X_WRAPPER_FOR_OPTEK_LIB_USED
#define DTS_VIRTUAL_X_TABLE

//#define AUDIO_OUT_2CH_TO_6CH_FOR_DIS_VIRTUAL_X_SEL_6CH_INPUT

#define VIRTUAL_X_PRE_GAIN
#ifdef VIRTUAL_X_PRE_GAIN

#define VIRTUAL_X_PRE_ALL_DATA_GAIN_xDB     0

#define PCM_DEC_VIRTUAL_X_PRE_GAIN_xDB      (-12 + VIRTUAL_X_PRE_ALL_DATA_GAIN_xDB)

#define DD_2p0_DEC_VIRTUAL_X_PRE_GAIN_xDB   (-6 + VIRTUAL_X_PRE_ALL_DATA_GAIN_xDB)
#define DD_5p1_DEC_VIRTUAL_X_PRE_GAIN_xDB   (-6 + VIRTUAL_X_PRE_ALL_DATA_GAIN_xDB)

#define DTS_2p0_DEC_VIRTUAL_X_PRE_GAIN_xDB  (-6 + VIRTUAL_X_PRE_ALL_DATA_GAIN_xDB)
#define DTS_5p1_DEC_VIRTUAL_X_PRE_GAIN_xDB  (-6 + VIRTUAL_X_PRE_ALL_DATA_GAIN_xDB)
#endif

// #define VIRTUAL_X_POST_GAIN
#ifdef VIRTUAL_X_POST_GAIN

#define PCM_DEC_VIRTUAL_X_POST_GAIN_xDB  -12

#define DD_2p0_DEC_VIRTUAL_X_POST_GAIN_xDB  -6
#define DD_5p1_DEC_VIRTUAL_X_POST_GAIN_xDB  -1

#define DTS_2p0_DEC_VIRTUAL_X_POST_GAIN_xDB  -6
#define DTS_5p1_DEC_VIRTUAL_X_POST_GAIN_xDB  -3

#endif
#endif

//#define CLOCK_SYNC_WITH_MASTER_ENABLE

// #define DOLBY_DTS_DECODE_16BIT_OUT

// #define DSP_PRE_GAIN
#ifdef DSP_PRE_GAIN
#define DSP_PRE_ALL_DATA_GAIN_xDB   0

#define PCM_DEC_DSP_PRE_GAIN_xDB    (-12 + DSP_PRE_ALL_DATA_GAIN_xDB)

#define DD_DEC_DSP_PRE_GAIN_xDB     (-6 + DSP_PRE_ALL_DATA_GAIN_xDB)

#define DTS_DEC_DSP_PRE_GAIN_xDB    (-6 + DSP_PRE_ALL_DATA_GAIN_xDB)

#endif

// #define DSP_POST_GAIN
#ifdef DSP_POST_GAIN
#define PCM_DEC_DSP_POST_GAIN
#ifdef PCM_DEC_DSP_POST_GAIN
#define PCM_DEC_DSP_POST_GAIN_xDB  0
#endif

#define DD_DEC_DSP_POST_GAIN
#ifdef DD_DEC_DSP_POST_GAIN
#define DD_DEC_DSP_POST_GAIN_xDB  0
#endif

#define DTS_DEC_DSP_POST_GAIN
#ifdef DTS_DEC_DSP_POST_GAIN
#define DTS_DEC_DSP_POST_GAIN_xDB  0
#endif
#endif


/***********************************************
*******       AUDIO CLOCK SYN SET       ********
***********************************************/
#define AUDIO_CLOCK_SYNC_ENABLE

#ifdef AUDIO_CLOCK_SYNC_ENABLE
#define LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE
#define AUDIO_CLOCK_SYNC_PRINTF
//#define IIS_IN_SYNC_W_MASTER
#endif

/***********************************************
*******        save module set          ********
***********************************************/
#define SAVE_MODULE_ENABLE

#ifdef SAVE_MODULE_ENABLE
/*
 * #define SAVE_MODULE_FROM_EEPROM
 * #define SAVE_MODULE_FROM_FLASH
*/
#define SAVE_MODULE_FROM_EEPROM

#define SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
#ifdef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
#define SAVE_GLOBAL_VARI_STRU_DELAY_TIME      1000
#endif

#ifdef SAVE_MODULE_FROM_EEPROM
/*
 * #define I2C0_ENABLE
 * #define I2C1_ENABLE
 * #define I2C2_ENABLE
*/
#define I2C0_ENABLE

/*
 * #define EEPROM_AT24C01
 * #define EEPROM_AT24C02
 * #define EEPROM_AT24C04
 * #define EEPROM_AT24C08
 * #define EEPROM_FT24C64A
*/
#define EEPROM_AT24C08

#endif
#endif

/***********************************************
*****    AUDIO IN/OUT STREAM SET        ********
***********************************************/
#define FSK_GFSK_5P8G
#define FSK_GFSK_5P8G_MASTER 0
#define FSK_GFSK_5P8G_SLAVE 1
#define FSK_GFSK_5P8G_ROLE FSK_GFSK_5P8G_MASTER
#define LC3_ENC_5P8G_MODE
#define LC3_ENC_FIFO_BUF_PSRAM
/***********************************************
*******         AUDIO SOURCE SET        ********
***********************************************/
#define AUX_ENABLE
#define BT_ENABLE
// #define HDMI_ENABLE
#define OPTICAL_ENABLE
// #define COAXIAL_ENABLE
#define ARC_ENABLE
// #define USB_HOST_MSC_ENABLE
#define USB_SPEAKER_ENABLE


/***********************************************
*******         POWER ON/OFF SET        ********
***********************************************/
#define STANDBY_SOURCE_ENABLE

#ifdef STANDBY_SOURCE_ENABLE
#define SYSTEM_POWER_ON_TO_STANDBY_MODE

#ifdef SYSTEM_POWER_ON_TO_STANDBY_MODE
#define STANDBY_MODE_DELAY_RELEASE

#ifdef STANDBY_MODE_DELAY_RELEASE
#define STANBY_RELEASE_DELAY_TIME_MS    1000
#endif
#endif
#endif

/*
* HW SUPPORT SETTING
* hardware I2C support
* #define I2C0_ENABLE
* #define I2C1_ENABLE
* #define I2C2_ENABLE
* #define I2C3_ENABLE
* */

/***********************************************
*******         BT MODULE SET          ********
***********************************************/
#ifdef BT_ENABLE
#define BTDM52_ENABLE
//#define BT_MODULE_ENABLE
#endif

#ifdef BTDM52_ENABLE
//#define BT_TX_ROLE_ENABLE
#ifdef BT_TX_ROLE_ENABLE
#define SBC_ENCODE_ENABLE
#endif

//#define BT_HFP_ENABLE

#ifdef BT_HFP_ENABLE
#define mSBC_DEC
#define mSBC_ENC
#endif

//#define BLE_ENABLE

#ifdef SAVE_MODULE_FROM_EEPROM
#define BT_W_EEPROM
#endif

#define BT_RELINK_ENABLE
#define BT_PAIRING_ENABLE

#define BT_VOLUME_SET
#define BT_VOLUME_SYC
#endif


/***********************************************
*******         bt rf test SET          ********
***********************************************/
// #define BT_RF_TEST_ENABLE

#ifdef BT_RF_TEST_ENABLE
#define BT_RF_DUT_MODE_ENABLE
#ifdef BT_RF_DUT_MODE_ENABLE
#define BT_RF_DELAY_SET_DUT_MODE
#endif
#endif

/***********************************************
*******         HDMI MODULE SET         ********
***********************************************/
#ifdef HDMI_ENABLE
#define HDMI_TV_ARC_ENABLE

#define SPDIF_ENABLE
#ifdef SPDIF_ENABLE
#define HDMI_SPDIF_OUT_ENABLE
#endif
#define I2C0_ENABLE


// #define HDMI_ISR_ENABLE

#define HDMI_MODULE_IT66320
#define HDMI_MODULE_IT66320_RESET
#define HDMI_MODULE_IT66320_CUST_CEC
#define HDMI_MODULE_IT66320_CEC_FACTORY_TEST
#define HDMI_MODULE_IT66320_UPGRADE_ENABL
#endif


/***********************************************
*******       Optical MODULE SET        ********
***********************************************/
#ifdef OPTICAL_ENABLE
#define SPDIF_ENABLE
//#define SPDIF_OUT_ENABLE
//#define SPDIF_OUT_DIRECTLY
#endif

/***********************************************
*******       Coaxial MODULE SET        ********
***********************************************/
#ifdef COAXIAL_ENABLE
#define SPDIF_ENABLE
#endif


/***********************************************
*******         ARC MODULE SET          ********
***********************************************/
#ifdef ARC_ENABLE
#define SPDIF_ENABLE
#define CEC_ENABLE

#define OPTEK_ARC_CEC_ENABLE

#ifdef OPTEK_ARC_CEC_ENABLE
/*
only support OPTEK_HW_ARC_CEC_ENABLE or OPTEK_GPIO_ARC_CEC_ENABLE
if open OPTEK_GPIO_ARC_CEC_ENABLE,need add gpio cec lib file:optek_lib_cec_gpio.a
*/
#define OPTEK_HW_ARC_CEC_ENABLE
// #define OPTEK_GPIO_ARC_CEC_ENABLE
#endif

#ifdef OPTEK_GPIO_ARC_CEC_ENABLE
#define XTENSA_TIMER1_ENABLE
#endif

#if (defined OPTEK_HW_ARC_CEC_ENABLE || defined OPTEK_GPIO_ARC_CEC_ENABLE)
//#define CEC_CMD_RX_POLLING_ENABLE
#define ARC_PLUG_TIMER_ENABLE
#define ARC_UNPLUG_TIMER_ENABLE

#define SONY_TV_KD_43X85K
#define SONY_TV_KD_49X7500F
#define SAMSUNG_TV_UA43TU8000JXXZ
#define ONE_PLUS_TV_32HD2A00 //Produced in 2000
#define HISENSE_HZ32E35A
#define PHILIPS_40PFF5661_T3

//#define ARC_CEC_CERTIFICATION_ENABLE

#define ARC_PLUG_AT_STABNDBY_ENABLE
#define ARC_PLUG_AT_OTHER_SRC_ENABLE

#define CEC_SYNC_STATUS_VOLUME
#define CEC_SYNC_STATUS_MUTE
#define CEC_SYNC_STATUS_POWER_ON
#define CEC_SYNC_STATUS_POWER_STANBY
#define CEC_SYNC_STATUS_RELEASE_POWER_STANBY
#endif

#endif

/***********************************************
*******         USB MODULE SET          ********
***********************************************/
#ifdef USB_HOST_MSC_ENABLE
#define USB_HOST_ENABLE
#endif

#ifdef USB_SPEAKER_ENABLE
#define USB_DEV_ENABLE
#endif



/***********************************************
*******     USB MSC AND USB DEV SET     ********
***********************************************/
#ifdef USB_HOST_ENABLE

#define PROJECT_MP3_SKIP_ENABLE

/*
* link with libusbhost_msc_and_audio.a or libusbhost_audio_driver.a
* */
#define USB_HOST_LIB
#define FOR_ESD_PROTECT
#define IPOD_USB_AUDIO_CHARGE_ONLY
//#define USB_HOST_AUDIO_ENABLE
//#define TASK_HDD_PRIORITY_CHANGE

#define FILE_SYSTEM_ENABLE
//#define FILE_DELETE_ENABLE

#define FAST_SEARCH_ENABLE

#define WAV_DEC
#define MP3_DECODE_ENABLE
//#define CODEC_LIB_IN_MASK_ROM

#define USBHOST_OPEN_TIMER_ENABLE
#define USBHOST_CLOSE_TIMER_ENABLE
#define USBHOST_PWR_ON_TIMER_ENABLE

#endif //USB_HOST_ENABLE

#define UPGRADE_PROJECT_ENABLE
#define UPGRADE_ENABLE

#ifdef UPGRADE_ENABLE
#define UPGRADE_BY_USB
// #define UPGRADE_BY_BT
#endif


#ifdef USB_DEV_ENABLE
#define USB_SPEAKER
//#define USB_DEV_LIB
#define HID_FUNC_ENABLE
//#define USB_SPEAKER_AUTO_DETECT
#define USB_SPEAKER_VOL_ENABLE

#define USB_DEV_UPDATA_MIC_ENABLE

#ifdef HID_FUNC_ENABLE
#define HID_FUNC_TEST
#endif

#endif //USB_DEV_ENABLE


#if ( (defined USB_HOST_ENABLE) || (defined USB_DEV_ENABLE) )
#define USB_INT_CHANGEs_TO_LEVEL1
#endif

#define USB_OTG_INIT_FUN_CHANGED

//#define FAST_SEARCH_ENABLE
//#define FOLDER_SWITCH
#define AUTO_PLAY_ENABLE



//#define SYSTEM_CRASH_CHECK_ENABLE


/***********************************************
*******        DISPLAY MODULE SET       ********
***********************************************/
#define DISPLAY_MODULE_ENABLE
#ifdef DISPLAY_MODULE_ENABLE

#define LCD_MODULE_ENABLE
//#define LED_MODULE_ENABLE

#ifdef LCD_MODULE_ENABLE
#define LCD_ST7032
//#define OLED_ELW1301
// #define OLED_SP8140

#ifdef LCD_ST7032
#define SPI1_ENABLE
#endif

#ifdef OLED_ELW1301
#define SPI1_ENABLE
#endif

#ifdef OLED_SP8140
#define SPI2_ENABLE
#endif

#endif

#ifdef LED_MODULE_ENABLE
//#define LED_5050RGB
#define LED_GPIO_LIGHT

#ifdef LED_5050RGB
#define SPI1_ENABLE
#endif

#ifdef LED_GPIO_LIGHT
#define LED_GPIO_LIGHT_COUNT_MAX    1
#endif
#endif
#endif



/***********************************************
*******        audio device set         ********
***********************************************/
#define AUDIO_PROCESSOR

#ifdef AUDIO_PROCESSOR
#define AUDIO_OPTEK_DAPP

// #define AUDIO_SONA_DAPP

#ifdef AUDIO_SONA_DAPP
#define SONA_DSP_ENABLE
#ifdef SONA_DSP_ENABLE
#define SONA_DSP_PSRAM_BUF

#define SONA_DSP_DECODE_ENABLE
#ifdef SONA_DSP_DECODE_ENABLE
#define SONA_DSP_DECODE_INIT
#endif

/*
* SONA_DSP_AISOUND_ENABLE close
* used libXiaomiBar_L64_240613.a,and not include ai sound function
*
* SONA_DSP_AISOUND_ENABLE open
* used libXiaomiBar_L64_aisound.a,and include ai sound function
* */
#define SONA_DSP_AISOUND_ENABLE

//#define SONA_DSP_PRINTF_ENABLE

// #define SONA_DSP_MALLOC_PRINTF
// #define SONA_DSP_MALLOC_USED_CODE_MALLOC
#endif

#define SONA_DSP_PC_TOOLS_BY_UART
#ifdef SONA_DSP_PC_TOOLS_BY_UART
#define SONA_DSP_PC_TOOLS_BY_UART_0
// #define SONA_DSP_PC_TOOLS_BY_UART_1
// #define SONA_DSP_PC_TOOLS_BY_UART_2

// #define SONA_DSP_PC_TOOLS_PRINTF_BY_UART_0
// #define SONA_DSP_PC_TOOLS_PRINTF_BY_UART_1
#define SONA_DSP_PC_TOOLS_PRINTF_BY_UART_2

#define SONA_DSP_PC_TOOLS_DTS_VIRTUAL_X_SET

#define SONA_DSP_PC_TOOLS_DTSX_ENABLE
#ifdef SONA_DSP_PC_TOOLS_DTSX_ENABLE
// #define SONA_DSP_PC_TOOLS_DTSX_LOUDNESS_CONTROL_MODULE
#define SONA_DSP_PC_TOOLS_DTSX_VIRTUAL_X_MODULE
#define SONA_DSP_PC_TOOLS_DTSX_TBHDX_MODULE
// #define SONA_DSP_PC_TOOLS_DTSX_GRAPHIC_EQ_MODULE
// #define SONA_DSP_PC_TOOLS_DTSX_ACTIVE_EQ_MODULE
#define SONA_DSP_PC_TOOLS_DTSX_MBHL_MODULE

#ifdef SONA_DSP_PC_TOOLS_DTSX_LOUDNESS_CONTROL_MODULE

#endif

#ifdef SONA_DSP_PC_TOOLS_DTSX_VIRTUAL_X_MODULE
#define DTX_SET_GLOBAL_MODULE
#ifdef DTX_SET_GLOBAL_MODULE
#define DTX_SET_GLOBAL_MODULE_ENABLE_MODE
#define DTX_SET_GLOBAL_MODULE_DISCARD_MODE
// #define DTX_SET_GLOBAL_MODULE_INPUT_MODE
// #define DTX_SET_GLOBAL_MODULE_OUTPUT_MODE
#define DTX_SET_GLOBAL_MODULE_HEADROOM_GAIN_MODE
#define DTX_SET_GLOBAL_MODULE_PROCESSING_OUTPUT_GAIN_MODE
// #define DTX_SET_GLOBAL_MODULE_REFERENCE_LEVEL_MODE
#endif

#define DTX_SET_HORIZONTAL_MODULE
#ifdef DTX_SET_HORIZONTAL_MODULE
#define DTX_SET_HORIZONTAL_MODULE_ENABLE_MODE
#define DTX_SET_HORIZONTAL_MODULE_PASSIVE_MATRIX_MODE
#define DTX_SET_HORIZONTAL_MODULE_FRONT_SURROUND_MODE
#define DTX_SET_HORIZONTAL_MODULE_FRONT_WIDENING_MODE
#define DTX_SET_HORIZONTAL_MODULE_CS2TO3_MODE
#define DTX_SET_HORIZONTAL_MODULE_HORIZONTAL_VIRTUALIZATION_EFFECT_MODE
#define DTX_SET_HORIZONTAL_MODULE_PHANTOM_CENTER_GAIN_MODE
#define DTX_SET_HORIZONTAL_MODULE_CENTER_GAIN_MODE
#define DTX_SET_HORIZONTAL_MODULE_VIRTUAL_FRONT_MODE
#define DTX_SET_HORIZONTAL_MODULE_VIRTUAL_SURROUND_MODE
#define DTX_SET_HORIZONTAL_MODULE_LEF_GAIN_MODE
#define DTX_SET_HORIZONTAL_MODULE_LR_MIX_RATIO_TO_CENTER_MODE
#endif

#define DTX_SET_HEIGHT_MODULE
#ifdef DTX_SET_HEIGHT_MODULE
#define DTX_SET_HEIGHT_MODULE_ENABLE_MODE
#define DTX_SET_HEIGHT_MODULE_HEIGHT_UPMIX_MODE
#define DTX_SET_HEIGHT_MODULE_HEIGHT_MIX_COEFICIENT_MODE
#define DTX_SET_HEIGHT_MODULE_HEIGHT_CHAN_GAIN_MODE
#endif

#define DTX_SET_DEFINITION_MODULE
#ifdef DTX_SET_DEFINITION_MODULE
#define DTX_SET_DEFINITION_MODULE_DIALOG_CLARITY_ON_OFF_MODE
#define DTX_SET_DEFINITION_MODULE_DIALOG_CLARITY_CONTROL_MODE
#define DTX_SET_DEFINITION_MODULE_DEFINITION_ON_OFF_MODE
#define DTX_SET_DEFINITION_MODULE_DEFINITION_CONTROL_MODE
#endif

#define DTX_SET_TBHDx_BASE_MODULE
#ifdef DTX_SET_TBHDx_BASE_MODULE
// #define DTX_SET_TBHDx_BASE_MODULE_ENABLE_MODE
#define DTX_SET_TBHDx_BASE_MODULE_DISCARD_MODE
// #define DTX_SET_TBHDx_BASE_MODULE_DELAY_MATCHING_GAIN
#endif

#define DTX_SET_TBHDx_FRONT_MODULE
#ifdef DTX_SET_TBHDx_FRONT_MODULE
#define DTX_SET_TBHDx_FRONT_MODULE_ENABLE_MODE

#define DTX_SET_TBHDx_FRONT_MODULE_CUSTOM_SPEAKER_SIZE_MODE
#define DTX_SET_TBHDx_FRONT_MODULE_TEMPORAL_GAIN_MODE
#define DTX_SET_TBHDx_FRONT_MODULE_HP_RATIO_MODE
#define DTX_SET_TBHDx_FRONT_MODULE_APP_EXTENDED_BASS_MODE
#define DTX_SET_TBHDx_FRONT_MODULE_SPEAKER_SIZE_MODE

#define DTX_SET_TBHDx_FRONT_MODULE_INPUT_GAIN_MODE
#define DTX_SET_TBHDx_FRONT_MODULE_DYNAMICS_MODE
#define DTX_SET_TBHDx_FRONT_MODULE_HPF_ENABLE_MODE
#define DTX_SET_TBHDx_FRONT_MODULE_HPF_ORDER_MODE
#define DTX_SET_TBHDx_FRONT_MODULE_BASS_LEVEL_MODE
#define DTX_SET_TBHDx_FRONT_MODULE_EXTENDED_BASS_MODE
#endif


#define DTX_SET_TBHDx_CENTER_MODULE
#ifdef DTX_SET_TBHDx_CENTER_MODULE

#define DTX_SET_TBHDx_CENTER_MODULE_ENABLE_MODE

#define DTX_SET_TBHDx_CENTER_MODULE_CUSTOM_SPEAKER_SIZE_MODE
#define DTX_SET_TBHDx_CENTER_MODULE_TEMPORAL_GAIN_MODE
#define DTX_SET_TBHDx_CENTER_MODULE_HP_RATIO_MODE
#define DTX_SET_TBHDx_CENTER_MODULE_APP_EXTENDED_BASS_MODE
#define DTX_SET_TBHDx_CENTER_MODULE_SPEAKER_SIZE_MODE

#define DTX_SET_TBHDx_CENTER_MODULE_INPUT_GAIN_MODE
#define DTX_SET_TBHDx_CENTER_MODULE_DYNAMICS_MODE
#define DTX_SET_TBHDx_CENTER_MODULE_HPF_ENABLE_MODE
#define DTX_SET_TBHDx_CENTER_MODULE_HPF_ORDER_MODE
#define DTX_SET_TBHDx_CENTER_MODULE_BASS_LEVEL_MODE
#define DTX_SET_TBHDx_CENTER_MODULE_EXTENDED_BASS_MODE

#endif


#define DTX_SET_TBHDx_SURROUND_MODULE
#ifdef DTX_SET_TBHDx_SURROUND_MODULE

#endif


#define DTX_SET_TBHDx_REAR_MODULE
#ifdef DTX_SET_TBHDx_REAR_MODULE

#endif

#define DTX_SET_MBHL_BASE_MODULE
#ifdef DTX_SET_MBHL_BASE_MODULE
#define DTX_SET_MBHL_BASE_MODULE_ENABLE_MODE
#define DTX_SET_MBHL_BASE_MODULE_DISCARD_MODE
#endif

#define DTX_SET_MBHL_BOOST_MODULE
#ifdef DTX_SET_MBHL_BOOST_MODULE
#define DTX_SET_MBHL_BOOST_MODULE_REFERENCE_LEVEL_MODE
#endif

#define DTX_SET_MBHL_COMPR_LOW_MODULE
#ifdef DTX_SET_MBHL_COMPR_LOW_MODULE

#endif

#define DTX_SET_MBHL_COMPR_MID_MODULE
#ifdef DTX_SET_MBHL_COMPR_MID_MODULE

#endif


#define DTX_SET_MBHL_COMPR_HIGH_MODULE
#ifdef DTX_SET_MBHL_COMPR_HIGH_MODULE

#endif


#define DTX_SET_MBHL_LIMITER_MODULE
#ifdef DTX_SET_MBHL_LIMITER_MODULE
#define DTX_SET_MBHL_LIMITER_MODULE_THRESHOLD_MODE
#endif








#endif

#ifdef SONA_DSP_PC_TOOLS_DTSX_TBHDX_MODULE

#endif

#ifdef SONA_DSP_PC_TOOLS_DTSX_GRAPHIC_EQ_MODULE

#endif

#ifdef SONA_DSP_PC_TOOLS_DTSX_ACTIVE_EQ_MODULE

#endif

#ifdef SONA_DSP_PC_TOOLS_DTSX_MBHL_MODULE

#endif

#endif


#define SONA_DSP_PC_TOOLS_BY_UART_FIFO

#endif
#endif

// #define AUDIO_AMP_DEV_ENABLE
//#define AUDIO_AMP_DAPP_ENABLE

#ifdef AUDIO_AMP_DEV_ENABLE
//#define AUDIO_MA12070P
// #define AUDIO_NTP8928
// #define AUDIO_ACM8625
//#define AUDIO_AMP_AD82178
#define AUDIO_AMP_TAS5822M
#endif

#ifdef AUDIO_AMP_TAS5822M
#define I2C2_ENABLE
#endif

#ifdef AUDIO_AMP_AD82178
#define I2C2_ENABLE
#endif

#ifdef AUDIO_ACM8625
#define I2C0_ENABLE
#endif

#if defined(AUDIO_MA12070P) || defined(HDMI_MODULE_IT66320_UPGRADE_ENABL)
#define I2C1_ENABLE
#endif

#if ((defined AUDIO_OPTEK_DAPP) || (defined AUDIO_SONA_DAPP))
#define AUDIO_CODEC_USED_VOL
// #define AUDIO_CODEC_USED_BASS_TREBLE
#define AUDIO_CODEC_USED_EQ_MODE
#define AUDIO_CODEC_USED_AUDIO_SYNC
#define AUDIO_CODEC_USED_WOOFER
//#define AUDIO_CODEC_USED_MUTE
#endif

#ifdef AUDIO_AMP_DEV_ENABLE
#ifdef AUDIO_AMP_DAPP_ENABLE
#define AUDIO_AMP_USED_VOL
#define AUDIO_AMP_USED_BASS_TREBLE
#define AUDIO_AMP_USED_EQ_MODE
#define AUDIO_AMP_USED_MUTE
#endif//AUDIO_AMP_DAPP_ENABLE
#endif//AUDIO_AMP_DEV_ENABLE
#endif//AUDIO_PROCESSOR


/***********************************************
*******        DSP EQ tuning set        ********
***********************************************/
#define DSPEQ_TUNING_ENABLE

#ifdef DSPEQ_TUNING_ENABLE
#define DSPEQ_SETTING_ENABLE

#ifdef DSPEQ_SETTING_ENABLE
#define DSPEQ_TUNING_LR_CHANNEL
#define DSPEQ_TUNING_CENTER_CHANNEL
#define DSPEQ_TUNING_SUB_CHANNEL
#endif
#endif

/*
* if defined DSP_HIFI_LIB_OLD
* lib need used optek_lib_dsp_dapp_V1.0.8_Q30_old_mode_20240112.zip
*
* if not defined DSP_HIFI_LIB_OLD
* lib need used optek_lib_dsp_dapp_V1.0.a_Q30_for_HiFi4_20240321.zip
*/
//#define DSP_HIFI_LIB_OLD



// #define DSP_CCHIP_EQ_SETTING_ENABLE
/*
* if defined DSP_CCHIP_EQ_SETTING_16BIT_DECODE
* lib need used libcchipeq_16bit.a
*
* if defined DSP_CCHIP_EQ_SETTING_24BIT_DECODE
* lib need used libcchipeq_24bit.a
*/

#ifdef DSP_CCHIP_EQ_SETTING_ENABLE
// #define DSP_CCHIP_EQ_SETTING_16BIT_DECODE
#define DSP_CCHIP_EQ_SETTING_24BIT_DECODE
#endif


/***********************************************
*******         OPTEK Link set          ********
***********************************************/


/*******optek link macro*************/
//#define OPTEK_LINK_ENABLE

#ifdef OPTEK_LINK_ENABLE
#define OPL_MASTER_ENABLE
#define OPL_SLAVE_ENBALE

//#define OPL_MODE_SWF
#define OPL_MODE_SINGLE_WAY
#else
#define OPL_MODE_NULL
#endif


#if ((defined OPL_MODE_TWO_WAY) || \
    (defined OPL_MODE_SINGLE_WAY && defined OPL_MASTER_ENABLE)||\
    (defined OPL_MODE_TWS && defined OPL_MASTER_ENABLE)||\
    (defined OPL_MODE_WIRELESS_MIC && defined OPL_SLAVE_ENBALE)||\
    (defined OPL_MODE_THREE_WAY)||\
    (defined OPL_MODE_THREE_WAY_HIGH_QUALITY)||\
    (defined OPL_MODE_TWO_WAY_LONG_DISTANCE)||\
    (defined OPL_MODE_MICx2)||\
    (defined OPL_MODE_MICx2_HIGH_QUALITY))
#define LC3_ENCODE_ENABLE
#endif

#if ((defined OPL_MODE_TWO_WAY) ||\
    (defined OPL_MODE_SINGLE_WAY && defined OPL_SLAVE_ENBALE)||\
    (defined OPL_MODE_TWS && defined OPL_SLAVE_ENBALE)||\
    (defined OPL_MODE_WIRELESS_MIC && defined OPL_MASTER_ENABLE)||\
    (defined OPL_MODE_THREE_WAY)||\
    (defined OPL_MODE_THREE_WAY_HIGH_QUALITY)||\
    (defined OPL_MODE_TWO_WAY_LONG_DISTANCE)||\
    (defined OPL_MODE_MICx2)||\
    (defined OPL_MODE_MICx2_HIGH_QUALITY))
#define LC3_DECODE_ENABLE
#endif

/*******optek link macro end*************/


#define OPL_ROLE_MASTER  (1<<0)
#define OPL_ROLE_SLAVE   (1<<1)
#define OPL_ROLE_SLAVE2  (1<<2)
#define nonOPL_ROLE      (1<<3)    //classical bluetooth
#define OPL_ROLE_UNDEF   (0)




/***********************************************
*******           SPI TEST set          ********
***********************************************/
//#define SPI_TEST

#ifdef SPI_TEST
#define SPI1_ENABLE
//#define SPI_TX_TEST
#define SPI_RX_TEST
//#define SPI_DMA_TX_TEST
//#define SPI_DMA_RX_TEST
#endif



#define OUTPUT_FIFO_BUF_SET
#ifdef OUTPUT_FIFO_BUF_SET
#define AUDIO_DEC_DSP_BUF
#endif



#define DTS_DMP_DEC_HD_SUPPORT
//#define PRINTF_OUT_DOUBLE_FOLAT_ENABLE

#endif //__OEM_H__
