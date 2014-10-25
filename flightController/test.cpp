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

	std::string name;

	pid_ctrl_t() {
		p_gain       = 0.0f;
		i_gain       = 0.0f;
		d_gain       = 0.0f;
		pError       = 0.0f;
		last_i_gain  = 0.0f;
		low_v        = 0.0f;
		high_v       = 0.0f;
		time_next    = millis();
		time_last    = 0;
		name         = "";
	}

	pid_ctrl_t& Use(pid_t * o) {
		out = o;
		return *this;
	}

	void Set( pid_t p_gain, pid_t i_gain, pid_t d_gain, pid_t low_v, pid_t high_v ) {
		this->p_gain = p_gain;
		this->i_gain = i_gain;
		this->d_gain = d_gain;
		this->low_v = low_v;
		this->high_v = high_v;
	}

	void Compute( pid_t mError, pid_t dError, pid_t setP ) {
		time_next = millis();
		pid_t time_derivative = time_next - time_last;
		pid_t P               = p_gain * mError;
		pid_t D               = d_gain * ( (setP*1000.0f/time_derivative) - dError );
		pError                = mError;
		pid_t I               = last_i_gain + (i_gain * mError * time_derivative/1000.0f);
		pid_t U               = P + I + D;
		last_i_gain           = I;
		time_last             = time_next;

		*out = U>=high_v?high_v:U<=low_v?low_v:U;
	}

	void Compute( pid_t mError ) {
		time_next = millis();
		pid_t time_derivative = time_next - time_last;
		pid_t P               = p_gain * mError;
		// below... (1000/time_der) first?
		pid_t D               = d_gain * (mError - pError) * 1000.0f / time_derivative;
		pError                = mError;
		pid_t I               = last_i_gain + (i_gain * mError * time_derivative/1000.0f);
		pid_t U               = P + I + D;
		std::cout << D << "   " << time_derivative << "  " << P << "  " << I << "  " << D << std::endl;  
		last_i_gain           = I;
		time_last             = time_next;


		/*		
		std::cout << this->name
				<< "\t" << FLOAT_FORMAT << P 
				<< "\t" << FLOAT_FORMAT << I
				<< "\t" << FLOAT_FORMAT << D << std::endl;
		*/

		*out = (pid_t)U>=high_v?high_v:U<=low_v?low_v:U;
	}

	void Reset() {
		last_i_gain = 0.0f;
		time_last   = millis();
	}

	friend std::ostream& operator<< (std::ostream& out, const pid_ctrl_t& p) {
		out << FLOAT_FORMAT
			<< p.high_v << ' '
			<< p.low_v  << ' '
			<< p.p_gain << ' '
			<< p.i_gain << ' '
			<< p.d_gain << ' '
			<< p.pError;
		return out;
	}	
};



/*
*  Inertia Measurement Unit Worker, does the constant gathering of IMU data 
*  this class provides the required higher level data for the PIDctrl
*/
#include "arfcDefines.hpp"
#include "imuInterface.hpp"
#include "dataTypes.hpp"
#include "subSystem.hpp"
#include <iostream>
#include <unistd.h>
#ifndef TEST
#define TEST
using namespace std;

class test : public AsyncWorker {
public:
	test()  {}
	~test() {}
	virtual void* worker_run() {
		std::cout<< "hello thread" <<endl<<flush;
		
		while( !Disposed() );
		cout << " :( bye dad! " <<endl<<flush;

		return 0;
	}
};


int main(int argc, char const *argv[])
{
	test *t = new test;

	t->Start();
	sleep(2);
	delete t;
	sleep(2);

	return 0;
}
#endif