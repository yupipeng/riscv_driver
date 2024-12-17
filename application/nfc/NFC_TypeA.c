#include "NFC.h"
#include <stdio.h>

FormatPICC PICC;

//*************************************************************//
//funcname:TypeAInit
//function:MS520 A����ʼ��
//parameter:
//return:
//Author:Mr.Wang
//Date:2018-10-19
//**************************************************************/
unsigned char TypeAInit()     //�Ĵ�������
{
  WriteReg(0x0C,0x10);          //MS520�Ĵ�������
  WriteReg(0x28,0x3F);          //
  WriteReg(0x12,0x00);          //
  WriteReg(0x13,0x00);          //
  WriteReg(0x14,0x83);          //
  WriteReg(0x15,0x40);          //100% �������

  return SUCCESS;
}

//*************************************************************//
//funcname:REQA
//function:Ѱ��������
//parameter:
//return:  SUCCESS:Ѱ���ɹ�     FAIL:Ѱ��ʧ��
//Author:Mr.Wang
//Date:2018-10-19
//**************************************************************/
unsigned char REQA(void)
{
  unsigned char FIFOLevel;
  WriteReg(0x12,0x00);          //TX CRC
  WriteReg(0x13,0x00);          //RX CRC

  WriteReg(0x0a,0x80);
  WriteReg(0x0D,0x07);          //

  WriteReg(0x09,0x26);          // REQA ����

  WriteReg(0x01,0x0C);          //trans and receive
  WriteReg(0x0D,0x87);          //start send
  delay_ms(3);
  FIFOLevel = ReadReg(0x0a);
  printf("FIFOLevel: %d\n", FIFOLevel);
  if(FIFOLevel != 2)
  {
    return FAIL;
  }
  else
  {
    PICC.ATQA.LSB = ReadReg(0x09);
    PICC.ATQA.MSB = ReadReg(0x09);
  }
  printf("ATQA: 0x%02x%02x\n", PICC.ATQA.MSB, PICC.ATQA.LSB);
  return SUCCESS;
}

//*************************************************************//
//funcname:AntiCollisionAndSelect
//function:����ͻѭ��
//parameter:
//return:  Select AcKnowledge
//Author:Mr.Wang
//Date:2018-10-22
//**************************************************************/
unsigned char AntiCollisionLoop(void) //
{
  ANTICOLLISION(0);                         //cascade level 1
  PICC.SAK = SELECT(0);
  if(PICC.SAK&0x04)
  {
    ANTICOLLISION(1);                       //cascade level 2
    SELECT(1);
  }
  if(PICC.SAK&0x04)
  {
    ANTICOLLISION(2);                       //cascade level 3
    SELECT(2);
  }
  return PICC.SAK;
}
//*************************************************************//
//funcname:ANTICOLLISION
//function:����ͻ
//parameter:CascadeLevel:  single UID;
//                         double UID;
//                         triple UID;
//return:  CollCount:��ͻ��������
//Author:Mr.Wang
//Date:2018-10-22
//**************************************************************/
unsigned char ANTICOLLISION(char CascadeLevel)
{
  unsigned char FIFOLevel;
  unsigned char RxAlign,CollByte,CollPos,CollCount,TxLastBits,NVB,CollData[16];

  CollCount = 0;                   //ͳ�Ʒ�����ͻ�Ĵ���
  WriteReg(0x12,0x00);             //�ر� TX CRC
  WriteReg(0x13,0x00);             //�ر� RX CRC

  WriteReg(0x0E,0x20);             //all receiving bits will be cleared after a collision
  WriteReg(0x0A,0x80);

  WriteReg(0x09,0x93+CascadeLevel*2);
  WriteReg(0x09,0x20);

  WriteReg(0x01,0x0C);             //trans and receive
  WriteReg(0x0D,0x80);             //start send
  delay_ms(1);

  CollPos  = ReadReg(0x0E);
  if((CollPos&0x20))               //detected collision  δ������ͻ
  {
        FIFOLevel = ReadReg(0x0A);
        if(FIFOLevel != 5)             //�����ֽ����ж�
        {
          return FAIL;
        }
        if(0 == ((PICC.ATQA.LSB&0xC0)>>6 - CascadeLevel))      //���������Ƿ���ATQA�ڵ���Ϣһֱ
        {
          for(int i=0;i<5;i++)
            PICC.UID[i+CascadeLevel*4] = ReadReg(0x09);
        }
        return SUCCESS;
  }
  else                                  //������ͻ
  {
    for(int i=0;i<5;i++)      CollData[i] = 0;

    while(!(CollPos&0x20))      //
    {
        CollByte = ReadReg(0x0A);
        for(int i=0;i<CollByte;i++)
          CollData[i] |= ReadReg(0x09);                         //���ܷ�����γ�ͻ

        TxLastBits = (CollPos&0x07);                            //��ͻλ��
        NVB        = ((CollByte + 0x01)<<4) + TxLastBits;       //Number of Valid Bits, Type A
        RxAlign    = (TxLastBits << 4);                         //

        WriteReg(0x0A,0x80);                                    //clear FIFO
        WriteReg(0x09,0x93+CascadeLevel*2);                     //select code, Type A
        WriteReg(0x09,NVB);
        for(int i=0;i<CollByte;i++)
          WriteReg(0x09,CollData[i]);

        WriteReg(0x01,0x0C);                                    //trans and receive
        WriteReg(0x0D,TxLastBits|RxAlign|0x80);                 //
        delay_ms(3);
        CollPos = ReadReg(0x0E);
        CollCount ++;                                           //��ͻ�������жϼ��ſ��ڳ���
    }
    FIFOLevel = ReadReg(0x0A);
    for(int i=0;i<FIFOLevel;i++)
      PICC.UID[i+CascadeLevel*4+CollByte-1] = ReadReg(0x09);

    for(int i=0;i<5;i++)
      PICC.UID[i+CascadeLevel*4] |= CollData[i];
  }
  return CollCount;             //���س�ͻ����
}

//*************************************************************//
//funcname:   SELECT
//function:   ѡ��
//parameter:  CascadeLevel:  single UID;
//                           double UID;
//                           triple UID;
//return:     Select AcKnowledge
//Author:Mr.Wang
//Date:2018-10-22
//**************************************************************/
unsigned char SELECT(char CascadeLevel)
{
  unsigned char FIFOLevel;
  WriteReg(0x0a,0x80);

  WriteReg(0x12,0x80);                          //ʹ��TX CRC
  WriteReg(0x13,0x80);                          //ʹ��RX CRC

  WriteReg(0x09,0x93+CascadeLevel*2);           //SEL(ѡ�����)��ѡ��������
  WriteReg(0x09,0x70);                          //NVB ����Ч������

  for(int i=0;i<5;i++)                          //UID and BCC
    WriteReg(0x09,PICC.UID[i+CascadeLevel*4]);

  WriteReg(0x01,0x0C);          //trans and receive
  WriteReg(0x0D,0x80);          //start send
  delay_ms(2);

  FIFOLevel = ReadReg(0x0a);
  if(FIFOLevel == 1)
    PICC.SAK = ReadReg(0x09);

  return PICC.SAK;
}

//*************************************************************//
//funcname:   MFAuthent                                        //M1��
//function:   �����໥��֤
//parameter:  blockNum: ���
//            key:      ��Կ
//return:     SUCCESS:  ��֤�ɹ�
//            FAIL:     ��֤ʧ��
//Author:Mr.Wang
//Date:2018-10-22
//**************************************************************/
unsigned char MFAuthent(unsigned blockNum,unsigned char *key)
{
  unsigned char MFCrypto,ProtocolErr;
  WriteReg(0x0a,0x80);
  WriteReg(0x12,0x80);             //TX CRC
  WriteReg(0x13,0x80);             //TX CRC

  WriteReg(0x09,0x60);             //Authent command
  WriteReg(0x09,blockNum);
  for(int i=0;i<6;i++)
    WriteReg(0x09,key[i]);

  for(int i=0;i<4;i++)
    WriteReg(0x09,PICC.UID[i]);

  WriteReg(0x01,0x0e);          //MFAuthent

  delay_ms(3);
  ProtocolErr = ReadReg(0x06)&0x01;     //�ֽ����Ƿ���ȷ
  MFCrypto    = ReadReg(0x08)&0x08;
  if(MFCrypto&&(!ProtocolErr))
  {
    return SUCCESS;
  }
  _NOP();

  return FAIL;
}

//*************************************************************//
//funcname:   ReadBlock                                        //M1��
//function:   ����
//parameter:  blockNum: ���
//            data:     ���ݱ����λ��
//return:     SUCCESS:  �ɹ�
//
//Author:Mr.Wang
//Date:2018-10-22
//**************************************************************/
unsigned char ReadBlock(unsigned char blockNum,unsigned char *data)
{
  unsigned char FIFOLevel;
  WriteReg(0x0a,0x80);

  WriteReg(0x12,0x80);             //TX CRC
  WriteReg(0x13,0x80);             //TX CRC

  WriteReg(0x09,0x30);             //read command
  WriteReg(0x09,blockNum);

  WriteReg(0x01,0x0C);             //trans and receive
  WriteReg(0x0D,0x80);             //start send
  delay_ms(3);
  WriteReg(0x0D,0x00);             //stop send
  FIFOLevel = ReadReg(0x0a);
  for(int i=0;i<FIFOLevel;i++)
  {
    data[i] = ReadReg(0x09);
  }
  _NOP();

  return SUCCESS;
}
//*************************************************************//
//funcname:   WriteBlock                                   //M1��
//function:   ����
//parameter:  blockNum: ���
//            data:     д�������
//return:     WriteInformation:  ��Ϣ
//
//Author:Mr.Wang
//Date:2018-10-22
//**************************************************************/
unsigned char WriteBlock(unsigned char blockNum,unsigned char *data)
{
  unsigned char FIFOLevel;
  unsigned char WriteInformation;
  WriteReg(0x0a,0x80);

  WriteReg(0x12,0x80);             //TX CRC
  WriteReg(0x13,0x80);             //TX CRC

  WriteReg(0x09,0xA0);             //write command
  WriteReg(0x09,blockNum);

  WriteReg(0x01,0x0C);               //trans and receive
  WriteReg(0x0D,0x80);               //start send
  delay_ms(3);
  WriteReg(0x0D,0x00);              //stop send
  FIFOLevel = ReadReg(0x0a);
  for(int i=0;i<FIFOLevel;i++)
  {
    WriteInformation = ReadReg(0x09);
  }
  WriteReg(0x0a,0x80);
  for(int i=0;i<16;i++)
    WriteReg(0x09,*(data+i));

  WriteReg(0x01,0x0C);            //trans and receive
  WriteReg(0x0D,0x80);            //start send
  delay_ms(3);
  WriteReg(0x0D,0x00);            //stop send

  return WriteInformation;
}

