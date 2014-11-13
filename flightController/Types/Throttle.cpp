#include <string>
#include <stdint.h>
#include "../Defines.hpp"
#include "Throttle.hpp"

//#include <iostream>
//using namespace std;

Throttle_t::Throttle_t()  {
	Zero();
	enabled = false;
}
Throttle_t::Throttle_t(const std::string n) {
	name = n;
	Zero();
	enabled = false;
}
Throttle_t::~Throttle_t() {}
/* i really hate floats */
void Throttle_t::setReserveRatio( var_float_t n ) {
	if( n > 1.0f ) return;
	var_float_t range   = (var_float_t)MOTOR_MAX_LEVEL - MOTOR_ARM_START;
	//cout << range << "=" << MOTOR_MAX_LEVEL << "-" << MOTOR_ARM_START << endl;
	reserved = range * n;
	//cout << (int)reserved << " = " << range << "*" << res << endl;
	power    = (var_float_t)range - reserved;
	//cout << (int)power << " = " << range << "*" << (int)reserved << endl;
}
void Throttle_t::setReserve( var_float_t n ) {
	if( n > 1.0f ) return;
	set_r = reserved * n;
}
void Throttle_t::setPower( var_float_t n ) {
	if( n > 1.0f ) return;
	set_p = power * n;
}
uint8_t Throttle_t::SPI_data() {
	return this->enabled?Throttle() + MOTOR_ZERO_LEVEL : MOTOR_ZERO_LEVEL;
}
uint8_t Throttle_t::Throttle() {
	uint8_t ret = (set_p + set_r);
	return ret>THROTTLE_MAX?THROTTLE_MAX:ret<0?0:ret;
}
void Throttle_t::Enable(bool b) {
	this->enabled = b;
}
void Throttle_t::Zero() {
	this->set_p = this->set_r = 0.0f;
}