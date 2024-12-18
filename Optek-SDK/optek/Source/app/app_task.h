#ifndef __APP_TASK_H__
#define __APP_TASK_H__

#include "sdk_com.h"



/**
 ****************************************************************************************
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
typedef enum {
	KEY_DOWN,
	KEY_UP,
	KEY_HOLD,
	GRUOP_KEY_DOWN,
	GROUP_KEY_UP
} enumKEY_ACTIONs;

typedef enum {
	MAIN_BOARD_KEY,//local key
	REMOTE_KEY,
	CONSOLE_KEY,
	KEY_TYPE_MAX
} enumKeyType;

typedef enum {
	TIMER_KeyHold = 0,
	TIMER_CombKey,
	TIMER_WindowTimer,
	TIMER_BT_Relink,
	TIMER_BT_Relink_Reset,
	TIMER_BT_Relink_Fail,
	TIMER_BT_Pairing,
	TIMER_BT_Pairing_TimeOut,
	TIMER_BT_RF_DUT_MODE,
	TIMER_SAVE_GlobalVari,
	
	#ifdef IT66322_M031_ENABLE
	TIMER_IT66322_M031_GetAudioStatus,
	#endif

	#ifdef IT66322_M031_ENABLE
	TIMER_IT66322_M031_GetAudioStatus,
	TIMER_IT66322_M031_Init,
	#endif

	#ifdef HDMI_MODULE_IT66320//def IT66320
	TIMER_IT66320,
    #ifdef HDMI_MODULE_IT66320_UPGRADE_ENABL	
	TIMER_IT66320_UPGRADE,
	#endif
	#endif

	TIMER_IDLE,
	TIMER_SAVE_SYSTEM_DATA,

	TIMER_AMP_MUTE,

	#ifdef XIAOMI_S27_SWITCH_SOURCE_MODE_ENABLE
	TIMER_SET_SOURCE,
	#endif

    #ifdef SONA_DSP_PC_TOOLS_BY_UART_FIFO
	TIMER_Sona_UartData_Analyse,
    #endif

	TIMER_Upgrade_No_FW,


	TIMER_MAX,
} enAPP_TASK_TIMER_ID;


/*
 ****************************************************************************************
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void app_task_init(void);
void func_wo_param_msgSend ( pFUNCTION func );

void func_w_one_param_msgSend ( pFUNCTION func, U32 parm );
void func_w_two_param_msgSend ( pFUNCTION_W_TWO_PARM func, U32 parm1, U32 parm2 );

void key_msgSend ( U16 key_type , U16 action, U32 key_id);

void app_task_timer_ms_set(U32 timer_id, U32 ms);
U32 app_task_timer_ms_get(U32 timer_id);

#endif //__APP_TASK_H__
