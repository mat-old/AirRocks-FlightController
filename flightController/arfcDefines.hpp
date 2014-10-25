#include <sys/time.h>
#include <stdint.h>
#include <iomanip>
#include <time.h>
#ifndef ARFDEF
#define ARFDEF
#define ATOMIC volatile bool
#define FLOAT_FORMAT std::fixed << std::setprecision(3) << std::setw(6)
/* i knew it was a reserved type when i used it */
#define pid_t float
#define angel_t pid_t
const bool IMU_ENABLED = true;
const bool SPI_ENABLED = true;
const bool PID_ENABLED = true;

/* blades
*	A  CW    -x axis
*	B  CCW   -y axis
*	C  CW    +x axis
*	D  CCW   +y axis
*	-yaw     turn right
*   +yaw     turn left
*   +roll    roll right (if looking at back)
*   -roll    roll left  (if looking at back)
*   +pitch   tilt forward
*   -pitch   tilt backward
 */


namespace Def {
	/* IMU Interface */
	const useconds_t imuBAUD_RATE  = 20000u;
	const float field_width        = 6.0f;
	const float imuCorrection      = 1.0f;
	const float imuSample_Count    = 32.0f;
	const float imuAccel_Scale     = 0.000244;
	const float imuGyro_Scale      = 0.07f * 3.14159265f / 180.0f;//0.00122173; //0.07*Pi/180.0
	const char *imu_calibration    = "~/.minimu9-ahrs-cal";
	const char *i2c_device         = "/dev/i2c-1";

	/* Motor Const */ 
	const uint8_t MOTOR_ZERO_LEVEL = 125u;
	const uint8_t MOTOR_ARM_START  = 140u;
	const uint8_t MOTOR_MAX_LEVEL  = 254u;
	/* SPI Worker */
	const useconds_t ioBAUD_RATE   = 500000; // .5 MHZ transmission
	const uint8_t ioDelay          = 0u;
	const uint8_t ioLength         = 8u;
	const uint8_t ioBits           = 8u;
	const uint8_t ioFlag_Start     = 0xA;
	const uint8_t ioFlag_End       = 0xB;
	const uint8_t ioMsg_Length     = 4u;
	const uint8_t ioMsg_Offset     = 1u;
	const char *spi_device         = "/dev/spidev0.1";

	/* PIDctrl */
	const pid_t pitch_zero = 0.035f;
	const pid_t roll_zero  = 0.022f;
	const pid_t yaw_zero   = 1.020f;

	/* X */
	const pid_t pitch_P    = 1.0f;
	const pid_t pitch_I    = 0.0f;
	const pid_t pitch_D    = 0.0f;
	const pid_t pitch_MIN  = -1.0f; /* roughly 45degrees forward*/
	const pid_t pitch_MAX  = +1.0f; /* roughly 45degrees backward*/  
	/* Y */
	const pid_t roll_P     = 0;
	const pid_t roll_I     = 0;
	const pid_t roll_D     = 0;
	const pid_t roll_MIN   = -1.0f; /* tilt right */
	const pid_t roll_MAX   = +1.0f; /* tilt left */
	/* Z */
	const pid_t yaw_P      = 0;
	const pid_t yaw_I      = 0;
	const pid_t yaw_D      = 0;
	const pid_t yaw_MIN    = 0; /* tilt and axis */
	const pid_t yaw_MAX    = 1.030f; /* level */

	/* angel values from resting IMU accel */
	const pid_t x_P        = 0.043;
	const pid_t x_I        = 0.0f;
	const pid_t x_D        = 0.0f;
	const pid_t x_MIN      = 0.0f;
	const pid_t x_MAX      = 0.0f;

	const pid_t y_P        = 0.011;
	const pid_t y_I        = 0.0f;
	const pid_t y_D        = 0.0f;
	const pid_t y_MIN      = 0.0f;
	const pid_t y_MAX      = 0.0f;




	inline uint8_t MOTOR_SAFE_SPEED(uint8_t s) {
		if( s > MOTOR_MAX_LEVEL  ) return MOTOR_MAX_LEVEL;
		else if( s < MOTOR_ZERO_LEVEL ) return MOTOR_ZERO_LEVEL;
		else return s;
	}

	const uint8_t InitialMotorState[ioLength] = {
		ioFlag_Start,
		MOTOR_ZERO_LEVEL,
		MOTOR_ZERO_LEVEL,
		MOTOR_ZERO_LEVEL,
		MOTOR_ZERO_LEVEL,
		0x0,	/* optional flag */
		0x0,	/* optional flag */
		ioFlag_End
	};
}

unsigned long millis() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}
#endif