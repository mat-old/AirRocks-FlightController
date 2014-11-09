#ifndef COMMAND_T
#define COMMANT_T

#include "../Includes.hpp"
using namespace Defines;

class Command {
public:
	std::string name, val;
	bool hashed, processed;
	CMD_CODES hash;
	Command() {
		hashed = false;
		processed = false;
	}
	Command(std::string s) {
		int f = s.find(":");
		val  = s.substr(f+1);
		name = s.substr(0,f); 
		hashed = false;
	}
	CMD_CODES Hash() {
		if( hashed ) return hash;
		if( codes_map.find(name) == codes_map.end() )
			hash = NONE;
		else
			hash = codes_map[name];
		hashed = true;
		return hash;
	}
	pid_t getValue() {
		try {
			return std::stof(val);
		}
		catch( int e ) {
			return -1.0f;
		}
	}
};
#endif