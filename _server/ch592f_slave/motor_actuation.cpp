/** Yi Zhang
 *
 */
#pragma once
#include <cmath>
#include <cstdint>

// todo, merge constants into header / config
int32_t CH592F_PWM_BIT_RATE = 8;
int32_t CH592F_PWM_MAX_INT = (1 << CH592F_PWM_BIT_RATE) - 1;
uint32_t FRONT_LEFT_MOTOR_PIN = 27;
uint32_t FRONT_LEFT_MOTOR_DIR_PIN = 25;
uint32_t FRONT_RIGHT_MOTOR_PIN = 28;
uint32_t FRONT_RIGHT_MOTOR_DIR_PIN = 26;
uint32_t REAR_LEFT_MOTOR_PIN = 15;
uint32_t REAR_LEFT_MOTOR_DIR_PIN = 11;
uint32_t REAR_RIGHT_MOTOR_PIN = 14;
uint32_t REAR_RIGHT_MOTOR_DIR_PIN = 10;

typedef enum {
	LOW = false, HIGH = true
} Digital_Logic_t;

typedef enum {
	STOP, STRAIGHT, TURN_LEFT, TURN_RIGHT 
} Drive_Direction;


void digital_write(uint32_t pin_number, Digital_Logic_t logic_level);
void configure_lidar_motor(bool should_lidar_be_on = true, uint32_t lidar_pin_number = 16, uint32_t pwm_frequency = 20000);
// todo, merge constants into header / config

// import these methods:
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
 *	@param pin_number
 *	@param input_value
 */
void analog_write(int pin_number, int input_value)
{
	// steps: GPIOA_ModeCfg/GPIOA_ModeCfg -> PWMX_CLKCfg?PWMX_16bit_CycleCfg? -> PWMX_16bit_ACTOUT
	// https://github.com/openwch/ch592/blob/main/EVT/EXAM/PWMX/src/Main.c#L58C4-L73C1
}

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
 *	@param pin_number - ()
 *	@param is_high - ()
 *	@param gpio_mode - ()
 */
void digital_write(uint32_t pin_number, Digital_Logic_t logic_level)
{
	// steps: 
	// if (logic_level == Digital_Logic_t::HIGH)
	if (logic_level)
	{
		// figure out appropriate GPIO(A/B)_ModeCfg(..., gpio_mode);

	} else {
		// figure out appropriate GPIO(A/B)_ResetBits(...);
	}
}
// import these methods:

int32_t bound_motor_duty(int32_t input_integer, bool &is_reversing)
{
	is_reversing = (input_integer < 0) ? !is_reversing : is_reversing;
	
	input_integer = abs(input_integer);
	input_integer = (input_integer < CH592F_PWM_MAX_INT) ? input_integer : CH592F_PWM_MAX_INT;

	return input_integer;
	// let the compiler optimize a merge between this and the previous line lol
}

/**
 *	@brief	  
 *	
 *	for non-continuous mode, the method that calls this should call it with speed set to 0 by reference,
 *	specifically:
 *	```cpp
 *	while(1) // `loop()`
 *	{
 *		// ... code above
 *	
 *		if (!IS_CONTINUOUS_DRIVE_MODE)
 *		{
 *			speed_pwm_duty = 0;
 *		}
 *	
 *		// ... code below
 *		DelayMs(100);
 *	} // end of while(1)
 *	```
 *	@param drive_direction - (Drive_Direction) 
 *	@param speed_pwm_duty - (int32_t &)
 *	@param is_reversing - (bool)
 */
void drive(Drive_Direction drive_direction, int32_t &speed_pwm_duty, bool is_reversing)
{
	speed_pwm_duty = bound_motor_duty(speed_pwm_duty, is_reversing);

	if (is_reversing)
	{
		// `GPIO_ModeOut_PP_20mA` = `HIGH`
		digital_write(FRONT_LEFT_MOTOR_DIR_PIN, HIGH);
		digital_write(FRONT_RIGHT_MOTOR_DIR_PIN, HIGH);
		digital_write(REAR_LEFT_MOTOR_DIR_PIN, HIGH);
		digital_write(REAR_RIGHT_MOTOR_DIR_PIN, HIGH);
		// calls `GPIO(A/B)_ModeCfg(..., GPIO_ModeOut_PP_20mA)` (HIGH)
		// digital_write(uint32_t pin_number, bool is_logic_high, uint32_t out_mode = GPIO_ModeOut_PP_20mA)
	} else {
		digital_write(FRONT_LEFT_MOTOR_DIR_PIN, LOW);
		digital_write(FRONT_RIGHT_MOTOR_DIR_PIN, LOW);
		digital_write(REAR_LEFT_MOTOR_DIR_PIN, LOW);
		digital_write(REAR_RIGHT_MOTOR_DIR_PIN, LOW);
		// calls `GPIO(A/B)_ResetBits(corr._pin);` appropriately
	}

	if (drive_direction == Drive_Direction::STOP)
	{
		analog_write(FRONT_LEFT_MOTOR_PIN, 0);
		analog_write(FRONT_RIGHT_MOTOR_PIN, 0);
		analog_write(REAR_LEFT_MOTOR_PIN, 0);
		analog_write(REAR_RIGHT_MOTOR_PIN, 0);

	} else {
		switch (drive_direction)
		{
			case Drive_Direction::STRAIGHT:
				analog_write(FRONT_LEFT_MOTOR_PIN, speed_pwm_duty);
				analog_write(FRONT_RIGHT_MOTOR_PIN, speed_pwm_duty);
				analog_write(REAR_LEFT_MOTOR_PIN, speed_pwm_duty);
				analog_write(REAR_RIGHT_MOTOR_PIN, speed_pwm_duty);
				break;
			case Drive_Direction::TURN_LEFT:

				if (is_reversing)
				{
					// reverse right turn to recover the forwards left turn; set the appropriate ones to high
					digital_write(FRONT_LEFT_MOTOR_DIR_PIN, HIGH);
					digital_write(REAR_LEFT_MOTOR_DIR_PIN, HIGH);
				} else {
					// set the appropriate ones to low
					digital_write(FRONT_LEFT_MOTOR_DIR_PIN, LOW);
					digital_write(REAR_LEFT_MOTOR_DIR_PIN, LOW);
				}

				// TODO: this is crabwalk right turn; enable additional turning models later with another helper
				// e.g. if no differential exists, force motors at different speeds to account for turning radius
				analog_write(FRONT_LEFT_MOTOR_PIN, speed_pwm_duty);
				analog_write(FRONT_RIGHT_MOTOR_PIN, speed_pwm_duty);
				analog_write(REAR_LEFT_MOTOR_PIN, speed_pwm_duty);
				analog_write(REAR_RIGHT_MOTOR_PIN, speed_pwm_duty);
				break;
			case Drive_Direction::TURN_RIGHT:

				if (is_reversing)
				{
					// reverse left turn to recover the forwards right turn; set the appropriate ones to high
					digital_write(FRONT_RIGHT_MOTOR_DIR_PIN, HIGH);
					digital_write(REAR_RIGHT_MOTOR_DIR_PIN, HIGH);
				} else {
					// set the appropriate ones to low
					digital_write(FRONT_RIGHT_MOTOR_DIR_PIN, LOW);
					digital_write(REAR_RIGHT_MOTOR_DIR_PIN, LOW);
				}

				// TODO: this is crabwalk right turn; enable additional turning models later with another helper
				// e.g. if no differential exists, force motors at different speeds to account for turning radius
				analog_write(FRONT_LEFT_MOTOR_PIN, speed_pwm_duty);
				analog_write(FRONT_RIGHT_MOTOR_PIN, speed_pwm_duty);
				analog_write(REAR_LEFT_MOTOR_PIN, speed_pwm_duty);
				analog_write(REAR_RIGHT_MOTOR_PIN, speed_pwm_duty);
				break;
			default:
				// stop
				analog_write(FRONT_LEFT_MOTOR_PIN, 0);
				analog_write(FRONT_RIGHT_MOTOR_PIN, 0);
				analog_write(REAR_LEFT_MOTOR_PIN, 0);
				analog_write(REAR_RIGHT_MOTOR_PIN, 0);
				break;
		}
	}
}

/**
 *	@brief either turns on or shuts down lidar motor
 *	
 *	
 *	@param lidar_pin_number - (uint32_t)
 *	@param is_lidar_on - (bool)
 *	@param pwm_frequency - (uint32_t)
 */
void configure_lidar_motor(bool should_lidar_be_on, uint32_t lidar_pin_number, uint32_t pwm_frequency)
{
	if(should_lidar_be_on)
	{
		// set up pwm frequency
		// 
		
	} else {
		analog_write(lidar_pin_number, 0);
	}
}