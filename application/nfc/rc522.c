#include "rc522.h"

uint16_t g_surplusTime = 0;



static u8 SPI3_RW_Byte(u8 byte);

u8 FUNCTION_STR[60]={0};

/*全局变量*/
unsigned char CT[2];//卡类型
unsigned char SN[4]; //卡号
unsigned char RFID[32];			//存放RFID	16字节数据



u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};			//保存密码的缓存

//密码开光		//保存密码的缓存
const u8 BLOCK_KEY[16]={0xD2,0x17,0x02,0x06,0x22,0xD0,0xff,0x07,0x80,0x29,0x5A,0x89,0xE5,0x4B,0x71,0x32};

u8 DEF_A[6]={0xD2,0x17,0x02,0x06,0x22,0xD0};			//保存密码a
const u8 DEF_B[6]={0x5A,0x89,0xE5,0x4B,0x71,0x32};			//保存密码B

//默认的每个扇区最后块的数据
unsigned char RFID1[16]={0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x07,0x80,0x29,0xff,0xff,0xff,0xff,0xff,0xff};
/*函数声明*/
unsigned char status;

u8 rc522_init_flag=0;



//如果变成1	说明读取到	不会再读取
u8 read_control_ok_flag=0;

void Delay(unsigned int time)		//延时约1ms
{
	volatile unsigned int i;
	while(time--){
		i=12550;

		#ifdef NEED_FEED_DOG
		//stm8_1_wdog_feed();		//喂狗
		#endif

		while(i--);
	}
}

//读取卡片内固定块4的姓名
//返回108	表示名字是8字节
//u8 rc522_s50_read_name(u8 *buff)
//{
//	u8 temp8_ret=0;
//	temp8_ret=fast_read_s50_block(4);
//
//	if(temp8_ret==0)		//密码成功	读取姓名
//	{
//		if(PcdRead(4,RFID)==MI_OK)		//读取成功
//		{
//			//RFID[0]							姓名字节数		一个汉字=2字节		最多14字节
//			//RFID[1]-RFID[14]		姓名
//			//RFID[15]						校验	暂时不用
//			for(temp8_ret=1;temp8_ret<=RFID[0];temp8_ret++)
//			{
//				buff[temp8_ret-1]=RFID[temp8_ret];
//			}
//
//
//			//再读取块6		场馆名称	有效时间
//			if(PcdRead(6,&buff[16])==MI_OK)		//读取成功
//			{
//
//
//				temp8_ret=temp8_ret+100;
//
//			}
//			else
//			{
//				temp8_ret=7;		//7			读取失败
//			}
//		}
//		else
//		{
//			temp8_ret=6;		//6			读取失败
//		}
//
//	}
//	else
//	{
//		//return temp8_ret;
//	}
//

//
//
//
//	return temp8_ret;
//}



//0是距离MCU较远的那个		小天线
//1是距离MCU较近的			大天线
//void rc522_select_ic(u8 channel)
//{
//	rc522_channel=channel;
//
//}

//mode	0	擦除读取标志		1=连续读取  2=复位
//mode	3	只要电机在转动	就读取
//连续读取	直到	读取成功或者超时
//u8 rc522_s50_read_control(u8 mode,u16 max_cnt,u8 *buff)
//{
//
//	u8 read_error_flag=0;
//	u16 temp16_cnt1=0;
//
//	//强制变成0	会继续读卡
//	if(mode==0)
//	{
//
//		read_control_ok_flag=0;
//
//		//全局变量设定为默认值
//		status=MI_ERR;
//		SN[0]=0x00;
//		SN[1]=0x00;
//		SN[2]=0x00;
//		SN[3]=0x00;
//
//		rc522_select_ic(1);
//		rc522_init();
//
//		return 0;
//	}
//	else if(mode==1)
//	{

//		while((read_control_ok_flag==0)&&(temp16_cnt1<max_cnt))
//		{
//			//这里可以做一个计数		读卡累计1000次	就再次初始化
//			read_error_flag=0;

//			if(PcdRequest(PICC_REQALL,CT)==MI_OK)//寻卡成功
//			{
//
//			}
//			else
//			{
//				//rc522_init_flag=1;
//				//return 2;	//没有卡片
//				//printf("err 1\r\n");
//
//				read_error_flag=1;
//			}
//
//			if(read_error_flag==0)
//			{
//				if(PcdAnticoll(SN)==MI_OK)//防衝撞成功
//				{
//
//				}
//				else
//				{
//					//return 3;	//防衝撞失败
//
//					read_error_flag=2;
//				}
//			}

//			if(read_error_flag==0)
//			{
//				if(PcdSelect(SN)==MI_OK)//選卡成功
//				{
//					buff[0]=SN[0];
//					buff[1]=SN[1];
//					buff[2]=SN[2];
//					buff[3]=SN[3];
//
//					printf("read ok\r\n");
//					read_control_ok_flag=1;
//				}
//				else
//				{
//					//return 4;		//选卡失败
//
//					read_error_flag=3;
//				}
//			}
//


//			temp16_cnt1++;
//		}

//
//
//
//		if(temp16_cnt1>=max_cnt)
//		{
//			printf("rc522 timeout\r\n");
//
//			return 3;
//		}
//		else
//		{
//			printf("ok cnt=%d\r\n",temp16_cnt1);
//
//			return 0;
//		}
//
//
//	}
//	else if(mode==3)		//只要电机在转动	就读取
//	{

		while((read_control_ok_flag==0)&&(read_step_state()>0))
		{
			//这里可以做一个计数		读卡累计1000次	就再次初始化
			read_error_flag=0;

			if(PcdRequest(PICC_REQALL,CT)==MI_OK)//寻卡成功
			{

			}
			else
			{
				//rc522_init_flag=1;
				//return 2;	//没有卡片
				//printf("err 1\r\n");

				read_error_flag=1;
			}

			if(read_error_flag==0)
			{
				if(PcdAnticoll(SN)==MI_OK)//防衝撞成功
				{

				}
				else
				{
					//return 3;	//防衝撞失败

					read_error_flag=2;
				}
			}

			if(read_error_flag==0)
			{
				if(PcdSelect(SN)==MI_OK)//選卡成功
				{
					buff[0]=SN[0];
					buff[1]=SN[1];
					buff[2]=SN[2];
					buff[3]=SN[3];

					printf("read ok\r\n");
					read_control_ok_flag=1;
				}
				else
				{
					//return 4;		//选卡失败

					read_error_flag=3;
				}
			}



			temp16_cnt1++;
		}

//		if(read_control_ok_flag==1)	//選卡成功
//		{
//			//
//
//			printf("card ok\r\n");
//
//			//等待电动机停止
			while(read_step_state()>0)
			{
				Delay(1);
			}
//
//			return 0;
//		}
//		else
//		{
//			printf("motor stop\r\n");
//
//			return 4;
//		}
//
//
//	}

//}

u8 rc522_s50_read_sn(u8 *buff)
{
	if(rc522_init_flag)
	{
		rc522_init();
		rc522_init_flag=0;
	}

	//全局变量设定为默认值
	status=MI_ERR;
	SN[0]=0x00;
	SN[1]=0x00;
	SN[2]=0x00;
	SN[3]=0x00;

	//这里可以做一个计数		读卡累计1000次	就再次初始化

	if(PcdRequest(PICC_REQALL,CT)==MI_OK)//寻卡成功
	{

	}
	else
	{
		//rc522_init_flag=1;
		return 2;	//没有卡片
	}

	if(PcdAnticoll(SN)==MI_OK)//防衝撞成功
	{

	}
	else
	{
		return 3;	//防衝撞失败
	}

	if(PcdSelect(SN)==MI_OK)//選卡成功
	{
		buff[0]=SN[0];
		buff[1]=SN[1];
		buff[2]=SN[2];
		buff[3]=SN[3];
	}
	else
	{
		return 4;		//选卡失败
	}



	return 0;
}

快速读取返回当前的卡号	4个字节
//void read_current_card_sn(u8 *buff)
//{
//	buff[0]=SN[0];
//	buff[1]=SN[1];
//	buff[2]=SN[2];
//	buff[3]=SN[3];
//
//}

读取卡片内固定块5的余额
//u8 rc522_s50_read_money(u8 *buff)
//{
//	u8 temp8_ret;
//	temp8_ret=fast_read_s50_block(5);
//
//	if(temp8_ret==0)		//密码成功	读取金额
//	{
//		if(PcdRead(5,RFID)==MI_OK)		//读取成功
//		{
//			//RFID[0]		金额整数部分	MSB
//			//RFID[1]		金额整数部分	LSB
//			//RFID[2]		金额小数部分	0-99
//			//RFID[3]		正负符号			0正		1负		一般不用
//
//			//最大金额	65535.99
//			//最小金额	0.00
//
//			for(temp8_ret=0;temp8_ret<4;temp8_ret++)
//			{
//				buff[temp8_ret]=RFID[temp8_ret];
//			}
//
//
//			temp8_ret=0;
//		}
//		else
//		{
//			temp8_ret=6;		//6			读取失败
//		}
//
//	}
//	else
//	{
//		//return temp8_ret;
//	}
//
//
//	return temp8_ret;
//}

//修改卡片内固定块5的余额
//mode	0		直接写入一个新的金额
//mode	1		增加某个金额
//mode	2		减少某个金额
//u8 rc522_s50_write_money(u8 mode,u8 *buff)
//{
//	u8 temp8_ret;
//	u8 temp8_buff[4]={0};
//	u16 temp16_1,temp16_2;
//
//	temp8_ret=fast_read_s50_block(5);
//
//	if(temp8_ret==0)		//密码成功	读取金额
//	{
//		if(PcdRead(5,RFID)==MI_OK)		//读取成功
//		{
//			//RFID[0]		金额整数部分	MSB
//			//RFID[1]		金额整数部分	LSB
//			//RFID[2]		金额小数部分	0-99
//			//RFID[3]		正负符号			0正		1负		一般不用
//
//			//最大金额	65535.99
//			//最小金额	0.00
//			for(temp8_ret=0;temp8_ret<4;temp8_ret++)
//			{
//				temp8_buff[temp8_ret]=RFID[temp8_ret];
//			}
//		}
//		else
//		{
//			temp8_ret=6;		//6			读取失败
//		}
//
//		if(mode==0)		//mode	0		直接写入一个新的金额
//		{
//			for(temp8_ret=0;temp8_ret<4;temp8_ret++)
//			{
//				RFID[temp8_ret]=buff[temp8_ret];
//			}
//		}
//		else if(mode==1)		//mode	1		增加某个金额
//		{
//			temp16_1=buff[0]<<8;
//			temp16_1=temp16_1+buff[1];
//			temp16_2=RFID[0]<<8;
//			temp16_2=temp16_2+RFID[1];
//			RFID[2]=RFID[2]+buff[2];
//
//			//进位控制
//			if(RFID[2]>100)
//			{
//				RFID[2]=RFID[2]%100;
//				temp16_1++;
//			}
//
//			//金额整数部分
//			//不考虑超过65535元的处理
//			temp16_2=temp16_2+temp16_1;
//			RFID[0]=temp16_2>>8;
//			RFID[1]=temp16_2&0x00ff;
//
//		}
//		else if(mode==2)		//mode	2		减少某个金额
//		{
//			temp16_1=buff[0]<<8;
//			temp16_1=temp16_1+buff[1];
//			temp16_2=RFID[0]<<8;
//			temp16_2=temp16_2+RFID[1];
//
//			RFID[2]=RFID[2]+buff[2];
//
//			//进位控制
//			if(RFID[2]<buff[2])		//减法不顾
//			{
//				RFID[2]=RFID[2]+100;
//				temp16_1++;
//			}
//			RFID[2]=RFID[2]-buff[2];
//
//
//
//			//金额整数部分
//			if(temp16_1>temp16_2)			//减法不够	报错
//			{
//				temp8_ret=7;
//			}
//			else
//			{
//				temp16_2=temp16_2-temp16_1;
//				RFID[0]=temp16_2>>8;
//				RFID[1]=temp16_2&0x00ff;
//			}
//		}
//	}
//	else
//	{
//		//return temp8_ret;
//	}
//
//	if(temp8_ret==0)
//	{
//		if(PcdWrite(5,RFID)==MI_OK)		//写入成功
//		{
//			temp8_ret=0;
//		}
//		else
//		{
//			temp8_ret=8;			//写入失败
//		}
//	}
//
//	return temp8_ret;
//}

//0 成功
//2	没有卡片
//3	防衝撞失败
//4	选卡失败
//5	密码错误
//u8 fast_read_s50_block(u8 address)
//{
//	if(rc522_init_flag)
//	{
//		rc522_init();
//		rc522_init_flag=0;
//	}
//
//	//全局变量设定为默认值
//	status=MI_ERR;
//	SN[0]=0x00;
//	SN[1]=0x00;
//	SN[2]=0x00;
//	SN[3]=0x00;
//
//	//这里可以做一个计数		读卡累计1000次	就再次初始化
//
//	if(PcdRequest(PICC_REQALL,CT)==MI_OK)//寻卡成功
//	{
//
//	}
//	else
//	{
//		return 2;	//没有卡片
//	}
//
//	if(PcdAnticoll(SN)==MI_OK)//防衝撞成功
//	{
//
//	}
//	else
//	{
//		return 3;	//防衝撞失败
//	}
//
//	if(PcdSelect(SN)==MI_OK)//選卡成功
//	{
//
//	}
//	else
//	{
//		return 4;		//选卡失败
//	}
//
//
//
//	if(PcdAuthState(0x60,address,DEF_A,SN)==MI_OK)		//密码A
//	{
//		rc522_init_flag=1;
//		return 0;
//	}
//	else
//	{
//		return 5;		//密码错误
//	}

//
//}



//void test_write_name(void)
//{
//
//	u8 temp8_buff[16]={0};
//
//	if(fast_read_s50_block(4)==0)
//	{
//		printf("块4开启成功\r\n");
//
//	}
//	else
//	{
//		printf("块4开启失败\r\n");
//
//		return;
//	}
//
//	temp8_buff[0]=6;
//	memcpy(&temp8_buff[1], "王二小", 6);
//
//	if(PcdWrite(4,temp8_buff)==MI_OK)		//写入成功
//	{
//		printf("写入姓名成功\r\n");
//	}
//	else
//	{
//		printf("写入姓名失败\r\n");
//	}
//
//	Delay(200);
//
//}

//void test_write_block(u8 address,u8 *buff)
//{
//
//
//	if(fast_read_s50_block(address)==0)
//	{
//		printf("块%d开启成功\r\n",address);
//
//	}
//	else
//	{
//		printf("块%d开启失败\r\n",address);
//
//		return;
//	}
//
//
//	if(PcdWrite(address,buff)==MI_OK)		//写入成功
//	{
//		printf("写入成功\r\n");
//	}
//	else
//	{
//		printf("写入失败\r\n");
//	}
//
//	Delay(200);
//
//}

void rc522_init(void)
{
	u8 temp8_2;

	//复位
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	Delay(5);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	Delay(5);


	rc522_write_1_byte(CommandReg,PCD_RESETPHASE);
	Delay(1)                             ;
	rc522_write_1_byte(ModeReg,0x3D)             ;
	rc522_write_1_byte(TReloadRegL,30)           ;
	rc522_write_1_byte(TReloadRegH,0)            ;
	rc522_write_1_byte(TModeReg,0x8D)            ;
	rc522_write_1_byte(TPrescalerReg,0x3E)       ;

	//开启天线发射
	rc522_write_1_byte(TxASKReg,0x40)       ;
	Delay(5)                       ;
	temp8_2 = rc522_read_1_byte(TxControlReg)     ;
	if(!(temp8_2&0x03))
		rc522_write_setbit_mask(TxControlReg, 0x03);
	temp8_2=rc522_read_1_byte(TxASKReg)       ;


	Delay(10)                       ;
}


/*
 * 函数名：PcdAuthState
 * 描述  ：验证卡片密码
 * 输入  ：ucAuth_mode，密码验证模式
 *                     = 0x60，验证A密钥
 *                     = 0x61，验证B密钥
 *         u8 ucAddr，块地址
 *         pKey，密码
 *         pSnr，卡片序列号，4字节
 * 返回  : 状态值
 *         = MI_OK，成功
 * 调用  ：外部调用
// */
//char PcdAuthState ( u8 ucAuth_mode, u8 ucAddr, u8 * pKey, u8 * pSnr )
//{
//    char cStatus;
//	  u8 uc, ucComMF522Buf [ MAXRLEN ];
//    u32 ulLen;

//    ucComMF522Buf [ 0 ] = ucAuth_mode;
//    ucComMF522Buf [ 1 ] = ucAddr;
//
//    for ( uc = 0; uc < 6; uc ++ )
//	    ucComMF522Buf [ uc + 2 ] = * ( pKey + uc );
//
//    for ( uc = 0; uc < 6; uc ++ )
//	    ucComMF522Buf [ uc + 8 ] = * ( pSnr + uc );

//    cStatus = PcdComMF522 ( PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, & ulLen );
//
//    if ( ( cStatus != MI_OK ) || ( ! ( rc522_read_1_byte ( Status2Reg ) & 0x08 ) ) )
//
//		{
			if(cStatus != MI_OK)
					printf("密码无权限\n")	;
			else
					printf("密码错误!!!\n")	;
//			cStatus = MI_ERR;
//    }
//		else
//		{
//			//printf("密码A正确\n");
//
//		}
//
//    return cStatus;
//
//}



/*
 * 函数名：PcdSelect
 * 描述  ：选定卡片
 * 输入  ：pSnr，卡片序列号，4字节
 * 返回  : 状态值
 *         = MI_OK，成功
 * 调用  ：外部调用
 */
char PcdSelect ( u8 * pSnr )
{
    char ucN;
    u8 uc;
	  u8 ucComMF522Buf [ MAXRLEN ];
    u32  ulLen;

    ucComMF522Buf [ 0 ] = PICC_ANTICOLL1;
    ucComMF522Buf [ 1 ] = 0x70;
    ucComMF522Buf [ 6 ] = 0;

    for ( uc = 0; uc < 4; uc ++ )
    {
    	ucComMF522Buf [ uc + 2 ] = * ( pSnr + uc );
    	ucComMF522Buf [ 6 ] ^= * ( pSnr + uc );
    }

    CalulateCRC ( ucComMF522Buf, 7, & ucComMF522Buf [ 7 ] );

    rc522_write_clrbit_mask ( Status2Reg, 0x08 );

    ucN = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, & ulLen );

    if ( ( ucN == MI_OK ) && ( ulLen == 0x18 ) )
		{
      ucN = MI_OK;

		}
    else
      ucN = MI_ERR;

    return ucN;

}


/*
 * 函数名：PcdWrite
 * 描述  ：写数据到M1卡一块
 * 输入  ：u8 ucAddr，块地址
 *         pData，写入的数据，16字节
 * 返回  : 状态值
 *         = MI_OK，成功
 * 调用  ：外部调用
 */
//char PcdWrite ( u8 ucAddr, u8 * pData )
//{
//    char cStatus;
//	  u8 uc, ucComMF522Buf [ MAXRLEN ];
//    u32 ulLen;

//    ucComMF522Buf [ 0 ] = PICC_WRITE;
//    ucComMF522Buf [ 1 ] = ucAddr;
//
//    CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
//
//    cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );

//    if ( ( cStatus != MI_OK ) || ( ulLen != 4 ) || ( ( ucComMF522Buf [ 0 ] & 0x0F ) != 0x0A ) )
//      cStatus = MI_ERR;
//
//    if ( cStatus == MI_OK )
//    {
//			memcpy(ucComMF522Buf, pData, 16);
//      for ( uc = 0; uc < 16; uc ++ )
//			  ucComMF522Buf [ uc ] = * ( pData + uc );
//
//      CalulateCRC ( ucComMF522Buf, 16, & ucComMF522Buf [ 16 ] );

//      cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 18, ucComMF522Buf, & ulLen );
//
//			if ( ( cStatus != MI_OK ) || ( ulLen != 4 ) || ( ( ucComMF522Buf [ 0 ] & 0x0F ) != 0x0A ) )
//        cStatus = MI_ERR;
//
//    }

//    return cStatus;
//
//}


///*
// * 函数名：PcdRead
// * 描述  ：读取M1卡一块数据
// * 输入  ：u8 ucAddr，块地址
// *         pData，读出的数据，16字节
// * 返回  : 状态值
// *         = MI_OK，成功
// * 调用  ：外部调用
// */
//char PcdRead ( u8 ucAddr, u8 * pData )
//{
//    char cStatus;
//	  u8 uc, ucComMF522Buf [ MAXRLEN ];
//    u32 ulLen;

//    ucComMF522Buf [ 0 ] = PICC_READ;
//    ucComMF522Buf [ 1 ] = ucAddr;
//
//    CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
//
//    cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );
//
//    if ( ( cStatus == MI_OK ) && ( ulLen == 0x90 ) )
//    {
//			for ( uc = 0; uc < 16; uc ++ )
//        * ( pData + uc ) = ucComMF522Buf [ uc ];
//    }
//
//    else
//      cStatus = MI_ERR;
//
//    return cStatus;

//}

/*
 * 函数名：CalulateCRC
 * 描述  ：用RC522计算CRC16
 * 输入  ：pIndata，计算CRC16的数组
 *         ucLen，计算CRC16的数组字节长度
 *         pOutData，存放计算结果存放的首地址
 * 返回  : 无
 * 调用  ：内部调用
 */
void CalulateCRC ( u8 * pIndata, u8 ucLen, u8 * pOutData )
{
    u8 uc, ucN;

    rc522_write_clrbit_mask(DivIrqReg,0x04);

    rc522_write_1_byte(CommandReg,PCD_IDLE);

    rc522_write_setbit_mask(FIFOLevelReg,0x80);

    for ( uc = 0; uc < ucLen; uc ++)
	    rc522_write_1_byte ( FIFODataReg, * ( pIndata + uc ) );

    rc522_write_1_byte ( CommandReg, PCD_CALCCRC );

    uc = 0xFF;

    do
    {
        ucN = rc522_read_1_byte ( DivIrqReg );
        uc --;
    } while ( ( uc != 0 ) && ! ( ucN & 0x04 ) );

    pOutData [ 0 ] = rc522_read_1_byte ( CRCResultRegL );
    pOutData [ 1 ] = rc522_read_1_byte ( CRCResultRegM );

}

//****************************************************/
void rc522_write_setbit_mask(unsigned char reg,unsigned char mask)
{
  char tmp = 0x00;
  tmp = rc522_read_1_byte(reg)| mask;
  rc522_write_1_byte(reg,tmp | mask);  // set bit mask
}
void rc522_write_clrbit_mask(unsigned char reg,unsigned char mask)
{
  char tmp = 0x0              ;
  tmp = rc522_read_1_byte(reg)&(~mask);
  rc522_write_1_byte(reg, tmp)        ;  // clear bit mask
}


char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
  char status                                        ;
  unsigned int  unLen                                ;
  unsigned char ucComMF522Buf[MAXRLEN]               ;

  rc522_write_clrbit_mask(Status2Reg,0x08)                      ;
  rc522_write_1_byte(BitFramingReg,0x07)                     ;
  rc522_write_setbit_mask(TxControlReg,0x03)                      ;

  ucComMF522Buf[0] = req_code                        ;

	//PCD_TRANSCEIVE            0x0C           //发送并接收数据
  status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,
                       1,ucComMF522Buf,&unLen       );
  if ((status == MI_OK) && (unLen == 0x10))
  {
    *pTagType     = ucComMF522Buf[0]                 ;
    *(pTagType+1) = ucComMF522Buf[1]                 ;
  }
  else
    status = MI_ERR                                  ;
  return status                                      ;
}


char PcdComMF522(unsigned char Command  ,unsigned char *pInData ,
                 unsigned char InLenByte,unsigned char *pOutData,
                 unsigned int  *pOutLenBit                       )
{
  char status = MI_ERR                          ;
  unsigned char irqEn   = 0x00                  ;
  unsigned char waitFor = 0x00                  ;
  unsigned char lastBits                        ;
  unsigned char n                               ;
  unsigned int  i                               ;
	u8 j;

  switch (Command)
  {
    case PCD_AUTHENT:
      irqEn   = 0x12                            ;
      waitFor = 0x10                            ;
      break                                     ;
    case PCD_TRANSCEIVE:
      irqEn   = 0x77                            ;
      waitFor = 0x30                            ;
      break                                     ;
    default:
      break                                     ;
  }
  rc522_write_1_byte(ComIEnReg,irqEn|0x80)              ; //
  rc522_write_clrbit_mask(ComIrqReg,0x80)                  ;
  rc522_write_1_byte(CommandReg,PCD_IDLE)               ;
  rc522_write_setbit_mask(FIFOLevelReg,0x80)                 ; // 清空FIFO
  for(i=0; i<InLenByte; i++)
    rc522_write_1_byte(FIFODataReg,pInData[i])          ; // 数据写入FIFO
  rc522_write_1_byte(CommandReg, Command)               ; // 命令写入命令寄存器
  if(Command == PCD_TRANSCEIVE)
    rc522_write_setbit_mask(BitFramingReg,0x80)              ; // 开始发送
  i = 6000                                      ; //根据时钟频率调整，操作M1卡最大等待时间25ms
  do
  {
    n = rc522_read_1_byte(ComIrqReg)                    ;
    i--                                         ;
  }
  while((i!=0)&&!(n&0x01)&&!(n&waitFor))        ;
  rc522_write_clrbit_mask(BitFramingReg,0x80)              ;
  if(i!=0)
  {
		j=rc522_read_1_byte(ErrorReg);
    if(!(j&0x1B))
    {
      status = MI_OK                            ;
      if (n&irqEn&0x01)
        status = MI_NOTAGERR                    ;
      if(Command==PCD_TRANSCEIVE)
      {
        n = rc522_read_1_byte(FIFOLevelReg)             ;
        lastBits = rc522_read_1_byte(ControlReg)&0x07   ;
        if(lastBits)
          *pOutLenBit = (n-1)*8 + lastBits      ;
        else
          *pOutLenBit = n*8                     ;
        if(n==0)
          n = 1                                 ;
        if(n>MAXRLEN)
          n = MAXRLEN                           ;
        for (i=0; i<n; i++)
          pOutData[i] = rc522_read_1_byte(FIFODataReg)  ;
      }
    }
    else
		{
			//printf("命令错误=%x\n",j);

      status = MI_ERR                           ;
		}
  }
  rc522_write_setbit_mask(ControlReg,0x80)                   ;// stop timer now
  rc522_write_1_byte(CommandReg,PCD_IDLE)               ;
  return status;
}
char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN];

    rc522_write_clrbit_mask(Status2Reg,0x08);//寄存器包含接收器和发送器和数据模式检测器的状态标志
    rc522_write_1_byte(BitFramingReg,0x00);//不启动数据发送，接收的LSB位存放在位0，接收到的第二位放在位1，定义发送的最后一个字节的位数为8
    rc522_write_clrbit_mask(CollReg,0x80);//所有接收的位在冲突后将被清除。


    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])//返回四个字节，最后一个字节为校验位
         {   status = MI_ERR;    }
    }

    rc522_write_setbit_mask(CollReg,0x80);
    return status;
}



//*******************************//
u8 rc522_read_1_byte(u8 address)
{
	u8 temp8_ret;


		GPIO_ResetBits(GPIOB,GPIO_Pin_1);


	address<<=1;
	address|=0x80;



	SPI3_RW_Byte(address);

	temp8_ret=SPI3_RW_Byte(0xff);//读取1个字节数据


		GPIO_SetBits(GPIOB,GPIO_Pin_1);

	return temp8_ret;
}

void rc522_write_1_byte(u8 address,u8 input)
{
	//u8 temp8_ret;

		GPIO_ResetBits(GPIOB,GPIO_Pin_1);



	address<<=1;
	address&=0x7e;


	SPI3_RW_Byte(address);

	SPI3_RW_Byte(input);


		GPIO_SetBits(GPIOB,GPIO_Pin_1);

	//return temp8_ret;
}


//SPI分频是8				4不正常
static u8 SPI3_RW_Byte(u8 byte)
{

	while((SPI3->SR & SPI_I2S_FLAG_TXE)==RESET);
    /*发送一个字节*/
	SPI3->DR = byte;
	/* 等待接收寄存器有效*/
	while((SPI3->SR & SPI_I2S_FLAG_RXNE)==RESET);

	return(SPI3->DR);
}






//把4个字节	转换成为10位 10进制数字
void convert_4u8_dec(u8 *input,u8 *output)
{
	u32 temp32_1=0;
	u32 temp32_2=0;

	//printf("convert输入=%x %x %x %x\r\n",input[0],input[1],input[2],input[3]);

	temp32_2=(u32)input[0];
	temp32_1=temp32_1+temp32_2;
	temp32_1=temp32_1<<8;
	temp32_2=(u32)input[1];
	temp32_1=temp32_1+temp32_2;
	temp32_1=temp32_1<<8;
	temp32_2=(u32)input[2];
	temp32_1=temp32_1+temp32_2;
	temp32_1=temp32_1<<8;
	temp32_2=(u32)input[3];
	temp32_1=temp32_1+temp32_2;

	//printf("convert 1=%x %d\r\n",temp32_1,temp32_1);

	temp32_2=temp32_1/1000000000;
	temp32_1=temp32_1%1000000000;
	output[0]=((u8)temp32_2)+0x30;
	temp32_2=temp32_1/100000000;
	temp32_1=temp32_1%100000000;
	output[1]=((u8)temp32_2)+0x30;
	temp32_2=temp32_1/10000000;
	temp32_1=temp32_1%10000000;
	output[2]=((u8)temp32_2)+0x30;
	temp32_2=temp32_1/1000000;
	temp32_1=temp32_1%1000000;
	output[3]=((u8)temp32_2)+0x30;
	temp32_2=temp32_1/100000;
	temp32_1=temp32_1%100000;
	output[4]=((u8)temp32_2)+0x30;
	temp32_2=temp32_1/10000;
	temp32_1=temp32_1%10000;
	output[5]=((u8)temp32_2)+0x30;
	temp32_2=temp32_1/1000;
	temp32_1=temp32_1%1000;
	output[6]=((u8)temp32_2)+0x30;
	temp32_2=temp32_1/100;
	temp32_1=temp32_1%100;
	output[7]=((u8)temp32_2)+0x30;
	temp32_2=temp32_1/10;
	temp32_1=temp32_1%10;
	output[8]=((u8)temp32_2)+0x30;
	temp32_2=temp32_1;
	output[9]=((u8)temp32_2)+0x30;


	//printf("convert 2=%s\r\n",output);

}

/**
 @brief  每秒去做的事
 @param  无
 @retval 无
*/
void loop(void)
{
	u8 temp8_2;

	rc522_init();
	temp8_2=0;

	temp8_2=rc522_s50_read_sn(FUNCTION_STR);		//读卡
	if(temp8_2==2)
	{
		printf("天线首次读卡失败\r\n");
		temp8_2=rc522_s50_read_sn(FUNCTION_STR);		//读卡
	}



	if(temp8_2==2)
	{
		printf("天线无ic卡\r\n");

		WaterControlGpioSetOutput(1);
		g_surplusTime = 0;
		DisplayTime(g_surplusTime);

	}
	else if(temp8_2==0)
	{
		Delay(15);

		convert_4u8_dec(FUNCTION_STR,&FUNCTION_STR[10]);
		FUNCTION_STR[20]='#';
		FUNCTION_STR[21]=0x00;
		printf("report*input=");
		printf("%s",&FUNCTION_STR[10]);

		WaterControlGpioSetOutput(0);
		g_surplusTime++;
		DisplayTime(g_surplusTime);
	}
}
/* End of File ------------------------------------------------------------- */
