'''
Steering models available for this kit.
'''
from enum import Enum

class ManueverType(Enum):
	'''
		straight: don't turn
		turn left: ...
		turn right: ...
		maintain lane: maybe use line sensor OR camera to maintain lane (note camera is the most weather independent)
	'''
	STOP = 0
	STRAIGHT = 1
	TURN_LEFT = 2
	TURN_RIGHT = 3
	MAINTAIN_LANE = 4
		# for self-driving only; use in conjunction with a line detector library
		# probably needs a dedicated thread?
	
	EMERGENCY_BRAKE = 5
		# not implemented, but the idea is to mimic ABS?

MANUEVER_PREFIX_MAP = {
	'STOP': 'S',			# Stop
	'STRAIGHT': 'C',		# Continue straight
	'LEFT': 'L',			# turn Left
	'RIGHT': 'R',			# turn Right
	'MAINTAIN_LANE': 'M',	# TODO:need line sensors or camera viewer to decide either `L`, `R`, or `C`
	'EMERGENCY_BRAKE': 'S',	# TODO:need to mimic anti-locking brake system (ABS), "S" for now, eventually "E"
}