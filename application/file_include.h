
#ifndef __file_include_h
#define __file_include_h

// #include  "STM32F0XX_CONF.H"
// #include  "stm32f0xx.h"
#include  <string.h>     //字符串处理函数


// #include  "drv_init.h"   //系统初始化操作
// #include  "drv_uart1.h"  //系统串口1
// #include  "drv_ms520.h"  //NFC操作

#include  "NFC_TypeA.h"



#define  TRUE                     0x02   //确定，正确，有。
#define  FALSE                    0x08   //不正确，没有
#define  EEROR                    0X04   //错误


// #define  LED_RED_OFF    GPIO_ResetBits(GPIOA, GPIO_Pin_12)     //PA12-红色LED灯。 =1亮，=0灭
// #define  LED_RED_ON     GPIO_SetBits(GPIOA, GPIO_Pin_12)


// #define  RESTB_0        GPIO_ResetBits(GPIOA, GPIO_Pin_8)    //PA8-复位信号-飞线过去
// #define  RESTB_1        GPIO_SetBits(GPIOA, GPIO_Pin_8)



#endif


