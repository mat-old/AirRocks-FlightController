/*
*  Inertia Measurement Unit Worker, does the constant gathering of IMU data 
*  this class provides the required higher level data for the PIDctrl
*/
#include "arfcDefines.hpp"
#include "imuInterface.hpp"
#include "dataTypes.hpp"
#include "subSystem.hpp"
#include <iostream>
#include <unistd.h>
#ifndef TEST
#define TEST
using namespace std;

class test : public AsyncWorker {
public:
	test()  {}
	~test() {}
	virtual void* worker_run() {
		std::cout<< "hello thread" <<endl<<flush;
		
		while( !Disposed() );
		cout << " :( bye dad! " <<endl<<flush;

		return 0;
	}
};


int main(int argc, char const *argv[])
{
	test *t = new test;

	t->Start();
	sleep(2);
	delete t;
	sleep(2);

	return 0;
}
#endif