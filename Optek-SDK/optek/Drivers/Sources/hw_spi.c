#include "hw_spi.h"
#include "hal_gpio.h"
#include "hal_dma.h"
#include <stdint.h>
#include "c_def.h"
#define SPI_ENABLE
#ifdef SPI_ENABLE


//#define READ_SPI_FLASH
#define READ_SPI_FLASH_ONE_PIN //one pin for spi flash read and write


//#define SPI_SLAVE_TEST //spi slave

#ifndef SPI_SLAVE_TEST
#define MASTER_SOC_2 //spi master delay

#endif

#define SPI_CS_ACTIVE_LOW

#define SPI_CS0_USED SPI_CS0_USED
//#define SPI_CS1_USED

//#define SPI_READ_FUNCTION
#endif


void hw_spi_2_en_select(uint8_t sel)
{
	U32 val;
	val = REG_MISC_CPUCLK;
	MISC_CPUCLK_CONTROLs *pCpuClk = (MISC_CPUCLK_CONTROLs *) &val;

	if (sel == FALSE)
	{
		pCpuClk->spi_2_en = 0;
	}
	else if (sel == TRUE)
	{
		pCpuClk->spi_2_en = 1;
	}
	else
	{
		DBG_Assert(FALSE);
	}

	pCpuClk->jtag_en = 0;
	REG_MISC_CPUCLK = val;
}
void hw_spi_gpio_init(SPI_REGISTERS *hspi, uint32_t mode)
{
    if (hspi == SPI1)
    {
        hal_gpio_mux_func_select(SPI_CLK_PIN_INDEX, MUX_SEL_FUNCTION1);
        hal_gpio_mux_func_select(SPI_D0_PIN_INDEX, MUX_SEL_FUNCTION1);
        if (mode & SPI_CS0_USED)
        {
            hal_gpio_mux_func_select(SPI_CS0_PIN_INDEX, MUX_SEL_FUNCTION1);
        }
        if (mode & SPI_CS1_USED)
        {
            hal_gpio_mux_func_select(SPI_CS1_PIN_INDEX, MUX_SEL_FUNCTION4);
        }
        if ((mode & SPI_MISO_USED) && (!(mode & SPI_3WIRE)))
        {
            hal_gpio_mux_func_select(SPI_D1_PIN_INDEX, MUX_SEL_FUNCTION4);
        }
    } else if (hspi == SPI2) {

        uint32_t val;
        I2S_ST_CONFIG_AHB		*pI2sSt = (I2S_ST_CONFIG_AHB *) &val;
        val = I2S_STEREO_CONFIG;
        pI2sSt->i2s_rx_mode = 1;					//i2s master
        I2S_STEREO_CONFIG = val;
        // this code used in spi1 will cause problem

        hal_gpio_mux_func_select(SPI_2_CLK_PIN_INDEX, MUX_SEL_FUNCTION3);
        hal_gpio_mux_func_select(SPI_2_MOSI_PIN_INDEX, MUX_SEL_FUNCTION3);
        if (mode & SPI_CS0_USED)
        {
            hal_gpio_mux_func_select(SPI_2_CS_PIN_INDEX, MUX_SEL_FUNCTION3);
        }
        if (mode & SPI_MISO_USED)
        {
            hal_gpio_mux_func_select(SPI_2_MISO_PIN_INDEX, MUX_SEL_FUNCTION3);
        }
    }
}

void hw_spi_init(SPI_REGISTERS *hspi, uint32_t mode)
{
    volatile uint32_t val;
	MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *) &val;

	val = REG_MISC_CLKGATE;
	pGate->spi_gate = 1; //spi clk
	pGate->AHB_Audio_Controller_gate = 1;
	REG_MISC_CLKGATE = val;
    // ((MISC_CLKGATE_CONTROLs *)(0x18800000 + 8)) -> spi_gate = 1; //open clk gate
    // ((MISC_CLKGATE_CONTROLs *)(0x18800000 + 8)) -> AHB_Audio_Controller_gate = 1; //open clk gate


    hw_spi_gpio_init(hspi, mode);


    SPI_FORMATs *fmt = (SPI_FORMATs*)&val;
    val = hspi->format;
    if ((mode & SPI_MODE_MASK) == SPI_MODE_0)
    {
        fmt->cpha = 0;
        fmt->cpol = 0;
    }
    else if ((mode & SPI_MODE_MASK) == SPI_MODE_1)
    {
        fmt->cpha = 1;
        fmt->cpol = 0;
    }
    else if ((mode & SPI_MODE_MASK) == SPI_MODE_2)
    {
        fmt->cpha = 0;
        fmt->cpol = 1;
    }
    else if ((mode & SPI_MODE_MASK) == SPI_MODE_3)
    {
        fmt->cpha = 1;
        fmt->cpol = 1;
    }


    fmt->autocsActive = 0;
    if (mode & SPI_CS0_USED)
    {
        fmt->cssel = SPI_CS0;
    }
    else if (mode & SPI_CS1_USED)
    {
        fmt->cssel = SPI_CS1;
    }



    if (mode & SPI_3WIRE)
    {
        // printf("3-wire spi\n");
        fmt->din_out = 1;     // 3-wire spi
    }
    else
    {
        fmt->din_out = 0;     // 4-wire spi
    }

    hspi->format = val;



    SPI_CONTROLs *ctrl = (SPI_CONTROLs *)(&val);
    val = hspi->control;
    ctrl->reset = 0;
    ctrl->dmaSel = 1;

    if (mode & SPI_MODE_SLAVE)
    {
        ctrl->slave = 1;
    }
    else
    {
        ctrl->slave = 0;
    }

    // if (mode & SPI_CS_MANUAL)
    // {
    //     ctrl->enable = 0;
    // }
    // else
    // {
    //     ctrl->enable = 1;
    // }

    if (mode & SPI_QSPI_ENABLE)
    {
        if (!(mode & SPI_MISO_USED))
        {
            DBG_Assert(FALSE);
            // SPI_MISO_USED should be work with QSPI
        }
        ctrl->qspi = 1;
        // if (mode & SPI_MODE_SLAVE)
        // {
        //     DBG_Assert(FALSE);
        //     // SPI_MODE_SLAVE should be work with QSPI
        // }
        ctrl->bits = SPI_WORD_16BITS;
    }
    else
    {
        ctrl->qspi = 0;
        ctrl->bits = SPI_WORD_8BITS;
    }

    ctrl->div = SPI_CLK_DEFAULT;

    ctrl->txFifoEn = 1;
    ctrl->rxFifoEn = 1;


    ctrl->enable = 1;

    hspi->control = val;

    if (hspi == SPI2) {
        hw_spi_2_en_select(TRUE);
    }

    // spi int
    // SPI_INT_CONTROL *int_ctrl = (SPI_INT_CONTROL *) &val;
    // val = hspi->int_mask;


    // spi dma
}


void hw_spi_write_byte(SPI_REGISTERS *hspi, uint8_t data)
{
    while (!((hspi->stat.txBufEmpty) && (hspi->stat.idle)));

    hspi->max_trans_num = 1;
    hspi->rxtx_num = 1;
	__asm__("nop");
    hspi->tx_data = data;
}

void hw_spi_write_bytes(SPI_REGISTERS *hspi, uint8_t *buf, uint32_t byte_size)
{
    while (!((hspi->stat.txBufEmpty) && (hspi->stat.idle)));


    hspi->max_trans_num = byte_size;
    hspi->rxtx_num = byte_size;

    for (uint32_t i = 0; i < byte_size; i++)
    {
        hspi->tx_data = buf[i];
        while (hspi->stat.txBufFull);
    }
}

void hw_spi_write_reg(SPI_REGISTERS *hspi, uint32_t addr, const uint8_t *data, uint32_t length)
{
    while (!((hspi->stat.txBufEmpty) && (hspi->stat.idle)));

    hspi->rxtx_num = length+1;
    hspi->max_trans_num = length+1;

    hspi->tx_data = addr;

    for (uint32_t i = 0; i < length; i++)
    {
        while (hspi->stat.txBufFull);
        hspi->tx_data = data[i];
    }

}

void hw_spi_read(SPI_REGISTERS *hspi, uint8_t *buf, uint32_t byte_size)
{
    while (!((hspi->stat.txBufEmpty) && (hspi->stat.idle)));

    hspi->rxtx_num = 0;
    hspi->max_trans_num = byte_size;
    for (uint32_t i = 0; i < byte_size; i++) {
        while (hspi->stat.rxBufEmpty);
        buf[i] = hspi->rx_data;
    }

}

void hw_spi_read_reg(   SPI_REGISTERS *hspi, 
                        uint32_t addr, 
                        uint8_t *data, 
                        uint32_t length     )
{
    while (!((hspi->stat.txBufEmpty) && (hspi->stat.idle)));

    const int tx_num = 1;
    hspi->rxtx_num = tx_num;
    hspi->max_trans_num = tx_num + length;

    for (int i = 0; i < tx_num; i++) {
        while (hspi->stat.txBufFull);
        hspi->tx_data = addr;
    }
    // delay

    for (uint32_t i = 0; i < length; i++) {
        while (hspi->stat.rxBufEmpty);
        data[i] = hspi->rx_data;
    }

    // hw_spi_write_byte(hspi, addr);

    // hw_spi_read(hspi, data, length);
}


int32_t spi_dma_open(   int32_t tx_ena, 
                        int32_t rx_ena,
                        dma_done_callback *tx_cb, 
                        dma_done_callback * rx_cb)
{
    printf("%s\n" , __func__);
#ifdef SPI_DMA_ENABLE
    MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *)\
                                    &REG_MISC_CLKGATE;
    pGate->spi_gate = 1; //open clk gate
#ifdef SPI_DMA_TX_ENABLE
    if (tx_ena){
        hal_dma_transfer_init( SPI_TX_DMA_ID, 
                                (U32 *)(&REG_SPI_TX_DATA),
                                NULL, SOURCE_DMA_SPI_TX, 
                                DMA_TRANS_1BYTE, DMA_BURST_SINGLE, 
                                1, 0, 1, tx_cb);
        DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(SPI_TX_DMA_ID)));
    }
#endif // SPI_DMA_TX_ENABLE
#ifdef SPI_DMA_RX_ENABLE
    if (rx_ena){
            hal_dma_transfer_init(  SPI_RX_DMA_ID, 
                                    NULL, 
                                    (U32 *)(&REG_SPI_RX_DATA), 
                                    SOURCE_DMA_SPI_RX, 
                                    DMA_TRANS_1BYTE, 
                                    DMA_BURST_SINGLE, 
                                    0, 1, 1, rx_cb);
            DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(SPI_RX_DMA_ID)));
        }
#endif // DEBUG
    
#endif
        return 0;
}


void spi_dma_write_bytes(U32 *p_data, U32 size)
{
    hal_dma_send_data(SPI_TX_DMA_ID, p_data, size);
}


#define SPI_DMA_TX_TEST


#ifdef SPI_DMA_TX_TEST
#define TX_BUF_LEN    4
#define RX_BUF_LEN    4
U8 spi_tx_buf[2][TX_BUF_LEN];
U8 spi_rx_buf[2][RX_BUF_LEN];
U16 spi_tx_len;
U16 spi_rx_len;
U8 spi_tx_buf_index;
U8 spi_rx_buf_index;
#endif

#ifdef SPI_DMA_TX_TEST

void spi_dma_tx_callback(void)
{
	U32 val;
	SPI_STATUSs *p_status = (SPI_STATUSs *)&val;
	int index;
	int byte_length;
	U32 *buf;

	#ifdef SPI_STATUS_IDLE_POLLING
	while (1)
	{
		val = REG_SPI_STATUS;
		asm("nop");
		if ( (p_status->idle) && (p_status->txBufEmpty) ) 
		{
			break;
		}
	}
	#else
	delay_us(11); //delay 2.5us at CPU_FREQ_BASE_32X
	#endif //SPI_STATUS_IDLE_POLLING

	byte_length = spi_tx_len;
	REG_SPI_MAX_TRANS_NUM = byte_length;
	REG_SPI_RXTX_NUM = byte_length;
	index = spi_tx_buf_index;
    hal_dma_send_data(SPI_TX_DMA_ID, spi_tx_buf[index], byte_length);
	spi_tx_buf_index = !spi_tx_buf_index;
}
#endif

#ifdef SPI_DMA_TX_TEST
void spi_dma_tx_test(void)
{   
    // printf("%s\n" , __func__);
	int i, j;

	for (j=0; j<1; j++)
	{
		if (TX_BUF_LEN >= 4)
		{
			for (i=0; i<(TX_BUF_LEN/4); i++)
			{
				spi_tx_buf[j][i*4+0] = 0x00;
				spi_tx_buf[j][i*4+1] = 0x01;
				spi_tx_buf[j][i*4+2] = 0x02;
				spi_tx_buf[j][i*4+3] = 0x03;
			}
		}
	}

	for (j=1; j<2; j++)
	{
		if (TX_BUF_LEN >= 4)
		{
			for (i=0; i<(TX_BUF_LEN/4); i++)
			{
				spi_tx_buf[j][i*4+0] = 0x03;
				spi_tx_buf[j][i*4+1] = 0x02;
				spi_tx_buf[j][i*4+2] = 0x01;
				spi_tx_buf[j][i*4+3] = 0x00;
			}
		}
	}


	
	memset(spi_rx_buf, 0xFF, sizeof(spi_rx_buf));
	spi_dma_open(1, 0, NULL, NULL);

	REG_SPI_MAX_TRANS_NUM = spi_tx_len;
	REG_SPI_RXTX_NUM = spi_tx_len;
	spi_dma_write_bytes(spi_tx_buf, 10);
}
#endif //SPI_DMA_TX_TEST
