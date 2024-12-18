#include "os_config.h"

#define CHAR_REVERSE

#ifdef OLED_SP8140
#include "hw_spi2.h"
#include "hw_timer.h"
#include "LCDdevice.h"
#include "OLED_SP8140.h"

#include "app_string_oem.h"
#include "xm_24X40_font.h"


//#define LCD_SPI1_USED

#define OLED_SP8140_FUNCTION_ENABLE
#define DBG_Printf

//#define RTOS
#ifdef RTOS
static void *mutex_qspi;
//void *mutex_spi;
#define OS_MUTEX_QSPI_LOCK		xSemaphoreTake( mutex_qspi, portMAX_DELAY)
#define OS_MUTEX_QSPI_UNLOCK	xSemaphoreGive( mutex_qspi)
#endif

#define VERTICAL_ADDRESS_INCREMENT
/*
  	(0) 	!	(1) 	"	(2) 	#	(3) 	$	(4) 	%	(5) 	&	(6) 	'	(7)		(
	(8) 	)	(9) 	*	(10) 	+	(11) 	,	(12) 	-	(13) 	.	(14) 	/	(15)	0
  	(16) 	1	(17) 	2	(18) 	3	(19) 	4	(20) 	5	(21) 	6	(22) 	7	(23)	8
  	(24) 	9	(25) 	:	(26) 	;	(27) 	<	(28) 	=	(29) 	>	(30) 	?	(31)	@
  	(32) 	A	(33) 	B	(34) 	C	(35) 	D	(36) 	E	(37) 	F	(38) 	G	(39)	H
  	(40) 	I	(41) 	J	(42) 	K	(43) 	L	(44) 	M	(45) 	N	(46) 	O	(47)	P
  	(48) 	Q	(49) 	R	(50) 	S	(51) 	T	(52) 	U	(53) 	V	(54) 	W	(55)	X
  	(56) 	Y	(57) 	Z	(58) 	[	(59) 	\	(60) 	]	(61) 	^	(62) 	_	(63)	`
  	(64) 	a	(65) 	b	(66) 	c	(67) 	d	(68) 	e	(69) 	f	(70) 	g	(71)	h
  	(72) 	i	(73) 	j	(74) 	k	(75) 	l	(76) 	m	(77) 	n	(78) 	o	(79)	p
  	(80) 	q	(81) 	r	(82) 	s	(83) 	t	(84) 	u	(85) 	v	(86) 	w	(87)	x
  	(88) 	y	(89) 	z	(90) 	{	(91) 	|	(92) 	}	(93) 	~	(94)
	ol(95)
	as(96)
	t(97)
	v(98)
	/(99)
	a(100)
	rc(101)
	updown(102)
	light all(103)
	horizontal(104)
	vertical(105)
*/

const unsigned char *F24X40[0x8A-0x20]=
{
gImage_XM__24X40_0x20,       
gImage_XM__24X40_0x21,       
gImage_XM__24X40_0x22,       
gImage_XM__24X40_0x23,       
gImage_XM__24X40_0x24,       
gImage_XM__24X40_0x25,       
gImage_XM__24X40_0x26,       
gImage_XM__24X40_0x27,       
gImage_XM__24X40_0x28,       
gImage_XM__24X40_0x29,       
gImage_XM__24X40_0x2a,       
gImage_XM__24X40_0x2b,       
gImage_XM__24X40_0x2c,       
gImage_XM__24X40_0x2d,       
gImage_XM__24X40_0x2e,       
gImage_XM__24X40_0x2f,       
gImage_XM__24X40_0x30_0,     
gImage_XM__24X40_0x31_1,     
gImage_XM__24X40_0x32_2,     
gImage_XM__24X40_0x33_3,     
gImage_XM__24X40_0x34_4,     
gImage_XM__24X40_0x35_5,     
gImage_XM__24X40_0x36_6,     
gImage_XM__24X40_0x37_7,     
gImage_XM__24X40_0x38_8,     
gImage_XM__24X40_0x39_9,     
gImage_XM__24X40_0x3a,       
gImage_XM__24X40_0x3b,       
gImage_XM__24X40_0x3c,       
gImage_XM__24X40_0x3d,       
gImage_XM__24X40_0x3e,       
gImage_XM__24X40_0x3f,       
gImage_XM__24X40_0x40,       
gImage_XM__24X40_0x41_A,     
gImage_XM__24X40_0x42_B,     
gImage_XM__24X40_0x43_C,     
gImage_XM__24X40_0x44_D,     
gImage_XM__24X40_0x45_E,     
gImage_XM__24X40_0x46_F,     
gImage_XM__24X40_0x47_G,     
gImage_XM__24X40_0x48_H,     
gImage_XM__24X40_0x49_I,     
gImage_XM__24X40_0x4a_J,     
gImage_XM__24X40_0x4b_K,     
gImage_XM__24X40_0x4c_L,     
gImage_XM__24X40_0x4d_M,     
gImage_XM__24X40_0x4e_N,     
gImage_XM__24X40_0x4f_O,     
gImage_XM__24X40_0x50_P,     
gImage_XM__24X40_0x51_Q,     
gImage_XM__24X40_0x52_R,     
gImage_XM__24X40_0x53_S,     
gImage_XM__24X40_0x54_T,     
gImage_XM__24X40_0x55_U,     
gImage_XM__24X40_0x56_V,     
gImage_XM__24X40_0x57_W,     
gImage_XM__24X40_0x58_X,     
gImage_XM__24X40_0x59_Y,     
gImage_XM__24X40_0x5a_Z,     
gImage_XM__24X40_0x5b,       
gImage_XM__24X40_0x5c,       
gImage_XM__24X40_0x5d,       
gImage_XM__24X40_0x5e,       
gImage_XM__24X40_0x5f,       
gImage_XM__24X40_0x60,       
gImage_XM__24X40_0x61_a,     
gImage_XM__24X40_0x62_b,     
gImage_XM__24X40_0x63_c,     
gImage_XM__24X40_0x64_d,     
gImage_XM__24X40_0x65_e,     
gImage_XM__24X40_0x66_f,     
gImage_XM__24X40_0x67_g,     
gImage_XM__24X40_0x68_h,     
gImage_XM__24X40_0x69_i,     
gImage_XM__24X40_0x6a_j,     
gImage_XM__24X40_0x6b_k,     
gImage_XM__24X40_0x6c_l,     
gImage_XM__24X40_0x6d_m,     
gImage_XM__24X40_0x6e_n,     
gImage_XM__24X40_0x6f_o,     
gImage_XM__24X40_0x70_p,     
gImage_XM__24X40_0x71_q,     
gImage_XM__24X40_0x72_r,     
gImage_XM__24X40_0x73_s,     
gImage_XM__24X40_0x74_t,     
gImage_XM__24X40_0x75_u,     
gImage_XM__24X40_0x76_v,     
gImage_XM__24X40_0x77_w,     
gImage_XM__24X40_0x78_x,     
gImage_XM__24X40_0x79_y,     
gImage_XM__24X40_0x7a_z,     
gImage_XM__24X40_0x7b,       
gImage_XM__24X40_0x7c,       
gImage_XM__24X40_0x7d,       
gImage_XM__24X40_0x7e,       
gImage_XM__24X40_0x7f,       
gImage_XM__24X40_0x80,       
gImage_XM__24X40_0x81,       
gImage_XM__24X40_0x82,       
gImage_XM__24X40_0x83,       
gImage_XM__24X40_0x84,       
gImage_XM__24X40_0x85,       
gImage_XM__24X40_0x86,       
gImage_XM__24X40_0x87,       
gImage_XM__24X40_0x88,       
gImage_XM__24X40_0x89,       	
};


#define	Oled_Rs_Command         Oled_Rs_Write(0); \
                                delayus(1)

#define	Oled_Rs_Data            Oled_Rs_Write(1); \
                                delayus(1)

#ifdef LCD_SPI1_USED

#define Oled_Write_Command(x)   Oled_Rs_Command;	\
								Spi_1_Write_Bits(x); 	\
                                delayus(1)

#define Oled_Write_Data(x)      Oled_Rs_Data;	\
								Spi_1_Write_Bits(x);\
                                delayus(1)

#define Oled_Screen_Clr  		Oled_Rs_Data;	\
								Spi_1_Write_byte_value_fixed(0, 64*40);

#define Oled_Screen_Full  		Oled_Rs_Data;	\
								Spi_1_Write_byte_value_fixed(0xFF, 64*40);								

#define OLED_SPI_SELECT         spi_1_transfer_size_set(8, 0)


#else

#define Oled_Write_Command(x)   Oled_Rs_Command;	\
								Spi_2_Write_Bits(x); 	\
                                delayus(1)

#define Oled_Write_Data(x)      Oled_Rs_Data;	\
								Spi_2_Write_Bits(x);\
                                delayus(1)

#define Oled_Screen_Clr  		Oled_Rs_Data;	\
								Spi_2_Write_byte_value_fixed(0, 64*40);

#define Oled_Screen_Full  		Oled_Rs_Data;	\
								Spi_2_Write_byte_value_fixed(0xFF, 64*40);								

#define OLED_SPI_SELECT         spi_2_transfer_size_set(8, 0)

#endif

U8 LcdDisp_Roll = FALSE;
U8 LcdDispBuf_Index;
U8 LcdFirstDispPos_Index;
U8 LcdDisp_Roll_cnt;

LCD_SCROLL lcdScroll;

//U8 Disp_Message_Buf[33];
SCROLL_MODE scroll_mode;
U8 scroll_line;
U8 scroll_wait;
U8 LcdDispBuf_End_Index;
const U8 *scroll_buf;
U8 scroll_fixed_index;
U8 scroll_fixed_buf_size;
U8 scroll_fixed_enable;
U8 scroll_clear_enable;


U8 buf_end[STRING_SIZE] __attribute__ ((section (".codec_slow_memory_buf")));
U8 fw_buf[STRING_SIZE] __attribute__ ((section (".codec_slow_memory_buf")));
U8 vol_buf[STRING_SIZE] __attribute__ ((section (".codec_slow_memory_buf")));
U8 audio_sync_buf[STRING_SIZE] __attribute__ ((section (".codec_slow_memory_buf")));
U8 bt_dev_name_buf[STRING_SIZE] __attribute__ ((section (".codec_slow_memory_buf")));

char display_buf[480] __attribute__ ((section (".codec_slow_memory_buf")));

#define MAX_SHOW_STR_NUM 5
#define BUF_BREAK_STR_NUM 1


void Oled_Rs_Write(U16 Logic)
{
 	DBG_Printf("%s\n\r", __func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE
	if ( Logic )
	{
        OLED_A0_PIN_HIGH;
	}
	else
	{
        OLED_A0_PIN_LOW;
	}
    #endif
}

void OLED_SET_CONTRAST_DARK(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE
	int ret;
    #ifdef RTOS
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	#endif

	char buf[2] = {0x81, 0x00};// set contrast

	Oled_Rs_Command;

	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 2);
	#else
	Spi_2_Write_Bytes(buf, 2);
	#endif



	#ifdef RTOS
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	#endif

	#endif
}

void OLED_SET_CONTRAST_NORMAL(void)
{
    DBG_Printf("%s\n\r",__func__);
	int ret;
    #ifdef OLED_SP8140_FUNCTION_ENABLE

    #ifdef RTOS
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	#endif	

	char buf[2] = {0x81, 0x90};// Set Contrast Control

	Oled_Rs_Command;

	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 2);
	#else
	Spi_2_Write_Bytes(buf, 2);
	#endif

	#ifdef RTOS
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	#endif

	#endif
}

void OLED_SetAll(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE
	int ret;
	int i,j;

    #ifdef RTOS
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	#endif	

	Oled_Write_Command(0x15);
	Oled_Write_Command(0x00);	//Set Column Start Line
	Oled_Write_Command(0x3F);
	Oled_Write_Command(0x75);//Set Row Start Line
	Oled_Write_Command(0x00);
	Oled_Write_Command(0x27); 

	for(i=0;i<64;i++)
	{
		for(j=0;j<40;j++)
		{ 
			Oled_Write_Data(0xff); 
		}
	}

	#ifdef RTOS
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	#endif

	#endif
}

//x:0~0F
//y:0~39
void OLED_SP8140_ShowChar(U8 x,U8 y,U8 chr)
{   
	//DBG_Printf("%s\n\r",__func__);
	#ifdef OLED_SP8140_FUNCTION_ENABLE
	int ret;
	int i=0,j=0;
	U8 c=0;
	unsigned char *pdata;

	c=chr-' ';

    #ifdef RTOS
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	#endif

	
	char buf[3];
	buf[0] = 0x15;//Set Column Start Line
	buf[1] = x;
    buf[2] = 0x3F;
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 3);
	#else
	Spi_2_Write_Bytes(buf, 3);
	#endif

	buf[0] = 0x75;//Set Row Start Line
	buf[1] = y;
    buf[2] = 0x27;
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 3);
	#else
	Spi_2_Write_Bytes(buf, 3);
	#endif

	pdata = F24X40[c];
	#ifndef VERTICAL_ADDRESS_INCREMENT
	for(i=0;i<40;i++)
	{
		for(j=0;j<12;j++)
			{ Oled_Write_Data(*pdata++); }
	}
	#else
	#ifdef CHAR_REVERSE

	for(i=0;i<12;i++)
	{
		for(j=39;j>=0;j--)
		{ 
			Oled_Write_Data(*(pdata +j*12+i)); 
		}
	}		
	#else
	for(i=0;i<12;i++)
	{
		for(j=0;j<40;j++)
			{ Oled_Write_Data(*(pdata +j*12+i)); }
	}	
	#endif

	#endif
	#ifdef RTOS
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	#endif

	#endif
		
}

void OLED_SP8140_ShowChar_test(U8 x,U8 y,U8 *buf)
{   
	OLED_SPI_SELECT;
	// printf_info("%s\n\r",__func__);
	#ifdef OLED_SP8140_FUNCTION_ENABLE
	int ret;
	U8 c=0,i=0,p,q;
	int j=0;
	U8 temp;
	unsigned char *pdata;

	// c=chr-' ';

    #ifdef RTOS
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	#endif

	Oled_Rs_Command;
	char cmd_buf[3];
	cmd_buf[0] = 0x15;//Set Column Start Line
	cmd_buf[1] = x;
	cmd_buf[2] = 0x3F;

	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(cmd_buf, 3);
	#else
	Spi_2_Write_Bytes(cmd_buf, 3);
	#endif
	delayus(10);

	Oled_Rs_Command;
	cmd_buf[0] = 0x75;//Set Row Start Line
	cmd_buf[1] = y;
	cmd_buf[2] = 0x27;
	
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(cmd_buf, 3);
	#else
	Spi_2_Write_Bytes(cmd_buf, 3);
	#endif
	delayus(10);
	

	#ifndef VERTICAL_ADDRESS_INCREMENT
	for(i=0;i<40;i++)
	{
		for(j=0;j<12;j++)
		{
			{ Oled_Write_Data(*pdata++);}
		}
	}
	#else

	Oled_Rs_Data;
	char *pBuf;
	char *pTemp_Buf;
	for(q=0; q<5; q++)
	{
		c = buf[q] - ' ';

		if(c >= 65 && c <= 90)//Convert lowercase letters to uppercase letters
		{
			c = c-32;
		}

		pBuf = (char *)F24X40[c];
		pTemp_Buf = display_buf;
		
		for (i=0; i<12; i++)
		{
			for(j=39; j>=0; j--)
			{
				*pTemp_Buf++ = pBuf[j*12+i];
			}
		}

		#ifdef LCD_SPI1_USED
		Spi_1_Write_Bytes(display_buf, 480);
		#else
		Spi_2_Write_Bytes(display_buf, 480);
		#endif
		//delayus(10);
	}

	// remain 8pixel fill space for mantis #2694 oled display issue.
	#ifdef LCD_SPI1_USED
	Spi_1_Write_byte_value_fixed(0x00, 160);
	#else
	Spi_2_Write_byte_value_fixed(0x00, 160);
	#endif
	/*
	delayus(10);
	*/
	#endif


	#ifdef RTOS
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	#endif

	#endif
}

void OLED_SP8140_clear_internal (LCD_RANGE lcd_range)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE
    int i;
    int j;
	int ret;

	/*
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	*/

	Oled_Write_Command(0x15);
	Oled_Write_Command(0x00);	//Set Column Start Line
	Oled_Write_Command(0x3F);
	Oled_Write_Command(0x75);//Set Row Start Line
	Oled_Write_Command(0x00);
	Oled_Write_Command(0x27); 

	for(i=0;i<40;i++)
	{
		for(j=0;j<64;j++)
		{ 
			Oled_Write_Data(0x00); 
		}
	}

	/*
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	*/
    #endif
}

void OLED_SP8140_init(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE
	int ret;
	char buf[3];

	OLED_POWER_PIN_INIT;

	//RES pin (VCI)
    OLED_RES_PIN_INIT;
    OLED_RES_PIN_HIGH;


    //A0 pin (RES)
    OLED_A0_PIN_INIT;
    OLED_A0_PIN_HIGH;

    //POWER pin
	OLED_POWER_PIN_HIGH;
    
    #ifdef RTOS
	mutex_qspi =  xSemaphoreCreateMutex();
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	#endif

    //spi mode SPI_MODE_0/SPI_MODE_1/SPI_MODE_2/SPI_MODE_4
    hw_spi_2_mode_set(SPI_MODE_0);

    OLED_SPI_SELECT;

	OLED_RES_PIN_LOW;
	delayms(1);
    OLED_RES_PIN_HIGH;
	delayms(1);

	buf[0] = 0xfd;// set command unlock
	buf[1] = 0x12;
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 2);
	#else
	Spi_2_Write_Bytes(buf, 2);
	#endif

    Oled_Write_Command(0xae);//display off 
    
	buf[0] = 0xa8;//set multiplex ratio 40
	buf[1] = 0x27;
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 2);
	#else
	Spi_2_Write_Bytes(buf, 2);
	#endif

	buf[0] = 0x15;//Set Column Address
	buf[1] = 0x00;
    buf[2] = 0x3F;
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 3);
	#else
	Spi_2_Write_Bytes(buf, 3);
	#endif

	buf[0] = 0x75;//Set Row Address
	buf[1] = 0x00;
    buf[2] = 0x27;
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 3);
	#else
	Spi_2_Write_Bytes(buf, 3);
	#endif

	buf[0] = 0xa1;//set display start line
	buf[1] = 0x00;
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 2);
	#else
	Spi_2_Write_Bytes(buf, 2);
	#endif

	buf[0] = 0xa2;//set display offset 
	buf[1] = 0x28;
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 2);
	#else
	Spi_2_Write_Bytes(buf, 2);
	#endif

	buf[0] = 0xa0;//set remap 
	#ifdef VERTICAL_ADDRESS_INCREMENT
	#ifdef CHAR_REVERSE
	buf[1] = 0x41|0x04;// column remap, |0x04 address vertical increment
	#else
	buf[1] = 0x42|0x04;	// |0x04 address vertical increment	
	#endif
	#else
	buf[1] = 0x42;
	#endif
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 2);
	#else
	Spi_2_Write_Bytes(buf, 2);
	#endif
	
	buf[0] = 0xab;// set vdd internal 
	buf[1] = 0x01; // 3.3V��0x00:1.8V 
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 2);
	#else
	Spi_2_Write_Bytes(buf, 2);
	#endif

	buf[0] = 0x81;// // set contrasr 
	buf[1] = 0x90; // 
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 2);
	#else
	Spi_2_Write_Bytes(buf, 2);
	#endif
    
	buf[0] = 0xb1;// Set Phase Length
	buf[1] = 0x51;// 
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 2);
	#else
	Spi_2_Write_Bytes(buf, 2);
	#endif
   
    buf[0] = 0xb3;// Set Display Clock Divide Ratio/Oscillator Frequency 
	buf[1] = 0x01;
    buf[2] = 0xb9;
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 3);
	#else
	Spi_2_Write_Bytes(buf, 3);
	#endif

	buf[0] = 0xbc;// set pre_charge voltage/VCOMH
	buf[1] = 0x04;// 
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 2);
	#else
	Spi_2_Write_Bytes(buf, 2);
	#endif
    
    buf[0] = 0xbe;// set VCOMH
	buf[1] = 0x00;// 
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 2);
	#else
	Spi_2_Write_Bytes(buf, 2);
	#endif

	buf[0] = 0xb6; // Set second pre-charge period
	buf[1] = 0x01;// 
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 2);
	#else
	Spi_2_Write_Bytes(buf, 2);
	#endif
    
    buf[0] = 0xd5; // enable second precharge and enternal vsl
	buf[1] = 0x62;// 
	Oled_Rs_Command;
	#ifdef LCD_SPI1_USED
	Spi_1_Write_Bytes(buf, 2);
	#else
	Spi_2_Write_Bytes(buf, 2);
	#endif
    
    Oled_Write_Command(0xa4); // Set Display Mode 
    OLED_SP8140_clear_internal(0); // Clear screen
	/*
    Oled_Write_Command(0xaf); // display on
	*/

	#ifdef RTOS
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	#endif

    #endif
}

void OLED_SP8140_open(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE
	int ret;

    #ifdef RTOS
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	#endif

	Oled_Write_Command(0xaf); 	// display on 

	#ifdef RTOS
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	#endif

    #endif
}

void OLED_SP8140_close(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE
	int ret;

    #ifdef RTOS
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	#endif

    Oled_Write_Command(0xae);	//display off 

	#ifdef RTOS
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	#endif

    #endif
}
void OLED_SP8140_fini(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE
	
    #endif
}


void OLED_SP8140_clear (LCD_RANGE lcd_range)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE
    int i;
    int j;
	int ret;

    #ifdef RTOS
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	#endif

	char buf[3];


	buf[0] = 0x15;
	buf[1] = 0x00;
    buf[2] = 0x3F;
	Oled_Rs_Command;
	Spi_2_Write_Bytes(buf, 3);

	buf[0] = 0x75;
	buf[1] = 0x00;
    buf[2] = 0x27;
	Oled_Rs_Command;
	Spi_2_Write_Bytes(buf, 3);


	Oled_Screen_Clr(0);


	#ifdef RTOS
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	#endif

    #endif
}
void OLED_SP8140_display_icon  (LCD_ICON lcd_icon, U32 param)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE

    #endif
}

void OLED_SP8140_display_char (const U8 *buf, U16 line, U16 pos)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE

    U8 j=0;
	int ret;
	U8 temp;

	#ifdef RTOS
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	#endif

	OLED_SPI_SELECT;

	DBG_Assert(line < MAX_OLED_LINE);
	DBG_Assert(pos < MAX_OLED_POS);


	OLED_SP8140_ShowChar_test(0,0,buf);

	#ifdef RTOS
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	#endif

    #endif
}

void OLED_SP8140_display_char_scroll (const U8 *buf, U16 line, U16 pos)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE

    U8 j=0;
	int ret;
	U8 temp;

	#ifdef RTOS
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	#endif

	OLED_SPI_SELECT;

	int i;
	for (i=0;i<100;i++)
	{
		if(i == strlen((const char *)buf))
		{
			break;
		}
		else
		{
			if (buf[i]!='\0')
			{
				OLED_SP8140_ShowChar(pos,line,buf[i]);
				pos+=12;	
				//if (pos>(0x3F-12)) break;					
			}
			else
			{
				printf_info("buf[%d]=='\0'\r\n",i);
			}
		}
		
	}
		
	#ifdef RTOS
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	#endif

    #endif
}


void OLED_SP8140_display_char_fixed_len (const U8 *buf, U16 line, U16 spos, U16 epos)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE

    #endif
}

void OLED_SP8140_display_full(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE
	int ret;
	int i,j;

    #ifdef RTOS
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	#endif	

	char buf[3];

	Oled_Write_Command(0x15);
	Oled_Write_Command(0x00);	//Set Column Start Line
	Oled_Write_Command(0x3F);
	Oled_Write_Command(0x75);//Set Row Start Line
	Oled_Write_Command(0x00);
	Oled_Write_Command(0x27); 

	
	Oled_Screen_Full;

	#ifdef RTOS
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	#endif

    #endif
}

/*****************************************************************************************************************
 * @brief:config srcoll string
 * @param: buf:display character string
 * 		   line:Configure the x-coordinates
 * 		   pos:Configure the y-coordinates
 * 		   mode:scroll mode
 * 		   wait_sec:how long to wait is displayed
 * 		   fixed_enable:Fixed a character starting to scroll
 * 		   fix_index:fixed a character position
 * 		   fixed_buf_size:display fixed_buf size
 * 		   clear_enable: enable : Scroll to the end of the last character
 * 						 disable: Scroll to the last character to fill the space and continue scrolling
 * 
 * @note:  NULL
 * 
 * @Sample usage:
 * 			OLED_SP8140_scroll_string(STRING_OPTICAL_APP,0,0,SCROLL_FOREVER,1,FIXED_DISABLE,0,0,CLEAR_DISENABLE);
 * 
******************************************************************************************************************/
//if length > MAX_LCD_MODULE_POS, static display in x seconds,then scroll
void OLED_SP8140_scroll_string(const U8 *buf, U16 line, U16 pos, SCROLL_MODE mode, U8 wait_sec, U8 fixed_enable, U8 fixed_index, U8 fixed_buf_size, U8 clear_enable)
{
    DBG_Printf("%s\n\r",__func__);
	U16 size = 0;

//----------------disable scroll first

	LcdDisp_Roll = 0;
	LcdDispBuf_Index = 0;
	LcdDisp_Roll_cnt = 0;
	scroll_mode = mode;
	scroll_line = line;
	scroll_buf = buf;
	scroll_wait = wait_sec;

	scroll_fixed_enable = fixed_enable;
	scroll_fixed_buf_size = fixed_buf_size;
	scroll_fixed_index = fixed_index;

	scroll_clear_enable = clear_enable;
	
	LcdDispBuf_End_Index = 0;

//----------------display it
	app_lcd_device.display_char (buf, line, pos);

	if(scroll_fixed_enable)
	{
		size = fixed_buf_size;
	}
	else
	{
		size = strlen(buf);
	}

	/*
	//printf_info("buf : %d\n\r",size);
	if(0)//(size <= MAX_SHOW_STR_NUM)
	{
		LcdDisp_Roll = FALSE;
	}
	else
	*/
	{
		if ( (mode == SCROLL_ONCE) ||
		 (mode == SCROLL_TWICE) ||
		 (mode == SCROLL_FOREVER) )
		{
			LcdDisp_Roll = TRUE;

			if (wait_sec > 0)
			{

			}
			else
			{

			}
		}
		else if (mode == SCROLL_RDS_TEXT)
		{
			LcdDisp_Roll = TRUE;
		}

	}

}

void OLED_SP8140_disp_scroll(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE
	if (LcdDisp_Roll)
	{
		U8 display = TRUE;
		U8 len;

		U8 ret_len = 0;
		U8 i;
		
		if(scroll_fixed_enable)
		{
			len = scroll_fixed_buf_size;	

			ret_len = len - LcdDispBuf_Index;

			memset(buf_end, 0, scroll_fixed_buf_size);
			buf_end[0] = scroll_buf[0];

			memset(buf_end+scroll_fixed_index, ' ', MAX_SHOW_STR_NUM - scroll_fixed_index);
			
			if(ret_len < MAX_SHOW_STR_NUM)
			{
				if(LcdDispBuf_Index == len)
				{
					if(LcdDispBuf_End_Index == 0)
					{
						memcpy(buf_end + BUF_BREAK_STR_NUM+scroll_fixed_index, scroll_buf+scroll_fixed_index, MAX_SHOW_STR_NUM - BUF_BREAK_STR_NUM - scroll_fixed_index);
						LcdDispBuf_End_Index++;

						if (display)
						{
							app_lcd_device.display_char (buf_end, scroll_line, 0);
						}
					}
					else
					{
						if(LcdDispBuf_End_Index <= BUF_BREAK_STR_NUM)
						{
							if(LcdDispBuf_End_Index != BUF_BREAK_STR_NUM)
							{
								for(i = scroll_fixed_index; i < (BUF_BREAK_STR_NUM - LcdDispBuf_End_Index); i++)
								{
									buf_end[i] = ' ';
								}
							}
							
							memcpy(buf_end + (BUF_BREAK_STR_NUM - LcdDispBuf_End_Index) + scroll_fixed_index, scroll_buf + scroll_fixed_index, MAX_SHOW_STR_NUM - (BUF_BREAK_STR_NUM - LcdDispBuf_End_Index) + scroll_fixed_index);

							LcdDispBuf_End_Index++;	

							if (display)
							{
								app_lcd_device.display_char (buf_end, scroll_line, 0);
							}
						}
						else
						{
							LcdDispBuf_Index = 0;
							LcdDisp_Roll_cnt++;
							LcdDispBuf_End_Index = 0;

							if ( (scroll_mode == SCROLL_ONCE) ||
								( (scroll_mode == SCROLL_TWICE) && (LcdDisp_Roll_cnt >= 2) ) ||
								(scroll_mode == SCROLL_RDS_TEXT) )
							{
								LcdDisp_Roll = FALSE;
							}
							else if (scroll_mode == SCROLL_FOREVER)
							{
								if (scroll_wait > 0)
								{

								}
								else
								{

								}
							}

							if (display)
							{
								app_lcd_device.display_char ((const U8 *) scroll_buf+LcdDispBuf_Index, scroll_line, 0);
							}
						}
					}
					
				}
				else
				{
					if((MAX_SHOW_STR_NUM - ret_len)  == 0)
					{
						memcpy(buf_end+scroll_fixed_index, scroll_buf+LcdDispBuf_Index+scroll_fixed_index, MAX_SHOW_STR_NUM-scroll_fixed_index);
						LcdDispBuf_Index++;
					}
					else if((MAX_SHOW_STR_NUM - ret_len) < (BUF_BREAK_STR_NUM))
					{
						memcpy(buf_end+scroll_fixed_index, scroll_buf+LcdDispBuf_Index+scroll_fixed_index, ret_len);

						for(i = scroll_fixed_index; i < (MAX_SHOW_STR_NUM - ret_len); i++)
						{
							buf_end[ ret_len + i] = ' ';
						}

						LcdDispBuf_Index++;
					}
					else
					{
						memcpy(buf_end+scroll_fixed_index, scroll_buf+LcdDispBuf_Index, ret_len);

						if(ret_len + BUF_BREAK_STR_NUM <= MAX_SHOW_STR_NUM)
						{
							for(i = scroll_fixed_index; i < (BUF_BREAK_STR_NUM); i++)
							{
								buf_end[ ret_len + i] = ' ';
							}

							for(i = scroll_fixed_index; i < (MAX_SHOW_STR_NUM -  ret_len - BUF_BREAK_STR_NUM); i++)
							{
								buf_end[ ret_len + BUF_BREAK_STR_NUM + i] = *(scroll_buf+i);
							}
						}
						else
						{
							for(i = scroll_fixed_index; i < (BUF_BREAK_STR_NUM - ret_len); i++)
							{
								buf_end[ ret_len + i] = ' ';
							}
						}
						
						LcdDispBuf_Index++;
						
					}

					if (display)
					{
						app_lcd_device.display_char (buf_end, scroll_line, 0);
					}
				}
				
			}
			else
			{
				if (display)
				{
					memcpy(buf_end+scroll_fixed_index, scroll_buf+LcdDispBuf_Index+scroll_fixed_index, MAX_SHOW_STR_NUM - scroll_fixed_index);
					app_lcd_device.display_char (buf_end, scroll_line, 0);
				}

				LcdDispBuf_Index++;
			}
		}
		else
		{
			len = strlen((const char *)scroll_buf);

			ret_len = len - LcdDispBuf_Index;

			if(ret_len < MAX_SHOW_STR_NUM)
			{
				memset(buf_end, ' ', sizeof(buf_end));

				if(LcdDispBuf_Index == len)
				{
					if(LcdDispBuf_End_Index == 0)
					{
						if(scroll_clear_enable)
						{
							global_vari.display_scroll_finish = TRUE;

							app_lcd_device.display_char (buf_end, scroll_line, 0);

						}
						else
						{
							memcpy(buf_end + (BUF_BREAK_STR_NUM - LcdDispBuf_End_Index), scroll_buf, MAX_SHOW_STR_NUM - (BUF_BREAK_STR_NUM - LcdDispBuf_End_Index));
						}
						
						LcdDispBuf_End_Index++;

						if (display)
						{
							app_lcd_device.display_char (buf_end, scroll_line, 0);
						}
					}
					else
					{
						if(LcdDispBuf_End_Index <= BUF_BREAK_STR_NUM)
						{
							if(LcdDispBuf_End_Index != BUF_BREAK_STR_NUM)
							{
								for(i = 0; i < (BUF_BREAK_STR_NUM - LcdDispBuf_End_Index); i++)
								{
									buf_end[i] = ' ';
								}
							}
		
							if(scroll_clear_enable)
							{

							}
							else
							{
								memcpy(buf_end + (BUF_BREAK_STR_NUM - LcdDispBuf_End_Index), scroll_buf, MAX_SHOW_STR_NUM - (BUF_BREAK_STR_NUM - LcdDispBuf_End_Index));
							}
							
							LcdDispBuf_End_Index++;	

							if (display)
							{
								app_lcd_device.display_char (buf_end, scroll_line, 0);
							}

							if((scroll_mode == SCROLL_FOREVER))
							{
								LcdDispBuf_Index = 0;
								LcdDisp_Roll_cnt++;
								LcdDispBuf_End_Index = 0;
							}
						}
						else
						{
							LcdDispBuf_Index = 0;
							LcdDisp_Roll_cnt++;
							LcdDispBuf_End_Index = 0;

							if ( (scroll_mode == SCROLL_ONCE) ||
								( (scroll_mode == SCROLL_TWICE) && (LcdDisp_Roll_cnt >= 2) ) ||
								(scroll_mode == SCROLL_RDS_TEXT) )
							{
								LcdDisp_Roll = FALSE;
							}
							else if (scroll_mode == SCROLL_FOREVER)
							{
								if (scroll_wait > 0)
								{

								}
								else
								{

								}
							}

							if (display)
							{
								if(scroll_clear_enable)
								{

								}
								else
								{
									app_lcd_device.display_char ((const U8 *) scroll_buf+LcdDispBuf_Index, scroll_line, 0);
								}
								
							}
						}
					}
					
				}
				else
				{
					if((MAX_SHOW_STR_NUM - ret_len)  == 0)
					{
						memcpy(buf_end, scroll_buf+LcdDispBuf_Index, MAX_SHOW_STR_NUM);
						LcdDispBuf_Index++;
					}
					else if((MAX_SHOW_STR_NUM - ret_len) < (BUF_BREAK_STR_NUM))
					{
						memcpy(buf_end, scroll_buf+LcdDispBuf_Index, ret_len);

						for(i = 0; i < (MAX_SHOW_STR_NUM - ret_len); i++)
						{
							buf_end[ ret_len + i] = ' ';
						}
						
						LcdDispBuf_Index++;
					}
					else
					{
						memcpy(buf_end, scroll_buf+LcdDispBuf_Index, ret_len);

						if(ret_len + BUF_BREAK_STR_NUM <= MAX_SHOW_STR_NUM)
						{
							for(i = 0; i < (BUF_BREAK_STR_NUM); i++)
							{
								buf_end[ ret_len + i] = ' ';
							}

							for(i = 0; i < (MAX_SHOW_STR_NUM -  ret_len - BUF_BREAK_STR_NUM); i++)
							{

								if(scroll_clear_enable)
								{

								}
								else
								{
									buf_end[ ret_len + BUF_BREAK_STR_NUM + i] = *(scroll_buf+i);
								}
							}
							
						}
						else
						{
							for(i = 0; i < (BUF_BREAK_STR_NUM - ret_len); i++)
							{
								buf_end[ ret_len + i] = ' ';
							}
						}
						
						LcdDispBuf_Index++;
						
					}

					if (display)
					{
						app_lcd_device.display_char (buf_end, scroll_line, 0);
					}
				}
				
			}
			else
			{
				if(scroll_mode == SCROLL_FOREVER)
				{
					if(LcdDispBuf_Index == 0)
					{
						LcdDispBuf_Index++;
					}
				}
				
				if (display)
				{
					app_lcd_device.display_char ((const U8 *) scroll_buf+LcdDispBuf_Index, scroll_line, 0);
				}

				LcdDispBuf_Index++;
			}
	
		}
		
	}
	#endif
}

void OLED_SP8140_disp_vol(U8 vol)
{
    #ifdef OLED_SP8140_FUNCTION_ENABLE

	#endif
}

void OLED_SP8140_disp_source(U8 audio_source)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef OLED_SP8140_FUNCTION_ENABLE
	
	app_lcd_device.display_char("     ", 0, 0);
		
	switch (audio_source)
	{
		case SOURCE_AUX:
			app_lcd_device.display_char("    AUX Mode    ", 0, 0);
			break;

		case SOURCE_ARC:
			app_lcd_device.display_char("    ARC Mode    ", 0, 0);
			break;

		case SOURCE_HDMI:
			app_lcd_device.display_char(STRING_HDMI_APP, 0, 0);
			break;

		case SOURCE_COAXIAL:
			app_lcd_device.display_char(STRING_COAXIAL_APP, 0, 0);
			break;

		case SOURCE_OPTICAL:
			app_lcd_device.display_char(STRING_OPTICAL_APP, 0, 0);
			break;			

		case SOURCE_BT:
			app_lcd_device.display_char(STRING_BT_APP, 0, 0);
			break;

		case SOURCE_USB_MSC:
			app_lcd_device.display_char(STRING_USB_HOST_APP, 0, 0);
			break;

		case SOURCE_USB_SPEAKER:
			app_lcd_device.display_char("  USB Speaker   ", 0, 0);
			break;

		case SOURCE_INTERNAL:
			app_lcd_device.display_char(" Internal Audio ", 0, 0);
			break;

		#ifdef HDMI_TV_ARC_ENABLE
		case SOURCE_HDMI_TV_ARC:
			app_lcd_device.display_char(STRING_ARC_EARC_APP, 0, 0);
			break;
		#endif

		default:
			app_lcd_device.display_char("                ", 0, 0);
			break;
	}
	#endif
}

void OLED_SP8140_disp_music_play(U32 music_level)
{
	
}

void OLED_SP8140_disp_audio_sync(U8 value)
{
    #ifdef OLED_SP8140_FUNCTION_ENABLE

	#endif
}

void OLED_SP8140_disp_fw(U8 value, U8 *in_buf, U8 sub_enable)
{
    #ifdef OLED_SP8140_FUNCTION_ENABLE

	U8 len = strlen(in_buf);

	memset(fw_buf, ' ', sizeof(fw_buf));
	
	memcpy(fw_buf, in_buf, len);
	if(value < 10 && value >= 0)
	{
		if(len == 3)
		{
			fw_buf[len] = '0';
			fw_buf[len+1] = value + '0';
		}
		else
		{
			fw_buf[len+1] = '0';
			fw_buf[len+2] = value + '0';
		}
	}
	else 
	{
		if(len == 3)
		{
			fw_buf[len] = ((value/10) + '0');
			fw_buf[len+1] = ((value%10) + '0');
		}
		else
		{
			if(value >= 100)
			{
				fw_buf[len]   = ((value/100) + '0');
				fw_buf[len+1] = ((value%100/10) + '0');
				fw_buf[len+2] = ((value%100%10) + '0');
			}
			else
			{
				fw_buf[len+1] = ((value/10) + '0');
				fw_buf[len+2] = ((value%10) + '0');
			}
		}

	}
	app_lcd_device.display_char ((const U8 *)fw_buf, 0, 0);
	
	#endif
}

void app_nav_lcd_test(U8 con)
{
	printf_info("%s,%d\n\r",__func__,con);
	U8 *cbuf = "OPTEK";
	app_lcd_device.display_char (cbuf, 0, 0);
}

void app_display_buf_init(void)
{
	memset(buf_end, 0, sizeof(buf_end));
	memset(display_buf, ' ', sizeof(display_buf));
}
#endif //OLED_SP8140_FUNCTION_ENABLE

