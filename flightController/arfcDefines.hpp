#include <stdint.h>
#ifndef ARFDEF
#define ARFDEF
namespace Def {
	/* IMU Interface */
	const useconds_t imuBAUD_RATE  = 20000u;
	const float field_width        = 6.0f;
	const float imuSample_Count    = 32.0f;
	const float imuAccel_Scale     = 0.000244;
	const float imuGyro_Scale      = 0.00122173; //0.07*Pi/180.0
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
#endif