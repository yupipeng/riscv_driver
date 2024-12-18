/*
 * FT24C64A.h
 *
 *  Created on: 2024/3/7
 *      Author: Windows
 */

#ifndef __FT24C64A_H__
#define __FT24C64A_H__

#ifdef EEPROM_FT24C64A
#define EEPROM_PAGE_SIZE      256
#define EEPROM_PAGE_BYTE      32 //32 bytes

void FT24c64A_Init(void);
void FT24c64A_Open(void);
void FT24c64A_Close(void);
void FT24c64A_Finish(void);
U8 FT24c64A_ReadPage(U16 page_addr, BYTE *pbuf, WORD page_len);
U8 FT24c64A_WritePage(U16 page_addr, BYTE *pbuf, WORD page_len);
U8 FT24c64A_Write(U16 page_addr, BYTE *pbuf, WORD len);
U8 FT24c64A_Read(U16 page_addr, BYTE *pbuf, WORD len);

#define EEPROM_DEV_FT24C64A		FT24c64A_Init, \
								FT24c64A_Open, \
								FT24c64A_Close, \
								FT24c64A_Finish, \
								FT24c64A_ReadPage, \
								FT24c64A_WritePage, \
								FT24c64A_Read,	\
								FT24c64A_Write

#endif

#endif /* __FT24C64A_H__ */
