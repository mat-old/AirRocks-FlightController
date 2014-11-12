/*

	JCommand -
	SHOULD be the main JSON parsing type
	WILL be exception safe
	boost causes hangs and weird assemlber errors, v8 embedding is worse...
	i NEED c++11 support... next time its going to be an Intel Edison

*/
#ifndef JSON_COMMAND_T
#define JSON_COMMAND_T

#include "../Defines.hpp"
#include <sstream>
#include <exception>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class JCommand {
public:
	boost::property_tree::ptree obj;
	JCommand();

	std::string Action();

	std::string Name();

	var_float_t Value();

	int Value(char);

	bool tryParse(char * s);

};
#endif