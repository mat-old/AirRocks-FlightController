#ifndef MOTORGROUP
#define MOTORGROUP

#include "../Defines.hpp"
#include "Throttle.hpp"
#include <stdint.h>


//#include "../Types/Types.hpp"
class Motorgroup {
public:
	Throttle_t motors[MOTORS];
	Motorgroup& All( var_float_t percent );
	Motorgroup& All( bool b );
	Motorgroup& PID_ratio( var_float_t percent );

	Motorgroup& Zero() ;
	Throttle_t& operator[] (int x);

	void PitchOnly();
	void RollOnly();
	void YawOnly();

	uint8_t A();
	uint8_t B();
	uint8_t C();
	uint8_t D();
	void A(var_float_t p);
	void B(var_float_t p);
	void C(var_float_t p);
	void D(var_float_t p);
	void A(bool p);
	void B(bool p);
	void C(bool p);
	void D(bool p);
};


#endif