/** Yi Zhang
 *
 */
#pragma once
#include <cmath>

// Temperature Sensor: PA9/AIN13 (PIN7)

/**
 *	@brief generic analog read function
 *	
 *	@param analog_pin - target pin
 *	@return an integer
 */
int analog_read(int analog_pin)
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
 *	@param is_kelvin_output - output in kelvin instead of celsius?
 *	@return temperature
 */
float poll_temperature_handler(int temperature_analog_pin, char is_kelvin_output)
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
	float mV_per_degree_celsius = (mV_50_degree_celsius - mV_neg_10_degree_celsius) / (TEMPERATURE_UPPER_LIMIT50 - TEMPERATURE_LOWER_LIMIT);
	output_temperature = TEMPERATURE_LOWER_LIMIT + ((analog_read_mV - mV_neg_10_degree_celsius) / mV_per_degree_celsius);

	if (is_kelvin_output)
	{
		output_temperature += 273.15;
	}

	return output_temperature;
}