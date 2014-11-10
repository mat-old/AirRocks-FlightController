#include "Potential.hpp"
	Potential_t::Potential_t()  {}
	Potential_t::Potential_t(std::string n)  { name = n; }

/*	Potential_t(vector v)  {
		this->Gather(v);
	}*/
	Potential_t::~Potential_t() {}

	/* until Eigen is fixed... */
	void Potential_t::Gather(var_float_t v0,var_float_t v1,var_float_t v2) {
		x = v0;
		y = v1;
		z = v2;
	}

	void Potential_t::copy( Potential_t& p ) {
		x = p.x;
		y = p.y;
		z = p.z;
	}

	void Potential_t::Zero() {
		x = 0;
		y = 0;
		z = 0;
	}
