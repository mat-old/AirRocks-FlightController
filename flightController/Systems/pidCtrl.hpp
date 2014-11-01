/*
*  Proportional-integral-derivative controller
*  read about http://en.wikipedia.org/wiki/PID_controller
*/
#ifndef PIDCONTROLLER 
#define PIDCONTROLLER
#include "../Includes.hpp"

class PIDctrl : public SubSystem {
public:
	PID_t roll
		, pitch
		, yaw;

	PIDctrl() : SubSystem() {
		if( !PID_ENABLED ) Disable();
		yaw.name  = "yaw";
		roll.name = "roll";
		pitch.name= "pitch";

		// 20ms
		yaw.SetTime(Defines::STD_DELAY_MS);
		roll.SetTime(Defines::STD_DELAY_MS);
		pitch.SetTime(Defines::STD_DELAY_MS);

		yaw.set_point   = Defines::yaw_zero;
		roll.set_point  = Defines::roll_zero;
		pitch.set_point = Defines::pitch_zero;

		yaw.SetPID(  yaw_P, yaw_I, yaw_D);
		roll.SetPID(  roll_P, roll_I, roll_D);
		pitch.SetPID(  pitch_P, pitch_I, pitch_D);
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
	}
};
#endif