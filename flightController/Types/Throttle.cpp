#include <string>
#include <stdint.h>
#include "../Defines.hpp"
#include "Throttle.hpp"


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

void Throttle_t::setReserveRatio( var_float_t res ) {
	var_float_t r  = (var_float_t)THROTTLE_MAX * res;
	power    = (uint8_t)((var_float_t)THROTTLE_MAX - r);
	reserved = (uint8_t)r;
}
void Throttle_t::setReserve( var_float_t in ) {
	set_r = reserved * in;
}
void Throttle_t::setPower( var_float_t in ) {
	set_p = power * in;
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