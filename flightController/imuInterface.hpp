/*
*  Inertia Measurement Unit Interface
*  base class for IMU interface based on 
*  David E Grayson https://github.com/DavidEGrayson/minimu9-ahrs
*  All sources in this file and in the ARHS directory are 
*  Note less code will be adapted as development continues. 
*  The basic driving code for reading the Pololu-MinIMU-9 will remain the same
*/
#include "arfcDefines.hpp"
#include "subSystem.hpp"

#include "ARHS/exceptions.h"
#include "ARHS/vector.h"
#include "ARHS/LSM303.h"
#include "ARHS/L3G.h"
#include "ARHS/IMU.h"

#include <sys/time.h>
#include <wordexp.h>
#include <fstream>
#include <time.h>

#define IMU_DEBUG
#ifdef  IMU_DEBUG
	#include <stdio.h>
	#include <iostream>
#endif
#ifndef IMUINTERFACE
#define IMUINTERFACE
class IMUinterface : public SubSystem {
private:
	LSM303 * compass;
	L3G    * gyro;

	int_vector    mag_min
				, mag_max;
public:
	IMUinterface() : SubSystem() {}
	~IMUinterface() {
		delete compass;
		delete gyro;
	}
	/* load calibration & enable */
	IMUinterface& Open() { 
		try {
			compass = new LSM303(Def::i2c_device);
			gyro    = new L3G(Def::i2c_device);
		} catch( errCodes e ) {
			err.Response(e);
		}

		wordexp_t path;
		wordexp(Def::imu_calibration, &path, 0);
		#ifdef  IMU_DEBUG
			std::cout<<"Opening Calibration "<<path.we_wordv[0]<<std::endl;
		#endif
		std::ifstream dev(path.we_wordv[0]);
		if( dev.bad() )
			throw FAIL_I2C_CAL_OPEN;

		dev >> mag_min(0)
			>> mag_max(0)
			>> mag_min(1)
			>> mag_max(1)
			>> mag_min(2)
			>> mag_max(2);

		if( dev.fail() || dev.bad() )
			throw FAIL_I2C_CAL_READ;

		return *this;
	}

	IMUinterface& Enable() {
		compass->enable();
		gyro->enable();
		return *this;
	}

};

#endif