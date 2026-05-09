/** Yi Zhang
 *
 */
#pragma once
#include <cmath>
#include <cstdint>

float poll_temperature_handler(uint32_t temperature_analog_pin, bool is_celsius_output = false);

// Temperature Sensor: PA9/AIN13 (PIN7)

/**
 *	@brief generic analog read function
 *	
 *	@param pin_number - target pin
 *	@return an integer
 */
uint32_t analog_read(uint32_t pin_number)
{
	// calibration?: https://github.com/openwch/ch592/blob/main/EVT/EXAM/ADC/src/Main.c#L71C6-L71C46
		// https://github.com/openwch/ch592/blob/main/EVT/EXAM/ADC/src/Main.c#L90
		// GPIOA_MODECfg, sets the pin to read on. AD_EXTCh sampling stuff, ADC_ChannelCfg
		// do a DIY analog_read(pin)
}

/**	
 *	@brief poll the temperature sensor
 *
 *	@param temperature_analog_pin - temperature sensor pin
 *	@param is_celsius_output - output in celsius instead of kelvin?
 *	@return temperature
 */
float poll_temperature_handler(uint32_t temperature_analog_pin, bool is_celsius_output)
{
	float output_temperature = 0;
	int analog_read_mV = analog_read(temperature_analog_pin);

	// LMT87 Exact Formula: https://www.ti.com/lit/ds/symlink/lmt87.pdf (pp10)
	// output_temperature = 30 + (13.582 - sqrt((-13.582)**2 + (4 * 0.00433) * (2230.8 - analog_read_mV))) / (2 * -0.00433)
		// TODO: magic ##!
	
	// LMT87 Cheap Approximation (-10 to 50 C)
	// todo upload into cpp::map<int, int>
	int TEMPERATURE_LOWER_LIMIT = -10;
	int TEMPERATURE_UPPER_LIMIT = 50;
	int mV_neg_10_degree_celsius = 2767;	// Table 3
	int mV_50_degree_celsius = 1958;		// Table 3
	float mV_per_degree_celsius = (mV_50_degree_celsius - mV_neg_10_degree_celsius) / (TEMPERATURE_UPPER_LIMIT - TEMPERATURE_LOWER_LIMIT);
	
	output_temperature = TEMPERATURE_LOWER_LIMIT + ((analog_read_mV - mV_neg_10_degree_celsius) / mV_per_degree_celsius);
	output_temperature = round(output_temperature * 100.0f) / 100.0f;

	if (!is_celsius_output)
	{
		output_temperature += 273.15;
	}

	return output_temperature;
}

void save_temperature_reading(RingBufferQueue &temperature_rbq_obj)
{
	float temperature_reading = poll_temperature_handler(TEMPERATURE_PIN, false);
	temperature_rbq_obj.enqueue(temperature_reading);
}

float return_environment_temperature(RingBufferQueue &temperature_rbq_obj)
{
	return temperature_rbq_obj.peek();
}