#ifndef __MS520_H__
#define __MS520_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MS520_SPEED_106 0x00
#define MS520_SPEED_212 0x10
#define MS520_SPEED_424 0x20
#define MS520_SPEED_848 0x30
#define MS520_SPEED_1696 0x40
#define MS520_SPEED_3392 0x50

#define MS520_REG_RFU00         0x00  // 保留
#define MS520_REG_Command       0x01  // 启动和停止命令的执行
#define MS520_REG_cmd_idle      0x00  // 命令寄存器空闲状态
#define MS520_REG_cmd_mem           0x01  // 命令寄存器存储数据
#define MS520_REG_cmd_gen_randomid  0x02  // 命令寄存器生成随机UID
#define MS520_REG_cmd_calc_crc      0x03  // 命令寄存器计算CRC
#define MS520_REG_cmd_transmit      0x04  // 命令寄存器发送数据
#define MS520_REG_cmd_no_cmd        0x07  // 命令寄存器无命令
#define MS520_REG_cmd_receive       0x08  // 命令寄存器接收数据
#define MS520_REG_cmd_transceive    0x0c  // 命令寄存器发送并接收数据
#define MS520_REG_cmd_reserve       0x0d  // 命令寄存器保留
#define MS520_REG_cmd_soft_reset    0x0f  // 命令寄存器软复位
#define MS520_REG_Status1       0x07  // 接收器和发送器状态寄存器
#define MS520_REG_Status2       0x08  // 接收器和发送器状态寄存器
#define MS520_REG_FIFOData      0x09  // FIFO 数据寄存器
#define MS520_REG_FIFOLevel     0x0a  // [6:0]FIFO 中的字节数 [7]清空 FIFO
#define MS520_REG_FIFOLevel_clr_mask 0x80
#define MS520_REG_FIFOLevel_num_mask 0x7f
#define MS520_REG_Control       0x0c  // 控制寄存器
#define MS520_REG_BitFraming    0x0d  // 控制发送和接收的位帧 [7]启动数据发送 [4:6]接受帧bit offset [0:2] 发送最后一个字节的位数
#define MS520_REG_BitFraming_send_mask 0x80
#define MS520_REG_CWGsP         0x28  // 定义 P 驱动器输出端非调制时的电导值（可以用于调节阻抗匹配）
#define MS520_REG_Mode          0x11
#define MS520_REG_TxMode        0x12  // 定义发送过程的数据速率
#define MS520_REG_RxMode        0x13  // 定义接受过程的数据速率
#define MS520_REG_TxControl     0x14  // 控制天线驱动器管脚 TX1 和 TX2 的逻辑状态
#define MS520_REG_TxASK         0x15  // 控制发送调制的设置
#define MS520_REG_TModeReg      0x2a
#define MS520_REG_TPrescalerReg 0x2b
#define MS520_REG_TReloadL      0x2c
#define MS520_REG_TReloadH      0x2d
#define MS520_REG_AutoTest      0x36
#define MS520_REG_Version       0x37  // 版本寄存器
#define MS520_REG_Version_value 0x92  // 版本寄存器的值

typedef enum {
    MS520_SUCCESS,
    MS520_ERROR
} MS520_Status;



void ms520_init(void);
void ms520_write_reg(uint8_t reg, const uint8_t value);
uint8_t ms520_read_reg(uint8_t reg);
void delay_ms(uint32_t ms);
void ms520_write_regs(uint8_t reg, const uint8_t *buf, uint32_t len);
void ms520_read_regs(uint8_t reg, uint8_t *buf, uint32_t len);
void ms520_wuqa(void);
MS520_Status ms520_reqa(void);
void ms520_anticolision(void);
void ms520_select(void);
void ms520_hlta(void);
void ms520_atqa(void);



//关闭天线
void ms520_close_antenna(void);
//打开天线
void ms520_open_antenna(void);


//**************************ATQA返回说明****************************//
//                0x0044 = Mifare_UltraLight                        //
//                0x0004 = Mifare_One(S50)                          //
//                0x0002 = Mifare_One(S70)                          //
//                0x0008 = Mifare_Pro(X)                            //
//                0x0344 = Mifare_DESFire                           //
//******************************************************************//
//*****************************PICC返回*****************************//
typedef struct {
    union {
        struct {                            //Answer To Request, Type A//
            unsigned char MSB;
            unsigned char LSB;
        } ATQA;
        uint16_t ATQA_value;
    };
    unsigned char UID[15];      //include UID check byte and cascade tag;   串联级别最多2
    unsigned char SAK;          //
} FormatPICC;

#ifdef __cplusplus
}
#endif

#endif // __MS520_H__