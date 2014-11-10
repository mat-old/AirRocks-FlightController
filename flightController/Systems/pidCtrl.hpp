/*
*  Proportional-integral-derivative controller
*  read about http://en.wikipedia.org/wiki/PID_controller
*/
#ifndef PIDCONTROLLER 
#define PIDCONTROLLER
#include "../Defines.hpp"
#include "../Types/PID.hpp"
#include "../Types/Potential.hpp"
#include "../Types/Motorgroup.hpp"
#include "../Cores/SubSystem.hpp"

class PIDctrl : public SubSystem {
public:
	PID_t roll
		, pitch
		, yaw;

	PIDctrl();
	~PIDctrl() ;

	PID_t& getRoll();
	PID_t& getPitch();
	PID_t& getYaw();

	PIDctrl& Calculate( Motorgroup& motors,
						const Potential_t& steering,
						const Potential_t& gyro,
						const Potential_t& accel );
};
#endif