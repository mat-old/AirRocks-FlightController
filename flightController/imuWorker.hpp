/*
*  Inertia Measurement Unit Worker, does the constant gathering of IMU data 
*  this class provides the required higher level data for the PIDctrl
*/
//#define IMUW_DEBUG
#ifdef IMUW_DEBUG
	#include <iostream>
#endif

#ifndef IMUWORKER
#define IMUWORKER 
#include "arfcDefines.hpp"
#include "imuInterface.hpp"
#include "dataTypes.hpp"

class IMUworker : public IMUinterface {
private:
	Potential_t apot;
	Potential_t gpot;
public:
	IMUworker() : IMUinterface() {
		apot.Zero();
		gpot.Zero();
		if( !IMU_ENABLED ) Disable();
	}
	~IMUworker() {}
	IMUworker& Update(Potential_t& gyroPot, Potential_t& accelPot) {
		if( Data_Valid() && access.try_lock() ) {
			accelPot.copy(apot);
			gyroPot.copy(gpot);
			access.unlock();
			Set_Data_Valid(false);
		}
		return *this;
	}
private:
	virtual void *worker_run() {
		#ifdef IMUW_DEBUG
			std::cout << "\n> IMUworker about to start...\n" << std::flush;
		#endif
		time_next = millis();
		while(true) {
			IMUinterface::Read();
			/* Critical *//* Critical */
				access.lock();
				apot.Gather(data_a);
				gpot.Gather(data_g);
				access.unlock();
			/* Critical *//* Critical */
			Set_Data_Valid(true);
	        while( !timer->Allow() ) {
	        	if( Disposed() ) return 0;
	            usleep(1000);
	        }
	    }
	}
};

#endif