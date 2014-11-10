#include "SubSystem.hpp"
	ErrorMap SubSystem::err;
	GenericWriter SubSystem::emit;
	SubSystem::SubSystem() {
		_fail             = false;
		_valid            = false;
		_active           = false;
		_dispose          = false;
		_disabled         = false;
	}
	SubSystem::~SubSystem(){
		Dispose();
	}
	void SubSystem::Set_Data_Valid(bool tof)	{ _valid = tof; }
	void SubSystem::Dispose()                  { _dispose = true; }
	void SubSystem::Set_Active(bool tof) 		{ _active = tof;}
	void SubSystem::Set_Fail(bool tof) 		{ _fail = tof;  }
	bool SubSystem::Data_Valid() 	{ return _valid; }
	bool SubSystem::Disposed()     { return _dispose; }
	bool SubSystem::Active() 		{ return _active;}
	bool SubSystem::Fail()			{ return _fail;  }
	bool SubSystem::Disabled()     { return _disabled; }
	void SubSystem::Disable() {
		_disabled = true;
		Set_Data_Valid(false);
		Set_Active(false);
		Set_Fail(true);
		Dispose();
	}