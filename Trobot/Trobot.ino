/*
Name:		Trobot.ino
Created:	10/02/2017 10:36:42 AM
Author:	Van Than
Never give up!
*/
/*************************************************************
===================================
Trobot rev 0.1
===================================

Hardware:
- Arduino nano (atmega328p).
- mpu6050 gyro-accelerometer (GY_521).
- Bluetooth module like HC-05.
- A4988 motor drivers with 1/16 microstepping.
- Nema 17 stepper motors.
- SRF005 ULTRASONIC RANGE SENSOR.

Pinout for Arduino nano (atmega328p):
- A0 - BATPIN: after the resistor divisor we should get [0V;5V]->[0;1023] on analogue BATPIN with R1=22k and R2=47k.

I2C:
- A4 - SDA
- A5 - SCL

Serial:
- TX - RX HC05
- RX - TX HC05

Motor driver pins:
- D12 - DIR1 (PORTB 4)
- D11 - STEP1 (PORTB 3)
- D10 - ENABLE1 (PORTB 2)

- D9 - DIR2 (PORTB 1)
- D8 - STEP2 (PORTB 0)
- D7 - ENABLE2 (PORTD 7)

If you look to the tail of the robot:
- right motor = STEP2 & DIR2 & ENABLE2
- left motor  = STEP1 & DIR1 & ENABLE1

Big thanks for the projects:
- B-Robot https://github.com/JJulio/b-robot
- BalancingWii https://github.com/mahowik/BalancingWii

3D print file STL: https://www.thingiverse.com/thing:2563756

***************************************************************/
