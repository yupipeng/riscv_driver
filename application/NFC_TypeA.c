
#include "file_include.H"
#include "NFC_TypeA.h"

// void delay_ms(unsigned int Delay)
// {
// 	 unsigned int i=0x00,j=0x00,multiple=0x00;
// 	 multiple = 6000;
//    for( i=0;i<Delay;i++) {for( j=0;j<=multiple;j++);}
// }

//*************************************************************//
// funcname:TypeAInit
// function:MS520 A����ʼ��
// parameter:
// return:
// Author:Mr.Wang
// Date:2018-10-19
//**************************************************************/
unsigned char TypeAInit(void) // �Ĵ�������
{
    WriteReg(0x0C, 0x10); // MS520�Ĵ�������
    WriteReg(0x28, 0x3F); //
    WriteReg(0x12, 0x00); //
    WriteReg(0x13, 0x00); //
    WriteReg(0x14, 0x83); //
    WriteReg(0x15, 0x40); // 100% �������

    return NFC_SUCCESS;
}

//*************************************************************//
// funcname:REQA
// function:Ѱ��������
// parameter:
// return:  NFC_SUCCESS:Ѱ���ɹ�     NFC_FAIL:Ѱ��ʧ��
// Author:Mr.Wang
// Date:2018-10-19
//**************************************************************/
unsigned char REQA(void)
{
    unsigned char FIFOLevel;
    WriteReg(0x12, 0x00); // TX CRC �������ݷ��͵����ʺͽṹ
    WriteReg(0x13, 0x00); // RX CRC �������ݽ��յ����ʺͽṹ

    WriteReg(0x0a, 0x80); // BIT7=1 ʱ �ڲ� FIFO �������Ķ���дָ���Լ��Ĵ��� ErrReg �� BufferOvfl λ���̱����

    //	FIFOLevel = ReadReg(0x0a);
    //	printf("  FIFOLevel-1= "); putbyte(0x30+FIFOLevel/100); putbyte(0x30+FIFOLevel%100/10); putbyte(0x30+FIFOLevel%100%10);
    WriteReg(0x0D, 0x07); // ����λ��֡�ĵ���

    WriteReg(0x09, 0x26); // FIFO��д�뷢�͸���Ƭ������
    //  FIFOLevel = ReadReg(0x0a);
    //	printf("  FIFOLevel-2= "); putbyte(0x30+FIFOLevel/100); putbyte(0x30+FIFOLevel%100/10); putbyte(0x30+FIFOLevel%100%10);

    WriteReg(0x01, 0x0C); // trans and receive ������ָֹͣ��
    WriteReg(0x0D, 0x87); // start send ����λ��֡�ĵ���
    delay_ms(3);
    FIFOLevel = ReadReg(0x0a);
    //	printf("  FIFOLevel= "); putbyte(0x30+FIFOLevel/100); putbyte(0x30+FIFOLevel%100/10); putbyte(0x30+FIFOLevel%100%10);
    printf("FIFOLevel: %d\n", FIFOLevel);
    if (FIFOLevel != 2) {
        return NFC_FAIL;
    } else {
        PICC.ATQA.LSB = ReadReg(0x09);
        PICC.ATQA.MSB = ReadReg(0x09);
        printf("  PICC.ATQA.MSB= 0X%02x\n", PICC.ATQA.MSB);
        printf("  PICC.ATQA.LSB= 0X%02x\n", PICC.ATQA.LSB);
        printf("ATQA: 0x%02x%02x\n", PICC.ATQA.MSB, PICC.ATQA.LSB);
    }
    return NFC_SUCCESS;
}

//*************************************************************//
// funcname:AntiCollisionAndSelect
// function:����ͻѭ��
// parameter:
// return:  Select AcKnowledge
// Author:Mr.Wang
// Date:2018-10-22
//**************************************************************/
unsigned char AntiCollisionLoop(void) //
{
    printf(" A_0_1 \n");
    ANTICOLLISION(0); // cascade level 1
    PICC.SAK = SELECT(0);
    if (PICC.SAK & 0x04) {
        printf(" A_0_2 \n");
        ANTICOLLISION(1); // cascade level 2
        SELECT(1);
    }
    if (PICC.SAK & 0x04) {
        printf(" A_0_3 \n");
        ANTICOLLISION(2); // cascade level 3
        SELECT(2);
    }
    return PICC.SAK;
}
//*************************************************************//
// funcname:ANTICOLLISION
// function:����ͻ
// parameter:CascadeLevel:  single UID;
//                         double UID;
//                         triple UID;
// return:  CollCount:��ͻ��������
// Author:Mr.Wang
// Date:2018-10-22
//**************************************************************/
unsigned char ANTICOLLISION(char CascadeLevel)
{
    unsigned char FIFOLevel, i;
    unsigned char RxAlign, CollByte, CollPos, CollCount, TxLastBits, NVB, CollData[16];

    CollCount = 0;        // ͳ�Ʒ�����ͻ�Ĵ���
    WriteReg(0x12, 0x00); // �ر� TX CRC
    WriteReg(0x13, 0x00); // �ر� RX CRC

    WriteReg(0x0E, 0x20); // all receiving bits will be cleared after a collision
    WriteReg(0x0A, 0x80);

    WriteReg(0x09, 0x93 + CascadeLevel * 2);
    WriteReg(0x09, 0x20);

    WriteReg(0x01, 0x0C); // trans and receive
    WriteReg(0x0D, 0x80); // start send
    delay_ms(1);

    CollPos = ReadReg(0x0E);
    if ((CollPos & 0x20)) // detected collision  δ������ͻ
    {
        FIFOLevel = ReadReg(0x0A);
        if (FIFOLevel != 5) // �����ֽ����ж�
        {
            return NFC_FAIL;
        }
        if (0 == ((PICC.ATQA.LSB & 0xC0) >> 6 - CascadeLevel)) // ���������Ƿ���ATQA�ڵ���Ϣһֱ
        {
            for (i = 0; i < 5; i++)
                PICC.UID[i + CascadeLevel * 4] = ReadReg(0x09);
        }
        return NFC_SUCCESS;
    } else // ������ͻ
    {
        for (i = 0; i < 5; i++) CollData[i] = 0;

        while (!(CollPos & 0x20)) //
        {
            CollByte = ReadReg(0x0A);
            for (i = 0; i < CollByte; i++)
                CollData[i] |= ReadReg(0x09); // ���ܷ�����γ�ͻ

            TxLastBits = (CollPos & 0x07);                      // ��ͻλ��
            NVB        = ((CollByte + 0x01) << 4) + TxLastBits; // Number of Valid Bits, Type A
            RxAlign    = (TxLastBits << 4);                     //

            WriteReg(0x0A, 0x80);                    // clear FIFO
            WriteReg(0x09, 0x93 + CascadeLevel * 2); // select code, Type A
            WriteReg(0x09, NVB);
            for (i = 0; i < CollByte; i++)
                WriteReg(0x09, CollData[i]);

            WriteReg(0x01, 0x0C);                        // trans and receive
            WriteReg(0x0D, TxLastBits | RxAlign | 0x80); //
            delay_ms(3);
            CollPos = ReadReg(0x0E);
            CollCount++; // ��ͻ�������жϼ��ſ��ڳ���
        }
        FIFOLevel = ReadReg(0x0A);
        for (i = 0; i < FIFOLevel; i++)
            PICC.UID[i + CascadeLevel * 4 + CollByte - 1] = ReadReg(0x09);

        for (i = 0; i < 5; i++)
            PICC.UID[i + CascadeLevel * 4] |= CollData[i];
    }
    return CollCount; // ���س�ͻ����
}

//*************************************************************//
// funcname:   SELECT
// function:   ѡ��
// parameter:  CascadeLevel:  single UID;
//                           double UID;
//                           triple UID;
// return:     Select AcKnowledge
// Author:Mr.Wang
// Date:2018-10-22
//**************************************************************/
unsigned char SELECT(char CascadeLevel)
{
    unsigned char FIFOLevel, i = 0x00;
    WriteReg(0x0a, 0x80);

    WriteReg(0x12, 0x80); // ʹ��TX CRC
    WriteReg(0x13, 0x80); // ʹ��RX CRC

    WriteReg(0x09, 0x93 + CascadeLevel * 2); // SEL(ѡ�����)��ѡ��������
    WriteReg(0x09, 0x70);                    // NVB ����Ч������

    for (i = 0; i < 5; i++) // UID and BCC
        WriteReg(0x09, PICC.UID[i + CascadeLevel * 4]);

    WriteReg(0x01, 0x0C); // trans and receive
    WriteReg(0x0D, 0x80); // start send
    delay_ms(2);

    FIFOLevel = ReadReg(0x0a);
    if (FIFOLevel == 1)
        PICC.SAK = ReadReg(0x09);

    return PICC.SAK;
}

//*************************************************************//
// funcname:   MFAuthent                                        //M1��
// function:   �����໥��֤
// parameter:  blockNum: ���
//            key:      ��Կ
// return:     NFC_SUCCESS:  ��֤�ɹ�
//            NFC_FAIL:     ��֤ʧ��
// Author:Mr.Wang
// Date:2018-10-22
//**************************************************************/
unsigned char MFAuthent(unsigned blockNum, unsigned char *key)
{
    unsigned char MFCrypto, ProtocolErr, i = 0x00;
    WriteReg(0x0a, 0x80); // ���λ=1 ����ڲ�FIFO���������ݣ�������Ҳ����
    WriteReg(0x12, 0x80); // ���λ=1 ���ݷ��͹��̲���CRC�����ݷ����ٶȣ�106KBd
    WriteReg(0x13, 0x80); // ���λ=1 ���ݽ��չ��̲���CRC�����ݽ����ٶȣ�106KBd

    WriteReg(0x09, 0x60); // Authent command  ��Ȩ����
    WriteReg(0x09, blockNum);

    for (i = 0; i < 6; i++) WriteReg(0x09, key[i]);

    for (i = 0; i < 4; i++) WriteReg(0x09, PICC.UID[i]);

    WriteReg(0x01, 0x0e); // MFAuthent

    delay_ms(3);
    ProtocolErr = ReadReg(0x06) & 0x01; // �ֽ����Ƿ���ȷ
    MFCrypto    = ReadReg(0x08) & 0x08;
    if (MFCrypto && (!ProtocolErr)) {
        return NFC_SUCCESS;
    }
    //  _NOP();

    return NFC_FAIL;
}

//*************************************************************//
// funcname:   ReadBlock                                        //M1��
// function:   ����
// parameter:  blockNum: ���
//            data:     ���ݱ����λ��
// return:     NFC_SUCCESS:  �ɹ�
//
// Author:Mr.Wang
// Date:2018-10-22
//**************************************************************/
unsigned char ReadBlock(unsigned char blockNum, unsigned char *data)
{
    unsigned char FIFOLevel, i = 0x00;
    WriteReg(0x0a, 0x80);

    WriteReg(0x12, 0x80); // TX CRC
    WriteReg(0x13, 0x80); // TX CRC

    WriteReg(0x09, 0x30); // read command
    WriteReg(0x09, blockNum);

    WriteReg(0x01, 0x0C); // trans and receive
    WriteReg(0x0D, 0x80); // start send
    delay_ms(3);
    WriteReg(0x0D, 0x00); // stop send
    FIFOLevel = ReadReg(0x0a);
    printf("\r\n");
    for (i = 0; i < FIFOLevel; i++) {
        data[i] = ReadReg(0x09);
        printf("  Data[0X");
        uart1_print_hex(i);
        printf("]= 0X");
        uart1_print_hex(data[i]);
    }

    return NFC_SUCCESS;
}
//*************************************************************//
// funcname:   WriteBlock                                   //M1��
// function:   ����
// parameter:  blockNum: ���
//            data:     д�������
// return:     WriteInformation:  ��Ϣ
//
// Author:Mr.Wang
// Date:2018-10-22
//**************************************************************/
unsigned char WriteBlock(unsigned char blockNum, unsigned char *data)
{
    unsigned char FIFOLevel, i = 0x00;
    unsigned char WriteInformation;
    WriteReg(0x0a, 0x80);

    WriteReg(0x12, 0x80); // TX CRC
    WriteReg(0x13, 0x80); // TX CRC

    WriteReg(0x09, 0xA0); // write command
    WriteReg(0x09, blockNum);

    WriteReg(0x01, 0x0C); // trans and receive
    WriteReg(0x0D, 0x80); // start send
    delay_ms(3);
    WriteReg(0x0D, 0x00); // stop send
    FIFOLevel = ReadReg(0x0a);
    for (i = 0; i < FIFOLevel; i++) {
        WriteInformation = ReadReg(0x09);
    }
    WriteReg(0x0a, 0x80);
    printf("\r\n");
    for (i = 0; i < 16; i++) {
        WriteReg(0x09, *(data + i));
        printf("  Wata[0X");
        uart1_print_hex(i);
        printf("]= 0X");
        uart1_print_hex(*(data + i)); // putbyte(0x30+(*(data+i)/100)); putbyte(0x30+(*(data+i)%100/10)); putbyte(0x30+(*(data+i)%100%10));
    }

    WriteReg(0x01, 0x0C); // trans and receive
    WriteReg(0x0D, 0x80); // start send
    delay_ms(3);
    WriteReg(0x0D, 0x00); // stop send

    return WriteInformation;
}
