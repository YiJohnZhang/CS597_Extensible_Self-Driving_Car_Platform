# COMMAND DESIGN
- Command Design 4BY_op|4BY_target|7BY_OP (optional, 17 bytes total, < 32 bytes limit)
- e.g. `READ|U001|xxxx` read ultrasonic distance from channel 1, ignore remaining bits
- e.g. `READ|T000|xxxx` read the temperature sensor (only one), ignore remaining bits
- e.g. `POST|_DRV|L100xxx` // turn left at full speed
- e.g. `POST|_DRV|L000xxx` // "left turn" but stop
- e.g. `POST|STOP|xxxxxxx` // halt all motors

## 4 BYTE_op
### Data Transfer
- _SET (configure a setting), expect `int (0/1)` for success
- READ (read from buffer), expect a return message
- POST (post some data?), expect no return message

### Actuation
- STOP (emergency halt)
- _RUN (run some motion)

- 4 BYTE_target
### Sensors
- U### (Ultrasonic Sensor ###, up to 999)
- T### (Temperature Sensor ###)
- Technically 95*999 sensors (95/128 ASCII characters are printable), 95^4 = 81450625 if you are liberal with naming
- 94^4 = 78074896 if excluding any with `|` depending on splitting algo (either read next 4 bytes or split on `|` if variable)

### Acutators
- SE## (servomotor)
- _DRV (driving motor)

## 7 BYTE_op Examples
- _RUN|_DRV|A###### (A => (L)eft, (R)ight, (S)top; ### => speed from 000 - 100% duty cycle)