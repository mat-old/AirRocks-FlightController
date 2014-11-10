/*
*  Inertia Measurement Unit Worker, does the constant gathering of IMU data 
*  this class provides the required higher level data for the PIDctrl
*/
#ifndef IMUWORKER
#define IMUWORKER 
#include "../Defines.hpp"
#include "../Types/Potential.hpp"
#include "imuInterface.hpp"


class IMUworker : public IMUinterface {
private:
	Potential_t apot;
	Potential_t gpot;
public:
	IMUworker() ;
	~IMUworker() ;
	IMUworker& Update(Potential_t& gyroPot, Potential_t& accelPot);
private:
	virtual void *worker_run();
};
#endif