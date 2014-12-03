/*
*  Inertia Measurement Unit Worker, does the constant gathering of IMU data 
*  this class provides the required higher level data for the PIDctrl
*/
#include "arfcDefines.hpp"
#include "imuInterface.hpp"
#include "dataTypes.hpp"

#define IMUW_DEBUG
#ifdef IMUW_DEBUG
	#include <iostream>
#endif

#ifndef IMUWORKER
#define IMUWORKER 
class IMUworker : public IMUinterface {
private:
	Potential apot;
	Potential gpot;
public:
	IMUworker() : IMUinterface() {
		apot.Zero();
		gpot.Zero();
		if( !IMU_ENABLED ) Disable();
	}
	~IMUworker() {}
	IMUworker& Update(Potential& gyroPot, Potential& accelPot) {
		if( Data_Valid() && access.try_lock() ) {
			gyroPot.copy(gpot);
			accelPot.copy(apot);
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
		time_next = Def::millis();
		while(true) {
			IMUinterface::Read();
			/* Critical *//* Critical */
				access.lock();
				apot.Gather(data_a);
				gpot.Gather(data_g);
				access.unlock();
			/* Critical *//* Critical */
			Set_Data_Valid(true);
	        while(Def::millis() - time_next < 20) {
	        	if( Disposed() ) return 0;
	            usleep(1000);
	        }
	    }
	}
};

#endif