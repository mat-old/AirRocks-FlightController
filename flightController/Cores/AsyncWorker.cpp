#include "AsyncWorker.hpp"
	AsyncWorker::AsyncWorker() : SubSystem() {
		timer = new TimerMS(STD_DELAY_MS);
	}
	AsyncWorker::~AsyncWorker() {
		/* rejoin thread, make sure it stops, init will reap guardedness */
		/* works when called from deferred cancelable state */
		Dispose();
		pthread_join(worker, NULL);
		delete timer;
	}

	AsyncWorker& AsyncWorker::Detach() {
		if( Disabled() ) return *this;
		pthread_detach(worker);
		return *this;
	}
	AsyncWorker& AsyncWorker::Start() {
		if( Disabled() ) return *this;
		if( !Fail() ) {
			int ret = pthread_create(&worker, NULL, &worker_helper, this);
			if( ret != 0 ) throw FAIL_START_WORKER;
		} else throw FAIL_FLAG_SET;
		return *this;
	}
