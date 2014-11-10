#include "../Defines.hpp"
#include <stdint.h>
#include "Motorgroup.hpp"



Motorgroup& Motorgroup::All( var_float_t percent ) {
	for (int i = 0; i < MOTORS; ++i)
		motors[i].setPower(percent);
	return *this;
}
Motorgroup& Motorgroup::All( bool b ) {
	for (int i = 0; i < MOTORS; ++i)
		motors[i].Enable(b);
	return *this;
}
Motorgroup& Motorgroup::PID_ratio( var_float_t percent ) {
	for (int i = 0; i < MOTORS; ++i)
		motors[i].setReserveRatio(percent);
	return *this;
}

Motorgroup& Motorgroup::Zero() {
	All((var_float_t)0.0);
	return *this;
}
Throttle_t& Motorgroup::operator[] (int x) {
	return motors[x];
}

void Motorgroup::PitchOnly() {
	/* disables Roll, enables pitch */
	A(false);
	B(true);
	C(false);
	D(true);
}
void Motorgroup::RollOnly() {
	/* disables Pitch, enables pitch */
	A(true);
	B(false);
	C(true);
	D(false);
}
void Motorgroup::YawOnly() {
	/* enables all */
	A(true);
	B(true);
	C(true);
	D(true);
}

uint8_t Motorgroup::A() {
	return motors[0].SPI_data();
}
uint8_t Motorgroup::B() {
	return motors[1].SPI_data();
}
uint8_t Motorgroup::C() {
	return motors[2].SPI_data();
}
uint8_t Motorgroup::D() {
	return motors[3].SPI_data();
}
void Motorgroup::A(var_float_t p) {
	motors[0].setPower(p);
}
void Motorgroup::B(var_float_t p) {
	motors[1].setPower(p);
}
void Motorgroup::C(var_float_t p) {
	motors[2].setPower(p);
}
void Motorgroup::D(var_float_t p) {
	motors[3].setPower(p);
}
void Motorgroup::A(bool p) {
	motors[0].Enable(p);
}
void Motorgroup::B(bool p) {
	motors[1].Enable(p);
}
void Motorgroup::C(bool p) {
	motors[2].Enable(p);
}
void Motorgroup::D(bool p) {
	motors[3].Enable(p);
}