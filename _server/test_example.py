#	test_example.py
#	A script to provide an example of using the library and a
#	centralized test for the sensors connected to the SBC.

import sys
from car import Car

def build_debug_message(message: str, method_name: str = '') -> str:
	test_example_scope = 'test_example.py()::'
	caller = f'{method_name}()::' if len(method_name) else ''

	return f'{test_example_scope}{caller}{message}'

if __name__ == '__main__':
	arg_count = len(sys.argv)
	if (arg_count != 1) or (arg_count != 2):
		raise SyntaxError(f'{build_debug_message('len(sys.argv) expected to be 1/2, not {arg_count}', '__main__')}')
	
	car_object = Car(name = 'Test_Car', _is_debug_mode = True, _is_continuous_driving_mode = False)

	if arg_count == 1:
		# without add. args
		# ping sensors

		# go forward

		# stop

		# go left
		# go back
		# go right
		# stop
		pass
	elif arg_count == 2:
		# ping sensors

		# go forward
		# go left
		# go back
		# go right
		# stop
		pass