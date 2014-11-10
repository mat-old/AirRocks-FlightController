#include "PID.hpp"

	PID_t::PID_t() {
		output=last_input=error_last=set_point=error_sum=kp=ki=kd = 0.0f;
		sample_time = 1000; 
		last_time 	= millis();
	}

	void PID_t::Compute(var_float_t input) {
		last_input = input;
		uint64_t now = millis();
		if( ( now - last_time ) >= sample_time ) {
			var_float_t error      = set_point - input;
			error_sum        += error;
			var_float_t d_error    = error - error_last;

			output = kp * error + ki * error_sum + kd * d_error;

			error_last = error;
			last_time  = now;
		}
	}

	PID_t& PID_t::SetPID(var_float_t kp, var_float_t ki, var_float_t kd) {
		var_float_t sample/*in seconds*/ = sample_time / 1000.0f;
		this->kp = kp;
		this->ki = ki * sample;
		this->kd = kd / sample;
		return *this;
	}
	PID_t& PID_t::setP(var_float_t kp) {
		this->kp = kp;
		return *this;
	}
	PID_t& PID_t::setI(var_float_t ki) {
		var_float_t sample/*in seconds*/ = sample_time / 1000.0f;
		this->ki = ki * sample;
		return *this;
	}
	PID_t& PID_t::setD(var_float_t kd) {
		var_float_t sample/*in seconds*/ = sample_time / 1000.0f;
		this->kd = kd / sample;
		return *this;
	}
	PID_t& PID_t::SetTime(int16_t newsample) {
		if( newsample > 0 ) {
			var_float_t ratio = (var_float_t)newsample / (var_float_t)sample_time;
			ki *= ratio;
			kd /= ratio;
			sample_time = newsample;
		}
	}