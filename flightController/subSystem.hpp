/* Note: boring source file, asyncWorker is a little neat */
/* For properties and members all sub systems should have */
#ifndef SUBSYSTEM
#define SUBSYSTEM
#include "arfcDefines.hpp"
#include "dataTypes.hpp"
#include "errorMap.hpp"
#include <stdint.h>
#include <mutex>
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
	ErrorMap  err;
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

/* AsyncWorker   *  provide a class base when there is a need for threading a single job
*  worker_run()  -  must override
*  Start()       -  runs the worker_run() method in a posix thread
*  Detach()      -  detaches the thread, 
*  worker        -  thread handle
*  timer         -  synchronization timer, default in 'arfcDefines.hpp'
*  access        -  std::mutex variable
*/
class AsyncWorker : public SubSystem {
protected:
	pthread_t worker;
	std::mutex access;
	TimerMS * timer;
public:
	AsyncWorker() : SubSystem() {
		timer = new TimerMS(Def::STD_DELAY_MS);
	}
	~AsyncWorker() {
		/* rejoin thread, make sure it stops, init will reap guardedness */
		/* works when called from deferred cancelable state */
		Dispose();
		pthread_join(worker, NULL);
		delete timer;
	}
	virtual void* worker_run() = 0;

	AsyncWorker& Detach() {
		if( Disabled() ) return *this;
		pthread_detach(worker);
		return *this;
	}
	AsyncWorker& Start() {
		if( Disabled() ) return *this;
		if( !Fail() ) {
			int ret = pthread_create(&worker, NULL, &worker_helper, this);
			if( ret != 0 ) throw FAIL_START_WORKER;
		} else throw FAIL_FLAG_SET;
		return *this;
	}
	static void *worker_helper(void *context) {
		return static_cast<AsyncWorker*>(context)->worker_run();
	}
};
#endif