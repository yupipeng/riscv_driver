
#include "core_feature_base.h"

void DelayMs(uint32_t ms)
{
    uint64_t cur_cycle = __get_rv_cycle();
    uint64_t end_cycle = cur_cycle + (12000000  / 1000 ) * ms;

    do {
        cur_cycle = __get_rv_cycle();
    } while (cur_cycle < end_cycle);
}


void DelayUs(uint32_t us)
{
    uint64_t cur_cycle = __get_rv_cycle();
    uint64_t end_cycle = cur_cycle + (12000000  / 1000 / 1000) * us;

    do {
        cur_cycle = __get_rv_cycle();
    } while (cur_cycle < end_cycle);
}

// void sleep_us(uint32_t us) {
//     for (int i = 0; i < us*3; i+=2) {
//         __asm__ __volatile__("nop");
//     }
// }

// void sleep_ms(uint32_t ms) {
// 	sleep_us(ms * 1000);
// }
// void timer_delayus(U32 xus)
// {
// 	U32 StartTime;
// 	U32 CurTime;
// 	U32 EndTime;
// 	U8 bTimeOut;

// 	StartTime = read_ccount();
// 	EndTime = StartTime + (xus*CLOCKS_PER_US);

// 	while (1)
// 	{
// 		CurTime = read_ccount();

// 		if (StartTime <= EndTime)
// 		{
// 			bTimeOut = ((CurTime >= StartTime) && (CurTime < EndTime))? FALSE : TRUE;
// 			if (bTimeOut)
// 			{
// 				break;
// 			}
// 		}
// 		else
// 		{
// 			bTimeOut = ((CurTime >= StartTime) || (CurTime < EndTime))? FALSE : TRUE;
// 			if (bTimeOut)
// 			{
// 				break;
// 			}
// 		}
// 	}
// }

