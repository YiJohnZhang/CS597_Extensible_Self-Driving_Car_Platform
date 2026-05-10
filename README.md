# CS597_Extensible_Self-Driving_Car_Platform
Extensible self-driving car platform for CS437; can be extended for other courses.

## Project Hardware Architecture
![CS597_Extensible_Self-Driving_Car_Platform_architecture](https://github.com/YiJohnZhang/CS597_Extensible_Self-Driving_Car_Platform/blob/main/_assets/CS597_architecture_block_diagram_testing_annot.jpg)

## Project Software Architecture
![CS597_Extensible_Self-Driving_Car_Platform_software_architecture](https://github.com/YiJohnZhang/CS597_Extensible_Self-Driving_Car_Platform/blob/main/_assets/CS597_codebase_architecture.png)

## Project Build
One Line Setup: `python3 -m .vevn; venv .venv/bin/activate; pip install -r requirements.txt`
```sh
python3 -m venv .venv
source .venv/bin/activate
# end of optional step

pip install -r requirements.txt
python3 test_example.py		# to test
python3 your_app_name.py	# to run
```

# Contributors
- Yi Zhang
- Minqiang Liu

# Todo
## Codebase Overhaul
- [ ] Build an Arduino-like interface.
	- Map between Pin outputs to `GPIOA_ModeCfg(pin#, mode) / GPIOB_ModeCfg(pin#, mode)`\
		- e.g. on CH592F `#define`-ing `25 (PA15)` automatically does `GPIOA_ModeCFG(GPIO_Pin_15, ...)`
	- `digital_read()`
	- `digital_write()`
	- `analog_read()`
	- `analog_write()`
	- `pin_mode()`
- [ ] Test Arduino-like interface.
- [ ] See below

## Codebase
- [x] `RingBufferQwewe.cpp` for data storage and on-demand retrieval (in case it is mid-sensing, this assumes the refresh rate is fast enough that it is still accurate)
- [ ] U5 (CH592F) (C)
	- [ ] `main.cpp`:
		- immediately handles any pwm actuation requests
		- no responses to confirm request packet handled
		- [x] travelling motor: runs 8835 in `PH/EN` mode;
			- [x] request types: reconfigure pwm frequency; can start for a finite duration (default 0); can stop
		- [ ] LiDaR motor: default run on startup **LOW PRIORITY** 
			- request types: reconfigure pwm frequency; request to start/stop
		- [ ] servomotor: **LOW PRIORITY**
- [ ] U6 (CH592F) (C)
	- [ ] `main.cpp`:
		- receives instructions (data requests) from rpi by i2c
		- pulls `INTR` pin (`6`) when high when `data_buffer` is ready
		- usually running `ultrasonic.c` unless, a data request is for `bt_gps` data?
	- [x] `ultrasonic.cpp`:
		- pins `25 - 28`
		- occasionally queries external T from `thermometer.cpp`
		- [ ] needs some stale-ness algo (develop a pseudo watchdog timer, e.g. one that increments at the end of a loop and then at a certain tick sample temperature again?)
	- [x] `thermometer.cpp`
	- [ ] `bt_gps.c`: **LOW PRIORITY**
		- [ ] sniffs for bluetooth packets (`bt_sniffer.c`)
		- [ ] filters for relevant bt packets that allow it to calculate location data (`bt_sniffer.c`)
		- [ ] calculates location data (`bt_gps.c`)
		- [ ]
- [ ] RPi (python)
	- [x] `car.py`
		- initializes a self-driving car object for this library
	- [x] `i2c.py`
		- using WCH family (`CH592F`; considering `CH595M` w/ exact same interface but just 16 more pins)
	- [ ] `test_example.py`
		- has individual tests for each of the modules
	- [ ] `camera.py`: **LOW PRIORITY**
	- [ ] `tcp_server.py`: **LOW PRIORITY**
- License Details

## Lab Instructions
- [ ] Lab 01A: assembly + pcb exercise, naive mapping, naive self-driving, 
- [ ] Lab 01B: advanced mapping, object detection w/ camera, self-driving + maze navigation
- [ ] Lab 02: wifi and bt; remote control from front-end server!
