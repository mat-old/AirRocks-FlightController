/*
*  Inertia Measurement Unit Interface
*  base class for IMU interface based on 
*  David E Grayson https://github.com/DavidEGrayson/minimu9-ahrs
*  All sources in this file and in the ARHS directory are 
*  Note less code will be adapted as development continues. 
*  The basic driving code for reading the Pololu-MinIMU-9 will remain the same
*/
#ifndef IMUINTERFACE
#define IMUINTERFACE
#include "../Includes.hpp"

#include "../ARHS/exceptions.h"
#include "../ARHS/vector.h"
#include "../ARHS/LSM303.h"
#include "../ARHS/L3G.h"
#include "../ARHS/IMU.h"

#include <sys/time.h>
#include <wordexp.h>
#include <fstream>
#include <time.h>

class IMUinterface : public AsyncWorker {
private:
	LSM303 * compass;
	L3G    * gyro;

	int_vector    min_magn
				, max_magn
				, raw_magn
				, raw_accl
				, raw_gyro;
	vector        gyro_offset;

	bool fenabled, fopened, foffsets, fprepared;
public:
	quaternion    rotation;
	vector		  data_m
				, data_a
				, data_g
				, data_e;
	int64_t       time_next
				, time_last;

	IMUinterface() : AsyncWorker() {
		fenabled = fopened = foffsets = fprepared = false;
		if( !IMU_ENABLED ) Disable();
	}
	~IMUinterface() {
		delete compass;
		delete gyro;
	}
	/* load calibration & enable */
	IMUinterface& Open() { 
		if( !this->fopened ) {
			try {
				compass = new LSM303(Defines::i2c_device);
				gyro    = new L3G(Defines::i2c_device);
			} catch( ERR_CODES e ) {
				err.Response(e);
			}

			wordexp_t path;
			wordexp(Defines::imu_calibration, &path, 0);
			emit("IMU Calibration loaded");
			/*TODO: report calibration success*/
			std::ifstream dev(path.we_wordv[0]);
			if( dev.bad() )
				throw FAIL_I2C_CAL_OPEN;

			dev >> min_magn(0)
				>> max_magn(0)
				>> min_magn(1)
				>> max_magn(1)
				>> min_magn(2)
				>> max_magn(2);

			if( dev.fail() || dev.bad() )
				throw FAIL_I2C_CAL_READ;

			dev.close();

			this->fopened = true;
		}
		Set_Active(true);
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
			for (int i = 0; i < Defines::imuSample_Count; ++i) {
				gyro->read();
				gyro_offset += vector_from_ints(&gyro->g);
				usleep(Defines::imuBAUD_RATE);
			}
			gyro_offset /= Defines::imuSample_Count;
			this->foffsets = true;
		}
		return *this;
	}

	IMUinterface& Prepare() {
		if(  !this->fprepared ) {
			this->Open().Enable().MeasureOffsets();
			rotation = quaternion::Identity();
			this->time_next = millis();
			this->fprepared = true;
		}
		return *this;
	}

	inline void Read() {
		vector correction = vector( 0, 0, 0 );
		float time_derivative;

		time_last = time_next;
		time_next = millis();

		time_derivative = ( time_next - time_last ) / 1000.0f;

		data_g = ReadGyro();
		data_a = ReadAccl();
		data_m = ReadMagn();

		if( abs(data_a.norm() - 1) <= 0.3f ) {

			matrix rc = RotationFromCompass(data_a, data_m);
			matrix rm = rotation.toRotationMatrix();

			correction = (
				  rc.row(0).cross(rm.row(0))
				+ rc.row(1).cross(rm.row(1))
				+ rc.row(2).cross(rm.row(2))
			) * Defines::imuCorrection;
		}

		Rotate( rotation, data_g + correction, time_derivative );

		data_e = rotation.toRotationMatrix().eulerAngles(2,1,0) * (180 / M_PI);
	}

	/* ahrs methods */
	void Rotate(quaternion& rotation, const vector& w, float dt) {
		rotation *= quaternion( 1, w(0)*dt/2, w(1)*dt/2, w(2)*dt/2 );
		rotation.normalize();
	}
	matrix RotationFromCompass(const vector& acceleration, const vector& magnetic_field ) {
		matrix R;
		vector down  = -acceleration;
		vector east  = down.cross(magnetic_field);
		vector north = east.cross(down);

		east.normalize();
		north.normalize();
		down.normalize();

		R.row(0) = north;
		R.row(1) = east;
		R.row(2) = down;
		return R;
	}

	/* read methods */
	vector ReadMagn() {
		compass->readMag();
		raw_magn = int_vector_from_ints(&compass->m);

		vector v;
		v(0) = (float)(compass->m[0] - min_magn(0)) / (max_magn(0) - min_magn(0)) * 2 - 1;
		v(1) = (float)(compass->m[1] - min_magn(1)) / (max_magn(1) - min_magn(1)) * 2 - 1;
		v(2) = (float)(compass->m[2] - min_magn(2)) / (max_magn(2) - min_magn(2)) * 2 - 1;
		return v;
	}
	vector ReadAccl() {
		compass->read();
		raw_accl = int_vector_from_ints(&compass->a);
		return ( vector_from_ints(&compass->a) ) * Defines::imuAccel_Scale;
	}
	vector ReadGyro() {
		gyro->read();
		raw_gyro = int_vector_from_ints(&gyro->g);
		return ( vector_from_ints(&gyro->g) - gyro_offset ) * Defines::imuGyro_Scale;
	}

};
#endif