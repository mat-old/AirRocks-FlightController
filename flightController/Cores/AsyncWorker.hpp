#ifndef ASYNCWORKER
#define ASYNCWORKER
#include "../Defines.hpp"
#include "SubSystem.hpp"
#include "../Types/Timer.hpp"
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
	AsyncWorker() ;
	~AsyncWorker();
	virtual void* worker_run() = 0;

	AsyncWorker& Detach();
	AsyncWorker& Start();
	AsyncWorker& Stop();
	static void *worker_helper(void *context) {
		return static_cast<AsyncWorker*>(context)->worker_run();
	}
};
#endif