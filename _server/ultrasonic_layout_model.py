'''
Ultrasonic Layout model for this kit.
'''

SENSOR_CHANNEL_MAP = {
	'FRONT_L': 0,			# FRONT facing sensor, left
	'FRONT_C': 1,			# FRONT facing sensor, center
	'FRONT_R': 2,			# FRONT facing sensor, right
	'L_FRONT': 10,			# LEFT LCA sensor, front (typically not used for short vehicles, non-semi-trailers)
	'L_CENTER': 11,			# LEFT LCA sensor, center (typically one except for longer vehicles)
	'L_REAR': 12,			# LEFT LCA sensor, rear (typically not used for short vehicles, non-semi-trailers)
	'R_FRONT': 20,			# RIGHT LCA sensor, front (typically not used for short vehicles, non-semi-trailers)
	'R_CENTER': 21,			# RIGHT LCA sensor, center (typically one except for longer vehicles)
	'R_REAR': 22,			# RIGHT LCA sensor, rear (typically not used for short vehicles, non-semi-trailers)
	'REAR_L': 30,			# REAR facing sensor, left
	'REAR_C': 31,			# REAR facing sensor, center
	'REAR_R': 32,			# REAR facing sensor, right
}