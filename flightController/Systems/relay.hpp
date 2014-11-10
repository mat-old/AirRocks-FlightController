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
#include "../Cores/AsyncWorker.hpp"
#include "../Types/Motorgroup.hpp"
#include "../Types/Potential.hpp"
#include "../Types/Arming.hpp"
#include "../Types/JCommand.hpp"
#include "pidCtrl.hpp"
#include <vector>
#include <string>
#include <map>

class Relay : public AsyncWorker {
public:
	typedef enum {
		AC_set
	  , AC_inactive /*implicit*/
	  , AC_throttle_arm
	  , AC_throttle_start
	  , AC_throttle_stop
	  , AC_throttle_torque
	  , AC_pitch_activate
	  , AC_pitch_reset
	  , AC_pitch_save
	  , AC_pitch_p
	  , AC_pitch_i
	  , AC_pitch_d
	  , AC_roll_active
	  , AC_roll_reset
	  , AC_roll_save
	  , AC_roll_p
	  , AC_roll_i
	  , AC_roll_d
	  , AC_yaw_active
	  , AC_yaw_reset
	  , AC_yaw_save
	  , AC_yaw_p
	  , AC_yaw_i
	  , AC_yaw_d
	  , AC_err
	} AC_action_codes;
	std::map<std::string, AC_action_codes>::iterator AC_start, AC_end;
	std::map<std::string, AC_action_codes> action;
	std::vector<std::string> pre_parse;

	bool new_data;
	bool ARM_FLAG;
	AC_action_codes AC_tuneing_state;
	Relay();
	~Relay();

	bool Disarmed();

	void Process(Motorgroup  & m
				, PIDctrl    & p
				, Potential_t& g
				, Potential_t& a 
				, Arming     & s);


	void Update(Motorgroup& motors, PIDctrl& P, Arming& safety ) ;

	void setActiveTuner( AC_action_codes tuner, Motorgroup& motors);

	bool inactive() ;

	void powerDown(Motorgroup& motors);

	AC_action_codes getActionCode(std::string s);

	void armPending( Arming& safety ) ;

private:
	virtual void *worker_run();
};
#endif
