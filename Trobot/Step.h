// Step.h

#ifndef STEP_h_
#define STEP_h_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

void initOutput();
void setMotorSpeed(uint8_t motorNum, int16_t tspeed);
void readVmotor(uint8_t motorNum, long* Vmotor_);
void clearVmotor(uint8_t motorNum);

#endif

