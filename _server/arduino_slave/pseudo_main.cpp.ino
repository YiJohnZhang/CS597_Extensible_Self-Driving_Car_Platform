/**	Yi Zhang
 *	main.cpp.ino
 *	An arduino pseudo-script. To port to CH592F. Note arduino cpp does NOT have `string`; too lazy to do `char[]`.
 *	
 *	# COMMAND DESIGN
 *	Command Design 4BY_op|4BY_target|7BY_OP
 *	e.g. `READ|U001|
 * 
 *	## 4 BYTE_op
 *	### Data Transfer
 *	_SET (configure a setting)
 *	READ (read from buffer)
 *	POST (post some data?)
 * 
 *	### Actuation
 *	STOP (emergency halt)
 *	_RUN (run some motion)
 * 
 *	4 BYTE_target
 *	### Sensors
 *	U### (Ultrasonic Sensor ###, up to 999)
 *	T### (Temperature Sensor ###)
 *	Technically 95*999 sensors (95/128 ASCII characters are printable), 95^4 = 81450625 if you are liberal with naming
 *	94^4 = 78074896 if excluding any with `|` depending on splitting algo (either read next 4 bytes or split on `|` if variable)
 * 
 *	### Acutators
 *	SE## (servomotor)
 *	_DRV (driving motor)
 * 
 *	## 7 BYTE_op Examples
 *	_RUN|_DRV|A###### (A => (L)eft, (R)ight, (S)top; ### => speed from 000 - 100% duty cycle)
 * 
 */

#include <Wire.h>
I2C_ACTUATOR_SLAVE_ADDRESS = 0x51;
I2C_SENSOR_SLAVE_ADDRESS = 0x52;

I2C_SLAVE_ADDRESS = I2C_ACTUATOR_SLAVE_ADDRESS;
// I2C_SLAVE_ADDRESS = I2C_SENSOR_SLAVE_ADDRESS;

string receive_data;
string transmit_data = 0;
string command_string = '';
volatile bool is_new_command_received = false;
volatile bool is_drive_mode_intermittent = true;

void I2C_slave_receive_command_handler(int length)
{
	command_string = '';
	while(Wire.available())
	{
		char c = Wire.read();
		command_string += c;
	}
	
	command_string += '\0';
	is_new_command_received = true;
}

void I2C_slave_transmit_data_handler()
{
	Wire.write(transmit_data);
}

int poll_temperature_handler()
{
	// read analog pin
	// store into temperature buffer
}

int return_temperature_from_buffer()
{
	// fetch from appropriate buffer
}

int calculate_ultrasonic_distance(int trigger_pin, int echo_pin)
{
	// send a ping, await return ping; calculate time
	// use `return_temperature_from_buffer()` to calculate speed of sound
	// store into appropriate ultrasonic distance ch buffer
}

int poll_ultrasonic_distance_handler(int sensor_channel)
{
	// use `calculate_ultrasonic_distance()`
	// store into appropriate ultrasonic distance ch buffer
}

int return_ultrasonic_distance_from_buffer(int sensor_channel)
{
	// fetch from appropriate buffer
}

void motor_actuation(int front_left, int front_right, int rear_left, int rear_right)
{
	// do pwm stuff
}

void servomotor_actuation()
{

}

void setup() {
	Wire.begin(I2C_SLAVE_ADDRESS);
	Wire.onReceive(I2C_slave_receive_command_handler);
	Wire.onRequest(I2C_slave_transmit_data_handler);
	Serial.begin(9600);	// for quicker testing by console
}

void loop() {
	// put your main code here, to run repeatedly:

	temperature_data = poll_temperature_handler();
	// go through all ultrasonic sensor channels
	ultrasonic_distance = poll_ultrasonic_distance_handler();

	if (is_new_command_received)
	{
		// note `.split()` DNE in arduino cpp
		// Serial:
		/**	char operation_code[5];
		 *	char target_code[5];
		 *	char operation_instruction[8];
		 *	sscanf(command, "%[^|]|%[^|]|%s", operation_code, target_code, operation_instruction);
		 */
		command_string_split = command_string.split('|');
		operation_code = command_string_split[0];
		target_code = command_string_split[1];
		operation_instruction = command_string_split[2];

		if (I2C_SLAVE_ADDRESS == I2C_ACTUATOR_SLAVE_ADDRESS)
		{
			switch(operation_code)
			{
				case '_RUN':
					// actuate motor
					if (is_drive_mode_intermittent)
					{
						delay(1000);
						motor_actuation(0, 0, 0, 0);
					}
					break;
				case 'STOP':
					// halt all motors.
					break;
				default:
					Serial.println('loop()::error: \"' + command_string + '\" is not recognized');
					break;
			}

		} else if (I2C_SLAVE_ADDRESS == I2C_SENSOR_SLAVE_ADDRESS)
		{
			// Serial.println('loop()::error: \"' + command_string + '\" is not recognized');
		} else
		{
			Serial.println('loop()::device: \"' + I2C_SLAVE_ADDRESS + '\" is not recognized');
		}

		Serial.println('rx:' + command_string);
		is_new_command_received = false;
	}

	delay(50);
}