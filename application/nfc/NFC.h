#ifndef _NFC_
#define _NFC_

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

#define SUCCESS  0
#define FAIL     1

#include <stdint.h>
void ms520_write_reg(uint8_t reg, const uint8_t value);
uint8_t ms520_read_reg(uint8_t reg);

void ms520_reset_low(void);
void ms520_reset_high(void);

#define WriteReg(Addr,Data)  ms520_write_reg(Addr,Data)
#define ReadReg(Addr)        ms520_read_reg(Addr)

#define NFC_WAKEUP ms520_reset_high()                   //����MS52X
#define NFC_SLEEP  ms520_reset_low()                    //Ӳ������
#define OPEN_ANTENNA   WriteReg(0x14,0x83)              //������
#define CLOSE_ANTENNA  WriteReg(0x14,0x80)              //�ر�����
#include <stdint.h>
//**********************��ʱ����**********************//
extern void delay_4us(int x);
extern void delay_ms(uint32_t ms);

//*************************SPI����************************//

// extern char ReadReg(char Address);
// extern void WriteReg(char Address,char Data);

//*************************A������****************************//
extern unsigned char TypeAInit();
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

#endif