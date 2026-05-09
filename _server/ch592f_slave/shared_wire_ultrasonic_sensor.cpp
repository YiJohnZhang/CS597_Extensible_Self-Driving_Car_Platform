// design decision to resolve: either all sensor objects are instances of an inherited RBQ; OR they are obj instances containing an RBQ to store values.
#include <cstdint>
#include <cmath>
#include <map>

typedef struct Ultrasonic_Sensor_GPIO_Pin {
	uint32_t trigger_pin;
	uint32_t echo_pin;
};

typedef enum {
	LOW = false, HIGH = true
} Digital_Logic_t;

std::map<uint32_t, Ultrasonic_Sensor_GPIO_Pin> CHANNEL_TO_PIN_MAPPING;

void pin_mode(uint32_t pin_number, GPIOModeTypeDef gpio_mode = GPIOModeTypeDef::GPIO_ModeIN_Floating);
void analog_write(uint32_t pin_number, int input_value);
void digital_write(uint32_t pin_number, Digital_Logic_t logic_level);
uint32_t digital_read(uint32_t pin_number);
uint32_t analog_read(uint32_t pin_number);

void pin_mode(uint32_t pin_number, GPIOModeTypeDef gpio_mode)
{
	// static_assert((gpio_mode == GPIOModeTypeDef::GPIO_ModeIN_Floating) || (gpio_mode == GPIOModeTypeDef::GPIO_ModeIN_PU) || (gpio_mode == GPIOModeTypeDef::GPIO_ModeIN_PD), "::digital_write(): expect `gpio_mode` to be `GPIO_ModeIN_Floating` or `GPIO_ModeIN_PU` or `GPIO_ModeIN_PD`.");
	// do stuff to setup pin_mode for gpio.
	
}

uint32_t digital_read(uint32_t pin_number)
{

	// read pin logic, return

}

/** make these obj attr*/
uint32_t MAX_SENSOR_DISTANCE = 15;			// 15 [cm]
uint32_t TRIGGER_PULSE_TIME_LENGTH = 0.001;	// 1 [us]
/** make these obj attr*/

/**
 *	@brief [m/s]
 *	
 *	@param
 *	@return
 */
float get_speed_of_sound(TemperatureSensor &temperature_sensor_obj)
{
	float temperature = 0;
		// call `return_environment_temperature() (K)` or just pull it directly from the rbq if the reading isn't stale
	float speed_of_sound = 20.05 * sqrt(temperature);
	// Equation Source: https://www.engineeringtoolbox.com/air-speed-sound-d_603.html
	return round(speed_of_sound * 100.0f) / 100.0f;
}

/**
 *	@brief
 *	
 *	@param
 *	@return
 */
float poll_ultrasonic_distance_handler(int trigger_pin, int echo_pin)
{
	float ultrasonic_distance = 0;
	uint32_t pulse_start = 0;
	uint32_t pulse_end = 0;
	float speed_of_sound_cm_p_s = get_speed_of_sound() * 100;
	int timeout_time_length = (2 * MAX_SENSOR_DISTANCE) / (int)(ceil(speed_of_sound_cm_p_s));
		// MAX_SENSOR_DISTANCE should be an object attr; 2 is a magic number

	// use `TMR0/1/2/3_TimerInit` from `CH59x_timer` to timeout reading (26-bit)
	digital_write(trigger_pin, HIGH);
	DelayMs(TRIGGER_PULSE_TIME_LENGTH);
	digital_write(trigger_pin, LOW);

	if (trigger_pin == echo_pin) {
		pin_mode(trigger_pin, GPIOModeTypeDef::GPIO_ModeIN_PD);
	}

	// start_timer at last `0` reading
	while(digital_read(echo_pin) == 0)
	{
		// pulse_start
	}

	// end_timer at last `1` reading
	while(digital_read(echo_pin) == 1)
	{
		// pulse_end
	}

	uint32_t pulse_duration = pulse_end - pulse_start;
	ultrasonic_distance = speed_of_sound_cm_p_s * pulse_duration / 2;

	pin_mode(trigger_pin, GPIOModeTypeDef::GPIO_ModeOut_PP_5mA);
		// substitute with `_setup_GPIO()` inst. method

	// save to RBQ
	return round(ultrasonic_distance * 10.0f) / 10.0f;
}

/**
 *	@brief
 *	
 *	@param
 *	@return
 */
void save_ultrasonic_distance_reading(uint32_t channel, RingBufferQueue &ultrasonic_rbq_obj)
{
	// uint32_t should map to a `ultrasonic_rbq_obj`?
	// uint32_t should map to typedef struct {trigger_pin, echo_pin} Ultrasonic_Sensor_GPIO_Pin;

	Ultrasonic_Sensor_GPIO_Pin mapped_channel_pin_struct = CHANNEL_TO_PIN_MAPPING[channel];
	uint32_t trigger_pin = mapped_channel_pin_struct.trigger_pin;
	uint32_t echo_pin = mapped_channel_pin_struct.echo_pin;

	float ultrasonic_distance = poll_ultrasonic_distance_handler(trigger_pin, echo_pin);
	ultrasonic_rbq_obj.enqueue(ultrasonic_distance);
	// save to rbq
}


/**
 *	@brief
 *	
 *	@param
 *	@return
 */
float return_ultrasonic_distance(int channel)
{

}