#include <stdint.h>
#ifndef ARFDEF
#define ARFDEF
namespace Def {
	const uint8_t MOTOR_ZERO_LEVEL = 125;
	const uint8_t MOTOR_ARM_START  = 140;
	const uint8_t MOTOR_MAX_LEVEL  = 254;
	const uint8_t ioBAUD_RATE      = 500000; // .5 MHZ transmission
	const uint8_t ioDelay          = 0;
	const uint8_t ioLength         = 8;
	const uint8_t ioFlag_Start     = 0xA;
	const uint8_t ioFlag_End       = 0xB;
	const uint8_t ioMsg_Size       = 4;
	const char *spi_device = "/dev/spidev0.1";
}
#endif