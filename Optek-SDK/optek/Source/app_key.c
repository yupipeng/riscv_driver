#include "app_key.h"

#define V_TOLERANCE 0x16  // it is about 0.08v

static const KEY_AD_ENTRY oem_main_kbd_entry0[] = {

	// #ifdef PROJECT_OTK6267x_V3P1 ****
	{(0x000 - 0x00000000), (0x000 + V_TOLERANCE ), APP_KEY_SOURCE},  		//Source
	{(0x031 - V_TOLERANCE ), (0x031 + V_TOLERANCE ), APP_KEY_PLAY},    	//Play/Pause
	{(0x07B - V_TOLERANCE ), (0x07B + V_TOLERANCE ), APP_KEY_PLUS},    	//Vol +
	{(0x0C6 - V_TOLERANCE ), (0x0C6 + V_TOLERANCE ), APP_KEY_MINUS},   	//Vol -
	{(0x118 - V_TOLERANCE ), (0x118 + V_TOLERANCE ), APP_KEY_EQ},      	//EQ0
	// #endif
};


static const KEY_AD_ENTRY *oem_main_kbd_map[] = {
	oem_main_kbd_entry0
	// oem_main_kbd_entry1,
	// oem_main_kbd_entry2
};

APP_KEY vAdMapMainKey (U8 ad_channel, U16 ad_val)
{
	KEY_AD_ENTRY *p ;
	APP_KEY key = APP_KEY_INVALID;

	for (p = oem_main_kbd_map[ad_channel]; p->key != APP_KEY_INVALID; p++)
	{
		if ( (ad_val >= p->ad_min) && (ad_val <= p->ad_max) )
		{
			key = p->key;
			break;
		}
	}

	return key;
}