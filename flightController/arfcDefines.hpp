#include <stdint.h>
#ifndef ARFDEF
#define ARFDEF
namespace Def {
	const uint8_t MOTOR_ZERO_LEVEL = 125u;
	const uint8_t MOTOR_ARM_START  = 140u;
	const uint8_t MOTOR_MAX_LEVEL  = 254u;
	const uint32_t ioBAUD_RATE     = 500000u; // .5 MHZ transmission
	const uint8_t ioDelay          = 0u;
	const uint8_t ioLength         = 8u;
	const uint8_t ioFlag_Start     = 0xA;
	const uint8_t ioFlag_End       = 0xB;
	const uint8_t ioMsg_Length     = 4u;
	const uint8_t ioMsg_Offset     = 1u;
	const char *spi_device = "/dev/spidev0.1";
}
#endif