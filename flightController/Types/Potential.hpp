#include "../Includes.hpp"
class Potential_t {
public:
	pid_t x, y, z;
	std::string name;
	Potential_t()  {}
	Potential_t(std::string n)  { name = n; }

	Potential_t(vector v)  {
		this->Gather(v);
	}
	~Potential_t() {}

	void Gather(vector v) {
		x = v(0);
		y = v(1);
		z = v(2);
	}

	void copy( Potential_t& p ) {
		x = p.x;
		y = p.y;
		z = p.z;
	}

	void Zero() {
		x = 0;
		y = 0;
		z = 0;
	}

	friend std::ostream& operator<< (std::ostream& out, const Potential_t& p) {
		out << FLOAT_FORMAT << p.x << ' '<< p.y << ' '<< p.z;
		return out;
	}
};