/** Yi Zhang
 *
 */
#pragma once

/**
 *	@brief
 *
 *	CH592F Process
 *	https://github.com/openwch/ch592/blob/main/EVT/EXAM/PWMX/src/Main.c#L58C4-L73C1
 *	Step 1: 
 *	Step 2: 
 *	Step 3: 
 *		- PWMX_16bit_ACTOUT(PIN_##, , , ) (`ch59x_pwm.c`)
 *	Step 4: 
 *	Step 5: 
 *	
 *	@param target_pin
 *	@param input_value
 */
void analog_write(int target_pin, int input_value)
{
	// steps: GPIOA_ModeCfg/GPIOA_ModeCfg -> PWMX_CLKCfg?PWMX_16bit_CycleCfg? -> PWMX_16bit_ACTOUT
	// https://github.com/openwch/ch592/blob/main/EVT/EXAM/PWMX/src/Main.c#L58C4-L73C1
}