/* Note: boring source file, asyncWorker is a little neat */
/* For properties and members all sub systems should have */
#ifndef SUBSYSTEM
#define SUBSYSTEM
#include "../Defines.hpp"
/* SubSystem     *  basic system subclass, with multiple flags
*  Set_Data_Valid( bool )
*  Set_Active( bool )
*  Set_Fail( bool )
*  Dispose()
*  bool Data_Valid
*  bool Disposed
*  bool Active
*  bool Fail
*  Disable()     -  sets all flags, indicates inactive system
*/
class SubSystem { 
protected:
	ATOMIC    _fail
			, _valid
			, _active
			, _dispose
			, _disabled;
public:
	SubSystem() {
		_fail             = false;
		_valid            = false;
		_active           = false;
		_dispose          = false;
		_disabled         = false;
	}
	~SubSystem(){
		Dispose();
	}
	void Set_Data_Valid(bool tof)	{ _valid = tof; }
	void Dispose()                  { _dispose = true; }
	void Set_Active(bool tof) 		{ _active = tof;}
	void Set_Fail(bool tof) 		{ _fail = tof;  }
	bool Data_Valid() 	{ return _valid; }
	bool Disposed()     { return _dispose; }
	bool Active() 		{ return _active;}
	bool Fail()			{ return _fail;  }
	bool Disabled()     { return _disabled; }
	void Disable() {
		_disabled = true;
		Set_Data_Valid(false);
		Set_Active(false);
		Set_Fail(true);
		Dispose();
	}
};
#endif