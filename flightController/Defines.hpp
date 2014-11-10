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
//#include <iomanip>
#include <ctime>

#define ATOMIC volatile bool // i know its the opposite
//#define FLOAT_FORMAT std::fixed << std::setprecision(3) << std::setw(6)
//#define pid_t float // pid_t, i know...
#define var_float_t float // start using this for floats



typedef enum {
	NO_MODE, TEST_MODE, TUNE_MODE, UAV_MODE
} DRONE_MODES;


#ifndef GLOBALVARS
#define GLOBALVARS
namespace global {

	const bool IMU_ENABLED = true;
	const bool SPI_ENABLED = true;
	const bool PID_ENABLED = true;
};
#endif

/* General */
#define   STD_DELAY_MS      20
/* IMU Interface */
#define imuBAUD_RATE        20000u
#define field_width         6.0f
#define imuCorrection       1.0f
#define imuSample_Count     32.0f
#define imuAccel_Scale      0.000244
#define imuGyro_Scale       0.07f * 3.14159265f / 180.0f  //0.00122173; //0.07*Pi/180.0
#define imu_calibration     "~/.minimu9-ahrs-cal"
#define i2c_device          "/dev/i2c-1"

/* Motor Const */
#define MOTORS 	          4
#define MOTOR_ZERO_LEVEL  125u
#define MOTOR_ARM_START   140u
#define MOTOR_MAX_LEVEL   254u
#define THROTTLE_MAX      MOTOR_MAX_LEVEL - MOTOR_ARM_START
#define PID_RESERVED      20u
#define   PID_RATIO       0.2f
#define USER_LIMIT        THROTTLE_MAX - PID_RESERVED

/* SPI Worker */
#define ioBAUD_RATE    500000 // .5 MHZ transmission
#define ioDelay        0u
#define ioLength       8u
#define ioBits         8u
#define ioFlag_Start   0xA
#define ioFlag_End     0xB
#define ioMsg_Length   4u
#define ioMsg_Offset   1u
#define   spi_device   "/dev/spidev0.1"

/* PIDctrl */
#define pitch_zero  0.035f
#define roll_zero   0.022f
#define yaw_zero    1.020f

/* X */
#define pitch_P     0.8f
#define pitch_I     0.0f
#define pitch_D     0.0f
#define pitch_MIN   -1.0f 
#define pitch_MAX   +1.0f  
/* Y */
#define roll_P      0
#define roll_I      0
#define roll_D      0
#define roll_MIN    -1.0f /* tilt right */
#define roll_MAX    +1.0f /* tilt left */
/* Z */
#define yaw_P       0
#define yaw_I       0
#define yaw_D       0
#define yaw_MIN     0 /* tilt and axis */
#define yaw_MAX     1.030f /* level */

/* angel values from resting IMU accel */
#define x_P         0.043
#define x_I         0.0f
#define x_D         0.0f
#define x_MIN       0.0f
#define x_MAX       0.0f

#define y_P         0.011
#define y_I         0.0f
#define y_D         0.0f
#define y_MIN       0.0f
#define y_MAX       0.0f

static const uint8_t InitialMotorState[ioLength] = {
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
	, IMU_BAD_CONNECT
	, ERR_ANY = 0xFFFF  /* catch all code */
} ERR_CODES;


inline char MOTOR_SAFE_SPEED(char s) {
	if( s > MOTOR_MAX_LEVEL  )
		return MOTOR_MAX_LEVEL;
	else if( s < MOTOR_ZERO_LEVEL )
		return MOTOR_ZERO_LEVEL;
	else return s;
}
inline unsigned long millis() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}



#endif