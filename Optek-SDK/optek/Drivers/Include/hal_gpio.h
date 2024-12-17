#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "c_def.h"
#include <stdint.h>
#include "hw_gpio.h"
#define GPIO_INT_ENABLE


/*
 ****************************************************************************************
 * CONSTANT DEFINITIONS
 ****************************************************************************************
 */

#define GPIO_INDEX2MASK(X)  (1<<((X)-0))

// #define GPIOx_HIGH(X)       do{if (X >= 0 && X < GPIO_MAX_INDEX) (REG_GPIO0_DOUT |= GPIO_INDEX2MASK(X));}while(0);
// #define GPIOx_LOW(X)        do{if (X >= 0 && X < GPIO_MAX_INDEX) (REG_GPIO0_DOUT &= ~GPIO_INDEX2MASK(X));}while(0);
#define GPIOx_HIGH(X)       do{if (X < GPIO_MAX_INDEX) (REG_GPIO0_DOUT |= GPIO_INDEX2MASK(X));}while(0);
#define GPIOx_LOW(X)        do{if (X < GPIO_MAX_INDEX) (REG_GPIO0_DOUT &= ~GPIO_INDEX2MASK(X));}while(0);


#define UART0_TX_GPIO_INDEX    GPIO0_01_INDEX
#define UART0_RX_GPIO_INDEX    GPIO0_02_INDEX

#define UART1_TX_GPIO_INDEX    GPIO_UNDEFINE_INDEX//GPIO0_10_INDEX
#define UART1_RX_GPIO_INDEX    GPIO_UNDEFINE_INDEX//GPIO0_11_INDEX

#define UART2_TX_GPIO_INDEX    GPIO_UNDEFINE_INDEX//GPIO0_09_INDEX
#define UART2_RX_GPIO_INDEX    GPIO_UNDEFINE_INDEX//GPIO0_12_INDEX

#define DEBEG_GPIO_PIN      GPIO_UNDEFINE_INDEX
#define DEBEG_GPIO_PIN1     GPIO_UNDEFINE_INDEX
#define DEBEG_GPIO_PIN2     GPIO_UNDEFINE_INDEX

typedef struct {
	uint32_t port;
	uint32_t pin;
	uint32_t mode : 8;
	uint32_t pull : 8;
	uint32_t speed : 8;
	uint32_t func : 8;
} GPIO_PinTypeDef;


/*
 ****************************************************************************************
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/******** app gpio functions' declaration ************/
void hal_gpio_mux_func_select(enumGPIO_INDEX gpio_index, enumGPIOMUX_SEL mux_sel);
void hal_gpio_output_level_set(enumGPIO_INDEX gpio_index, enumGPIO_LEVEL level);
U32 hal_gpio_input_level_read(enumGPIO_INDEX gpio_index);
U32 hal_gpio_group_input_level_read(U8 gpio_group);
void hal_gpio_interrupt_set(enumGPIO_INDEX gpio_index,U8 enable,enumGPIO_INT_TRIGGER_TYPE Trigger_type,enumGPIO_INT_TRIGGER_LEVEL Trigger_level);
U32 hal_gpio_interrupt_st_read_use_index(enumGPIO_INDEX gpio_index);
U32 hal_gpio_interrupt_st_read_use_group(enumGPIO_GROUP gpio_group);

/**
 * @brief gpio get pin
 * @details use this function to assign pin, avoid pin access different function at same time
 * @param port 0
 * @param pin pin number
 * @return uint32_t pin
 */
uint32_t hal_gpio_get_pin(uint32_t port, uint32_t pin);
uint32_t hal_gpio_free_pin(uint32_t port, uint32_t pin);

#ifdef __cplusplus
}
#endif

#endif //_HAL_GPIO_H_
