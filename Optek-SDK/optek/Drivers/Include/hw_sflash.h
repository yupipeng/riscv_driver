#ifndef _HW_SFLASH_H_
#define _HW_SFLASH_H_


#define SFLASH_PROGRAM_WITHOUT_CACHE

//#define PROGRAM_EASE_SUSPEND_ENABLE


//256 Bytes
#define FLASH_PAGE_SIZE            64//128//256

//4 kBytes
#define FLASH_SECTOR_SIZE          (1024*4)  //min erasing sector is 4k. 

//64 kBytes
#define FLASH_BLOCK_SIZE           (1024*64)

extern volatile int sflash_in_programming;

void hw_sflash_sector_erase(U32 sector_addr);
void hw_sflash_sector_write(U32 sector_addr, U32 *pBuf, U32 len);
void hw_sflash_sector_read(U32 sector_addr, U32 *pBuf, U32 len);

#endif //_HW_SFLASH_H_

