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
	pid_alt 	  Proll
				, Ppitch
				, Pyaw
				, Px
				, Py;
	pid_t     roll
			, pitch	
			, yaw
			, x
			, y;

	int time_next;

	uint8_t *a, *b, *c, *d;
public:
	PIDctrl() : SubSystem() {
		time_next = millis();
		Ppitch.Attach (&pitch).name= "pitch";
		Proll.Attach  (&roll).name = "roll ";
		Pyaw.Attach   (&yaw).name  = "yaw  ";
		//Px.Use     (&x).name    = "x    ";
		//Py.Use     (&y).name    = "y    ";
		//p  i, pid_t d, min, max

		Ppitch.SetTime(20);  /*  20 ms for each */
		Proll.SetTime(20);
		Pyaw.SetTime(20);

		Ppitch.set_point = Def::pitch_zero;
		Proll.set_point  = Def::roll_zero;
		Pyaw.set_point   = Def::yaw_zero;

		Proll.SetPID(
			  roll_P
			, roll_I
			, roll_D);
			//, roll_MIN
			//, roll_MAX);
		Ppitch.SetPID(
			  pitch_P
			, pitch_I
			, pitch_D);
			//, pitch_MIN
			//, pitch_MAX);
		Pyaw.SetPID(
			  yaw_P
			, yaw_I
			, yaw_D);
			//, yaw_MIN
			//, yaw_MAX);
/*		Px.Set(
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
*/
		if( !PID_ENABLED ) Disable();
	}
	~PIDctrl() {
		Dispose();
	}


	PIDctrl& Calculate( uint8_t throttle,
						const Potential& steering,
						const Potential& gyro,
						const Potential& accel ) {

		Ppitch.Compute (  accel.x ); 
		Proll.Compute  (  accel.y );
		Pyaw.Compute   (  accel.z );  

		/*1*/*c = throttle - roll + pitch + yaw;
		/*3*/*d = throttle + roll + pitch - yaw;
		/*2*/*b = throttle - roll - pitch - yaw;
		/*4*/*a = throttle + roll - pitch + yaw;

		if( (millis() - time_next) > 200 ) {
			std::cout
				/*<< "  " << Proll
				<< "  " << (int)*a
				<< "  " << (int)*b
				<< "  " << (int)*c
				<< "  " << (int)*d*/

				<< " P " << FLOAT_FORMAT << pitch
				//<< "  " << FLOAT_FORMAT << roll
				//<< "  " << FLOAT_FORMAT << yaw

				<< " A " << FLOAT_FORMAT << accel.x // PITCH
				//<< "  " << FLOAT_FORMAT << accel.y // ROLL
				//<< "  " << FLOAT_FORMAT << accel.z // YAW
			<< std::endl;
			time_next = millis();
		}
	}
	PIDctrl& Use(uint8_t *motors) {
		a = motors+0;
		b = motors+1;
		c = motors+2;
		d = motors+3;
	}
};
#endif