// Trobot.h

#ifndef _TROBOT_h
#define _TROBOT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


long timer_old;
long timer_value;
float dt, error, error_old, I_mem;
float Kp = KP, Ki = KI, Kd = KD;
float PIDout = 0;;
float steering = 0;
int	val_set_point = 0, set_point = 0;

bool BATlow = false;
bool Extra = false;

float y,p,r;

int8_t SpeedTrobot = 0;
char Read_serial = 'S';

#endif

