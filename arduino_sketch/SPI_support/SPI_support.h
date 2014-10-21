// SPI_support.h
#ifndef _SPI_SUPPORT_h
#define _SPI_SUPPORT_h
#include "Arduino.h"

/* Types & defs */
typedef enum {
	MOTOR_ZERO_LEVEL = 125
	, MOTOR_ARM_START = 140
	, MOTOR_MAX_LEVEL = 254
} SpeedConstant;

typedef enum {
	  mpA = 6
	, mpB = 10
	, mpC = 9
	, mpD = 5
} MotorPin;

typedef union {
	byte data;
	char char_t;
	int  int_t;
} Speed;

/* debug methods */
char * pinToID(MotorPin pin) {
	switch (pin) {
	case mpA: return "  (A)";
	case mpB: return " (B)";
	case mpC: return "  (C)";
	case mpD: return "  (D)";
	}
	return "Err";
}

class Motor {
public:
	MotorPin pin;
	byte newSpeed;
	byte oldSpeed;
	Motor() {
		newSpeed = MOTOR_ZERO_LEVEL;
	}
	bool changed() {
		return newSpeed == oldSpeed;
	}
	void update() {
		oldSpeed = newSpeed;
	}
	void update_if_change() {
		if(changed()) {
			analogWrite( pin, (int)newSpeed ); 
			oldSpeed = newSpeed;
			#ifdef DEBUG
				Serial.print(pin);
				Serial.print(pinToID(pin));
				Serial.print(newSpeed);
				Serial.print('\n');
			#endif
		}
	}
	void usepin(MotorPin pin) {
		this->pin = pin;
	}
};


#endif

