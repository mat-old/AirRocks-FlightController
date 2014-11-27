/* For properties and members all sub systems should have */
#include "errorMap.hpp"
#include <stdint.h>
#ifndef SUBSYSTEM
#define SUBSYSTEM
class SubSystem {
protected:
ErrorMap err;
public:
	SubSystem() {}
	~SubSystem(){}

};
#endif