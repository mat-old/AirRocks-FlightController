/*
*  Inertia Measurement Unit Worker, does the constant gathering of IMU data 
*  this class provides the required higher level data for the PIDctrl
*/
#include "arfcDefines.hpp"
#include "imuInterface.hpp"
#include "dataTypes.hpp"

//#define IMUW_DEBUG
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
		if( !fail_flag ) {
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
		if( Data_Valid() && access.try_lock()) {
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
			std::cout << "\r> IMUworker about to start...\n" << std::flush;
		#endif

		quaternion rotation = quaternion::Identity();
		time_next = Def::millis();
		while(true) {
			access.lock();
			
			//IMUinterface::Read();
			time_last = time_next;
			time_next = Def::millis();
			/* time derivative */
			float time_derivative = ( time_next - time_last ) / 1000.0f;

			vector gyro       = readGyro();
			vector accel      = readAcc();
			vector field      = readMag();
			vector correction = vector(0, 0, 0);

			if( abs(accel.norm() - 1) <= 0.3 ) {

				matrix rc = rotationFromCompass(accel, field);
				matrix rm  = rotation.toRotationMatrix();

				correction = (
					  rc.row(0).cross(rm.row(0))
					+ rc.row(1).cross(rm.row(1))
					+ rc.row(2).cross(rm.row(2))
				) * Def::imuCorrection;
			}
			rotate( rotation, gyro + correction, time_derivative );

			vector euler = rotation.toRotationMatrix().eulerAngles(2, 1, 0) * (180 / M_PI);

			apot.Gather(accel);
			gpot.Gather(gyro);
			
			access.unlock();
			Set_Data_Valid(true);

	        //while(Def::millis() - time_next < 20) {
			usleep(Def::imuBAUD_RATE);
	        if( dispose ) return 0;
	        //    usleep(1000);
	        //}
	    }

	}
};

#endif




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
				, mag_max
				, raw_m
				, raw_a
				, raw_g;
	vector        gyro_offset;

	bool fenabled, fopened, foffsets, fprepared;
public:
	vector		  data_m
				, data_a
				, data_g
				, euler_reading;
	int 		  time_next
				, time_last;

	IMUinterface() : SubSystem() {
		fenabled = fopened = foffsets = fprepared = false;
	}
	~IMUinterface() {
		delete compass;
		delete gyro;
	}
	/* load calibration & enable */
	IMUinterface& Open() { 
		if( !this->fopened ) {
			try {
				compass = new LSM303(Def::i2c_device);
				gyro    = new L3G(Def::i2c_device);
			} catch( errCodes e ) {
				err.Response(e);
			}

			wordexp_t path;
			wordexp(Def::imu_calibration, &path, 0);
			#ifdef  IMU_DEBUG
				std::cout<<"> Opening Calibration "<<path.we_wordv[0]<<std::endl;
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

			dev.close();

			this->fopened = true;
		}
		return *this;
	}

	IMUinterface& Enable() {
		if( !this->fenabled ) {
			compass->enable();
			gyro->enable();
			this->fenabled = true;
		}
		return *this;
	}
	IMUinterface& MeasureOffsets() {
		if( !this->foffsets ) {
			gyro_offset = vector::Zero();
			const int sampleCount = 32;
			for(int i = 0; i < sampleCount; i++)
			{
				gyro->read();
				gyro_offset += vector_from_ints(&gyro->g);
				usleep(20*1000);
			}
			gyro_offset /= sampleCount;
		}
		return *this;
	}

	IMUinterface& Prepare() {
		if(  !this->fprepared ) {
			this->Open().Enable().MeasureOffsets();
			this->time_next = Def::millis();
			this->fprepared = true;
		}
		return *this;
	}

	inline void Read() {
/*		vector correction = vector( 0, 0, 0 );
		float time_derivative;

		time_last = time_next;
		time_next = Def::millis();

		time_derivative = ( time_next - time_last ) / 1000.0f;

		gyro_data = ReadGyro();
		accl_data = ReadAccl();
		magn_data = ReadMagn();

		if( abs(accl_data.norm() - 1) <= 0.3f ) {

			matrix rc = RotationFromCompass(accl_data, magn_data);
			matrix rm = rotation.toRotationMatrix();

			correction = (
				  rc.row(0).cross(rm.row(0))
				+ rc.row(1).cross(rm.row(1))
				+ rc.row(2).cross(rm.row(2))
			) * Def::imuCorrection;
		}

		Rotate( rotation, gyro_data + correction, time_derivative );

		euler_reading = rotation.toRotationMatrix().eulerAngles(2,1,0) * (180 / M_PI);*/
	}

	/* ahrs methods */
	void rotate(quaternion& rotation, const vector& w, float dt) {
		rotation *= quaternion( 1, w(0)*dt/2, w(1)*dt/2, w(2)*dt/2 );
		rotation.normalize();
	}
	matrix rotationFromCompass(const vector& acceleration, const vector& magnetic_field) {
		vector down = -acceleration;     // usually true
		vector east = down.cross(magnetic_field); // actually it's magnetic east
		vector north = east.cross(down);

		east.normalize();
		north.normalize();
		down.normalize();

		matrix r;
		r.row(0) = north;
		r.row(1) = east;
		r.row(2) = down;
		return r;
	}

	/* read methods */
	vector readMag() {
		compass->readMag();
		raw_m = int_vector_from_ints(&compass->m);
		
		vector v;
		v(0) = (float)(compass->m[0] - mag_min(0)) / (mag_max(0) - mag_min(0)) * 2 - 1;
		v(1) = (float)(compass->m[1] - mag_min(1)) / (mag_max(1) - mag_min(1)) * 2 - 1;
		v(2) = (float)(compass->m[2] - mag_min(2)) / (mag_max(2) - mag_min(2)) * 2 - 1;
		return v;
	}
	vector readAcc() {
		//const float accel_scale = 0.000244;

		compass->readAcc();
		raw_a = int_vector_from_ints(&compass->a);
		return vector_from_ints(&compass->a) * Def::imuAccel_Scale;
	}
	vector readGyro() {
		gyro->read();
		raw_g = int_vector_from_ints(&gyro->g);
		return ( vector_from_ints(&gyro->g) - gyro_offset ) * Def::imuGyro_Scale;
	}

};

#endif