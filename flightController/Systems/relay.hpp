/*  this is a class for testing
	TODO: replace this with a NodeJS module
	NOTE: do to platform restrictions this may become the main interop object
	      i wish i had c++11
*/
#ifndef RELAY_TEST
#define RELAY_TEST
#include "../Includes.hpp"

#include <vector>
#include <string>
#include <map>

class Relay : public AsyncWorker {
public:
	std::vector<Command> cmds;
	
	bool new_data;
	Relay()  {
		/*for initial disarmed state*/
		Set_Active(false);
	}
	~Relay() {}

	bool Disarmed() {
		return !Active();
	}

	void Transact(Motorgroup & m
				, PIDctrl    * p
				, Potential_t& g
				, Potential_t& a ) {
		// process always 
		if( !cmds.empty() )
			Update(m, p);
		// feedback sometimes 
		if( !timer->Allow() ) return;

		if( Data_Valid() ) {
			Set_Data_Valid(false);
			emit(m);
			emit(p->pitch);
			emit(p->roll);
			emit(p->yaw);
			emit(g);
			emit(a);
		}
	}


	void Update(Motorgroup& motors, PIDctrl* PID ) {
		if( access.try_lock() ) {
			try {
			for (std::vector<Command>::iterator i = cmds.begin(); i != cmds.end(); ++i) {
				std::cout << "> " << (*i).name << " : " << (*i).Hash() << std::endl;
				switch( (*i).Hash() ) {
					case PP:
						PID->pitch.SetP( (*i).getValue() );
					break;
					case PI:
						PID->pitch.SetI( (*i).getValue() );
					break;
					case PD:
						PID->pitch.SetD( (*i).getValue() );
					break;
					case RP:
						PID->roll.SetP( (*i).getValue() );
					break;
					case RI:
						PID->roll.SetI( (*i).getValue() );
					break;
					case RD:
						PID->roll.SetD( (*i).getValue() );
					break;
					case YP:
						PID->yaw.SetP( (*i).getValue() );
					break;
					case YI:
						PID->yaw.SetI( (*i).getValue() );
					break;
					case YD:
						PID->yaw.SetD( (*i).getValue() );
					break;
					case TTHROTTLE:
						motors.All( (*i).getValue()/100.0f );
					break;
					case FORCEU:
						/* force update */
						//std::cout << "force update:" << std::endl;
						Set_Data_Valid(true);
					break;
					case DOWN:
					/* shut down / disarm etc... */
					SHUTDOWN_FLAG = true;
					throw SHUTDOWN;
					break;
					case RESET:
						motors.All( 0.0 );
					break;
					case ARM:
						//std::cout << "WARNING!!! NOW ARMED" << std::endl;
						emit("WARNING!, ARMED");
						Set_Active(true);
					break;
				}
			}
			} catch(ERR_CODES e) {
				err.Response(e);
			}
			cmds.clear();
			access.unlock();
			Set_Data_Valid(true);
		}
	}

private:
	virtual void *worker_run() {
		//std::cout << "> Now accepting commands on stdin " << std::endl << std::flush;
		while(true) {
			std::string buf;
			std::cin >> buf;
			//std::cout << "> Processing " << buf << std::endl << std::flush;
			if( valid_cmd_syntax(buf) ) {
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
/* fuck this class */