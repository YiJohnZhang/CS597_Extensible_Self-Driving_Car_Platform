#ifndef CH592F_PERIPHERAL_H
#define CH592F_PERIPHERAL_H

#pragma once
#include <string>
#include <map>

typedef struct {
	uint32_t pin_definition;
	void* GPIOAB_setup_function;
} Pin_Initialization_Struct;

class CH592F_Peripherial
{
public:
	CH592F_Peripherial(std::string device_name);
	CH592F_Peripherial();
	~CH592F_Peripherial();
	void __repr__();

	std::map<uint8_t, Pin_Initialization_Struct> pin_mapping;
	// struct should map to [uint32_t pin_def, generic setup_fn (GPIOA/B)]


private:


};

#endif