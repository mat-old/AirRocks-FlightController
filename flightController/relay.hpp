/*  this is a class for testing
	TODO: replace this with a NodeJS module
	NOTE: to do platform restrictions this may become the main interop object
*/
#ifndef RELAY_TEST
#define RELAY_TEST
#include "arfcDefines.hpp"
#include "dataTypes.hpp"
#include "subSystem.hpp"
#include <iostream>	
#include <vector>
#include <string>
#include <map>

typedef enum { NONE=0, DOWN, RP, RI, RD, PP, PI, PD, YP, YI, YD, FORCEU, TTHROTTLE } CMD_CODES;
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
	cm["TTHROTTLE"] = TTHROTTLE;
	return cm;
}
std::map<std::string, CMD_CODES> codes_map = CMD_CODEMAP();

class Command {
public:
	std::string name, val;
	bool hashed;
	CMD_CODES hash;
	Command() {
		hashed = false;
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

bool valid(std::string s) {
	if( s == "DOWN" || s == "FORCEU" ) return true;
	int f = s.find(":");
	int l = s.length();
	if( l < 3 ) return false;
	return (f < l && f > 0 && f < l-1 );
}

class Relay : public AsyncWorker {
public:
	std::vector<Command> cmds;
	
	bool new_data;
	Relay()  {}
	~Relay() {}

	void Transact(Motorgroup & m
				, PIDctrl    * P
				, Potential_t& g
				, Potential_t& a ) {
		// process always 
		if( !cmds.empty() )
			Update(m, P);
		// feedback sometimes 
		if( !timer->Allow() ) return;

		if( Data_Valid() ) {
			Set_Data_Valid(false);
			Emit(/* motor */
				  m[0].Throttle()
				, m[1].Throttle()
				, m[2].Throttle()
				, m[3].Throttle()
				, P->pitch.output
				, P->roll.output
				, P->yaw.output
				);
		}
	}

	void Emit(/* motor */
		      int A
			, int B
			, int C
			, int D
			  /* PID */
			, pid_t pitch
			, pid_t roll
			, pid_t yaw 
			) {

		std::cout << ""
		"{ \"motor\" : {"
			"\"A\" : \"" << A << "\","
			"\"B\" : \"" << B << "\","
			"\"C\" : \"" << C << "\","
			"\"D\" : \"" << D << "\""
		"},\"PID\"   : {"
			"\"pitch\" : \"" << pitch << "\","
			"\"roll\" : \"" << roll << "\","
			"\"yaw\" : \"" << yaw << "\""
		"}, \"type\":\"relay\" }"
		<< std::endl;
	}

	void Update(Motorgroup& motors, PIDctrl* PID ) {
		if( access.try_lock() ) {
			try {
			for (std::vector<Command>::iterator i = cmds.begin(); i != cmds.end(); ++i) {
				std::cout << "> " << (*i).name << " : " << (*i).Hash() << std::endl;
				switch( (*i).Hash() ) {
					case PP:
						PID->pitch.kp = (*i).getValue();
					break;
					case PI:
						PID->pitch.ki = (*i).getValue();
					break;
					case PD:
						PID->pitch.kd = (*i).getValue();
					break;
					case RP:
						PID->roll.kp = (*i).getValue();
					break;
					case RI:
						PID->roll.ki = (*i).getValue();
					break;
					case RD:
						PID->roll.kd = (*i).getValue();
					break;
					case YP:
						PID->yaw.kp = (*i).getValue();
					break;
					case YI:
						PID->yaw.ki = (*i).getValue();
					break;
					case YD:
						PID->yaw.kd = (*i).getValue();
					break;
					case TTHROTTLE:
						motors.All( (*i).getValue()/100.0f );
					break;
					case FORCEU:
						/* force update */
						std::cout << "force update:" << std::endl;
						Set_Data_Valid(true);
					break;
					case DOWN:
					/* shut down / disarm etc... */
					throw SHUTDOWN;
					break;
				}
			}
			} catch(errCodes e) {
				err.Response(e);
			}
			cmds.clear();
			access.unlock();
			Set_Data_Valid(true);
		}
	}

private:
	virtual void *worker_run() {
		std::cout << "> Now accepting commands on stdin " << std::endl << std::flush;
		while(true) {
			std::string buf;
			std::cin >> buf;
			//std::cout << "> Processing " << buf << std::endl << std::flush;
			if( valid(buf) ) {
				access.lock();
				Command c = Command(buf);
				cmds.push_back(c);
				access.unlock();
			}
			buf.clear();
			if( Disposed() ) return 0;
		}
	}
};
#endif