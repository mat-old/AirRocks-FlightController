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
/*for all constant definitions*/
#ifndef DEFINES
#define DEFINES
#include <sys/time.h>
#include <stdint.h>
#include <iomanip>
#include <time.h>
#define ATOMIC volatile bool // i know its the opposite
#define FLOAT_FORMAT std::fixed << std::setprecision(3) << std::setw(6)
#define pid_t float // pid_t, i know...

const bool IMU_ENABLED = true;
const bool SPI_ENABLED = true;
const bool PID_ENABLED = true;

namespace Defines {
	/* General */
	const int   STD_DELAY_MS       = 20;
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
	const size_t  MOTORS 		   = 4; 
	const uint8_t MOTOR_ZERO_LEVEL = 125u;
	const uint8_t MOTOR_ARM_START  = 140u;
	const uint8_t MOTOR_MAX_LEVEL  = 254u;
	const uint8_t THROTTLE_MAX     = MOTOR_MAX_LEVEL - MOTOR_ARM_START;
	const uint8_t PID_RESERVED     = 20u;
	const pid_t   PID_RATIO        = 0.2f;
	const uint8_t USER_LIMIT       = THROTTLE_MAX - PID_RESERVED;

	/* SPI Worker */
	const useconds_t ioBAUD_RATE   = 500000; // .5 MHZ transmission
	const uint8_t ioDelay          = 0u;
	const uint8_t ioLength         = 8u;
	const uint8_t ioBits           = 8u;
	const uint8_t ioFlag_Start     = 0xA;
	const uint8_t ioFlag_End       = 0xB;
	const uint8_t ioMsg_Length     = 4u;
	const uint8_t ioMsg_Offset     = 1u;
	const char   *spi_device       = "/dev/spidev0.1";

	/* PIDctrl */
	const pid_t pitch_zero = 0.035f;
	const pid_t roll_zero  = 0.022f;
	const pid_t yaw_zero   = 1.020f;

	/* X */
	const pid_t pitch_P    = 0.8f;
	const pid_t pitch_I    = 0.0f;
	const pid_t pitch_D    = 0.0f;
	const pid_t pitch_MIN  = -1.0f; 
	const pid_t pitch_MAX  = +1.0f;  
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

	/* Error Codes */
	typedef enum {
		  BAD_IO
		, FAIL_SET_SPI
		, FAIL_GET_SPI
		, FAIL_SET_BIT
		, FAIL_GET_BIT
		, FAIL_SET_SPEED
		, FAIL_GET_SPEED
		, FAIL_START_WORKER
		, FAIL_FLAG_SET
		, FAIL_I2C_PERM
		, FAIL_I2C_DEV
		, FAIL_I2C_WRITE
		, FAIL_I2C_READ
		, FAIL_I2C_BLOCK
		, FAIL_I2C_CAL_OPEN
		, FAIL_I2C_CAL_READ
		, UNREACHABLE
		, SHUTDOWN
		, ERR_ANY = 0xFFFF  /* catch all code */
	} ERR_CODES;

}
#endif