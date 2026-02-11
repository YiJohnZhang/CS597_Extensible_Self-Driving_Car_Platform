# CS597_Extensible_Self-Driving_Car_Platform
Extensible self-driving car platform for CS437; can be repurposed for other course offerings.

## Project Architecture Architectural
![CS597_Extensible_Self-Driving_Car_Platform_architecture](./_assets/CS597_architecture_block_diagram)

## Contributors
- Yi Zhang
- Minqiang Liu

# Todo
## Codebase
- [ ] U5 (CH592F) (C)
	- [ ] `main.c`:
		- immediately handles any pwm actuation requests
		- no responses to confirm request packet handled
		- travelling motor: runs 8835 in `PH/EN` mode;
			- request types: reconfigure pwm frequency; can start for a finite duration (default 0); can stop
		- LiDaR motor: default run on startup; 
			- request types: reconfigure pwm frequency; request to start/stop
		- servomotor: **LOW PRIORITY**
- [ ] U6 (CH592F) (C)
	- [ ] `main.c`:
		- receives instructions (data requests) from rpi by i2c
		- pulls `INTR` pin (`6`) when high when `data_buffer` is ready
		- usually running `ultrasonic.c` unless, a data request is for `bt_gps` data?
	- [ ] `ultrasonic.c`:
		- pins `25 - 28`
		- occasionally queries external T from `thermometer.c` 
	- [ ] `thermometer.c`
	- [ ] `bt_gps.c`: **LOW PRIORITY**
		- [ ] sniffs for bluetooth packets (`bt_sniffer.c`)
		- [ ] filters for relevant bt packets that allow it to calculate location data (`bt_sniffer.c`)
		- [ ] calculates location data (`bt_gps.c`)
		- [ ]
- [ ] RPi (python)
	- [ ] `main.py`
	- [ ] `tests.py`
		- has individual tests for each of the modules
	- [ ] `i2c.py`
		- using WCH family (`CH592F`; considering `CH595M` w/ exact same interface but just 16 more pins)
	- [ ] `camera.py`: **LOW PRIORITY**
	- [ ] `tcp_server.py`: **LOW PRIORITY**
- License Details

## Lab Instructions
	- [ ] Lab 01A: assembly + pcb exercise, naive mapping, naive self-driving, 
	- [ ] Lab 01B: advanced mapping, object detection w/ camera, self-driving + maze navigation
	- [ ] Lab 02: wifi and bt; remote control from front-end server!
