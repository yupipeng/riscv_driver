#ifndef _NFC_
#define _NFC_

//#include "msp430x44x.h"
#include <stdint.h>

#define SPEED_106 0x00
#define SPEED_212 0x10
#define SPEED_424 0x20
#define SPEED_848 0x30
#define SPEED_1696 0x40
#define SPEED_3392 0x50

#define _NOCARD            0
#define _S50               1
#define _S70               2
#define _DESFIRE           3
#define _Pro               4

#define NFC_SUCCESS  0
#define NFC_FAIL     1

#define NFC_WAKEUP     P3OUT |=  (1<<5)                 //����MS52X
#define NFC_SLEEP      P3OUT &= ~(1<<5)                 //Ӳ������
#define OPEN_ANTENNA   WriteReg(0x14,0x83)              //������
#define CLOSE_ANTENNA  WriteReg(0x14,0x80)              //�ر�����

//**********************��ʱ����**********************//
extern void delay_4us(int x);
extern void delay_ms(uint32_t x);

//*************************SPI����************************//
extern void MS430SPIInit(void);
extern char ReadReg(char Address);
extern void WriteReg(char Address,char Data);

//*************************A������****************************//
unsigned char TypeAInit(void);
extern unsigned char REQA(void);
extern unsigned char AntiCollisionLoop(void);
extern unsigned char ANTICOLLISION(char CascadeLevel);
extern unsigned char SELECT(char CascadeLevel);
extern unsigned char MFAuthent(unsigned blockNum,unsigned char *key);
extern unsigned char ReadBlock(unsigned char blockNum,unsigned char *data);
extern unsigned char WriteBlock(unsigned char blockNum,unsigned char *data);

//**************************ATQA����˵��****************************//
//                0x0044 = Mifare_UltraLight                        //
//                0x0004 = Mifare_One(S50)                          //
//                0x0002 = Mifare_One(S70)                          //
//                0x0008 = Mifare_Pro(X)                            //
//                0x0344 = Mifare_DESFire                           //
//******************************************************************//
//*****************************PICC����*****************************//
typedef struct FormatPICC
{
  struct ATQA                            //Answer To Request, Type A//
  {
      unsigned char MSB;
      unsigned char LSB;
  }ATQA;
  unsigned char UID[15];      //include UID check byte and cascade tag;   �����������2
  unsigned char SAK;          //
}FormatPICC;

extern FormatPICC PICC;

#endif

