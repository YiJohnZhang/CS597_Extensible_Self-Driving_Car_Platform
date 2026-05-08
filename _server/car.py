
from math import abs, round
from typing import Tuple

from config import GENERAL_SETTINGS, ULTRASONIC_SENSOR_SETTINGS
from i2c_device import CH592F_Device
from actuation_model import ManueverType, MANUEVER_PREFIX_MAP
from ultrasonic_layout_model import SENSOR_CHANNEL_MAP

_IS_DEBUG_MODE = GENERAL_SETTINGS['_IS_DEBUG_MODE']
_IS_REALISTIC_DRIVING = GENERAL_SETTINGS['_IS_REALISTIC_DRIVING']
	# realistic drive is continuous driven motor actuation until an obstacle/opportunity 
	# interrupts the current speed otherwise it it is a choppy "drive, stop, evaluate, repeat" actuation

DELIMIT_CHARACTER = GENERAL_SETTINGS['DELIMIT_CHARACTER']

ARE_MOTORS_REVERSE_MOUNTED = GENERAL_SETTINGS['ARE_MOTORS_REVERSE_MOUNTED']
EXISTING_SENSORS = GENERAL_SETTINGS['SENSOR_LIST']
OBJECT_THRESHOLD = ULTRASONIC_SENSOR_SETTINGS['OBJECT_THRESHOLD']

class Car:
	def __init__(self, name: str, _is_debug_mode: bool = _IS_DEBUG_MODE, 
			  _is_continuous_driving_mode: bool = _IS_REALISTIC_DRIVING):
		
		self.name = name if len(name) > 0 else 'Default Name'
		self._is_debug_mode = _is_debug_mode
		self.i2c_slave_1 = CH592F_Device(address = 0x40, name = 'CH592F_Actuator', _is_debug_mode = self._is_debug_mode)
			#TODO: allow user to edit in config
		self.i2c_slave_2 = CH592F_Device(address = 0x41, name = 'CH592F_Sensor', _is_debug_mode = self._is_debug_mode)
			#TODO: allow user to edit in config

		self._is_realistic_driving_mode = _is_continuous_driving_mode
		self.drive_motor_pwm_bit_resolution = 12
			# TODO:magicnumber, allow user to edit in config

	def __repr__(self) -> None:
		print(f'<Car::{self.name}@{id(self)}>')

	def _bound_drive_speed(self, speed: int = 0) -> int:
		'''
		An efficient bounding function for motor speed.
		`maximum_speed` = (2 ** pwm_bits) - 1

		Args:
			speed (int): the target speed to drive the motor
		
		Returns:
			int: an integer bounded \in [0, maximum_speed]
		'''
		maximum_speed = (1 << self.drive_motor_pwm_bit_resolution) - 1
		speed = abs(int(speed))

		return speed if speed < maximum_speed else maximum_speed

	def _get_ultrasonic_data(self, channel: int) -> float:
		#TODO
		return self.i2c_slave_2.get_ultrasonic_distance_reading()

	def _build_surroundings_map(self):
		'''
		A bit/int map of perceived obstacles surrounding the car.
		'''
		pass
	
	def get_ultrasonic_distance(self, channel_name: str) -> float:
		return self._get_ultrasonic_data(SENSOR_CHANNEL_MAP[channel_name])

	def is_object_within_threshold(self, channel_name: str) -> bool:
		return round(self.get_ultrasonic_distance(channel_name), 0.1) < OBJECT_THRESHOLD

	def lane_change_assist(self) -> Tuple[bool, bool]:
		# TODO: make it so it will detect some array of used sensor values, maybe have the user edit 
		# the config file to list an arr of used sensors 
		return self.is_object_within_threshold('L_CENTER'), self.is_object_within_threshold('R_CENTER')

	def drive(self, speed: int = 0, manuever: ManueverType = ManueverType.STRAIGHT, is_reversed: bool = False) -> None:
		'''
		Order the car to drive. Default stop (speed = 0, )
		'''
		# Input Validation
		is_reversed = is_reversed if (not ARE_MOTORS_REVERSE_MOUNTED) else (not is_reversed)
			# negate if motors are reverse mounted
		is_reversed = is_reversed if (speed >= 0) else (not is_reversed)
			# negate `is_reversed` if `speed` is a negative value
		speed = self._bound_drive_speed(speed)
		manuever_string_prefix = ''

		if speed:
			match manuever:
				case ManueverType.STRAIGHT:
					manuever_string_prefix = MANUEVER_PREFIX_MAP['STRAIGHT']
				case ManueverType.TURN_LEFT:
					manuever_string_prefix = MANUEVER_PREFIX_MAP['LEFT']
					# NOTE for actuation i2c slave mcu: for a servomotor turning one,
						# if no differential exists: this will also send the servomotor turn angle and motor speed adjustment
						# to prevent inner/outer wheel slippage (if no differential exists)
						# if differential exists: just turn the servomotor to some angle and execute STRAIGHT
					# if no servomotor turning
						# if crabwalk/mecanum: calculate the independent wheel speeds to skid/slide appropriately
				case ManueverType.TURN_RIGHT:
					manuever_string_prefix = MANUEVER_PREFIX_MAP['RIGHT']
					# NOTE for actuation i2c slave mcu: for a servomotor turning one,
						# if no differential exists: this will also send the servomotor turn angle and motor speed adjustment
						# to prevent inner/outer wheel slippage (if no differential exists)
						# if differential exists: just turn the servomotor to some angle and execute STRAIGHT
					# if no servomotor turning
						# if crabwalk/mecanum: calculate the independent wheel speeds to skid/slide appropriately
				case ManueverType.EMERGENCY_BRAKE:
					# TODO: need to some how mimic ABS. need skid detection sensors to "release breaks"
					# in more advacned model where there is a motor and brake pad connection
					manuever_string_prefix = MANUEVER_PREFIX_MAP['EBRAKE']
					speed = 0
				# TODO: maintain lane logic
				# case ManueverType.MAINTAIN_LANE:
					# insert ML vision logic to decide between `manuever_prefix_map['LEFT/RIGHT/STRAIGHT']`
				case _:
					raise Exception(f'Car::drive()::invalid manuever type {manuever} (drive({speed}, {manuever}, {is_reversed}))')
		else:
			# speed == 0
			manuever_string_prefix = MANUEVER_PREFIX_MAP['STOP']

		manuever_message = f'{manuever_string_prefix}{DELIMIT_CHARACTER}{speed}{DELIMIT_CHARACTER}{is_reversed}'
			# 1 byte + ceil(pwm_bits >> 3) bytes + 1 bit
		self.i2c_slave_1.acutate_motor(manuever_message)		

class SelfDrivingCar(Car):
	def __init__(self, name: str, camera_name_list: list,
			  _is_debug_mode: bool = _IS_DEBUG_MODE, 
			  _is_continuous_driving_mode: bool = _IS_REALISTIC_DRIVING,
			  ):
		# idea is to allow n cameras (rear camera, forward camera, dash camera, side mirror camera(s))
		super().__init__()
	
	def __repr__(self) -> None:
		print(f'<SelfDrivingCar::{self.name}@{id(self)}>')
	# get_camera_stream(self, channel: int)
	# _build_surroundings(self) (parent override)
		'''
		- using cameras, build the surroundings map
		- map should be byte/smallint/int 2d-arr
		- 0 => empty/it CAN (not MAY) drive on, e.g. road/solid white line when the lane is blocked
		- 1 => estimated to be [0.15 (6"), 1] m off the ground such as a sidewalk's curb, and elevated barrier
			- used to parallel park against
		- 2 => estimated to be 2 m/as tall as the vehicle itself (if said vehicle is not as high as semitruck)
			- something that could potentially cause road rash to the underside OR dent/crush the body
		- 3 => ~ 4 m (e.g. small building or semi trailer / train)
			- something that could potentially dent/crush the body
		- 4 => >= 6 m (e.g. something tall enough that is beyond its horizon, e.g. skyscraper, something >4 meters)
			- something that could potentially dent/crush the body
		'''
	# lane_change_assist() with cameras (parent override)
		'''
		- such a shield for cameras should send (image_stream, are_there_detected_objects)
		- `image_stream`: byte string to reproduce a stream of images (video), potentially for blind side video 
		feed AND/OR to store on the cloud
		- `are_there_detected_objects`: whether or not the AI model thinks there are objects occupying the lane
		'''
	
	# _are_there_obstacles_ahead() with camera (parent override)