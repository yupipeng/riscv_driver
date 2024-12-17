#include "regmap.h"
#include "hw_gpio.h"

#define DEBUG_PIN 2



void gpio_enable_output(GPIO_Typedef *gpio, U32 mask)
{

}

void gpio_enable_input(GPIO_Typedef *gpio, U32 mask)
{

}

I32 gpio_read(GPIO_Typedef *gpio, U32 mask)
{
	DBG_assert(gpio != NULL);
	return ((((GPIO_Typedef2*)gpio)->DIN) & mask);
}

void gpio_write(GPIO_Typedef *gpio, U32 mask, U32 val)
{
	DBG_assert(gpio != NULL);
	((GPIO_Typedef2*)gpio)->DOUT = (((GPIO_Typedef2*)gpio)->DOUT & ~mask) | (mask & val);
}

void gpio_toggle(GPIO_Typedef *gpio, U32 mask)
{
	DBG_assert(gpio != NULL);
	((GPIO_Typedef2*)gpio)->DOUT ^= mask;
}

void gpio_debug_pin_init(void)
{
	//open gpio02
	REG_GPIO0_DOE |= PIN_TO_MASK(DEBUG_PIN);
}

void gpio_debug_pin_high(void)
{
	// set gpio02 hight
	REG_GPIO0_DOUT |= PIN_TO_MASK(DEBUG_PIN);
}

void gpio_debug_pin_low(void)
{
	//set gpio02 low
	REG_GPIO0_DOUT &= ~PIN_TO_MASK(DEBUG_PIN);
}

void gpio_out_as_32reg_debug_init(void)
{
	//open
	REG_GPIO0_DOE |= PIN_TO_MASK(DEBUG_PIN);;
}

void gpio_out_as_32reg_set(unsigned int val)
{
	REG_GPIO0_DOUT |= ( 1>>val );
}

//simple test,only output test
void hw_gpio_test(void)
{
	int i;


    REG_GPIO0_DOUT = 0;
	REG_GPIO0_DOE = 0xffffffff; //open all gpio
	REG_GPIO0_IE  = 0x00000000;

	for (i=0; i<32; i++)
	{
        REG_MISC_STATUS = i<<16;
		REG_GPIO0_DOUT = (0x1<<i);
		delaynop(1);
	}

	asm ("WFI");
	while(1);

	while(i++){
        REG_GPIO0_DOUT = (0x1<<2);
        delaynop(1000);
        REG_GPIO0_DOUT = 0;
        delaynop(10);
        REG_MISC_STATUS = i<<16;
    }
}

#undef __inline__
#define __inline__

__inline__ void gpio0_MUX_select(U32 mask, U8 select)
{

	switch(select)
	{
		case MUX_SEL_GPIO_INPUT:
			REG_GPIO0_FUNC &= ~mask;
	    	REG_GPIO3_FUNC &= ~mask;
			REG_GPIO4_FUNC &= ~mask;

			REG_GPIO0_DOE &= ~mask;
			REG_GPIO0_IE  |=  mask;
			break;

		case MUX_SEL_GPIO_OUTPUT:
			REG_GPIO0_FUNC &= ~mask;
	    	REG_GPIO3_FUNC &= ~mask;
			REG_GPIO4_FUNC &= ~mask;

		    REG_GPIO0_IE  &= ~mask;
			REG_GPIO0_DOE |=  mask;
			break;

		case MUX_SEL_FUNCTION1:
			REG_GPIO3_FUNC &= ~mask;
			REG_GPIO4_FUNC &= ~mask;
			REG_GPIO0_FUNC |= mask;
			break;

		case MUX_SEL_FUNCTION2:
		case MUX_SEL_FUNCTION3:
			REG_GPIO0_FUNC &= ~mask;
			REG_GPIO4_FUNC &= ~mask;
			REG_GPIO3_FUNC |= mask;
			break;
		case MUX_SEL_FUNCTION4:
			REG_GPIO0_FUNC &= ~mask;
			REG_GPIO3_FUNC &= ~mask;
			REG_GPIO4_FUNC |= mask;
			break;

		default:
			DBG_Assert(FALSE);
			break;
	}

}

__inline__ void gpio0_SetOutput_level(U32 mask,U8 level)
{

	if (level == GPIO_LOW)
	{
		REG_GPIO0_DOUT &= ~mask;
	}
	else
	{
		REG_GPIO0_DOUT |= mask;
	}

}