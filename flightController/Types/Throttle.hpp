#ifndef THROTTLE_T
#define THROTTLE_T

#include <string>
#include <stdint.h>
#include "../Defines.hpp"

class Throttle_t {
public:
	std::string name;
	uint8_t reserved
		  , power;

	var_float_t set_p
		, set_r;

	bool enabled;

	Throttle_t();
	Throttle_t(const std::string n);
	~Throttle_t();

	void setReserveRatio( var_float_t res );
	void setReserve( var_float_t in );
	void setPower( var_float_t in ) ;
	uint8_t SPI_data();
	uint8_t Throttle();
	void Enable(bool b);
	void Zero();
};
#endif