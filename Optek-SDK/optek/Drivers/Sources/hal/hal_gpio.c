#include "regmap.h"

#include "hw_gpio.h"
#include "hal_gpio.h"



/**
 ****************************************************************************************
 *
 * @file app_gpio.c
 * @defgroup GPIO_profile_commons
 * @ingroup GPIO_profile
 * @brief GPIO profile commons implement.
 * @param gpio_index: GPIO_INDEX, from 0 to 31
 * @param mux_sel:
 * MUX_SEL_GPIO_INPUT,
 * MUX_SEL_GPIO_OUTPUT,
 * MUX_SEL_FUNCTION1,
 * MUX_SEL_FUNCTION2,
 * MUX_SEL_FUNCTION3,
 * MUX_SEL_FUNCTION4
 *
 ****************************************************************************************
 */
void hal_gpio_mux_func_select(enumGPIO_INDEX gpio_index, enumGPIOMUX_SEL mux_sel)
{
	if ((gpio_index >= 0) && gpio_index < GPIO_MAX_INDEX)
	{
		//valid GPIO port
		U8 gpio_group = gpio_index>>5;

		switch (gpio_group)
		{
			case GPIO_GROUP0:
				gpio0_MUX_select(1<<gpio_index, mux_sel);
				break;
			default:
				DBG_Assert(FALSE);
				break;
		}
	}
}

void hal_gpio_output_level_set(enumGPIO_INDEX gpio_index, enumGPIO_LEVEL level) __attribute__ ((section (".internal_ram_1_text")));
void hal_gpio_output_level_set(enumGPIO_INDEX gpio_index, enumGPIO_LEVEL level)
{
	if ((gpio_index >= 0) && gpio_index < GPIO_MAX_INDEX)
	{
		U8 gpio_group = gpio_index>>5;

		switch (gpio_group)
		{
			case GPIO_GROUP0:
				gpio0_SetOutput_level(1<<gpio_index, level);
				break;
			default:
				DBG_Assert(FALSE);
				break;
		}
	}
}

/* the function is running at sflash or psram*/
void hal_gpio_output_level_set_for_slow(enumGPIO_INDEX gpio_index, enumGPIO_LEVEL level)
{
	if ((gpio_index >= 0) && gpio_index < GPIO_MAX_INDEX)
	{
		U8 gpio_group = gpio_index>>5;

		switch (gpio_group)
		{
			case GPIO_GROUP0:
				gpio0_SetOutput_level(1<<gpio_index, level);
				break;
			default:
				DBG_Assert(FALSE);
				break;
		}
	}
}

/********gpio_index->GPIO_INDEX   return GPIO_LEVEL (TRUE(HIGH) FALSE(LOW)) ************/
U32 hal_gpio_input_level_read(enumGPIO_INDEX gpio_index)
{
	if ((gpio_index >= 0) && gpio_index < GPIO_MAX_INDEX)
	{
		U8 gpio_group = gpio_index>>5;
		U32 temp;

		switch (gpio_group)
		{
			case GPIO_GROUP0:
				temp = gpio0_input_level_read(1<<gpio_index);
				break;
			default:
				DBG_Assert(FALSE);
				temp = 0;
				break;
		}

		return temp;
	}

	return -1;
}

U32 hal_gpio_group_input_level_read(U8 gpio_group)
{
	if (gpio_group >= 0 && gpio_group < GPIO_GROUP_MAX)
	{
		U32 temp;

		switch (gpio_group)
		{
			case GPIO_GROUP0:
				temp = gpio0_group_input_level_read();
				break;

			#ifdef POWER_KEY_ENABLE
			case GPIO_GROUP_SREG:
				temp = gpio_sreg_group_input_level_read();
				break;
			#endif

			default:
				DBG_Assert(FALSE);
				temp = 0;
				break;
		}

		return temp;
	}
	return -1;
}

/**
 * @brief config how the gpio interrupt work
 *
 * @param gpio_index GPIO_INDEX
 * @param enable
 * @param Trigger_type GPIO_INT_TRIGGER_TYPE
 * @param Trigger_level GPIO_INT_TRIGGER_LEVEL
 */
void hal_gpio_interrupt_set(enumGPIO_INDEX gpio_index, U8 enable, enumGPIO_INT_TRIGGER_TYPE Trigger_type, enumGPIO_INT_TRIGGER_LEVEL Trigger_level)
{
	if ((gpio_index >= 0) && gpio_index < GPIO_MAX_INDEX)
	{
		U8 gpio_group = gpio_index>>5;

		switch (gpio_group)
		{
			case GPIO_GROUP0:
				gpio0_interrupt_set(1<<gpio_index, enable, Trigger_type, Trigger_level);
				break;
			default:
				DBG_Assert(FALSE);
				break;
		}
	}
}

/********gpio_index->GPIO_INDEX return gpio_index int status and clear************/
U32 hal_gpio_interrupt_st_read_use_index(enumGPIO_INDEX gpio_index)
{
	if ((gpio_index >= 0) && gpio_index < GPIO_MAX_INDEX)
	{
		U8 gpio_group = gpio_index>>5;
		U32 temp;

		switch (gpio_group)
		{
			case GPIO_GROUP0:
				temp = gpio0_interrupt_st_mask_read(1<<gpio_index);
				break;
			default:
				temp = 0;
				DBG_Assert(FALSE);
				break;
		}

		return temp;
	}
	return -1;
}

void gpio0_interrupt_set(U32 mask, U8 enable, U8 Trigger_type, U8 Trigger_level)
{
	// TX_INTERRUPT_SAVE_AREA;

	// TX_DISABLE;

	if (enable)
	{
		if (Trigger_type == TRIGGER_LEVEL)
		{
			//trigger by levle
			REG_GPIO0_INTTP0 |= mask;
		}
		else
		{
			//trigger by edge
			REG_GPIO0_INTTP0 &= ~mask;
		}

		if (Trigger_level == GPIO_INT_RISING_EDGE)
		{
			//rising or high
			REG_GPIO0_INTTP1 |= mask;
		}
		else
		{
			//falling or low
			REG_GPIO0_INTTP1 &= ~mask;
		}
		REG_GPIO0_INTEN |= mask;
	}
	else
	{
		REG_GPIO0_INTEN &= ~mask;
	}

	// TX_RESTORE;
}
__inline__ U32 gpio0_interrupt_st_group_read(void)
{
	U32 st;
	// TX_INTERRUPT_SAVE_AREA;

	// TX_DISABLE;

	st = REG_GPIO0_INTST;
	REG_GPIO0_INTST = st;        //clear int

	// TX_RESTORE;

	return st;
}

__inline__ U32 gpio0_interrupt_st_mask_read(U32 mask)
{
	U32 st;
	// TX_INTERRUPT_SAVE_AREA;

	// TX_DISABLE;

	st = (REG_GPIO0_INTST & mask);
	REG_GPIO0_INTST = st;        //clear int
	st = !(!st);

	// TX_RESTORE;

	return st;
}

/********gpio_group->GPIO_GROUP   return gpio_group all int status and clear ************/
U32 hal_gpio_interrupt_st_read_use_group(enumGPIO_GROUP gpio_group)
{
	U32 temp;

	switch (gpio_group)
	{
		case GPIO_GROUP0:
			temp = gpio0_interrupt_st_group_read();
			break;
		default:
			temp = 0;
			DBG_Assert(FALSE);
			break;
	}

	return temp;
}

uint32_t gpio_usage[1] = {0};

uint32_t hal_gpio_get_pin(uint32_t port, uint32_t pin)
{
	if (gpio_usage[port] & (1 << pin))
	{
		// you can't use the same pin at the same time
		DBG_Assert(0);
	}
	return pin;
}

uint32_t hal_gpio_free_pin(uint32_t port, uint32_t pin)
{
	if (gpio_usage[port] & (1 << pin) == 0)
	{
		// no pin assign before, check your code
		DBG_Assert(0);
	}
	gpio_usage[port] &= ~(1 << pin);
	return 0;
}



