#include <stdio.h>
#include "hw_uart.h"


#define __SYSTIMER_PRESENT 1
#define __ECLIC_PRESENT 1
// #define __ONLY_FOR_DOXYGEN_DOCUMENT_GENERATION__
#include "demosoc.h"
#include "core_feature_base.h"
#include "core_feature_timer.h"
#include "core_feature_eclic.h"
#include "riscv_encoding.h"


#include "appcpp.h"



#define RAM_BASE 0x3FDD0000
typedef struct {
    uint32_t count; // current enter loader_main
    uint32_t addr;  // storage address
} backup_ram;
backup_ram *backup_ram_addr = (backup_ram *)RAM_BASE;

const int loop = 10;
int32_t cnt = 0;

void setup_timer(void) {
//    printf("init timer and start \r\n");
    uint64_t now = SysTimer_GetLoadValue();
    uint64_t then = now + 65535;
    SysTimer_SetCompareValue(then);
}
#define timer_sw_irq_handler eclic_msip_handler
#define timer_irq_handler eclic_mtip_handler
void timer_irq_handler(void) {
    setup_timer();
    cnt ++;
    printf("%s triggered@cnt:%d\r\n", __func__, cnt);
}

void timer_sw_irq_handler(void) {
    printf ("%s triggered\r\n", __func__);
}

void timer_test(void) {
    uint32_t returnCode;
    cnt = 0;
    returnCode = ECLIC_Register_IRQ(
        7, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 0, timer_irq_handler
    );
    // uart_write();


    __enable_irq();
//    SysTimer_Stop();
    SysTimer_Start();
    setup_timer();

    while (cnt < loop) {
//        printf("cnt: %d\r\n", SysTimer_GetLoadValue());
    }
    ECLIC_DisableIRQ(7);

    returnCode = ECLIC_Register_IRQ(
        3, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_POSTIVE_EDGE_TRIGGER, 2, 0, timer_sw_irq_handler
    );
}




void test(void) {
    // char str[] = "Hello World From Nuclei RISC-V Processor!\r\n";
    char str[128];
    cpp_main();
    printf("built at %s %s\r\n", __DATE__, __TIME__);
    printf("this is build from optek-riscv-template\r\n");
    int len = sprintf(str, "Hello World From Nuclei %s Processor: %d!!!!\r\n", "RISC-V", 2);
    uart_write(UART0, str, len);
    printf("shang hai zi lai shui lai zi hai shang\r\n");
    printf("where is the code\r\n");
    printf("hello world from %s!!!", __func__);
    printf("Cluster %lu, Hart %lu, MISA: 0x%lx\r\n", (__RV_CSR_READ(CSR_MHARTID) >> 8) & 0xFF, (__RV_CSR_READ(CSR_MHARTID)), __RV_CSR_READ(CSR_MISA));
    printf("JEDEC ID: 0x%lx\r\n", ADDR_SPI_JEDEC_ID);

    // for (int i = 0; i < backup_ram_addr->count; i++) {
    //     printf("backup_ram_addr[%d]: %d\r\n", i, backup_ram_addr[i]);
    // }
    printf("backup_ram_addr->count: %d\r\n", backup_ram_addr->count);
    backup_ram_addr->count = 0;

    timer_test();
}

