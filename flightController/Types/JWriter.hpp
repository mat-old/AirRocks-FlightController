#ifndef JWRITER 
#define JWRITER
#include <cstdlib>   // rand
#include <cstdarg>
#include <sstream>
#include <stdlib.h> // rand
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "../Defines.hpp"
#include "Motorgroup.hpp"
#include "Throttle.hpp"
#include "PID.hpp"
#include "Potential.hpp"

using boost::property_tree::write_json;
using boost::property_tree::ptree;

class JWriter {
private:
	std::ostringstream s;
	bool pretty;
	ptree t;
protected:
	void Write();
	ptree array(const char* fmt...);
	template <typename T>
	void put(const std::string name, T val);
public:
	JWriter();
	JWriter(bool p);
	~JWriter();
	template <typename T>
	void operator()(std::string const type, T data);
	void operator()(std::string const type, ptree child);
	void Pretty(bool p);
};
/* i name things opposites a lot... */
class GenericWriter : public JWriter {
public:
	void operator()(std::string data);
	void operator()(Motorgroup& mg);
	void operator()(PID_t& pt);
	void operator()(Potential_t& pot);
	void operator()(Throttle_t& t);
	/* where, number of extra args, argv... */
	void err(const char* what, const int argc, ...);
	void cmd(std::string data, bool processed);
};
#endif