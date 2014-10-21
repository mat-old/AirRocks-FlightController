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
	pthread_t worker;
	Potential apot;
	Potential gpot;
public:
	IMUworker() : IMUinterface() {
		apot.Zero();
		gpot.Zero();
	}
	~IMUworker() {
		Dispose();
	}
	IMUworker& Open() {
		IMUinterface::Open();
	}
	IMUworker& Prepare() {
		IMUinterface::Prepare();
	}
	/* start thread */
	IMUworker& Start() {
		Set_Active(true);
		if( !Fail() ) {
			int ret = pthread_create(&worker, NULL, &worker_helper, this);
			if( ret != 0 ) throw FAIL_START_WORKER;
		} else throw FAIL_FLAG_SET;
		return *this;
	}
	/* "just saves resources, dont do if switch has too much over head" */
	IMUworker& Detach() {
		pthread_detach(worker);
		return *this;
	}

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
	static void *worker_helper(void *context) {
		return ((IMUworker *)context)->worker_run();
	}
	void *worker_run() {
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