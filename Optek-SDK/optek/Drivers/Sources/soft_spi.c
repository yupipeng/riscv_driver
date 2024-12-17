#include "regmap.h"

#include "hw_gpio.h"
#include "hal_gpio.h"
#include "soft_spi.h"


#if 0
#define LOW_LEVEL_W_DELAY   delayus(1)
#else
#define LOW_LEVEL_W_DELAY
//#define LOW_LEVEL_W_DELAY   asm("nop")
#endif

#if 0
#define HIGH_LEVEL_W_DELAY  delayus(1)
#else
#define HIGH_LEVEL_W_DELAY  asm("nop");\
                            asm("nop");
#endif

#if 0
#define LOW_LEVEL_R_DELAY   delayus(1)
#else
#define LOW_LEVEL_R_DELAY   asm("nop");\
                            asm("nop");
#endif

#if 0
#define HIGH_LEVEL_DELAY  delayus(1)
#else
//#define HIGH_LEVEL_R_DELAY  asm("nop")
#define HIGH_LEVEL_R_DELAY
#endif


void soft_spi_cs_active(SOFT_SPI_HandleTypeDef *sspi)
{
#if SOFT_SPI_CS_ACTIVE_LEVEL == 0
    GPIOx_LOW(sspi->cs);
#elif SOFT_SPI_CS_ACTIVE_LEVEL == 1
    GPIOx_HIGH(sspi->cs);
#endif
}

void soft_spi_cs_idle(SOFT_SPI_HandleTypeDef *sspi)
{
#if SOFT_SPI_CS_ACTIVE_LEVEL == 0
    GPIOx_HIGH(sspi->cs);
#elif SOFT_SPI_CS_ACTIVE_LEVEL == 1
    GPIOx_LOW(sspi->cs);
#endif
}

void soft_spi_clk_idle(SOFT_SPI_HandleTypeDef *sspi)
{
#if SOFT_SPI_CPOL == 0
    GPIOx_LOW(sspi->sck);
#elif SOFT_SPI_CPOL == 1
    GPIOx_HIGH(sspi->sck);
#endif
}

void soft_spi_clk_active(SOFT_SPI_HandleTypeDef *sspi)
{
#if SOFT_SPI_CPOL == 0
    GPIOx_HIGH(sspi->sck);
#elif SOFT_SPI_CPOL == 1
    GPIOx_LOW(sspi->sck);
#endif
}


void soft_spi_gpio_init(SOFT_SPI_HandleTypeDef* sspi)
{
    hal_gpio_mux_func_select(sspi->cs, MUX_SEL_GPIO_OUTPUT);
    soft_spi_cs_idle(sspi);
    hal_gpio_mux_func_select(sspi->sck, MUX_SEL_GPIO_OUTPUT);
    soft_spi_clk_idle(sspi);
    hal_gpio_mux_func_select(sspi->sda, MUX_SEL_GPIO_OUTPUT);
    hal_gpio_mux_func_select(sspi->miso, MUX_SEL_GPIO_INPUT);

    // GPIOx_LOW(sspi->sda);
}


#if 0//GPIO_HS
// Write a byte to the SPI bus
void gpio_spi_write_byte(unsigned char byte) __INTERNAL_RAM_TEXT;
void gpio_spi_write_byte(unsigned char byte)
{
    int i;
    unit32_t dout_reg;


    dout_reg = REG_GPIO0_DOUT;

    // Iterate over each bit in the byte
    for (i = 0; i < 8; ++i)
    {
         //Set clock low
        //GPIO_LOW(GPIO_SCK_PIN);

        dout_reg &= (~INDEX2MASK(GPIO_SCK_PIN);
        REG_GPIO0_DOUT = dout_reg;

        // Write the MSB to the MOSI line
        if (byte & 0x80)
        {
            //GPIO_HIGH(GPIO_MOSI_PIN);
            dout_reg |= (~INDEX2MASK(GPIO_MOSI_PIN);
            REG_GPIO0_DOUT = dout_reg;
        }
        else
        {
            //GPIO_LOW(GPIO_MOSI_PIN);
            dout_reg &= (~INDEX2MASK(GPIO_MOSI_PIN);
            REG_GPIO0_DOUT = dout_reg;
        }

        // Shift the byte to the left
        byte <<= 1;
        LOW_LEVEL_W_DELAY

        // Set clock high
        //GPIO_HIGH(GPIO_SCK_PIN);
        dout_reg |= (INDEX2MASK(GPIO_SCK_PIN);
        REG_GPIO0_DOUT = dout_reg;

        HIGH_LEVEL_W_DELAY;
    }

    GPIO_LOW(GPIO_SCK_PIN);
    GPIO_LOW(GPIO_MOSI_PIN);
}


#else
void soft_spi_write_byte(SOFT_SPI_HandleTypeDef *sspi, uint8_t byte)
{

    //dout_reg = REG_GPIO0_DOUT;
    // soft_spi_cs_active(sspi);
    // Iterate over each bit in the byte
    for (int i = 0; i < 8; ++i)
    {
        //Set clock low
        soft_spi_clk_idle(sspi);
#if SOFT_SPI_CPHA == 1
        soft_spi_clk_active(sspi);
#endif

        // Write the MSB to the MOSI line
        if (byte & 0x80)
        {
            GPIOx_HIGH(sspi->sda);
        }
        else
        {
            GPIOx_LOW(sspi->sda);
        }

#if SOFT_SPI_CPHA == 0
        soft_spi_clk_active(sspi);
#endif

        byte <<= 1;
    }
    soft_spi_clk_idle(sspi);

    // GPIOx_LOW(sspi->sda);
    // keep the former or just set it to low is a question

    // soft_spi_cs_idle(sspi);
}

void soft_spi_write_bytes(SOFT_SPI_HandleTypeDef *sspi, uint8_t *buf, uint32_t length)
{
    //dout_reg = REG_GPIO0_DOUT;
    // soft_spi_cs_active(sspi);

    for (uint32_t i = 0; i < length; i++)
    {

        uint8_t byte = *buf++;
        // Iterate over each bit in the byte
        for (int i = 0; i < 8; ++i)
        {
            //Set clock low
            soft_spi_clk_idle(sspi);
    #if SOFT_SPI_CPHA == 1
            soft_spi_clk_active(sspi);
    #endif

            // Write the MSB to the MOSI line
            if (byte & 0x80)
            {
                GPIOx_HIGH(sspi->sda);
            }
            else
            {
                GPIOx_LOW(sspi->sda);
            }

    #if SOFT_SPI_CPHA == 0
            soft_spi_clk_active(sspi);
    #endif

            byte <<= 1;
        }


    }

    soft_spi_clk_idle(sspi);

    // GPIOx_LOW(sspi->sda);
    // keep the former or just set it to low is a question

    // soft_spi_cs_idle(sspi);
}
#endif


void soft_spi_read_byte(SOFT_SPI_HandleTypeDef *sspi, uint8_t *byte)
{

    // soft_spi_cs_active(sspi);
    // GPIO_LOW(GPIO_SCK_PIN);
    soft_spi_clk_idle(sspi);

    // Iterate over each bit in the byte
    for (int i = 0; i < 8; ++i)
    {
        (*byte) <<= 1;
        // soft_spi_clk_idle(sspi);
        soft_spi_clk_active(sspi);
#if SOFT_SPI_CPHA == 1
        soft_spi_clk_idle(sspi);
#endif

#if defined(SOFT_SPI_3WIRE)
        if (GPIO_READ(sspi->sda))
#else
        // Read the MISO line
        if (GPIO_READ(sspi->miso))
#endif
		{
			(*byte) |= 0x1;
		}
#if SOFT_SPI_CPHA == 0
        soft_spi_clk_idle(sspi);
        // soft_spi_clk_active(sspi);
#endif

        // HIGH_LEVEL_R_DELAY;

        // Generate a clock pulse
        // GPIO_LOW(GPIO_SCK_PIN);
        // LOW_LEVEL_R_DELAY;
    }

    soft_spi_clk_idle(sspi);
    // soft_spi_cs_idle(sspi);

}

void soft_spi_write_reg(SOFT_SPI_HandleTypeDef *sspi, uint8_t addr, const uint8_t *buf, uint32_t length)
{

    // GPIO_HIGH(GPIO_CS_PIN);
    // GPIO_LOW(GPIO_SCK_PIN);
    // GPIO_LOW(GPIO_MOSI_PIN);
	// hal_gpio_mux_func_select(GPIO_CS_PIN, MUX_SEL_GPIO_OUTPUT);
    // hal_gpio_mux_func_select(GPIO_SCK_PIN, MUX_SEL_GPIO_OUTPUT);
    // hal_gpio_mux_func_select(GPIO_MOSI_PIN, MUX_SEL_GPIO_OUTPUT);
    // hal_gpio_mux_func_select(GPIO_MISO_PIN, MUX_SEL_GPIO_INPUT);

    // GPIO_LOW(GPIO_CS_PIN);

    //delayus(1);
    soft_spi_cs_active(sspi);

    soft_spi_write_byte(sspi, addr);

    // delayus(1);
    soft_spi_write_bytes(sspi, buf, length);


    //delayus(1);
    // GPIO_HIGH(GPIO_CS_PIN);
    soft_spi_cs_idle(sspi);


	// hal_gpio_mux_func_select(GPIO_CS_PIN, MUX_SEL_FUNCTION1);
    // hal_gpio_mux_func_select(GPIO_SCK_PIN, MUX_SEL_FUNCTION1);
    // hal_gpio_mux_func_select(GPIO_MOSI_PIN, MUX_SEL_FUNCTION1);
    // hal_gpio_mux_func_select(GPIO_MISO_PIN, MUX_SEL_FUNCTION1);

}

void soft_spi_read_reg(SOFT_SPI_HandleTypeDef *sspi, uint8_t addr, uint8_t *buf, uint32_t length)
{
    // GPIO_HIGH(GPIO_CS_PIN);
    // GPIO_LOW(GPIO_SCK_PIN);
    // GPIO_LOW(GPIO_MOSI_PIN);
	// hal_gpio_mux_func_select(GPIO_CS_PIN, MUX_SEL_GPIO_OUTPUT);
    // hal_gpio_mux_func_select(GPIO_SCK_PIN, MUX_SEL_GPIO_OUTPUT);
    // hal_gpio_mux_func_select(GPIO_MOSI_PIN, MUX_SEL_GPIO_OUTPUT);
    // hal_gpio_mux_func_select(GPIO_MISO_PIN, MUX_SEL_GPIO_INPUT);

    // GPIO_LOW(GPIO_CS_PIN);
    //delayus(1);
    soft_spi_cs_active(sspi);

    soft_spi_write_byte(sspi, addr);

    //delayus(1);
    //hal_gpio_mux_func_select(GPIO_MISO_PIN, MUX_SEL_GPIO_INPUT);
    // delayus(1);
#if defined(SOFT_SPI_3WIRE)
    hal_gpio_mux_func_select(sspi->sda, MUX_SEL_GPIO_INPUT);
#endif

    for (uint32_t i = 0; i < length; i++)
    {
       soft_spi_read_byte(sspi, buf++);
    }

#if defined(SOFT_SPI_3WIRE)
    hal_gpio_mux_func_select(sspi->sda, MUX_SEL_GPIO_OUTPUT);
#endif
    soft_spi_cs_idle(sspi);
    //delayus(1);
    // GPIO_HIGH(GPIO_CS_PIN);
    // delayus(1);

	// hal_gpio_mux_func_select(GPIO_CS_PIN, MUX_SEL_FUNCTION1);
    // hal_gpio_mux_func_select(GPIO_SCK_PIN, MUX_SEL_FUNCTION1);
    // hal_gpio_mux_func_select(GPIO_MOSI_PIN, MUX_SEL_FUNCTION1);
    // hal_gpio_mux_func_select(GPIO_MISO_PIN, MUX_SEL_FUNCTION1);

}

// mode 0 spi, readwrite
uint8_t soft_spi_readwrite_byte(SOFT_SPI_HandleTypeDef *sspi, uint8_t tx_data)
{
    uint8_t rx_data = 0x00;

    for (int i = 0; i < 8; ++i)
    {
        soft_spi_clk_idle(sspi);
        if (tx_data & 0x80)
        {
            GPIOx_HIGH(sspi->sda);
        }
        else
        {
            GPIOx_LOW(sspi->sda);
        }

        tx_data <<= 1;
        rx_data <<= 1;

        soft_spi_clk_active(sspi);
        if (GPIO_READ(sspi->miso))
        {
            rx_data |= 0x01;
        }
    }

    soft_spi_clk_idle(sspi);

    return rx_data;
}

