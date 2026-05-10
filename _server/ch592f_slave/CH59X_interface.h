/**	Yi Zhang
 *	CH59X_interface.h
 *	
 *	Cleans up the codebase for CH59x MCUs to improve
 *	legibility and maintainability.
 *	
 *	TODO: `analog_write()`, `analog_read()`, pwm_setup() (helper?), 
 *	TODO: I2C slave mode (implement `app_i2c.h`+`app_i2c.cpp` methods and `Main.c` from )
 *	TODO: to maximize compatiblity, reduce to C:
 *		- <functional>: std::fn => void*;
 *		- <(unordered_)map>: need a hash map substitute (implement hash map or separated array).
 *			- or enum physical_pins 
 *		- <stdexcept>: use void print_error(char* method_name, char* message) helper to print whenever an error is encountered
 *	
 *	Currently includes the map for:
 *	- CH592F (20260510)
 */
#ifndef __CH59X_INTERFACE_H__
#define __CH59X_INTERFACE_H__

#pragma once
#include <map>
#include <functional>
#include <stdexcept>
// #include "CH59x_common.h"
// #include "../StdPeriphDriver/inc/CH59x_common.h"

enum Digital_Logic_t{
	LOW = false,
	HIGH = true
};

struct PinInitializationFunctionStruct {
	// contains information how to setup a pin
	uint32_t internal_pin_number;
	uint32_t internal_pin_base_mask;
	char pin_alphabetical_prefix;
	// std::function<void(uint32_t, GPIOMOdeTypeDef)> configuration_fn;
		// o.w. use void*
};

std::map<uint32_t, PinInitializationFunctionStruct> CH592F_PIN_MAP = {
	{1,		{11,	GPIO_Pin_11,	'A'} },
	{2,		{10,	GPIO_Pin_10,	'A'} },
	{6,		{8,		GPIO_Pin_8,		'A'} },
	{7,		{9,		GPIO_Pin_9,		'A'} },
	{8,		{15,	GPIO_Pin_15,	'B'} },
	{9,		{14,	GPIO_Pin_14,	'B'} },
	{10,	{13,	GPIO_Pin_13,	'B'} },
	{11,	{12,	GPIO_Pin_12,	'B'} },
	{12,	{11,	GPIO_Pin_11,	'B'} },
	{13,	{10,	GPIO_Pin_10,	'B'} },
	{14,	{7,		GPIO_Pin_7,		'B'} },
	{15,	{4,		GPIO_Pin_4,		'B'} },
	{16,	{23,	GPIO_Pin_23,	'B'} },
	{17,	{22,	GPIO_Pin_22,	'B'} },
	{23,	{4,		GPIO_Pin_4,		'A'} },
	{24,	{5,		GPIO_Pin_5,		'A'} },
	{25,	{15,	GPIO_Pin_15,	'A'} },
	{26,	{14,	GPIO_Pin_14,	'A'} },
	{27,	{13,	GPIO_Pin_13,	'A'} },
	{28,	{12,	GPIO_Pin_12,	'A'} }
};

void return_pin_representation_parameters (uint32_t physical_pin_number, uint32_t &gpio_pin_mask, char &pin_alphabetical_prefix);
void pwm_pin_mode(uint32_t physical_pin_number, GPIOModeTypeDef gpio_mode = GPIOModeTypeDef::GPIO_ModeIN_Floating, uint32_t frequency);
void pin_mode(uint32_t physical_pin_number, GPIOModeTypeDef gpio_mode = GPIOModeTypeDef::GPIO_ModeIN_Floating);
void digital_write(uint32_t physical_pin_number, Digital_Logic_t logic_level);
void analog_write(uint32_t physical_pin_number, uint32_t input_value);
uint32_t digital_read(uint32_t physical_pin_number);
uint32_t analog_read(uint32_t physical_pin_number, uint32_t sample_count = 16);


/**
 *	@brief return internal pin info for a given physical pin number
 *	
 *	@throw `runtime_error()` if physical pin DNE for selected MCU
 *	@param physical_pin_number the physical pin number as indicated on the IC package
 *	@param gpio_pin_mask the internal pin's corresponding numerical mask to the physical pin (pass by reference - output)
 *	@param pin_alphabetical_prefix the internal pin's corresponding alphabetical mask to the physical pin (pass by reference - output)
 */
void return_pin_representation_parameters (uint32_t physical_pin_number, uint32_t &gpio_pin_mask, char &pin_alphabetical_prefix)
{
	if (CH592F_PIN_MAP.find(physical_pin_number) != CH592F_PIN_MAP.end())
	{
		PinInitializationFunctionStruct initialization_information = CH592F_PIN_MAP[physical_pin_number];
		gpio_pin_mask = initialization_information.internal_pin_base_mask;
		pin_alphabetical_prefix = initialization_information.pin_alphabetical_prefix;
	} else {
		throw std::runtime_error("return_pin_representation_parameters(): cannot find physical pin `" + physical_pin_number + std::string("`."));
	}
}

/**
 *	@brief setup the corresponding pin for i/o
 *	
 *	@details CH592X Process
 *	Step 1:	Figure out mapping to `internal_pin_number` and `alphabetical_mask`.
 *	Step 2:	Use appropriate `GPIO(X)_ModeCfg(internal_pin_number, gpio_mode)` (`CH59x_gpio.c`)
 *	@param physical_pin_number the physical pin number as indicated on the IC package
 *	@param gpio_mode the following enum: `GPIO_ModeIN_Floating` (floating), `GPIO_ModeIN_PU` (pull-up), `GPIO_ModeIN_PD` (pull-down), 
 *	`GPIO_ModeOut_PP_5mA` (push-pull out, 5 mA), `GPIO_ModeOut_PP_20mA` (push-pull out, 20 mA)
 */
void pin_mode(uint32_t physical_pin_number, GPIOModeTypeDef gpio_mode)
{
	uint32_t gpio_pin_mask;
	char pin_alphabetical_prefix = 'Z';
	return_pin_representation_parameters(physical_pin_number, gpio_pin_mask, pin_alphabetical_prefix);
		// make this return a `char` of 0/1 to allow it going into a nested if-block? idk

	if (pin_alphabetical_prefix == 'A')
	{
		GPIOA_ModeCfg(gpio_pin_mask, gpio_mode);
	} else if (pin_alphabetical_prefix == 'B') {
		GPIOB_ModeCfg(gpio_pin_mask, gpio_mode);
	}
}

/**
 *	@brief
 *
 *	@details CH592F Process
 *	Step 1: 
 *	Step 2: 
 *	Step 3: 
 *		- PWMX_16bit_ACTOUT(PIN_##, , , ) (`ch59x_pwm.c`)
 *	Step 4: 
 *	Step 5: 
 *	Source: https://github.com/openwch/ch592/blob/main/EVT/EXAM/PWMX/src/Main.c#L58C4-L73C1
 *	
 *	@param physical_pin_number the physical pin number as indicated on the IC package
 *	@param input_value
 */
void analog_write(uint32_t physical_pin_number, uint32_t input_value)
{
	// steps: GPIOA_ModeCfg/GPIOA_ModeCfg -> PWMX_CLKCfg?PWMX_16bit_CycleCfg? -> PWMX_16bit_ACTOUT
	// https://github.com/openwch/ch592/blob/main/EVT/EXAM/PWMX/src/Main.c#L58C4-L73C1
}

/**
 *	@brief set digital pin logic high/low
 *	
 *	@details CH592X Process
 *	Step 1:		Figure out mapping to `internal_pin_number` and `alphabetical_mask`.
 *	Step 2A:	If `HIGH` use the appropriate `GPIO(X)_SetBits(internal_pin_number)` macro
 *	Step 2B:	Else, `LOW`, use the appropriate `GPIO(X)_ResetBits(internal_pin_number)` macro
 *	Source: https://github.com/openwch/ch592/blob/a46e0086f1ffb5e5502703970bff94888e67f4cb/EVT/EXAM/BLE/IoCHub_NET/APP/peripheral.c#L872-L873
 *	@param physical_pin_number the physical pin number as indicated on the IC package
 *	@param logic_level `HIGH` (`1`/`true`) or `LOW` (`0`/`false`)
 */
void digital_write(uint32_t physical_pin_number, Digital_Logic_t logic_level)
{
	// static_assert((gpio_mode == GPIOModeTypeDef::GPIO_ModeIN_Floating) || (gpio_mode == GPIOModeTypeDef::GPIO_ModeIN_PU) || (gpio_mode == GPIOModeTypeDef::GPIO_ModeIN_PD), "::digital_write(): expect `gpio_mode` to be `GPIO_ModeIN_Floating` or `GPIO_ModeIN_PU` or `GPIO_ModeIN_PD`.");
	uint32_t gpio_pin_mask;
	char pin_alphabetical_prefix = 'Z';
	return_pin_representation_parameters(physical_pin_number, gpio_pin_mask, pin_alphabetical_prefix);
	// std::function<void(uint32_t)> set_bit_macro;

	// if (logic_level == Digital_Logic_t::HIGH)
	if (logic_level)
	{
		switch (pin_alphabetical_prefix)
		{
			case 'A':
				GPIOA_SetBits(gpio_pin_mask);
				break;
			case 'B':
				GPIOB_SetBits(gpio_pin_mask);
				break;
			default:
				// maybe an error or something later
				break;
		}
	} else {
		switch (pin_alphabetical_prefix)
		{
			case 'A':
				GPIOA_ResetBits(gpio_pin_mask);
				break;
			case 'B':
				GPIOB_ResetBits(gpio_pin_mask);
				break;
			default:
				break;
		}
	}
}

/**
 *	@brief read the state of a digital pin
 *	
 *	@details CH592X Process
 *	Step 1:	Figure out mapping to `internal_pin_number` and `alphabetical_mask`.
 *	Step 2:	Read `1`/`0` from the appropriate pin
 *	Source: `CH59X` Standard Peripheral Driver library documentation (`CH59x_gpio.h`:L132-L161).
 *	NOTE: ReadPort() returns 32-bits for GPIO(A/B): A: lower 16-bits, B: lower 24-bits\n
 *	USES: `GPIO(A/B)_ReadPortPin()`
 *	@param physical_pin_number the physical pin number as indicated on the IC package
 *	@return state of the pin (1 = HIGH/ 0 = LOW)
 */
uint32_t digital_read(uint32_t physical_pin_number)
{
	uint32_t return_value = 0;
	uint32_t gpio_pin_mask;
	char pin_alphabetical_prefix = 'Z';
	return_pin_representation_parameters(physical_pin_number, gpio_pin_mask, pin_alphabetical_prefix);
		// make this return a `char` of 0/1 to allow it going into a nested if-block? idk

	if (pin_alphabetical_prefix == 'A')
	{
		return_value = GPIOA_ReadPortPin(gpio_pin_mask);
	} else if (pin_alphabetical_prefix == 'B') {
		return_value = GPIOB_ReadPortPin(gpio_pin_mask);
	}

	return return_value;
}

/**
 *	@brief
 *	
 *	Implicitly sets the pin to `GPIOModeTypeDef::GPIO_ModeIN_Floating` mode.
 *
 *	CH592X Process\n
 *	Step 1:
 *	Step 2:
 *	Step 3:
 *	Step 4:
 *	- Source: https://github.com/openwch/ch592/blob/a46e0086f1ffb5e5502703970bff94888e67f4cb/EVT/EXAM/ADC/src/Main.c
 *	@param physical_pin_number the physical pin number as indicated on the IC package
 *	@return 
 */
uint32_t analog_read(uint32_t physical_pin_number, uint32_t sample_count)
{
	// calibration?: https://github.com/openwch/ch592/blob/main/EVT/EXAM/ADC/src/Main.c#L71C6-L71C46
		// https://github.com/openwch/ch592/blob/main/EVT/EXAM/ADC/src/Main.c#L90
		// GPIOA_MODECfg, sets the pin to read on. AD_EXTCh sampling stuff, ADC_ChannelCfg
		// do a DIY analog_read(pin)
}

#endif	// __CH59X_INTERFACE_H__