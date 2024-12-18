#ifndef __OLED_SP8140_H__
#define __OLED_SP8140_H__

//A0 pin
#define OLED_A0_PIN_INDEX       GPIO0_18_INDEX
#define OLED_A0_PIN_INIT        hal_gpio_mux_func_select(OLED_A0_PIN_INDEX, MUX_SEL_GPIO_OUTPUT)
#define OLED_A0_PIN_HIGH        hal_gpio_output_level_set(OLED_A0_PIN_INDEX, GPIO_HIGH)
#define OLED_A0_PIN_LOW         hal_gpio_output_level_set(OLED_A0_PIN_INDEX, GPIO_LOW)

//RS pin
#define OLED_RES_PIN_INDEX      GPIO0_19_INDEX
#define OLED_RES_PIN_INIT       hal_gpio_mux_func_select(OLED_RES_PIN_INDEX, MUX_SEL_GPIO_OUTPUT)
#define OLED_RES_PIN_HIGH       hal_gpio_output_level_set(OLED_RES_PIN_INDEX, GPIO_HIGH)
#define OLED_RES_PIN_LOW        hal_gpio_output_level_set(OLED_RES_PIN_INDEX, GPIO_LOW)

//power pin
#define OLED_POWER_PIN_INDEX      GPIO0_08_INDEX
#define OLED_POWER_PIN_INIT       hal_gpio_mux_func_select(OLED_POWER_PIN_INDEX, MUX_SEL_GPIO_OUTPUT)
#define OLED_POWER_PIN_HIGH       hal_gpio_output_level_set(OLED_POWER_PIN_INDEX, GPIO_HIGH)
#define OLED_POWER_PIN_LOW        hal_gpio_output_level_set(OLED_POWER_PIN_INDEX, GPIO_LOW)

//device support pos addr 0~0x0F;line addr 0~39
#define	MAX_OLED_POS			5*3
#define MAX_OLED_LINE			1

#define STRING_SIZE   256

extern U8 bt_dev_name_buf[STRING_SIZE];


void Oled_Rs_Write(U16 Logic);

#ifdef DISPLAY_MODULE_ENABLE

void OLED_SP8140_init(void);
void OLED_SP8140_open(void);
void OLED_SP8140_close(void);
void OLED_SP8140_fini(void);
void OLED_SP8140_clear (LCD_RANGE lcd_range);
void OLED_SP8140_display_icon (LCD_ICON, U32);
void OLED_SP8140_display_char (const U8 *, U16, U16);
void OLED_SP8140_display_char_fixed_len (const U8 *buf, U16 line, U16 spos, U16 epos);
void OLED_SP8140_display_full(void);

void OLED_SP8140_disp_scroll(void);
void OLED_SP8140_disp_vol(U8 vol);
void OLED_SP8140_disp_source(U8 audio_source);
void OLED_SP8140_disp_music_play(U32 music_level);
void OLED_SP8140_disp_audio_sync(U8 value);
void OLED_SP8140_disp_fw(U8 value, U8 *in_buf, U8 sub_enable);

void app_nav_lcd_test(U8 con);
void app_display_buf_init(void);
void OLED_SET_CONTRAST_DARK(void);
#endif

#define SP8140		OLED_SP8140_init,	\
					OLED_SP8140_open,	\
					OLED_SP8140_close, \
					OLED_SP8140_fini,	\
					OLED_SP8140_clear, \
					OLED_SP8140_display_icon,	\
					OLED_SP8140_display_char, \
					OLED_SP8140_display_char_fixed_len, \
					NULL,\
					OLED_SP8140_display_full,\
					NULL,\
					NULL,\
					NULL,\
					NULL,\
					OLED_SP8140_scroll_string,\
					OLED_SP8140_disp_scroll,\
					OLED_SP8140_disp_vol,\
					OLED_SP8140_disp_audio_sync,\
					OLED_SP8140_disp_fw,\
					OLED_SP8140_disp_source,\
					OLED_SP8140_disp_music_play

#endif /* __OLED_SP8140_H__ */
