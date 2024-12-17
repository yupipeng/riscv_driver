#ifndef _HW_GPIO_H
#define _HW_GPIO_H

#include <stdint.h>
#include "c_def.h"
#include "regmap.h"

typedef struct {
	GPIO_PINs		DIN;    // io input
	GPIO_PINs		DOUT;   // io output
	GPIO_PINs		DOE;    // io opendrain?
	GPIO_PINs		INTEN;  // io interrupt enable
	GPIO_PINs		INTST;  // io interrupt status
	GPIO_PINs		INTTP0; //
	GPIO_PINs		INTTP1; //
	GPIO_PINs		FUNC;   //
} GPIO_Typedef;

typedef struct {
	U32		DIN;    // io input
	U32		DOUT;   // io output
	U32		DOE;    // io opendrain?
	U32		INTEN;  // io interrupt enable
	U32		INTST;  // io interrupt status
	U32		INTTP0; //
	U32		INTTP1; //
	U32		FUNC;   //
} GPIO_Typedef2;
/*
 ****************************************************************************************
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
typedef enum {
	GPIO_GROUP0,
	GPIO_GROUP_SREG,
	GPIO_GROUP_MAX,
} enumGPIO_GROUP;

typedef enum {
	MUX_SEL_GPIO_INPUT,
	MUX_SEL_GPIO_OUTPUT,
	MUX_SEL_FUNCTION1,
	MUX_SEL_FUNCTION2,
	MUX_SEL_FUNCTION3,
	MUX_SEL_FUNCTION4,
} enumGPIOMUX_SEL;

// typedef enum {
// 	GPIO_INPUT,
// 	GPIO_OUTPUT,
// 	GPIO_FUNCTION1,
// 	GPIO_FUNCTION2,
// 	GPIO_FUNCTION3,
// 	GPIO_FUNCTION4,
// } GPIO_PinFuncTypeDef;


typedef enum {
	GPIO_LOW,
	GPIO_HIGH,
} enumGPIO_LEVEL;


typedef enum {
	TRIGGER_EDGE,
	TRIGGER_LEVEL,
} enumGPIO_INT_TRIGGER_TYPE;

typedef enum {
	GPIO_INT_LOW_LEVEL,
	GPIO_INT_HIGH_LEVEL,
	GPIO_INT_FALLING_EDGE,
	GPIO_INT_RISING_EDGE,
} enumGPIO_INT_TRIGGER_LEVEL;

typedef enum
{
	GPIO_UNDEFINE_INDEX = -1,
	GPIO0_00_INDEX = 0,
	GPIO0_01_INDEX,
	GPIO0_02_INDEX,
	GPIO0_03_INDEX,
	GPIO0_04_INDEX,
	GPIO0_05_INDEX,
	GPIO0_06_INDEX,
	GPIO0_07_INDEX,
	GPIO0_08_INDEX,
	GPIO0_09_INDEX,
	GPIO0_10_INDEX,
	GPIO0_11_INDEX,
	GPIO0_12_INDEX,
	GPIO0_13_INDEX,
	GPIO0_14_INDEX,
	GPIO0_15_INDEX,
	GPIO0_16_INDEX,
	GPIO0_17_INDEX,
	GPIO0_18_INDEX,
	GPIO0_19_INDEX,
	GPIO0_20_INDEX,
	GPIO0_21_INDEX,
	GPIO0_22_INDEX,
	GPIO0_23_INDEX,
	GPIO0_24_INDEX,
	GPIO0_25_INDEX,
	GPIO0_26_INDEX,
	GPIO0_27_INDEX,
	GPIO0_28_INDEX,
	GPIO0_29_INDEX,
	GPIO0_30_INDEX,
	GPIO0_31_INDEX,

	GPIO_MAX_INDEX
} enumGPIO_INDEX;


typedef enum
{
	SREG_UNDEFINE_INDEX = -1,
	SREG_PWR_INDEX = 0,

	SREG_MAX_INDEX
} enumSREG_INDEX;



#define REG_MASK(bitn) (((uint32_t)1) << ((bitn)))
#define PIN_TO_MASK(PIN) REG_MASK(PIN)

#define GPIO_SET(bitn) (REG_GPIO0_DOUT |= PIN_TO_MASK(bitn))
#define GPIO_CLR(bitn) (REG_GPIO0_DOUT &= ~PIN_TO_MASK(bitn))
#define GPIO_READ(bitn) (REG_GPIO0_DIN & PIN_TO_MASK(bitn))
#define GPIO_OUT_ENABLE(bitn) (REG_GPIO0_DOE |= PIN_TO_MASK(bitn))
#define GPIO_OUT_DISABLE(bitn) (REG_GPIO0_DOE &= ~PIN_TO_MASK(bitn))
#define PIN_INT_CLEAR(PIN) (REG_GPIO0_INTST |= PIN_TO_MASK(PIN))
#define PIN_INT_ENABLE(PIN) (REG_GPIO0_INTEN |= PIN_TO_MASK(PIN))
#define PIN_INT_DISABLE(PIN) (REG_GPIO0_INTEN &= ~PIN_TO_MASK(PIN))



void gpio_debug_pin_init(void);
void gpio_debug_pin_high(void);
void gpio_debug_pin_low(void);

void gpio_out_as_32reg_debug_init(void);
void gpio_out_as_32reg_set(unsigned int val);

void hw_gpio_test(void);

// -LPAdd
//#define ENABLE_SIMPLE_GPIO_CONTROL

#undef __inline__
#define __inline__


/*
 ****************************************************************************************
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
__inline__ void gpio0_SetOutput_level(U32 mask, U8 level);
__inline__ void gpio1_SetOutput_level(U32 mask, U8 level);

__inline__ U32 gpio0_input_level_read(U32 mask);
__inline__ U32 gpio1_input_level_read(U32 mask);
__inline__ U32 gpio0_group_input_level_read(void);

void gpio0_interrupt_set(U32 mask,U8 enable, U8 Trigger_type, U8 Trigger_level);
void gpio1_interrupt_set(U32 mask,U8 enable, U8 Trigger_type, U8 Trigger_level);

__inline__ U32 gpio0_interrupt_st_mask_read(U32 mask);
__inline__ U32 gpio1_interrupt_st_mask_read(U32 mask);

__inline__ U32 gpio0_interrupt_st_group_read(void);
__inline__ U32 gpio1_interrupt_st_group_read(void);

__inline__ void gpio0_MUX_select(U32 mask, U8 select);
__inline__ void gpio1_MUX_select(U32 mask, U8 select);
//Lp

#endif //_HW_GPIO_H

