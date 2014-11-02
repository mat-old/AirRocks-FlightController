/* for none statically declared globals */
#ifndef GLOBALS
#define GLOBALS

#include "Defines.hpp"
#include <map>

using namespace Defines;

ATOMIC SHUTDOWN_FLAG = false;

typedef enum { NONE=0, DOWN, RP, RI, RD, PP, PI, PD, YP, YI, YD, FORCEU, TTHROTTLE, RESET, ARM } CMD_CODES;
std::map<std::string, CMD_CODES> CMD_CODEMAP() {
	std::map<std::string, CMD_CODES> cm;
	cm["DOWN"]  = DOWN;
	cm["RP"]    = RP;
	cm["RI"]    = RI;
	cm["RD"]    = RD;
	cm["PP"]    = PP;
	cm["PI"]    = PI;
	cm["PD"]    = PD;
	cm["YP"]    = YP;
	cm["YI"]    = YI;
	cm["YD"]    = YD;
	cm["FORCEU"]= FORCEU;
	cm["T"]     = TTHROTTLE;
	cm["R"]     = RESET;
	cm["ARM"]   = ARM;
	return cm;
}
std::map<std::string, CMD_CODES> codes_map = CMD_CODEMAP();


inline char MOTOR_SAFE_SPEED(char s) {
	if( s > MOTOR_MAX_LEVEL  )
		return MOTOR_MAX_LEVEL;
	else if( s < MOTOR_ZERO_LEVEL )
		return MOTOR_ZERO_LEVEL;
	else return s;
}
inline unsigned long millis() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}
inline bool valid_cmd_syntax(std::string s) {
	if( s == "DOWN" || s == "FORCEU" || s == "R" || s == "ARM" ) return true;
	int f = s.find(":");
	int l = s.length();
	if( l < 3 ) return false;
	return (f < l && f > 0 && f < l-1 );
}

/* each should know its own precedence */
#include "./Types/Types.hpp"
/* global objects */
JWriter        jout;
GenericWriter  emit;
#include "./Cores/Cores.hpp"
#include "./Systems/Systems.hpp"


#endif