#ifndef _MINIMU9_H
#define _MINIMU9_H

//#include "include/includes.hpp"


#include "include/IMU.h"
#include "include/LSM303.h"
#include "include/L3G.h"
#include "include/vector.h"
#include "include/exceptions.h"

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <wordexp.h>
#include <iomanip>				// cout formatting

float field_width = 6;

#define FLOAT_FORMAT std::fixed << std::setprecision(3) << std::setw(field_width)
std::ostream & operator << (std::ostream & os, const vector & vector)
{
    return os << FLOAT_FORMAT << vector(0) << ' '
              << FLOAT_FORMAT << vector(1) << ' '
              << FLOAT_FORMAT << vector(2);
}

std::ostream & operator << (std::ostream & os, const matrix & matrix)
{
    return os << (vector)matrix.row(0) << ' '
              << (vector)matrix.row(1) << ' '
              << (vector)matrix.row(2);
}

std::ostream & operator << (std::ostream & os, const quaternion & quat)
{
    return os << FLOAT_FORMAT << quat.w() << ' '
              << FLOAT_FORMAT << quat.x() << ' '
              << FLOAT_FORMAT << quat.y() << ' '
              << FLOAT_FORMAT << quat.z();
}



class MinIMU9 : public IMU {
public:
	LSM303 compass;
	L3G gyro;


	MinIMU9(const char * i2cDeviceName) : compass(i2cDeviceName), gyro(i2cDeviceName)
	{
	}

	MinIMU9& enable()
	{
		compass.enable();
		gyro.enable();
		return *this;
	}

	MinIMU9& loadCalibration()
	{
		wordexp_t expansion_result;
		wordexp("~/.minimu9-ahrs-cal", &expansion_result, 0);

		std::ifstream file(expansion_result.we_wordv[0]);
		if (file.fail())
		{
			throw posix_error("Failed to open calibration file ~/.minimu9-ahrs-cal.");
		}
		
		file >> mag_min(0) >> mag_max(0) >> mag_min(1) >> mag_max(1) >> mag_min(2) >> mag_max(2);
		if (file.fail() || file.bad())
		{
			throw std::runtime_error("Failed to parse calibration file ~/.minimu9-ahrs-cal.");
		}
		return *this;
	}

	MinIMU9& measureOffsets()
	{
		// LSM303 accelerometer: 8 g sensitivity.  3.8 mg/digit; 1 g = 256.
		// TODO: unify this with the other place in the code where we scale accelerometer readings.
		gyro_offset = vector::Zero();
		const int sampleCount = 32;
		for(int i = 0; i < sampleCount; i++)
		{
			gyro.read();
			gyro_offset += vector_from_ints(&gyro.g);
			usleep(20*1000);
		}
		gyro_offset /= sampleCount;
		return *this;
	}

	vector readMag()
	{
		compass.readMag();
		IMU::raw_m = int_vector_from_ints(&compass.m);
		
		vector v;
		v(0) = (float)(compass.m[0] - mag_min(0)) / (mag_max(0) - mag_min(0)) * 2 - 1;
		v(1) = (float)(compass.m[1] - mag_min(1)) / (mag_max(1) - mag_min(1)) * 2 - 1;
		v(2) = (float)(compass.m[2] - mag_min(2)) / (mag_max(2) - mag_min(2)) * 2 - 1;
		return v;
	}

	vector readAcc()
	{
		// Info about linear acceleration sensitivity from datasheets:
		// LSM303DLM: at FS = 8 g, 3.9 mg/digit (12-bit reading)
		// LSM303DLHC: at FS = 8 g, 4 mg/digit (12-bit reading probably an approximation)
		// LSM303DLH: at FS = 8 g, 3.9 mg/digit (12-bit reading)
		// LSM303D: at FS = 8 g, 0.244 mg/LSB (16-bit reading)
		const float accel_scale = 0.000244;

		compass.readAcc();
		IMU::raw_a = int_vector_from_ints(&compass.a);
		return vector_from_ints(&compass.a) * accel_scale;
	}

	vector readGyro()
	{
		// Info about sensitivity from datasheets:
		// L3G4200D: at FS = 2000 dps, 70 mdps/digit
		// L3GD20: at FS = 2000 dps, 70 mdps/digit
		// L3GD20H: at FS = 2000 dps, 70 mdps/digit
		const float gyro_scale = 0.07 * 3.14159265 / 180;

		gyro.read();
		IMU::raw_g = int_vector_from_ints(&gyro.g);
		return ( vector_from_ints(&gyro.g) - gyro_offset ) * gyro_scale;
	}
};
#endif