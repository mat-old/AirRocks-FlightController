#include "imuWorker.hpp"
	IMUworker::IMUworker() : IMUinterface() {
		apot.Zero();
		gpot.Zero();
		if( !global::IMU_ENABLED ) Disable();
	}
	IMUworker::~IMUworker() {}
	IMUworker& IMUworker::Update(Potential_t& gyroPot, Potential_t& accelPot) {
		if( Data_Valid() && access.try_lock() ) {
			accelPot.copy(apot);
			gyroPot.copy(gpot);
			access.unlock();
			Set_Data_Valid(false);
		}
		return *this;
	}

	void *IMUworker::worker_run() {
		/*TODO: produce feed back for init stage*/
		emit("IMU about to start");
		time_next = millis();
		while(true) {
			IMUinterface::Read();
			/* Critical *//* Critical */
				access.lock();
				apot.Gather(data_a(0),data_a(1),data_a(2));
				gpot.Gather(data_g(0),data_g(1),data_g(2));
				access.unlock();
			/* Critical *//* Critical */
			Set_Data_Valid(true);
	        while( !timer->Allow() ) {
	        	if( Disposed() ) return 0;
	            usleep(1000);
	        }
	    }
	}