#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <ncurses.h>

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
void output_euler(quaternion &, vector&, vector&);
void loop9DOF();
int millis();

int main(int argc, char const *argv[])
{
	
	loop9DOF();

	return 0;
}

void rotate(quaternion& rotation, const vector& w, float dt)
{
	// Multiply by first order approximation of the
	// quaternion representing this rotation.
	rotation *= quaternion(1, w(0)*dt/2, w(1)*dt/2, w(2)*dt/2);
	rotation.normalize();
}

void vectorLine(vector & v)
{
	float  a = v(0)
		 , b = v(1)
		 , c = v(2);

	printw("%s%0.3f%s%0.3f%s%0.3f      ",(a<0?" ":"  "),a,(b<0?" ":"  "),b,(c<0?" ":"  "),c); 
}
void vectorLineCompass(vector & v)
{
	float  a = v(0)
		 , b = v(1)
		 , c = v(2);

	printw("%0.2lf\t%0.2lf\t%0.2lf\n  ",a,b,c);
}

void loop9DOF() {
	int time_next, time_last;

	MinIMU9 imu(I2CDEVICE);

	imu.loadCalibration().enable().measureOffsets();

	quaternion rotation = quaternion::Identity();

	/* start session */
	initscr();
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

		vector euler = rotation.toRotationMatrix().eulerAngles(2, 1, 0) * (180 / M_PI);

		move(5,4);
		printw("Gyroscope");
		move(5,15);		
		vectorLine(gyro);
		move(6,4);
		printw("Accel");
		move(6,15);
		vectorLine(accel);
		move(8,4);
		printw("Compass");
		move(8,15);
		printw("NORTH\tEAST\tDOWN\n");		
		move(9,15);
		vectorLineCompass(euler);
		//std::cout << "\rGyro[" << gyro << "]\tAccel[" << accel << "]" << std::flush;
		refresh();


		/* original is better, though this is fine */
		usleep(20*1000);
	}
	endwin(); /* end session */
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