#include "soft_i2c.h"


void soft_i2c_delay(uint32_t delay)
{
    for (uint32_t i = 0; i < delay; i++) {
        __asm__ __volatile__("nop");
    }
}


I2C_StatusTypeDef soft_i2c_init(SOFT_I2C_HandleTypeDef *si2c)
{
    return si2c->iic(SOFT_I2C_GPIO_INIT, NULL);
}

void soft_i2c_stop(SOFT_I2C_HandleTypeDef *si2c)
{
    GPIO_OUT_ENABLE(si2c->sda_pin);
    GPIO_OUT_DISABLE(si2c->scl_pin);
    // delay()
    //? clk stick wait
    GPIO_OUT_DISABLE(si2c->sda_pin);
    //? stop data high steady delay
}

I2C_StatusTypeDef soft_i2c_start(SOFT_I2C_HandleTypeDef *si2c)
{
    GPIO_OUT_DISABLE(si2c->sda_pin);
    GPIO_OUT_DISABLE(si2c->scl_pin);
    //? start clk high steady delay
    //? clk stick wait

    if (GPIO_READ(si2c->sda_pin))
    {
        GPIO_OUT_ENABLE(si2c->sda_pin);
        //? start data low steady delay
        GPIO_OUT_ENABLE(si2c->scl_pin);

        return I2C_OK;
    }
    else
    {
        GPIO_OUT_ENABLE(si2c->scl_pin);
        //? dummy clk low level delay

        for (uint8_t i = 0; i < 9; i++)
        {
            GPIO_OUT_DISABLE(si2c->scl_pin);
            //? dummy clk high level delay
            //? clk stick wait

            GPIO_OUT_ENABLE(si2c->scl_pin);
            //? dummy clk low level delay
        }

        soft_i2c_stop(si2c);

        //! dbg_printf("i2c start failed\n");

        return I2C_ERROR;
    }
}

I2C_StatusTypeDef soft_i2c_write_byte(SOFT_I2C_HandleTypeDef *si2c, uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        GPIO_OUT_ENABLE(si2c->scl_pin);

        if (data & 0x80)
        {
            GPIO_OUT_DISABLE(si2c->sda_pin);
        }
        else
        {
            GPIO_OUT_ENABLE(si2c->sda_pin);
        }
        //? w data steady delay

        data <<= 1;

        GPIO_OUT_DISABLE(si2c->scl_pin);
        //? clk high level delay
        //? clk stick wait

    }

    GPIO_OUT_ENABLE(si2c->scl_pin);
    GPIO_OUT_DISABLE(si2c->sda_pin);
    //? w data steady delay

    GPIO_OUT_DISABLE(si2c->scl_pin);
    //? w clk high steady delay
    //? clk stick wait

    if (GPIO_READ(si2c->sda_pin))
    {
        GPIO_OUT_ENABLE(si2c->scl_pin);
        // w clk low steady delay

        soft_i2c_stop(si2c);

        //! dbg_printf("i2c write byte failed\n");

        return I2C_ERROR;
    }
    else
    {
        GPIO_OUT_ENABLE(si2c->scl_pin);
        // w clk low steady delay

        return I2C_OK;
    }

}

uint8_t soft_i2c_read_byte(SOFT_I2C_HandleTypeDef *si2c, BOOL con)
{
    GPIO_OUT_DISABLE(si2c->sda_pin);
    uint8_t out = 0x00;
    for (uint8_t i = 0, bitmask = 0x80; i < 8; i++, bitmask >>= 1)
    {
        GPIO_OUT_ENABLE(si2c->scl_pin);
        //? r clk low steady delay
        GPIO_OUT_DISABLE(si2c->scl_pin);
        //? r clk high steady delay
        //? clk stick wait

        if (GPIO_READ(si2c->sda_pin))
        {
            out |= bitmask;
        }
    }

    GPIO_OUT_ENABLE(si2c->scl_pin);
    //? r clk low steady delay

    if (con)
    {
        GPIO_OUT_ENABLE(si2c->sda_pin);
    }
    else
    {
        GPIO_OUT_DISABLE(si2c->sda_pin);
    }

    GPIO_OUT_DISABLE(si2c->scl_pin);
    //? r clk high stady delay
    //? clk stick wait

    GPIO_OUT_ENABLE(si2c->scl_pin);

    if (con)
    {
        GPIO_OUT_DISABLE(si2c->sda_pin);
    }

    return out;
}

I2C_StatusTypeDef soft_i2c_write(SOFT_I2C_HandleTypeDef *si2c, uint8_t dev_addr, const uint8_t *data, uint32_t len)
{
    I2C_StatusTypeDef r;
    uint32_t num_error = 0;

    do
    {
        r = soft_i2c_start(si2c);

        if (r == I2C_OK)
        {
            r = soft_i2c_write_byte(si2c, dev_addr);
        }

        if (r != I2C_OK)
        {
            soft_i2c_stop(si2c);
            continue;
        }

        for (uint32_t i = 0; i < len; i++)
        {

            if (r == I2C_OK)
            {
                r = soft_i2c_write_byte(si2c, data[i]);
            }
            else
            {
                break;
            }
        }


        soft_i2c_stop(si2c);

    } while ((r != I2C_OK) && (num_error++ < I2C_Retries));


    if (r != I2C_OK)
    {
        //! dbg_printf("i2c write failed\n");
    }


    return r;
}

I2C_StatusTypeDef soft_i2c_read(SOFT_I2C_HandleTypeDef *si2c, uint8_t dev_addr, uint8_t *data, uint32_t len)
{
    I2C_StatusTypeDef r;
    uint32_t num_error = 0;

    do
    {
        r = soft_i2c_start(si2c);

        if (r == I2C_OK)
        {
            r = soft_i2c_write_byte(si2c, dev_addr | 0x01);
        }

        if (r != I2C_OK)
        {
            soft_i2c_stop(si2c);
            continue;
        }

        for (uint32_t i = 0; i < len - 1; i++)
        {
            data[i] = soft_i2c_read_byte(si2c, TRUE);
        }
        data[len - 1] = soft_i2c_read_byte(si2c, FALSE);

        soft_i2c_stop(si2c);

    } while ((r != I2C_OK) && (num_error++ < I2C_Retries));

    if (r != I2C_OK)
    {
        //! dbg_printf("i2c read failed\n");
    }

    return r;
}

I2C_StatusTypeDef soft_i2c_write_reg(SOFT_I2C_HandleTypeDef *si2c, uint8_t dev_addr, uint8_t reg_addr, const uint8_t *data, uint32_t len)
{
    I2C_StatusTypeDef r;
    uint32_t num_error = 0;
    do
    {
        r = soft_i2c_start(si2c);

        if (r == I2C_OK)
        {
            r = soft_i2c_write_byte(si2c, dev_addr);
        }

        if (r == I2C_OK)
        {
            r = soft_i2c_write_byte(si2c, reg_addr);
            // r = I2C_WRITE_BYTE(reg_addr); // rely on different i2c device, some need to write reg addr 16bit
        }

        if (r != I2C_OK)
        {
            soft_i2c_stop(si2c);
            continue;
        }

        for (uint32_t i = 0; i < len; i++)
        {
            r = soft_i2c_write_byte(si2c, data[i]);
            if (r != I2C_OK)
            {
                break;
            }
        }

        soft_i2c_stop(si2c);
    } while ( (r != I2C_OK) && (num_error++ < I2C_Retries) );

    if (r != I2C_OK)
    {
        //! dbg_printf("i2c write reg failed\n");
    }

    return r;
}

I2C_StatusTypeDef soft_i2c_read_reg(SOFT_I2C_HandleTypeDef *si2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint32_t len) {
    I2C_StatusTypeDef r;
    uint32_t num_error = 0;
    do
    {
        r = soft_i2c_start(si2c);

        if (r == I2C_OK)
        {
            r = soft_i2c_write_byte(si2c, dev_addr);
        }

        if (r == I2C_OK)
        {
            r = soft_i2c_write_byte(si2c, reg_addr);
            // r = I2C_WRITE_BYTE(reg_addr); // rely on different i2c device, some need to write reg addr 16bit
        }

        if (r != I2C_OK)
        {
            soft_i2c_stop(si2c);
            continue;
        }

        r = soft_i2c_start(si2c);

        if (r == I2C_OK)
        {
            r = soft_i2c_write_byte(si2c, dev_addr | 0x01);
        }

        if (r != I2C_OK)
        {
            soft_i2c_stop(si2c);
            continue;
        }

        for (uint32_t i = 0; i < len - 1; i++)
        {
            data[i] = soft_i2c_read_byte(si2c, TRUE);
        }
        data[len - 1] = soft_i2c_read_byte(si2c, FALSE);

        soft_i2c_stop(si2c);
    } while ( (r != I2C_OK) && (num_error++ < I2C_Retries) );

    if (r != I2C_OK)
    {
        //! dbg_printf("i2c read reg failed\n");
    }

    return r;
}

I2C_StatusTypeDef soft_i2c_scan(SOFT_I2C_HandleTypeDef *si2c)
{
    I2C_StatusTypeDef r = I2C_OK;
    U8 dev_id = 0;
    U8 dev_cnt = 0;
    printf ("I2C0 scaning\n\r");
    for(dev_id = 0;dev_id <127;dev_id++){
        r = soft_i2c_start(si2c);
        if (r == I2C_OK)
        {
            r = soft_i2c_write_byte (si2c, dev_id<<1);
        }

        if (r != I2C_OK)
        {
            goto IIC_STOP;
        }
        dev_cnt++;
        printf ("I2C0 device found at address 0x%x\n\r",dev_id<<1);
        IIC_STOP:
        soft_i2c_stop(si2c);
    }
    return r;
}

