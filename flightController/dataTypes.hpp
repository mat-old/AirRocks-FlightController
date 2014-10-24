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

/* leverage floats for speed */
class pid_ctrl_t {
public:
	pid_t p_gain;
	pid_t i_gain;
	pid_t d_gain;
	pid_t pError;
	pid_t last_i_gain;
	pid_t low_v;
	pid_t high_v;
	int64_t       time_next
				, time_last;
	pid_t * out;

	pid_ctrl_t() {
		p_gain       = 0.0f;
		i_gain       = 0.0f;
		d_gain       = 0.0f;
		pError       = 0.0f;
		last_i_gain  = 0.0f;
		low_v        = 0.0f;
		high_v       = 0.0f;
		time_next    = Def::millis();
		time_last    = 0;
	}

	void Use(pid_t * o) {
		out = o;
	}

	void Set( pid_t p_gain, pid_t i_gain, pid_t d_gain, pid_t low_v, pid_t high_v ) {
		this->p_gain = p_gain;
		this->i_gain = i_gain;
		this->d_gain = d_gain;
		this->low_v = low_v;
		this->high_v = high_v;
	}

	void Compute( pid_t mError, pid_t dError, pid_t setP ) {
		time_next = Def::millis();
		pid_t time_derivative = time_next - time_last;
		pid_t P               = p_gain * mError;
		pid_t D               = d_gain * ( (setP*1000.0f/time_derivative) - dError );
		pError                = mError;
		pid_t I               = last_i_gain + (i_gain * mError * time_derivative/1000.0f);
		pid_t U               = P + I + D;
		last_i_gain           = I;
		time_last             = time_next;

		*out = U>high_v?high_v:U<low_v?low_v:U;
	}

	void Compute( pid_t mError ) {
		time_next = Def::millis();
		pid_t time_derivative = time_next - time_last;
		pid_t P               = p_gain * mError;
		// below... (1000/time_der) first?
		pid_t D               = d_gain * (mError - pError) * 1000.0f / time_derivative;
		pError                = mError;
		pid_t I               = last_i_gain + i_gain * mError * time_derivative / 1000.0f;
		pid_t U               = P + I + D;
		last_i_gain           = I;
		time_last             = time_next;

		*out = (pid_t)U>high_v?high_v:U<low_v?low_v:U;
	}

	void Reset() {
		last_i_gain = 0.0f;
		time_last   = Def::millis();
	}
};
#endif