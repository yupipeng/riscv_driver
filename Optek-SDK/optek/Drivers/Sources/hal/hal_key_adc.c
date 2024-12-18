
#include "regmap.h"

#include "hw_saradc.h"

#include "hal_key_adc.h"

#include "app_key.h"
#include "key_scan.h"
/*
* MAX_SIMULT_KEY mean how many button key support when Press button at the same time
* 
* */
#define MAX_SIMULT_KEY         4   


void hal_key_adc_init(void)
{
	#ifdef HW_SARADC_ENABLE
	hw_sadc_init();
	#endif
}

U8 hal_key_adc_detect(APP_KEY *app_keys, U8 max_simult)
{
	APP_KEY ad_key_id = APP_KEY_INVALID;

	if (max_simult <= 0)
	{
		DBG_Assert(FALSE);
		my_printf_dma("hal_key_adc_detect max_simult == %d\n\r", MAX_SIMULT_KEY);
		return 0;
	}

	U8 key_adc_channel_cnt = 0;

    #if   (AD_USED_NUMBER == 1)
    	U16 kdata[1];
    #elif (AD_USED_NUMBER == 2)
       	U16 kdata[2];
    #elif (AD_USED_NUMBER == 3)
    	U16 kdata[3];
    #else

    #endif

	#ifdef BATT_CH_AD_SCAN_ENABLE
	batt_val_det_en = TRUE;
	U16 *pbatt_val = batt_val;
	#else
	U8 batt_val_det_en = FALSE;
	U16 *pbatt_val = NULL;	
	#endif

	#if (AD_USED_NUMBER > 0)

	if (key_adc_channel_cnt < max_simult)
	{
		#if (AD_USED_NUMBER == 1)
		#ifdef HW_SARADC_ENABLE
		hw_saradc_read_dma(kdata , 1);
		// hw_saradc_oneAdKey_read(kdata, AD_SCAN_FIRST_CH, batt_val_det_en, pbatt_val);
		#else
		kdata[0] = 0x3ff;    
		#endif

		if (kdata[0] < ADC_INVALID_VAL2)
		{
			ad_key_id = vAdMapMainKey(0, kdata[0]);
			my_printf_dma("the keyid is:%d\r\n", ad_key_id);

		}
		if (APP_KEY_INVALID != ad_key_id)
		{
			app_keys[key_adc_channel_cnt++] = ad_key_id;
		}

	    #elif (AD_USED_NUMBER == 2)
		#ifdef HW_SARADC_ENABLE
		hw_saradc_twoAdKey_read(kdata, AD_SCAN_FIRST_CH, AD_SCAN_SECOND_CH, batt_val_det_en, pbatt_val);
		#else
		kdata[0] = 0x3ff;
		kdata[1] = 0x3ff;
		#endif

       	if (kdata[0] < ADC_INVALID_VAL2)
		{
			ad_key_id = vAdMapMainKey(0, kdata[0]);
			if (APP_KEY_INVALID != ad_key_id)
			{
				app_keys[key_adc_channel_cnt++] = ad_key_id;//		
			}
		}

		if (key_adc_channel_cnt < max_simult)
		{
			if (kdata[1] < ADC_INVALID_VAL2)
			{
				ad_key_id = vAdMapMainKey(1, kdata[1]);
				if (APP_KEY_INVALID != ad_key_id)
				{
					app_keys[key_adc_channel_cnt++] = ad_key_id;//		
				}
			}			
		}

    	#elif (AD_USED_NUMBER == 3)
		#ifdef HW_SARADC_ENABLE
		hw_saradc_twoAdKey_read(kdata, AD_SCAN_FIRST_CH, AD_SCAN_SECOND_CH, AD_SCAN_THIRD_CH, batt_val_det_en, pbatt_val);
		#else
		kdata[0] = 0x3ff;
		kdata[1] = 0x3ff;
		kdata[2] = 0x3ff;
		#endif

       	if (kdata[0] < ADC_INVALID_VAL2)
		{
			ad_key_id = vAdMapMainKey(0, kdata[0]);
			if (APP_KEY_INVALID != ad_key_id)
			{
				app_keys[key_adc_channel_cnt++] = ad_key_id;
			}
		}

		if (key_adc_channel_cnt < max_simult)
		{
			if (kdata[1] < ADC_INVALID_VAL2)
			{
				ad_key_id = vAdMapMainKey(1, kdata[1]);
				if (APP_KEY_INVALID != ad_key_id)
				{
					app_keys[key_adc_channel_cnt++] = ad_key_id;
				}
			}			
		}

		if (key_adc_channel_cnt < max_simult)
		{
			if (kdata[2] < ADC_INVALID_VAL2)
			{
				ad_key_id = vAdMapMainKey(2, kdata[2]);
				if (APP_KEY_INVALID != ad_key_id)
				{
					app_keys[key_adc_channel_cnt++] = ad_key_id;
				}
			}			
		}		
		#endif
	}

	#else

	{
		#ifdef BATT_CH_AD_SCAN_ENABLE
		batt_val_det_en = TRUE;
		U16 *pbatt_val = batt_val;
		app_saradc_oneAdCh_read(pbatt_val, BATT_CH);
		#endif		
	}

	#endif

	return key_adc_channel_cnt;	
}


