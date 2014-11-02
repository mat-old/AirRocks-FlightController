#ifndef THROTTLE_T
#define THROTTLE_T
#include "../Includes.hpp"
using namespace Defines;
class Throttle_t {
public:
	std::string name;
	uint8_t reserved
		  , power;

	pid_t set_p
		, set_r;

	Throttle_t()  {}
	Throttle_t(const std::string n)  { name = n; }
	~Throttle_t() {}

	void setReserveRatio( pid_t res ) {
		pid_t r  = (pid_t)THROTTLE_MAX * res;
		power    = (uint8_t)((pid_t)THROTTLE_MAX - r);
		reserved = (uint8_t)r;
	}
	void setReserve( pid_t in ) {
		set_r = reserved * in;
	}
	void setPower( pid_t in ) {
		set_p = power * in;
	}
	uint8_t SPI_data() {
		return Throttle() + MOTOR_ZERO_LEVEL;
	}
	uint8_t Throttle() {
		uint8_t ret = (set_p + set_r);
		return ret>THROTTLE_MAX?THROTTLE_MAX:ret<0?0:ret;
	}
};
#endif