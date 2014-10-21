#include "ARHS/vector.h"

#include "arfcDefines.hpp"

#ifndef  DATATYPES
#define  DATATYPES
class Potential {
public:
	float x, y, z;
	Potential()  {}
	Potential(vector v)  {
		this->Gather(v);
	}
	~Potential() {}

	void Gather(vector v) {
		x = v(0);
		y = v(1);
		z = v(2);
	}

	void copy( Potential& p ) {
		x = p.x;
		y = p.y;
		z = p.z;
	}

	void Zero() {
		x = 0;
		y = 0;
		z = 0;
	}

	friend std::ostream& operator<< (std::ostream& out, const Potential& p) {
		out << FLOAT_FORMAT << p.x << ' '<< p.y << ' '<< p.z;
		return out;
	}
};
#endif