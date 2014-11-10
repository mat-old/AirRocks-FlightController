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
#include "../Defines.hpp"
#include "../Cores/AsyncWorker.hpp"

#include "../ARHS/exceptions.h"
#include "../ARHS/vector.h"
#include "../ARHS/LSM303.h"
#include "../ARHS/L3G.h"
#include "../ARHS/IMU.h"

#include <wordexp.h>
#include <fstream>

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

	IMUinterface();
	~IMUinterface();
	/* load calibration & enable */
	IMUinterface& Open();
	IMUinterface& Enable();
	IMUinterface& MeasureOffsets();

	IMUinterface& Prepare();
	void Read();
	/* ahrs methods */
	void Rotate(quaternion& rotation, const vector& w, float dt);
	matrix RotationFromCompass(const vector& acceleration, const vector& magnetic_field );

	/* read methods */
	vector ReadMagn();
	vector ReadAccl();
	vector ReadGyro();


};
#endif