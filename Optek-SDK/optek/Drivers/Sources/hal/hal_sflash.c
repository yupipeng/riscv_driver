/**
 ****************************************************************************************
 *
 * @file hal_sflash.c
 *
 * @brief sflash hal asynchronous read and write data implement.
 *
 ****************************************************************************************
 */


#include "regmap.h"
#include "hal_sflash.h"
#include <stdint.h>
/*
static U8 tempBuf[FLASH_SECTOR_SIZE];
*/

U32 hal_sflash_sector_erase(U32 sector_addr)
{
    U32 ret = TRUE;

    hw_sflash_sector_erase(sector_addr);


    return ret;
}


U8 hal_sflash_erase_verify(U32 addr, U32 byte_len)
{
	U32 *pAddr;
	U32 tmp;
	U32 i;

	// DBG_FlashAssert(addr >= SPI_FLASH_BASE_ADDR);

	if (byte_len == 0)
	{
		return FALSE;
	}

	pAddr = (U32 *) addr;


	//if not calling the fucntion below, verifing is error!
	// DataCache_Clean();


	for (i=0; i<(byte_len/4); i++)
	{
		tmp = *pAddr++;
		if (tmp != 0xFFFFFFFF)
		{
			// extern void uart_write_string(const char *str);
			// extern void uart_write_number_int(uint32_t num);
			// uart_write_string("Erase Check Err:addr-0x");
			// uart_write_number_int(addr);
			// uart_write_string("-");
			// uart_write_number_int(i);
			// uart_write_string("\n\r");
			// uart_write_string("Val:0x");
			// uart_write_number_int(tmp);
			// uart_write_string("\n\r");

			return FALSE;
		}
	}

	return TRUE;
}

U8 hal_sflash_write_verify(U32 addr, U32 *pBuf, U32 byte_len)
{
	U32 tmp;
	U32 *pSrc;
	U32 *pDest;
	U8 *pSrc_Byte;
	U8 *pDest_Byte;
	U8 ret = TRUE;
	long offset;
	long temp_dest, temp_src;

	// DBG_FlashAssert(addr >= SPI_FLASH_BASE_ADDR);
	// DBG_FlashAssert( ( !(addr & 0x03) ) );

	if (byte_len == 0)
	{
		return FALSE;
	}

	pSrc = pBuf;
	// U32 no_cache_addr;

	// no_cache_addr = (addr & 0x00FFFFFF) | 0X11000000;
	// pDest = (U32 *) no_cache_addr;
	pDest = (U32 *) addr;
	offset = 0;

	tmp = byte_len;
	tmp = ((tmp + 3)/4)*4; //added on Sep 3th,2020

	//if not calling the fucntion below, verifing is error!
	// DataCache_Clean();


	while (tmp >= 4)
	{
		//for debug
		temp_dest = *pDest++;
		temp_src = *pSrc++;
		if (temp_dest != temp_src)

		{
			extern void uart_write_string(const char *str);
			extern void uart_write_number_int(uint32_t num);
			uart_write_string("src:0x");
			uart_write_number_int(pSrc-1);
			uart_write_string("-0x");
			uart_write_number_int(temp_src);
			uart_write_string("\ndst:0x");
			uart_write_number_int(pDest-1);
			uart_write_string("-0x");
			uart_write_number_int(temp_dest);
			uart_write_string("\n\r");
			ret = FALSE;
			break;
		}

		tmp -= 4;
		offset += 4;
	}

	return ret;
}

/**
 ****************************************************************************************
 * @brief HAL layer Flash write function
 *
 * @param[in] flash_addr  Address in the Flash to start writing
 * @param[in] pBuf        Pointer to the data buffer to be written
 * @param[in] len         Length of the data to be written in bytes
 *
 * @return None
 ****************************************************************************************
 */
U32 hal_sflash_write(U32 flash_sectot_addr, U8 *pBuf, U32 len)
{
    // DBG_Assert( (flash_sectot_addr >= FLASH_START_ADDR) && (flash_sectot_addr <= FLASH_END_ADDR) );

    hw_sflash_sector_write(flash_sectot_addr, pBuf, len);

    return TRUE;
}

U32 hal_sflash_read(U32 flash_addr, U8 *pBuf, U32 byte_len)
{

    if ( ! (flash_addr >= FLASH_START_ADDR && flash_addr <= FLASH_END_ADDR) )
    {
        return FALSE;
    }

    if (pBuf == NULL)
    {
        return FALSE;
    }

    // DataCache_Clean();

    U8 *pSrc = (U8 *)flash_addr;
    U8 *pDest = (U8 *)pBuf;
    // memcpy(pDest, (void *)pSrc, byte_len);
	for (U32 i = 0; i < byte_len; i++)
	{
		*pDest++ = *pSrc++;
	}

    return TRUE;
}