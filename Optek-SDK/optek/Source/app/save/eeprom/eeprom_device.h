/*
 * eeprom_device.h
 *
 *  Created on: 2024/2/20
 *      Author: Windows
 */

#ifndef __EEPROM_DEVICE_H__
#define __EEPROM_DEVICE_H__

#include "oem.h"

void eeprom_device_Init(void);
void eeprom_device_Open(void);
void eeprom_device_Close(void);
U8 eeprom_device_ReadPage(U16 page_addr, BYTE *pbuf, WORD page_len);
U8 eeprom_device_WritePage(U16 page_addr, BYTE *pbuf, WORD page_len);
U8 eeprom_device_Write(U16 page_addr, BYTE *pbuf, WORD len);
U8 eeprom_device_Read(U16 page_addr, BYTE *pbuf, WORD len);

typedef struct {
	void (*init) (void);
	void (*open) (void);
	void (*close) (void);
	void (*fini) (void);
	void (*ReadPage) (U16 page_addr, BYTE *pbuf, WORD page_len);
	void (*WritePage) (U16 page_addr, BYTE *pbuf, WORD page_len);
	void (*Read) (U16 page_addr, BYTE *pbuf, WORD len);
	void (*Write) (U16 page_addr, BYTE *pbuf, WORD len);
} SAVE_EEPROM_MOUDLE;
extern const SAVE_EEPROM_MOUDLE eepromDev;


#ifdef EEPROM_AT24C02

#elif (defined EEPROM_AT24C08)

#elif (defined EEPROM_AT24C16)

#elif (defined EEPROM_FT24C64A)

#else
void eeprom_device_none_void(void);
U8 eeprom_device_none_ReadPage(U16 page_addr, BYTE *pbuf, WORD page_len);
U8 eeprom_device_none_WritePage(U16 page_addr, BYTE *pbuf, WORD page_len);
U8 eeprom_device_none_Read(U16 page_addr, BYTE *pbuf, WORD len);
U8 eeprom_device_none_Write(U16 page_addr, BYTE *pbuf, WORD len);

#define EEPROM_DEV_NONE			eeprom_device_none_void, \
								eeprom_device_none_void, \
								eeprom_device_none_void, \
								eeprom_device_none_void, \
								eeprom_device_none_ReadPage, \
								eeprom_device_none_WritePage, \
								eeprom_device_none_Read,	\
								eeprom_device_none_Write
#endif

#endif /* __EEPROM_DEVICE_H__ */
