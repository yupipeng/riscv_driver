
#include "regmap.h"


void jtag_enable(int con)
{
    MISC_CPUCLK_CONTROLs *cpuclk = &REG_MISC_CPUCLK;

    cpuclk->jtag_en = con;
}