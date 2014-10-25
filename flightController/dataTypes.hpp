#include "ARHS/vector.h"

#include "arfcDefines.hpp"

#ifndef  DATATYPES
#define  DATATYPES
class Potential {
public:
	float x, y, z;
	Potential()  {}
	Potential(vector v)  {
		this->Gather(v);
	}
	~Potential() {}

	void Gather(vector v) {
		x = v(0);
		y = v(1);
		z = v(2);
	}

	void copy( Potential& p ) {
		x = p.x;
		y = p.y;
		z = p.z;
	}

	void Zero() {
		x = 0;
		y = 0;
		z = 0;
	}

	friend std::ostream& operator<< (std::ostream& out, const Potential& p) {
		out << FLOAT_FORMAT << p.x << ' '<< p.y << ' '<< p.z;
		return out;
	}
};

/*  this is my version of a PID based on the formula found on Wikipedia */
class pid_alt {
public:
	int16_t   sample_time;
	uint64_t  last_time;
	pid_t    *output
			, error_last
			, set_point
			, error_sum
			, kp
			, ki
			, kd;
	std::string name;

	pid_alt() {
		sample_time = 1000; 
		last_time = millis();
	}

	pid_t Compute(pid_t input) {
		uint64_t now = millis();
		if( ( now - last_time ) >= sample_time ) {
			pid_t error      = set_point - input;
			error_sum        += error;
			pid_t d_error    = error - error_last;

			*output = kp * error + ki * error_sum + kd * d_error;

			error_last = error;
			last_time  = now;
		}
	}

	pid_alt& Attach(pid_t *output) {
		this->output = output;
		return *this;
	}

	pid_alt& SetPID(pid_t kp, pid_t ki, pid_t kd) {
		pid_t sample = sample_time / 1000.0f;
		this->kp = kp;
		this->ki = ki * sample;
		this->kd = kd / sample;
		return *this;
	}

	pid_alt& SetTime(int16_t newsample) {
		if( newsample > 0 ) {
			pid_t ratio = (pid_t)newsample / (pid_t)sample_time;
			ki *= ratio;
			kd /= ratio;
			sample_time = newsample;
		}
	}

};


#endif