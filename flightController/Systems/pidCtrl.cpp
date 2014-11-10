#include "pidCtrl.hpp"
	PIDctrl::PIDctrl() : SubSystem() {
		if( !global::PID_ENABLED ) Disable();
		yaw.name  = "yaw";
		roll.name = "roll";
		pitch.name= "pitch";

		// 20ms
		yaw.SetTime(STD_DELAY_MS);
		roll.SetTime(STD_DELAY_MS);
		pitch.SetTime(STD_DELAY_MS);

		yaw.set_point   = yaw_zero;
		roll.set_point  = roll_zero;
		pitch.set_point = pitch_zero;

		yaw.SetPID(  yaw_P, yaw_I, yaw_D);
		roll.SetPID(  roll_P, roll_I, roll_D);
		pitch.SetPID(  pitch_P, pitch_I, pitch_D);
	}
	PIDctrl::~PIDctrl() {
		Dispose();
	}

	PID_t& PIDctrl::getRoll() {
		return this->roll;
	}
	PID_t& PIDctrl::getPitch() {
		return this->pitch;
	}
	PID_t& PIDctrl::getYaw() {
		return this->yaw;
	}

	PIDctrl& PIDctrl::Calculate( Motorgroup& motors,
						const Potential_t& steering,
						const Potential_t& gyro,
						const Potential_t& accel ) {

		pitch.Compute (  accel.x ); 
		roll.Compute  (  accel.y );
		yaw.Compute   (  accel.z );  
		/* Thanks to narutov6 : http://robotics.stackexchange.com/questions/5116/how-to-find-a-solution-for-quadcopter-pid-control/5120#5120 */
		/*1*/motors[0].setReserve(   roll  - yaw ); 
		/*2*/motors[1].setReserve(   pitch + yaw );
		/*3*/motors[2].setReserve( - roll  - yaw );
		/*4*/motors[3].setReserve( - pitch + yaw );
	}