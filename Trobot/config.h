#pragma once
#define ZERO_SPEED  65535
#define MAX_ACCEL   6
#define MAX_SPEED          400  // should be <= 500
//#define REVERSE_MOTORS_DIRECTION  // reverse both motors direction
#define BATVOLREAD//vol read

#define RAD2GRAD 57.2957795
#define GRAD2RAD 0.01745329251994329576923690768489

//#define ITERM_MAX_ERROR 25   // Iterm windup constants for PI control //40
//#define ITERM_MAX 8000       // 5000

#define KP 18
#define KI 0.1
#define KD 8
			
#define BATPIN A0
#define BATLEDPIN 13

#define DIR1	12
#define STEP1	11
#define ENABLE1	10

#define DIR2	9
#define STEP2	8
#define ENABLE2	7