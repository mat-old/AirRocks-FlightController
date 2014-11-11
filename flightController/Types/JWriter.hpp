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
	bool pretty; /* FALSE in production */
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
	void Pretty(bool p); 

	/*  { type:name , data: simple }  */
	template <typename T>
	void operator()(std::string const type, T data);

	/*  { type:name : data { complex } }  */
	void operator()(std::string const type, ptree child);

	/*  { type:status, data:"s" }  */
	void operator()(std::string data);

	/*  { type:motorgroup, data: [ f, f, f, f ] }  */
	void operator()(Motorgroup& mg);

	/*  { type:PID, name:'s', input:'f', output:'f', data: { labels:[ s,s,s ],values:[ f, f, f ] }  }  */
	void operator()(PID_t& pt);

	/*  { type:'potential', data:{ name:'', values:[ float, float, float ] } }  */
	void operator()(Potential_t& pot);

	/*  { type:'throttle', data:{ name:'',speed:'' } }  */
	void operator()(Throttle_t& t);

	/* title, format, args... */
	void err(const char* what, const int argc, ...);
	/* use as promise? */
	void cmd(std::string data, bool processed);
};
#endif