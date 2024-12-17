// #include "os_config.h"

#include "regmap.h"

#include "hw_gpio.h"
#include "hw_sflash.h"

#include "hal_dma.h"

void hw_sflash_sector_erase(U32 addr) __attribute__ ((section (".internal_ram_1_text")));
void hw_sflash_sector_write(U32 addr, U32 *pBuf, U32 len) __attribute__ ((section (".internal_ram_1_text")));
static U8 hw_sflash_page_write(U32 addr, U32 *pBuf, U32 len) __attribute__ ((section (".internal_ram_1_text")));
static U32 Endian_32_Convert(U32 addr) __attribute__ ((section (".internal_ram_1_text")));

volatile int sflash_in_programming = 0;

static U32 Endian_32_Convert(U32 addr)
{
	U8 *pAddr;
	U8 *pSwapAddr;
	U32 tmp;

	pAddr = (U8 *)&addr;
	pSwapAddr = (U8 *)&tmp;

	*pSwapAddr++ =  *(pAddr + 3);
	*pSwapAddr++ =  *(pAddr + 2);
	*pSwapAddr++ =  *(pAddr + 1);
	*pSwapAddr =  *(pAddr + 0);

	return tmp;
}

void hw_sflash_sector_erase(U32 addr)
{
	u8 rd_status1;
	u8 rd_status2;
	SPI_FLASH_STATUS1s *pSatus1 = (SPI_FLASH_STATUS1s *) &rd_status1;
	#ifdef PROGRAM_EASE_SUSPEND_ENABLE
	SPI_FLASH_STATUS2s *pSatus2 = (SPI_FLASH_STATUS2s *) &rd_status2;
	#endif

	while (1)
	{
		rd_status1 = ADDR_SPI_STATUS;
		if (pSatus1->busy == 0)
		{
			break;
		}
	}

	ADDR_SPI_WR_ENABLE = 0x0;

	#ifdef PROGRAM_EASE_SUSPEND_ENABLE
	sflash_in_programming = 1;
	#endif

	ADDR_SPI_SECTOR_ERASE = Endian_32_Convert(addr);

	while (1)
	{
		rd_status1 = ADDR_SPI_STATUS;
		rd_status2 = ADDR_SPI_STATUS2;

		#ifdef PROGRAM_EASE_SUSPEND_ENABLE
		if 	(pSatus2->sus)
		{
			ADDR_SPI_RESUME = 0;
			sflash_in_programming = 1;
		}
		else if (pSatus1->busy == 0)
		{
			break;
		}
		#else
		if (pSatus1->busy == 0)
		{
			break;
		}
		#endif
	}

	#ifdef PROGRAM_EASE_SUSPEND_ENABLE
	sflash_in_programming = 0;
	#endif
}

static U8 hw_sflash_page_write(U32 page_addr, U32 *pBuf, U32 len)
{
	int i;
	U32 dummy;
	u8 *p8_dest,*p8_src;
	U32 *p32_dest,*p32_src;
	u8 rd_status1, rd_status2;
	SPI_FLASH_STATUS1s *pSatus1 = (SPI_FLASH_STATUS1s *) &rd_status1;

	#ifdef PROGRAM_EASE_SUSPEND_ENABLE
	SPI_FLASH_STATUS2s *pSatus2 = (SPI_FLASH_STATUS2s *) &rd_status2;
	#endif

	U32 integral_of_4 = (len + 3)/4;

	DBG_Assert( ((page_addr%FLASH_PAGE_SIZE) == 0) );

	while (1)
	{
		rd_status1 = ADDR_SPI_STATUS;
		if (pSatus1->busy == 0)
		{
			break;
		}
	}

	ADDR_SPI_WR_ENABLE = 0x0;

	TX_INTERRUPT_SAVE_AREA;

	TX_DISABLE;

	ADDR_SPI_FAST_PAGE_PROGRAM = Endian_32_Convert(page_addr);

	p32_dest = (U32 *)page_addr;
	p32_src = pBuf;

	for (i=0; i<integral_of_4; i++)
	{
		p32_dest[i] = p32_src[i];
	}


	#ifdef PROGRAM_EASE_SUSPEND_ENABLE
	sflash_in_programming = 1;
	#endif

	dummy = ADDR_SPI_PROGRAM_START; //start program

	TX_RESTORE;

	while (1)
	{
		rd_status1 = ADDR_SPI_STATUS;

		#ifdef PROGRAM_EASE_SUSPEND_ENABLE
		rd_status2 = ADDR_SPI_STATUS2;
		if 	(pSatus2->sus)
		{
			ADDR_SPI_RESUME = 0;
			sflash_in_programming = 1;
		}
		else if (pSatus1->busy == 0)
		{
			break;
		}
		#else
		if (pSatus1->busy == 0)
		{
			break;
		}
		#endif
	}

	#ifdef PROGRAM_EASE_SUSPEND_ENABLE
	sflash_in_programming = 0;
	#endif

	return TRUE;
}

void hw_sflash_sector_write(U32 sector_addr, U32 *pBuf, U32 len)
{
	#ifdef SFLASH_PROGRAM_WITHOUT_CACHE
	U32 no_cache_addr;

	no_cache_addr = (sector_addr & 0x00FFFFFF) | 0X11000000;

	while (len)
	{
		if (len > FLASH_PAGE_SIZE)
		{
			len -= FLASH_PAGE_SIZE;
			hw_sflash_page_write(no_cache_addr, pBuf, FLASH_PAGE_SIZE);
			pBuf += FLASH_PAGE_SIZE/4;
			no_cache_addr += FLASH_PAGE_SIZE;
		}
		else
		{
			hw_sflash_page_write(no_cache_addr, pBuf, len);
			len = 0;
			break;
		}
	}
	#else
	U32 addr = sector_addr;

	while (len)
	{
		if (len > FLASH_PAGE_SIZE)
		{
			len -= FLASH_PAGE_SIZE;
			hw_sflash_page_write(sector_addr, pBuf, FLASH_PAGE_SIZE);
			pBuf += FLASH_PAGE_SIZE/4;
			addr += FLASH_PAGE_SIZE;
		}
		else
		{
			hw_sflash_page_write(sector_addr, pBuf, len);
			len = 0;
			break;
		}
	}
	#endif
}

void hw_sflash_sector_read(U32 sector_addr, U32 *pBuf, U32 len)
{

}