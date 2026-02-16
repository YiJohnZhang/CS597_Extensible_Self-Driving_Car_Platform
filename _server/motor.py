class MotorStub:
	"""A simple motor controller stub that stores left/right speeds.
	Speeds are integer values; this stub only records state for testing.
	"""

	def __init__(self):
		self.left = 0
		self.right = 0

	def set_speed(self, left: int, right: int):
		self.left = int(left)
		self.right = int(right)

	def status(self):
		return {"left": self.left, "right": self.right}


def create_motor():
	return MotorStub()

