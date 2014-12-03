/* For properties and members all sub systems should have */
#include "arfcDefines.hpp"
#include "errorMap.hpp"
#include <stdint.h>
#include <mutex>
#ifndef SUBSYSTEM
#define SUBSYSTEM
class SubSystem { 
protected:
	ErrorMap  err;
	ATOMIC    _fail
			, _valid
			, _active
			, _dispose;
	std::mutex access;

public:
	SubSystem() {
		_fail             = false;
		_valid            = false;
		_active           = false;
		_dispose          = false;
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

	void Disable() {
		Set_Data_Valid(false);
		Set_Active(false);
		Set_Fail(true);
		Dispose();
	}

};


class AsyncWorker : public SubSystem {
protected:
	pthread_t worker;
public:
	AsyncWorker() : SubSystem() {}
	
	virtual void* worker_run() = 0;

	AsyncWorker& Detach() {
		pthread_detach(worker);
		return *this;
	}
	AsyncWorker& Start() {
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