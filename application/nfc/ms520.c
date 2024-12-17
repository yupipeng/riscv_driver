#include "ms520.h"
#include <stdio.h>

#define write_reg(reg, val) ms520_write_reg(reg, val)
#define read_reg(reg) ms520_read_reg(reg)


void ms520_init(void) {
    const uint8_t start_up_sequence[] = {
        MS520_REG_Control,   0x10,   // 关闭定时器
        MS520_REG_CWGsP,     0x00,   // 设置电导为最大
        MS520_REG_TxMode,    0x00,   // 设置速率最低，106kBd，发送数据不生成CRC校验，不发送调制数据的反相
        MS520_REG_RxMode,    0x00,   // 设置速率最低，106kBd，接收数据不产生CRC校验（仅在106kBd下可设置不产生），不忽略接收到的第一个无效数据流
        MS520_REG_TxControl, 0x83,   // 打开载波输出
        MS520_REG_TxASK,     0x40,   // 强制 100% ASK 调制
    };

    // write_reg(MS520_REG_Command, MS520_REG_cmd_soft_reset); // 软复位
    // delay_ms(5);
    // write_reg(MS520_REG_Mode, 0x3d);
    // write_reg(MS520_REG_TReloadL, 30);
    // write_reg(MS520_REG_TReloadH, 0);
    // write_reg(MS520_REG_TModeReg, 0x8d);
    // write_reg(MS520_REG_TPrescalerReg, 0x3e);

    // write_reg(MS520_REG_TxASK, 0x40);

    // delay_ms(5);
    // uint8_t temp = read_reg(MS520_REG_TxControl);
    // printf("TxASK: 0x%02x\n", temp);
    // if (!(temp & 0x03)) {
    //     write_reg(MS520_REG_TxControl, 0x03);
    // }
    // temp = read_reg(MS520_REG_TxASK);
    // printf("TxASK: 0x%02x\n", temp);

    for (int i = 0; i < sizeof(start_up_sequence); i += 2) {
        write_reg(start_up_sequence[i], start_up_sequence[i + 1]);
    }

}

void ms520_self_test(void)
{
    write_reg(MS520_REG_Command, MS520_REG_cmd_soft_reset); // 软复位
    for (int i = 0; i < 25; i++) {
        write_reg(MS520_REG_FIFOData, 0x00);
    }
    write_reg(MS520_REG_Command, MS520_REG_cmd_mem);
    write_reg(MS520_REG_AutoTest, 0x09);
    write_reg(MS520_REG_FIFOData, 0x00);
    write_reg(MS520_REG_Command, MS520_REG_cmd_calc_crc);
    delay_ms(5);
    uint8_t len = read_reg(MS520_REG_FIFOLevel) & MS520_REG_FIFOLevel_num_mask;
    printf("len: 0x%02x\n", len);
    len = 64;
    for (int i = 0; i < len; i++) {
        printf("0x%02x ", read_reg(MS520_REG_FIFOData));
    }
}


void ms520_close_antenna(void)
{
    write_reg(MS520_REG_TxControl, 0x00); //关闭天线 0x80
}

void ms520_open_antenna(void)
{
    write_reg(MS520_REG_TxControl, 0x03); //打开天线 0x83
}


MS520_Status ms520_reqa(void)
{
    write_reg(MS520_REG_TxMode, 0x00);
    write_reg(MS520_REG_RxMode, 0x00);

    write_reg(MS520_REG_FIFOLevel, 0X80);
    write_reg(MS520_REG_BitFraming, 0x07);

    write_reg(MS520_REG_FIFOData, 0x52);  // fifo 内 写入要发送给卡片的数据
/*    寻卡方式
*                      = 0x52：寻感应区内所有符合14443A标准的卡
 *                     = 0x26：寻未进入休眠状态的卡
 *         pTagType，卡片类型代码
 *                   = 0x4400：Mifare_UltraLight
 *                   = 0x0400：Mifare_One(S50)
 *                   = 0x0200：Mifare_One(S70)
 *                   = 0x0800：Mifare_Pro(X))
 *                   = 0x4403：Mifare_DESFire
*/
    write_reg(MS520_REG_Command, 0x0c);  // 发送和接收
    write_reg(MS520_REG_BitFraming, 0x87);  // 开始发送

    delay_ms(3);

    uint8_t fifo_level = 0;
    fifo_level = read_reg(MS520_REG_FIFOLevel) & MS520_REG_FIFOLevel_num_mask;

    printf("fifo_level: %d\n", fifo_level);
    if (fifo_level != 2) {
        return MS520_ERROR;
    }
    else
    {
        do {
            uint8_t a = read_reg(MS520_REG_FIFOData);
            uint8_t b = read_reg(MS520_REG_FIFOData);
            uint16_t atqa = a << 8 | b;
            printf("ATQA: 0x%04x\n", atqa);
        } while (read_reg(MS520_REG_FIFOLevel) & MS520_REG_FIFOLevel_num_mask);
    }


    return MS520_SUCCESS;
}



