#ifndef ASYNCWORKER
#define ASYNCWORKER
#include "../Defines.hpp"
#include <mutex>
#include <pthread.h>
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
		timer = new TimerMS(Defines::STD_DELAY_MS);
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