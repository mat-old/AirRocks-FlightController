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
	PID_t roll
		, pitch
		, yaw
		, x
		, y;

	int time_next;
public:
	PIDctrl() : SubSystem() {
		time_next = millis();
		//Ppitch.Attach (&pitch).name= "pitch";
		//Proll.Attach  (&roll).name = "roll ";
		//Pyaw.Attach   (&yaw).name  = "yaw  ";
		//Px.Use     (&x).name    = "x    ";
		//Py.Use     (&y).name    = "y    ";
		//p  i, pid_t d, min, max
		pitch.name= "pitch";
		roll.name = "roll ";
		yaw.name  = "yaw  ";

		pitch.SetTime(20);  /*  20 ms for each */
		roll.SetTime(20);
		yaw.SetTime(20);

		pitch.set_point = Def::pitch_zero;
		roll.set_point  = Def::roll_zero;
		yaw.set_point   = Def::yaw_zero;

		roll.SetPID(
			  roll_P
			, roll_I
			, roll_D);
			//, roll_MIN
			//, roll_MAX);
		pitch.SetPID(
			  pitch_P
			, pitch_I
			, pitch_D);
			//, pitch_MIN
			//, pitch_MAX);
		yaw.SetPID(
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


	PIDctrl& Calculate( Motorgroup& motors,
						const Potential_t& steering,
						const Potential_t& gyro,
						const Potential_t& accel ) {

		pitch.Compute (  accel.x ); 
		roll.Compute  (  accel.y );
		yaw.Compute   (  accel.z );  

		/*4*/motors[0].setReserve(   roll  - yaw );
		/*2*/motors[1].setReserve(   pitch + yaw );
		/*1*/motors[2].setReserve( - roll  - yaw );
		/*3*/motors[3].setReserve( - pitch + yaw );

		//int change = (int)(100.0f*pitch);
		/*if( (millis() - time_next) > 20 ) {
			std::cout
				<< "  " << Proll
				<< "  " << (int)*a
				<< "  " << (int)*b
				<< "  " << (int)*c
				<< "  " << (int)*d

				<< " P " << FLOAT_FORMAT << pitch
				//<< "  " << FLOAT_FORMAT << roll
				//<< "  " << FLOAT_FORMAT << yaw

				<< " A " << FLOAT_FORMAT << accel.x // PITCH
				//<< "  " << FLOAT_FORMAT << accel.y // ROLL
				//<< "  " << FLOAT_FORMAT << accel.z // YAW

				<< " E " << change
				<< " -> " << throttle + change
			<< std::endl;
			time_next = millis();
		}*/
	}
};
#endif