import random


class UltrasonicStub:
	"""Stub for ultrasonic distance sensor. Returns a simulated distance
	in centimeters.
	"""

	def distance_cm(self) -> float:
		# realistic HC-SR04-like distances: 2 cm to 400 cm
		return round(random.uniform(2.0, 400.0), 2)


def create_ultrasonic():
	return UltrasonicStub()

