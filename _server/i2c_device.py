'''
Python 3 I2C smbus lib Documentation: https://pypi.org/project/smbus3/
I2C smbus Documentation: https://www.kernel.org/doc/html/v4.13/driver-api/i2c.html
'''
from math import floor
from time import sleep
from smbus3 import SMBus

from config import GENERAL_SETTINGS, GENERAL_I2C_DEVICE_SETTINGS, GENERAL_I2C_PWM_DRIVER_SETTINGS, MAIN_I2C_DRIVER_CHANNEL_CONSTANTS

_IS_DEBUG_MODE = GENERAL_SETTINGS['_IS_DEBUG_MODE']
I2C_DRIVER_MAX_BITS = GENERAL_I2C_PWM_DRIVER_SETTINGS['I2C_MAX_BITS']


config_i2c_device_name = GENERAL_I2C_DEVICE_SETTINGS['I2C_DRIVER_DEVICE_NAME']
I2C_DRIVER_DEVICE_NAME = config_i2c_device_name if ((type(config_i2c_device_name) == str) and (len(config_i2c_device_name) > 0)) else 'Generic I2C PWM Driver'
I2C_CHANNEL = GENERAL_I2C_DEVICE_SETTINGS['I2C_BUS_CHANNEL_NUMBER']
DEFAULT_DEVICE_ADDRESS = GENERAL_I2C_DEVICE_SETTINGS['DEFAULT_I2C_DEVICE_ADDRESS']

LSB_MASK = 0XFF
MSB_MASK_BIT_SHIFTS = 8

class I2C_Device:

	def __init__(self, device_address: int = DEFAULT_DEVICE_ADDRESS, 
			  _is_debug_mode: bool = _IS_DEBUG_MODE, device_name: str = None):
		self.smbus_obj = SMBus(I2C_CHANNEL)
		self.device_address = device_address
		self._is_debug_mode = _is_debug_mode
		self.device_name = device_name or I2C_DRIVER_DEVICE_NAME
	
	def __enter__(self):
		return self

	def __exit__(self) -> None:
		self._teardown()

	def __repr__(self) -> str:
		return f'{self.device_name}@{hex(self.device_address)}'

	def _debug(self, method_name: str, message: str, class_name: str = 'I2C_Device') -> None:
		# if self._is_debug_mode:
		# 	print(f'{class_name}::{method_name}::{message}')
		raise NotImplementedError

	def _teardown(self) -> None:
		'''
			Teardown the object
			[Example 1a](https://pypi.org/project/smbus3/)
		'''
		self.smbus_obj.close()

	def _read_from_register(self, register_address: int) -> int:
		'''
			Reads from a given `register_address`.
			[Example 1a](https://pypi.org/project/smbus3/)
		'''
		return self.smbus_obj.read_byte_data(self.device_address, register_address)
		## return self.smbus_obj.read_byte_data(self.device_address, register_address, 0)
	
	def _write_value_to_register(self, register_address: int, data: int) -> None:
		'''
			Writers to a given `register_address`.
			[Example 3](https://pypi.org/project/smbus3/)
		'''		
		self.smbus_obj.write_byte_data(self.device_address, register_address, data)

	def _send_string(self, device_address: int, message: str) -> None:



		if self._is_debug_mode:
			print(f'I2C_Device::_send_string()::sent to {hex(device_address)}: {message}')

	
	def bound_duty_cycle(self, input_duty_cycle: int) -> int:
		output_duty_cycle = input_duty_cycle

		'''
		if self._is_debug_mode:
			if output_duty_cycle < 0:
				raise ValueError(f'I2C_Device::bound_duty_cycle()::{input_duty_cycle} < 0')
		'''
		output_duty_cycle = abs(output_duty_cycle)

		if self._is_debug_mode:
			if output_duty_cycle > I2C_DRIVER_MAX_BITS:
				raise ValueError(f'I2C_Device::bound_duty_cycle()::{input_duty_cycle} > max bits ({I2C_DRIVER_MAX_BITS})')
		output_duty_cycle = output_duty_cycle if output_duty_cycle < I2C_DRIVER_MAX_BITS else I2C_DRIVER_MAX_BITS
		
		return output_duty_cycle
	
	def read(self, register_address: int) -> int:
		'''
		'''
		read_value = self._read_from_register(register_address)
		self._debug('_read_from_register', f'read {read_value} from reg{register_address}@{self.device_address}')

		return read_value
	
	def write(self, register_address: int, data: int) -> None:
		'''
		just a nicer name to call I guess ¯\_(ツ)_/¯
		'''
		self._debug('write', f'writing {data} to reg{register_address}@{self.device_address}')
			# can `import inspect` and do `inspect.currentframe().f_code_co_name`
			# Source: https://stackoverflow.com/a/1140513

		self._write_value_to_register(register_address = register_address, data = data)

	def write_pwm(self, register_address: int, data: int) -> None:
		'''
			Expect subclasses to implement this where applicable.
		'''
		raise NotImplementedError

	def send_string() -> None:
		'''
		
		'''
		raise NotImplementedError

class ATMEGA328P_DEVICE(I2C_Device):
	# TODO: prototype for our first MCU I2C slave, before moving onto less-documented CH592F
	pass

class CH592F_Device(I2C_Device):
	def __init__(self, address: int = 0x40, name: str = None, _is_debug_mode: bool = _IS_DEBUG_MODE,
			  motor_pwm_frequency: int = 50, 
			  servomotor_pwm_frequency: int = 50, 
			  lidar_motor_pwm_frequency: int = 50):
		super().__init__(self, device_address = address or DEFAULT_DEVICE_ADDRESS, 
			  _is_debug_mode = _IS_DEBUG_MODE, device_name = name or 'CH592F_main')
		# ...

		self._motor_pwm_frequency = motor_pwm_frequency
		self._servomotor_pwm_frequency = servomotor_pwm_frequency
		self._lidar_motor_pwm_frequency = lidar_motor_pwm_frequency
		self._init_frequency()
		# note one reason for migration is that PCA9685 has a 
		# global frequency setting, each channel cannot be independently set
	
	def __repr__(self) -> str:
		parent_representation = super().__repr__()
		return f'[CH592F]:{parent_representation}'

	def _debug(self, method_name: str, message: str) -> None:
		print(f'{self.__repr__()}::{method_name}::{message}')

	def _init_frequency(self) -> None:
		pass
	
	def set_performance_mode(self, is_high_performance: bool) -> None:
		'''
		In high performance mode, the I2C slaves constantly poll sensors so that information
		requests can be fulfilled quicker by reading the previous cycle's information stored
		on onboard memory.
		In battery saver mode, a specific sensor is only polled upon request: slower but
		saves power.
		'''
	
	def acutate_motor(self, message: str) -> None:
		'''
		Send an actuation command to the MCU.
		'''
		# self.write_pwm(direction, duty_cycle)
		if self._is_debug_mode:
			decoded_message = ''
				# expected decoded message in the MCU console

			self._debug('acutate_motor', message)
			self._debug('actuate_motor', f'assert(decoded_message == {decoded_message})')
	
	def acutate_servomotor(self, channel: int, pulse: int) -> None:
		'''
		Note: servomotor pulse needs to be at a frequency of 50 Hz
		'''
		pulse = pulse * 4096 // 20000
			# TODO: magic ##: 4096 is resolution for 12-bits
			# TODO: magic ##: Servomotors like 50 Hz; 20k us is time period for that		
		self.write_pwm(channel, pulse)
		if self._is_debug_mode:
			self._debug('acutate_servomotor', '')
	
	def acutate_lidar_motor(self, channel: int, is_lidar_running: bool) -> None:
		'''
		Start or stop lidar motor.
		Note: servomotor pulse needs to be at a frequency of 50 Hz
		'''	
		self.write_pwm(channel, is_lidar_running)
		if self._is_debug_mode:
			self._debug('acutate_lidar_motor', '')

	def get_environment_temperature(self) -> int:
		'''
		
		'''
		if self._is_debug_mode:
			self._debug('get_environment_temperature', '')

	def get_ultrasonic_distance_reading(self, channel: int) -> float:
		'''
		
		'''

		return_distance = 0	#TODO: temp
		if self._is_debug_mode:
			self._debug('get_ultrasonic_distance_reading', '')

	def get_position(self, ):
		'''
		Currently returns Cartesian coordinate triple.
		TODO: later impelement the option to return spherical coordinates.
		'''
		if self._is_debug_mode:
			self._debug('get_position', f'({x_coordinate}, {y_coordinate}, {z_coordinate})')

	def get_ultrasonic_map(self, ):
		'''
		
		'''
		# pass
		return
		if self._is_debug_mode:
			self._debug('get_ultrasonic_map', f'\n{ultrasonic_map}')
		
		return ultrasonic_map

	def get_lidar_map(self, ):
		'''
		
		'''
		# pass
		return
		if self._is_debug_mode:
			self._debug('get_ultrasonic_map', f'\n{lidar_map}')
		
		return lidar_map
