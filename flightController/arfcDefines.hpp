#include <stdint.h>
#ifndef ARFDEF
#define ARFDEF
namespace Def {
	const uint8_t MOTOR_ZERO_LEVEL = 125u;
	const uint8_t MOTOR_ARM_START  = 140u;
	const uint8_t MOTOR_MAX_LEVEL  = 254u;
	const useconds_t ioBAUD_RATE   = 500000; // .5 MHZ transmission
	const uint8_t ioDelay          = 0u;
	const uint8_t ioLength         = 8u;
	const uint8_t ioBits           = 8u;
	const uint8_t ioFlag_Start     = 0xA;
	const uint8_t ioFlag_End       = 0xB;
	const uint8_t ioMsg_Length     = 4u;
	const uint8_t ioMsg_Offset     = 1u;
	const char *spi_device = "/dev/spidev0.1";


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