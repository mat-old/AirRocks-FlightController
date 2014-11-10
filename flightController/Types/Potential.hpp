#ifndef POTENTIAL_T
#define POTENTIAL_T

#include "../Defines.hpp"
//#include "../ARHS/includes.hpp"
#include <string>

class Potential_t {
public:
	var_float_t x, y, z;
	std::string name;
	Potential_t();
	Potential_t(std::string n);
/*	Potential_t(vector v)  {
		this->Gather(v);
	}*/
	~Potential_t();
	/* until Eigen is fixed... */
	void Gather(var_float_t v0,var_float_t v1,var_float_t v2);
	void copy( Potential_t& p );
	void Zero();
/*	friend std::ostream& operator<< (std::ostream& out, const Potential_t& p) {
		out << FLOAT_FORMAT << p.x << ' '<< p.y << ' '<< p.z;
		return out;
	}*/
};
#endif