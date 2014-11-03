/*  this is my version of a PID based on the formula found on Wikipedia */
#ifndef PID_T_LIB
#define PID_T_LIB
#include "../Defines.hpp"
class PID_t {
public:
	int16_t   sample_time;
	uint64_t  last_time;
	pid_t     output
			, last_input
			, error_last
			, set_point
			, error_sum
			, kp
			, ki
			, kd;
	std::string name;

	PID_t() {
		output=last_input=error_last=set_point=error_sum=kp=ki=kd = 0.0f;
		sample_time = 1000; 
		last_time 	= millis();
	}

	void Compute(pid_t input) {
		last_input = input;
		uint64_t now = millis();
		if( ( now - last_time ) >= sample_time ) {
			pid_t error      = set_point - input;
			error_sum        += error;
			pid_t d_error    = error - error_last;

			output = kp * error + ki * error_sum + kd * d_error;

			error_last = error;
			last_time  = now;
		}
	}

	PID_t& SetPID(pid_t kp, pid_t ki, pid_t kd) {
		pid_t sample/*in seconds*/ = sample_time / 1000.0f;
		this->kp = kp;
		this->ki = ki * sample;
		this->kd = kd / sample;
		return *this;
	}
	PID_t& SetP(pid_t kp) {
		this->kp = kp;
		return *this;
	}
	PID_t& SetI(pid_t ki) {
		pid_t sample/*in seconds*/ = sample_time / 1000.0f;
		this->ki = ki * sample;
		return *this;
	}
	PID_t& SetD(pid_t kd) {
		pid_t sample/*in seconds*/ = sample_time / 1000.0f;
		this->kd = kd / sample;
		return *this;
	}
	PID_t& SetTime(int16_t newsample) {
		if( newsample > 0 ) {
			pid_t ratio = (pid_t)newsample / (pid_t)sample_time;
			ki *= ratio;
			kd /= ratio;
			sample_time = newsample;
		}
	}

	friend pid_t operator+(const PID_t &p, const pid_t &o) {
		return p.output + o;
	}
	friend pid_t operator+(const PID_t &p, const PID_t &o) {
		return p.output + o.output;
	}
	friend pid_t operator+(const pid_t &o, const PID_t &p) {
		return o + p.output;
	}
	friend pid_t operator-(const PID_t &p, const pid_t &o) {
		return p.output - o;
	}
	friend pid_t operator-(const PID_t &p, const PID_t &o) {
		return p.output - o.output;
	}
	friend pid_t operator-(const pid_t &o, const PID_t &p) {
		return o - p.output;
	}
	friend pid_t operator-(const PID_t &p) {
		return - p.output;
	}
	friend pid_t operator*(const PID_t &p, const pid_t &o) {
		return p.output * o;
	}
	friend pid_t operator*(const pid_t &o, const PID_t &p) {
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
	/*PID_t& Attach(pid_t *output) {
		this->output = output;
		return *this;
	}*/

};
#endif