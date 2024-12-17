#include "NFC.h"
#include <stdio.h>

FormatPICC PICC;

//*************************************************************//
//funcname:TypeAInit
//function:MS520 A卡初始化
//parameter:
//return:
//Author:Mr.Wang
//Date:2018-10-19
//**************************************************************/
unsigned char TypeAInit()     //寄存器配置
{
  WriteReg(0x0C,0x10);          //MS520寄存器配置
  WriteReg(0x28,0x3F);          //
  WriteReg(0x12,0x00);          //
  WriteReg(0x13,0x00);          //
  WriteReg(0x14,0x83);          //
  WriteReg(0x15,0x40);          //100% 调制深度

  return SUCCESS;
}

//*************************************************************//
//funcname:REQA
//function:寻卡函数，
//parameter:
//return:  SUCCESS:寻卡成功     FAIL:寻卡失败
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

  WriteReg(0x09,0x26);          // REQA 命令

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
//function:防冲突循环
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
//function:防冲突
//parameter:CascadeLevel:  single UID;
//                         double UID;
//                         triple UID;
//return:  CollCount:冲突发生次数
//Author:Mr.Wang
//Date:2018-10-22
//**************************************************************/
unsigned char ANTICOLLISION(char CascadeLevel)
{
  unsigned char FIFOLevel;
  unsigned char RxAlign,CollByte,CollPos,CollCount,TxLastBits,NVB,CollData[16];

  CollCount = 0;                   //统计发生冲突的次数
  WriteReg(0x12,0x00);             //关闭 TX CRC
  WriteReg(0x13,0x00);             //关闭 RX CRC

  WriteReg(0x0E,0x20);             //all receiving bits will be cleared after a collision
  WriteReg(0x0A,0x80);

  WriteReg(0x09,0x93+CascadeLevel*2);
  WriteReg(0x09,0x20);

  WriteReg(0x01,0x0C);             //trans and receive
  WriteReg(0x0D,0x80);             //start send
  delay_ms(1);

  CollPos  = ReadReg(0x0E);
  if((CollPos&0x20))               //detected collision  未发生冲突
  {
        FIFOLevel = ReadReg(0x0A);
        if(FIFOLevel != 5)             //返回字节数判断
        {
          return FAIL;
        }
        if(0 == ((PICC.ATQA.LSB&0xC0)>>6 - CascadeLevel))      //串联级别是否与ATQA内的信息一直
        {
          for(int i=0;i<5;i++)
            PICC.UID[i+CascadeLevel*4] = ReadReg(0x09);
        }
        return SUCCESS;
  }
  else                                  //发生冲突
  {
    for(int i=0;i<5;i++)      CollData[i] = 0;

    while(!(CollPos&0x20))      //
    {
        CollByte = ReadReg(0x0A);
        for(int i=0;i<CollByte;i++)
          CollData[i] |= ReadReg(0x09);                         //可能发生多次冲突

        TxLastBits = (CollPos&0x07);                            //冲突位置
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
        CollCount ++;                                           //冲突计数，判断几张卡在场内
    }
    FIFOLevel = ReadReg(0x0A);
    for(int i=0;i<FIFOLevel;i++)
      PICC.UID[i+CascadeLevel*4+CollByte-1] = ReadReg(0x09);

    for(int i=0;i<5;i++)
      PICC.UID[i+CascadeLevel*4] |= CollData[i];
  }
  return CollCount;             //返回冲突次数
}

//*************************************************************//
//funcname:   SELECT
//function:   选择
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

  WriteReg(0x12,0x80);                          //使能TX CRC
  WriteReg(0x13,0x80);                          //使能RX CRC

  WriteReg(0x09,0x93+CascadeLevel*2);           //SEL(选择代码)：选择串联级别
  WriteReg(0x09,0x70);                          //NVB ：有效比特数

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
//funcname:   MFAuthent                                        //M1卡
//function:   三次相互验证
//parameter:  blockNum: 块号
//            key:      密钥
//return:     SUCCESS:  验证成功
//            FAIL:     验证失败
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
  ProtocolErr = ReadReg(0x06)&0x01;     //字节数是否正确
  MFCrypto    = ReadReg(0x08)&0x08;
  if(MFCrypto&&(!ProtocolErr))
  {
    return SUCCESS;
  }
  _NOP();

  return FAIL;
}

//*************************************************************//
//funcname:   ReadBlock                                        //M1卡
//function:   读块
//parameter:  blockNum: 块号
//            data:     数据保存的位置
//return:     SUCCESS:  成功
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
//funcname:   WriteBlock                                   //M1卡
//function:   读块
//parameter:  blockNum: 块号
//            data:     写入的数据
//return:     WriteInformation:  信息
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

