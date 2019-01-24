#include "Arduino.h"
#include "config.h"
#include "I2Cdev.h"
#include "Wire.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <avr/interrupt.h>
#include "MPUSenser.h"
#include "Trobot.h"
#include "Step.h"

float CalcuPID(float dt_, float error_, float set_point_) {
	float pid_out_;

	error = error_ - set_point_;
	I_mem += error * Ki;
	if (I_mem > MAX_SPEED) I_mem = MAX_SPEED;
	else if (I_mem < -MAX_SPEED) I_mem = -MAX_SPEED;
	pid_out_ = Kp * error + I_mem * dt_ + Kd * (error - error_old) / dt_;

	error_old = error;

	if (pid_out_ > MAX_SPEED) pid_out_ = MAX_SPEED;
	else if (pid_out_ < -MAX_SPEED) pid_out_ = -MAX_SPEED;

	if (pid_out_ < 1 && pid_out_ > -1) pid_out_ = 0;

	return pid_out_;
}
void ReadCMDsetpoint(char Serialread_) {
	if (Extra == true) {
		val_set_point = SpeedTrobot * 3;
	}
	else
	{
		val_set_point = SpeedTrobot;
	}
	switch (Serialread_)
	{
	case '0':SpeedTrobot = 0; Read_serial = 'S'; break;
	case '1':SpeedTrobot = 1; Read_serial = 'S'; break;
	case '2':SpeedTrobot = 2; Read_serial = 'S'; break;
	case '3':SpeedTrobot = 3; Read_serial = 'S'; break;
	case '4':SpeedTrobot = 4; Read_serial = 'S'; break;
	case '5':SpeedTrobot = 5; Read_serial = 'S'; break;
	case '6':SpeedTrobot = 6; Read_serial = 'S'; break;
	case '7':SpeedTrobot = 7; Read_serial = 'S'; break;
	case '8':SpeedTrobot = 8; Read_serial = 'S'; break;
	case '9':SpeedTrobot = 9; Read_serial = 'S'; break;
	case 'q':SpeedTrobot = 10; Read_serial = 'S'; break;
	case 'S':set_point = 0; break;
	case 'F':set_point = -val_set_point; break;
	case 'B':set_point = val_set_point; break;
	case 'L':set_point = 0; break;
	case 'R':set_point = 0; break;
	case 'G':set_point = -val_set_point; break;
	case 'I':set_point = -val_set_point; break;
	case 'H':set_point = val_set_point; break;
	case 'J':set_point = val_set_point; break;
	case 'X':Extra = true; break;
	case 'x':Extra = false; break;
	default:
		break;
	}
}
void ReadCMDsetPIDout(char Serialread_, float PID_out) {
	switch (Serialread_)
	{
	case 'S':steering = 0; break;
	case 'F':steering = 0; break;
	case 'B':steering = 0; break;
	case 'L':steering = -SpeedTrobot * 10; break;
	case 'R':steering = SpeedTrobot * 10; break;
	case 'G':steering = -SpeedTrobot * 7; break;
	case 'I':steering = SpeedTrobot * 7; break;
	case 'H':steering = -SpeedTrobot * 7; break;
	case 'J':steering = SpeedTrobot * 7; break;
	default:
		break;
	}
	PIDout = PID_out;
}
void setup() {
	setMotorSpeed(0, 0);
	setMotorSpeed(1, 0);
	Serial.begin(115200);
	pinMode(BATLEDPIN, OUTPUT);
	initOutput();
	InitMPU();
	timer_value = millis();
}

void loop() {
	timer_value = millis();
	dt = (timer_value - timer_old);
	timer_old = timer_value;

	ReadGyroValue(&y, &p, &r);
	//Serial.println(r);

	ReadCMDsetPIDout(Read_serial, CalcuPID(dt, r, set_point));

	//Serial.println(PIDout);

	if (r < 60 && r > -60) {
		setMotorSpeed(0, PIDout + steering);
		setMotorSpeed(1, PIDout - steering);
	}
	else if (r > 60) {
		setMotorSpeed(0, 0);
		setMotorSpeed(1, 0);
		//delay(1000);
		//setMotorSpeed(0, 100);
		//setMotorSpeed(1, 100);
	}
	else if (r < -60) {
		setMotorSpeed(0, 0);
		setMotorSpeed(1, 0);
		//delay(1000);
		//setMotorSpeed(0, -100);
		//setMotorSpeed(1, -100);
	}
}
