#include <iostream>
#include <sys/time.h>
#include <time.h>

#include "IMUStablizer.hpp"
#include "include/vector.h"

#define DEBUG
#define I2CDEVICE "/dev/i2c-1"

/******
* driver code from David E Grayson https://github.com/DavidEGrayson/minimu9-ahrs
* this is maybe the most important sub system as it keeps us in the air and in one piece. 
* sudo minimu9-ahrs --output euler -b /dev/i2c-1 
*
*/

void read9DOF(quaternion& , float , const vector& , const vector& , const vector& );
matrix rotationFromCompass(const vector&, const vector&);
void Irotate(quaternion&, const vector&, float);
void output_euler(quaternion &, vector&, vector&);
void loop6DOF();
void loop9DOF();
int millis();

int main(int argc, char const *argv[])
{
	
	loop6DOF();
	//loop9DOF();

	return 0;
}

void rotate(quaternion& rotation, const vector& w, float dt)
{
	// Multiply by first order approximation of the
	// quaternion representing this rotation.
	rotation *= quaternion(1, w(0)*dt/2, w(1)*dt/2, w(2)*dt/2);
	rotation.normalize();
}

void loop6DOF() {
	int time_next, time_last;

	MinIMU9 imu(I2CDEVICE);

	imu.loadCalibration().enable().measureOffsets();

	quaternion rotation = quaternion::Identity();

	time_next = millis();
	while(true) {
		time_last = time_next;
		time_next = millis();
		/* time derivative */
		float time_derivative = ( time_next - time_last ) / 1000.0f;

		vector gyro       = imu.readGyro();
		vector accel      = imu.readAcc();
		vector field      = imu.readMag();
		vector correction = vector(0, 0, 0);

		if( abs(accel.norm() - 1) <= 0.3 ) {

			matrix rc = rotationFromCompass(accel, field);
			matrix rm  = rotation.toRotationMatrix();

			correction = (
				  rc.row(0).cross(rm.row(0))
				+ rc.row(1).cross(rm.row(1))
				+ rc.row(2).cross(rm.row(2))
			) * 1.0f;


		}

		/* rotate */
		rotate( rotation, gyro + correction, time_derivative );
		std::cout << "\rGyro[" << gyro << "]\tAccel[" << accel << "]" << std::flush;

		/* original is better, though this is fine */
		usleep(20*1000);
	}

}


void loop9DOF() {
	MinIMU9 imu(I2CDEVICE);

	imu.loadCalibration();
	imu.enable();
	imu.measureOffsets();

	quaternion rotation = quaternion::Identity();
	int start = millis();
	while(1)
	{
		int last_start = start;
		start = millis();
		float dt = (start-last_start)/1000.0;
		if (dt < 0){ throw std::runtime_error("Time went backwards."); }

		vector angular_velocity = imu.readGyro();
		vector acceleration     = imu.readAcc();
		vector magnetic_field   = imu.readMag();

		read9DOF(rotation, dt, angular_velocity, acceleration, magnetic_field);

		output_euler(rotation, acceleration, magnetic_field);
		//std::cout << "  " << acceleration << "  " << magnetic_field << std::endl << std::flush;

		// Ensure that each iteration of the loop takes at least 20 ms.
		while(millis() - start < 20)
		{
			usleep(1000);
		}
	}
}

void output_euler(quaternion & rot, vector& accel, vector& magf)
{
    std::cout << '\r'
    		  << "R[" << (vector)(rot.toRotationMatrix().eulerAngles(2, 1, 0) * (180 / M_PI))
    		  << "]A[" << accel 
    		  << "]M[" << magf << "]"
    		  << std::flush;
}
int millis()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000 + (tv.tv_usec)/1000;
}

void read9DOF(
	  quaternion& rotation
	, float dt
	, const vector& angular_velocity
	, const vector& acceleration
	, const vector& magnetic_field
) {
	vector correction = vector(0, 0, 0);

	if (abs(acceleration.norm() - 1) <= 0.3)
	{
		// The magnetidude of acceleration is close to 1 g, so
		// it might be pointing up and we can do drift correction.
		const float correction_strength = 1;

		matrix rotationCompass = rotationFromCompass(acceleration, magnetic_field);
		matrix rotationMatrix = rotation.toRotationMatrix();

		correction = (
			rotationCompass.row(0).cross(rotationMatrix.row(0)) +
			rotationCompass.row(1).cross(rotationMatrix.row(1)) +
			rotationCompass.row(2).cross(rotationMatrix.row(2))
		  ) * correction_strength;

	}

	Irotate(rotation, angular_velocity + correction, dt);
}
void Irotate(quaternion& rotation, const vector& w, float dt)
{
	// Multiply by first order approximation of the
	// quaternion representing this rotation.
	rotation *= quaternion(1, w(0)*dt/2, w(1)*dt/2, w(2)*dt/2);
	rotation.normalize();
}
matrix rotationFromCompass(const vector& acceleration, const vector& magnetic_field)
{
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