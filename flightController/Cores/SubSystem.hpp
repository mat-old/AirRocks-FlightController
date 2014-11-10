/* Note: boring source file, asyncWorker is a little neat */
/* For properties and members all sub systems should have */
#ifndef SUBSYSTEM
#define SUBSYSTEM
#include "../Defines.hpp"
#include "ErrorMap.hpp"
#include "../Types/JWriter.hpp"

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
	static ErrorMap err;
	static GenericWriter emit;
	SubSystem();
	~SubSystem();
	void Set_Data_Valid(bool tof);
	void Dispose() ;
	void Set_Active(bool tof);
	void Set_Fail(bool tof);
	bool Data_Valid() ;
	bool Disposed();
	bool Active();
	bool Fail();
	bool Disabled();
	void Disable();
};
#endif