/*  this is my version of a PID based on the formula found on Wikipedia */
#ifndef PID_T_LIB
#define PID_T_LIB
#include "../Defines.hpp"
#include <string>
#include <stdint.h>
class PID_t {
public:
	int16_t   sample_time;
	uint64_t  last_time;
	var_float_t     output
			, last_input
			, error_last
			, set_point
			, error_sum
			, kp
			, ki
			, kd;
	std::string name;

	PID_t();
	void Compute(var_float_t input);

	PID_t& SetPID(var_float_t kp, var_float_t ki, var_float_t kd);
	PID_t& setP(var_float_t kp);
	PID_t& setI(var_float_t ki);
	PID_t& setD(var_float_t kd);
	PID_t& SetTime(int16_t newsample);

	friend var_float_t operator+(const PID_t &p, const var_float_t &o) {
		return p.output + o;
	}
	friend var_float_t operator+(const PID_t &p, const PID_t &o) {
		return p.output + o.output;
	}
	friend var_float_t operator+(const var_float_t &o, const PID_t &p) {
		return o + p.output;
	}
	friend var_float_t operator-(const PID_t &p, const var_float_t &o) {
		return p.output - o;
	}
	friend var_float_t operator-(const PID_t &p, const PID_t &o) {
		return p.output - o.output;
	}
	friend var_float_t operator-(const var_float_t &o, const PID_t &p) {
		return o - p.output;
	}
	friend var_float_t operator-(const PID_t &p) {
		return - p.output;
	}
	friend var_float_t operator*(const PID_t &p, const var_float_t &o) {
		return p.output * o;
	}
	friend var_float_t operator*(const var_float_t &o, const PID_t &p) {
		return o * p.output;
	}

/*	friend PID_t operator*(const PID_t &o, const PID_t &p) {
		return PID_t(o.output * p.output);
	}
	friend PID_t operator+(const PID_t &o, const PID_t &p) {
		return PID_t(o.output + p.output);
	}
	friend PID_t operator-(const PID_t &o, const PID_t &p) {
		return PID_t(o.output - p.output);
	}*/
	/*PID_t& Attach(var_float_t *output) {
		this->output = output;
		return *this;
	}*/

};
#endif