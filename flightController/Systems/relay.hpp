/*  this is a class for testing
	TODO: replace this with a NodeJS module
	NOTE: do to platform restrictions this may become the main interop object
	      i wish i had c++11
	Desc: Buffer JSON strings in pre_parse vector, process as many ptree's as possible in time allotment 
		  actions<map> to functionPtrs 
*/
#ifndef RELAY_TEST
#define RELAY_TEST
#include "../Defines.hpp"

#include "../Types/Motorgroup.hpp"
#include "../Types/Potential.hpp"
#include "../Types/JCommand.hpp"
#include "../Types/Arming.hpp"
#include "../Types/PID.hpp"

#include "dgramInterface.hpp"

#include <vector>
#include <string>
#include <map>

class Relay : public DGRAMinterface {
	char data[UDP_BUF_SIZE];

	std::map<std::string, AC_action_codes>::iterator AC_start, AC_end;
	std::map<std::string, AC_action_codes> action;
	std::vector<JCommand> post_parse;

	AC_action_codes AC_tuneing_state;
	/*  internal use > set witch motors will use throttle  */
	void setActiveTuner( AC_action_codes tuner, Motorgroup& motors);

	/*  gets the code from the action map  */
	AC_action_codes getActionCode(std::string s);

	/*  loc the drone if comms appear to have failed  */
	void lockIfDark();

	/*  set disables and enables specific motors */
	void powerDown(Motorgroup& motors);

public:
	Relay();
	~Relay();

	void Process(Motorgroup& m
				//, PIDctrl    & p /* SUBSYSTEMS CANNOT TALK LIKE THIS, get the data some other way */
				, PID_t& Pitch
				, PID_t& Roll
				, PID_t& Yaw
				, Potential_t& g
				, Potential_t& a 
				, Arming& s);

				
	void Update(Motorgroup& motors, PID_t& Pitch, PID_t& Roll, PID_t& Yaw, Arming& safety ) ;

	/* true IF the tuner is in a state it can move a motor */
	bool inactive() ;

	/* blocking - waiting for ARM code */
	void waitForARM( Arming& safety ) ;

	/* blocking - waitin for code */
	int waitFor( AC_action_codes );

	void Listen() {
		DGRAMinterface::Listen(this->data);
	}

	void Connect() {
		DGRAMinterface::Connect();
	}

private:
	virtual void *worker_run();
};
#endif
