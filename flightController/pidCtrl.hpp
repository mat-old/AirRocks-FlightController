/*
*  Special thanks to baselsw, https://github.com/baselsw
*/
/*
*  Proportional-integral-derivative controller
*  read about http://en.wikipedia.org/wiki/PID_controller
*/
#include "dataTypes.hpp"
using namespace Def;
#ifndef PIDCONTROLLER 
#define PIDCONTROLLER
class PIDctrl : public SubSystem {
private:
	pid_ctrl_t 	  Proll
				, Ppitch
				, Pyaw
				, Px
				, Py;
	pid_t     roll
			, pitch	
			, yaw
			, x
			, y;

	uint8_t *a, *b, *c, *d;
public:
	PIDctrl() : SubSystem() {
		Proll.Use  (&roll);
		Ppitch.Use (&pitch);
		Pyaw.Use   (&yaw);
		Px.Use     (&x);
		Py.Use     (&y);
		//p  i, pid_t d, min, max
		Proll.Set(
			  roll_P
			, roll_I
			, roll_D
			, roll_MIN
			, roll_MAX);
		Ppitch.Set(
			  pitch_P
			, pitch_I
			, pitch_D
			, pitch_MIN
			, pitch_MAX);
		Pyaw.Set(
			  yaw_P
			, yaw_I
			, yaw_D
			, yaw_MIN
			, yaw_MAX);
		Px.Set(
			  x_P
			, x_I 
			, x_D 
			, x_MIN
			, x_MAX);
		Py.Set(
			  y_P
			, y_I
			, y_D
			, y_MIN
			, y_MAX);

		if( !PID_ENABLED ) Disable();
	}
	~PIDctrl() {
		Dispose();
	}


	PIDctrl& Calculate(uint8_t throttle, const Potential& steering, const Potential& gyro, const Potential& accel) {

		Proll.Compute  ( steering.x - accel.y );
		Ppitch.Compute ( steering.y - accel.x ); 
		Pyaw.Compute   ( steering.z - accel.z );  

		*a = throttle + roll + pitch + yaw;
		*b = throttle - roll + pitch - yaw;
		*c = throttle + roll - pitch - yaw;
		*d = throttle - roll - pitch + yaw;




	std::cout << "  " << (int)*a
			 << "  " << (int)*b
			 << "  " << (int)*c
			 << "  " << (int)*d
			 << "  " << FLOAT_FORMAT << accel.x
			 << "  " << FLOAT_FORMAT << accel.y
			 << "  " << FLOAT_FORMAT << accel.z
			 << std::endl;


	}
	PIDctrl& Use(uint8_t *motors) {
		a = motors+0;
		b = motors+1;
		c = motors+2;
		d = motors+3;
	}
};
#endif