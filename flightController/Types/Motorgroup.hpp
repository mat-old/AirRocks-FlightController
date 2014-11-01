#ifndef MOTORGROUP
#define MOTORGROUP

#include "../Includes.hpp"

//#include "../Types/Types.hpp"
//#include "../Defines.hpp"
class Motorgroup {
public:
	Throttle_t motors[Defines::MOTORS];
	Motorgroup& All( pid_t percent ) {
		for (int i = 0; i < Defines::MOTORS; ++i)
			motors[i].setPower(percent);
		return *this;
	}
	Motorgroup& PID_ratio( pid_t percent ) {
		for (int i = 0; i < Defines::MOTORS; ++i)
			motors[i].setReserveRatio(percent);
		return *this;
	}

	Motorgroup& Zero() {
		All(0.0);
		return *this;
	}
	Throttle_t& operator[] (int x) {
		return motors[x];
	}

	uint8_t A() {
		return motors[0].SPI_data();
	}
	uint8_t B() {
		return motors[1].SPI_data();
	}
	uint8_t C() {
		return motors[2].SPI_data();
	}
	uint8_t D() {
		return motors[3].SPI_data();
	}
};
#endif