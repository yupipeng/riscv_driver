/*
 * FT24C64A.c
 *
 *  Created on: 2024/3/7
 *      Author: Windows
 */
#include "os_config.h"

#include "debug.h"
#include "hw_timer.h"

#ifdef EEPROM_FT24C64A
#include "app_i2c1.h"
#include "app_save.h"

#include "FT24C64A.h"

#define EEPROM_FT24C64A_FUNCTION_ENABLE
//#define DBG_Printf


#ifdef EMBEDED_HW_I2C
#define FT24c64A_read_sub    hw_i2c0_read_sub
#define FT24c64A_write_sub   hw_i2c0_write_sub
#elif (defined I2C2_ENABLE)
#define FT24c64A_read_sub    I2C2_read_sub_FT24C64A
#define FT24c64A_write_sub   I2C2_write_sub_FT24C64A
#elif (defined I2C1_ENABLE)
#define FT24c64A_read_sub    I2C1_read_sub_FT24C64A
#define FT24c64A_write_sub   I2C1_write_sub_FT24C64A
#elif (defined I2C0_ENABLE)
#define FT24c64A_read_sub    I2C0_read_sub
#define FT24c64A_write_sub   I2C0_write_sub
#else
#error have not setting i2c channel
#endif

#define I2C_ADDRESS    0xA8


void FT24c64A_Init(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef EEPROM_FT24C64A_FUNCTION_ENABLE

    #endif
}
void FT24c64A_Open(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef EEPROM_FT24C64A_FUNCTION_ENABLE

    #endif
}
void FT24c64A_Close(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef EEPROM_FT24C64A_FUNCTION_ENABLE

    #endif
}

void FT24c64A_Finish(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef EEPROM_FT24C64A_FUNCTION_ENABLE

    #endif
}
U8 FT24c64A_ReadPage(U16 page_addr, BYTE *pbuf, WORD page_len)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef EEPROM_FT24C64A_FUNCTION_ENABLE

    #endif
}
U8 FT24c64A_WritePage(U16 page_addr, BYTE *pbuf, WORD page_len)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef EEPROM_FT24C64A_FUNCTION_ENABLE

    #endif
}


U8 FT24c64A_Write(U16 page_addr, BYTE *pbuf, WORD len)
{
    DBG_Printf("%s\n\r",__func__);

    #ifdef EEPROM_FT24C64A_FUNCTION_ENABLE
	U8 i = 0;
	U8 j = 0;
	U8 *wbuf;
	U32 r;
	volatile U8 temp;
	U8 i2c_addr = 0;
	U16 len1;

	wbuf = pbuf;
	len1 = len / (EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE);

	for (j=0; j<len1; j++)
	{
		for (i=0; i<EEPROM_PAGE_SIZE; i++)
		{
			i2c_addr = I2C_ADDRESS;
			r = FT24c64A_write_sub (i2c_addr, page_addr, wbuf, EEPROM_PAGE_BYTE);
			page_addr += EEPROM_PAGE_BYTE;
			DBG_Assert(r == I2C_SUCCESS);
			wbuf += EEPROM_PAGE_BYTE;

			delayms(4); // min delay 4ms
		}
	}

	len1 = len - len1*(EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE);

	temp = len1/ EEPROM_PAGE_BYTE;
	for (i=0; i<temp; i++)
	{
		i2c_addr = I2C_ADDRESS;		
		r = FT24c64A_write_sub (i2c_addr, page_addr, wbuf, EEPROM_PAGE_BYTE);
		page_addr += EEPROM_PAGE_BYTE;
		DBG_Assert(r == I2C_SUCCESS);
		wbuf += EEPROM_PAGE_BYTE;

		delayms(4); // min delay 4ms		
	}

	temp = len1%EEPROM_PAGE_BYTE;
	if (temp)
	{
		i2c_addr = I2C_ADDRESS;
		r = FT24c64A_write_sub (i2c_addr, page_addr, wbuf, temp);
		DBG_Assert(r == I2C_SUCCESS);

		delayms(4); // min delay 4ms
	}

	if (r == I2C_SUCCESS)
	{
		return TRUE;
	}
	
	return FALSE;
	#endif
}

U8 FT24c64A_Read(WORD page_addr, BYTE *pbuf, WORD len)
{
    DBG_Printf("%s\n\r",__func__);

    #ifdef EEPROM_FT24C64A_FUNCTION_ENABLE
	U8 i = 0;
	U8 j = 0;
	U8 *rbuf;
	U32 r = I2C_SUCCESS;
	volatile U8 temp;
	U8 i2c_addr = 0;
	U16 len1;

	rbuf = pbuf;
	len1 = len / (EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE);

	for (j=0; j<len1; j++)
	{
		for (i=0; i<EEPROM_PAGE_SIZE; i++)
		{
			i2c_addr = I2C_ADDRESS;
			r = FT24c64A_read_sub (i2c_addr, page_addr, rbuf, EEPROM_PAGE_BYTE);
			page_addr += EEPROM_PAGE_BYTE;
			DBG_Assert(r == I2C_SUCCESS);
			delayms(8);										// min delay 4ms
			rbuf += EEPROM_PAGE_BYTE;
		}
	}

	len1 = len - len1*(EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE);

	temp = len1/ EEPROM_PAGE_BYTE;
	for (i=0; i<temp; i++)
	{
		i2c_addr = I2C_ADDRESS;
		r = FT24c64A_read_sub (i2c_addr, page_addr, rbuf, EEPROM_PAGE_BYTE);
		page_addr += EEPROM_PAGE_BYTE;
		DBG_Assert(r == I2C_SUCCESS);
		delayms(8);										// min delay 4ms
		rbuf += EEPROM_PAGE_BYTE;
	}

	temp = len1%EEPROM_PAGE_BYTE;
	if (temp)
	{
		i2c_addr = I2C_ADDRESS;
		r = FT24c64A_read_sub (i2c_addr, page_addr, rbuf, temp);
		DBG_Assert(r == I2C_SUCCESS);
		delayms(8);										// min delay 4ms
	}

	if (r == I2C_ERROR)
	{
		memset(pbuf, 0, len);
	}

	if (r == I2C_SUCCESS)
	{
		return TRUE;
	}
	
	return FALSE;
    #endif

}
#endif
