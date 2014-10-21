/* For properties and members all sub systems should have */
#include "errorMap.hpp"
#include <stdint.h>
#ifndef SUBSYSTEM
#define SUBSYSTEM

class SubSystem { 
protected:
ErrorMap err;
bool      fail_flag;
volatile bool dispose;
public:
	SubSystem() {
		dispose          = false;
		fail_flag        = false;
	}
	~SubSystem(){}

	SubSystem& Dispose() {
		dispose = true;
		return *this;
	}
};
#endif