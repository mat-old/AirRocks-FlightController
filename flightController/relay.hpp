/*  this is a class for testing
	TODO: replace this
*/

#ifndef RELAY_TEST
#define RELAY_TEST
#include "arfcDefines.hpp"
#include "dataTypes.hpp"
#include "subSystem.hpp"
#include <iostream>	
#include <vector>
class Command {
public:
	std::string name, val;
	bool hashed;
	int hash;
	Command() {}
	Command(std::string s) {
		int f = s.find(":");
		val  = s.substr(f+1);
		name = s.substr(0,f); 
	}
	int Hash() {
		if( hashed ) return hash;
		hash = name[0];
		for (int i = 1; i < name.length(); ++i)
			hash ^= name[i];
		hashed = true;
		return hash;
	}

};

bool valid(std::string s) {
	int f = s.find(":");
	int l = s.length();
	if( l < 3 ) return false;
	return (f < l && f > 0 && f < l-1 );
}



class Relay : public AsyncWorker {
public:
	std::vector<Command> cmds;
	Relay()  {}
	~Relay() {}

	void Update(Motorgroup& motors, PIDctrl* PID ) {
		if(  access.try_lock() ) {
			for (std::vector<Command>::iterator i = cmds.begin(); i != cmds.end(); ++i) {
				switch( (*i).Hash() ) {
					case 'P'^'P':

					break;
					case 'P'^'I':

					break;
					case 'P'^'D':

					break;
					case 'R'^'P':

					break;
					case 'R'^'I':

					break;
					case 'R'^'D':

					break;
					case 'Y'^'P':

					break;
					case 'Y'^'I':

					break;
					case 'Y'^'D':

					break;
					case 'T':

					break;
				}
			}
			access.unlock();
		}
	}

private:
	virtual void *worker_run() {
		while(true) {
			std::string buf;
			std::cin >> buf;
			if( valid(buf) ) {
				access.lock();
				cmds.push_back(Command(buf));
				access.unlock();
			}
		}
	}
};
#endif