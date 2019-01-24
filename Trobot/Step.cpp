// 
// 
// 

#include "config.h"
#include "Step.h"

/**************************************************************************************/
/************  Calculate and writes the motors values                ******************/
/**************************************************************************************/

uint16_t periodsCounter[2];      // counters for periods
uint16_t subPeriod[2][8];        // eight subperiodPaddings 
uint8_t subPeriodIndex[2];       // index for subperiodPaddings

int16_t actualMotorSpeed[2];     // actual speed of motors
uint8_t actualMotorDir[2];       // actual direction of steppers motors

long Vmotor1 = 0, Vmotor2 = 0;
								 
void calculateSubperiods(uint8_t motor) {

	uint8_t subperiodPadding;
	uint16_t absSpeed;
	uint8_t i;

	if (actualMotorSpeed[motor] == 0) {
		for (i = 0; i<8; i++) {
			subPeriod[motor][i] = ZERO_SPEED;
		}
		return;
	}

#ifdef REVERSE_MOTORS_DIRECTION
	actualMotorDir[motor] = (actualMotorSpeed[motor] > 0) ? 1 : 0;
#else
	actualMotorDir[motor] = (actualMotorSpeed[motor] > 0) ? 0 : 1;
#endif  

	absSpeed = abs(actualMotorSpeed[motor]);

	subPeriod[motor][0] = 1000 / absSpeed;
	for (i = 1; i<8; i++) {
		subPeriod[motor][i] = subPeriod[motor][0];
	}
	// Calculate the sub-period padding. 
	subperiodPadding = ((1000 % absSpeed) * 8) / absSpeed;
	if (subperiodPadding > 0) {
		subPeriod[motor][1]++;
	}
	if (subperiodPadding > 1) {
		subPeriod[motor][5]++;
	}
	if (subperiodPadding > 2) {
		subPeriod[motor][3]++;
	}
	if (subperiodPadding > 3) {
		subPeriod[motor][7]++;
	}
	if (subperiodPadding > 4) {
		subPeriod[motor][0]++;
	}
	if (subperiodPadding > 5) {
		subPeriod[motor][4]++;
	}
	if (subperiodPadding > 6) {
		subPeriod[motor][2]++;
	}
}
void clearVmotor(uint8_t motorNum) {
	if (motorNum == 0) {
		Vmotor1 = 0;
	}
	else {
		Vmotor2 = 0;
	}
}
void readVmotor(uint8_t motorNum, long* Vmotor_) {
	if (motorNum == 0) {
		*Vmotor_ = Vmotor1;
	}
	else
	{
		*Vmotor_ = Vmotor2;
	}
}

void setMotorSpeed(uint8_t motorNum, int16_t speed) {

	speed = constrain(speed, -MAX_SPEED, MAX_SPEED);

	// LIMIT MAX ACCELERATION
	int16_t acceleration = speed - actualMotorSpeed[motorNum];
	if (acceleration > MAX_ACCEL) {
		actualMotorSpeed[motorNum] += MAX_ACCEL;
	}
	else if (acceleration < -MAX_ACCEL) {
		actualMotorSpeed[motorNum] -= MAX_ACCEL;
	}
	else {
		actualMotorSpeed[motorNum] = speed;
	}

	calculateSubperiods(motorNum); 
							
	if ((actualMotorSpeed[0] == 0) && (actualMotorSpeed[1] == 0)) {
		digitalWrite(ENABLE1, HIGH);   // Disable motors
		digitalWrite(ENABLE2, HIGH);
	}
	else {
		digitalWrite(ENABLE1, LOW);   // Enable motors
		digitalWrite(ENABLE2, LOW);
	}
}

//#define CLR(x,y) (x&=(~(1<<y)))
//#define SET(x,y) (x|=(1<<y))

ISR(TIMER1_COMPA_vect) // DRIVER_PIN[6] = {12,11,10,9,8,7};
{
	periodsCounter[0]++;
	periodsCounter[1]++;

	if (periodsCounter[0] >= subPeriod[0][subPeriodIndex[0]]) {
		periodsCounter[0] = 0;

		if (subPeriod[0][0] != ZERO_SPEED) {
			if (actualMotorDir[0]) {
				digitalWrite(DIR1, 1);  // DIR Motor 1
				Vmotor1++;
			}
			else {
				digitalWrite(DIR1, 0);
				Vmotor1--;
			}
			subPeriodIndex[0] = (subPeriodIndex[0] + 1) & 0x07; // subPeriodIndex from 0 to 7

			digitalWrite(STEP1, 1);	// STEP Motor 1
			delayMicroseconds(1);
			digitalWrite(STEP1, 0);
		}
	}

	if (periodsCounter[1] >= subPeriod[1][subPeriodIndex[1]]) {
		periodsCounter[1] = 0;

		if (subPeriod[1][0] != ZERO_SPEED) {

			if (actualMotorDir[1]) {
				digitalWrite(DIR2, 1);   // DIR Motor 2
				Vmotor2++;
			}
			else {
				digitalWrite(DIR2, 0);
				Vmotor2--;
			}
			subPeriodIndex[1] = (subPeriodIndex[1] + 1) & 0x07;

			digitalWrite(STEP2, 1); // STEP Motor 2
			delayMicroseconds(1);
			digitalWrite(STEP2, 0);
		}
	}
}



/**************************************************************************************/
/************        Initialize the Timers and Registers         ******************/
/**************************************************************************************/
void initOutput() {
	/****************            mark all driver pins as Output             ******************/
	pinMode(DIR1, OUTPUT);
	pinMode(STEP1, OUTPUT);
	pinMode(ENABLE1, OUTPUT);
	pinMode(DIR2, OUTPUT);
	pinMode(STEP2, OUTPUT);
	pinMode(ENABLE2, OUTPUT);

	/********  Specific Timers & Registers for the atmega328P (Promini)   ************/
	digitalWrite(ENABLE1, HIGH);   // Disable motors
	digitalWrite(ENABLE2, HIGH);

							 //We are going to overwrite the Timer1 to use the stepper motors
							 // STEPPER MOTORS INITIALIZATION
							 // TIMER1 CTC MODE
	TCCR1B &= ~(1 << WGM13);
	TCCR1B |= (1 << WGM12);
	TCCR1A &= ~(1 << WGM11);
	TCCR1A &= ~(1 << WGM10);

	// output mode = 00 (disconnected)
	TCCR1A &= ~(3 << COM1A0);
	TCCR1A &= ~(3 << COM1B0);

	// Set the timer pre-scaler
	// Generally we use a divider of 8, resulting in a 2MHz timer on 16MHz CPU
	TCCR1B = (TCCR1B & ~(0x07 << CS10)) | (2 << CS10);

	//OCR1A = 160;  // 16Khz
	//OCR1A = 100;  // 20Khz
	OCR1A = 80;   // 25Khz
	TCNT1 = 0;

	TIMSK1 |= (1 << OCIE1A);  // Enable Timer1 interrupt
	digitalWrite(ENABLE1, LOW);   // Enable stepper drivers
	digitalWrite(ENABLE2, LOW);
}


