#include <pthread.h>
#include <iostream>
#include <unistd.h>

using namespace std;

class test {
public:
	int threadid;
	pthread_t td;
	bool stop;
	test() {
		stop = false;
	}

	test& start() {
		this->threadid = pthread_create(&td, NULL, &test::hello_helper, this);
		return *this;
	}

	void Stop() {
		stop = true;
	}

  	void *hello()
    {
    	while(true) {
        	cout << "Hello" << endl;
        	sleep(1);
        	if(stop) return 0;
        }
        return 0;
    }

    static void *hello_helper(void *context)
    {
        return ((test *)context)->hello();
    }

};

int main(int argc, char const *argv[])
{
	test t;

	t.start();
	sleep(3);
	t.Stop();
	sleep(3);
	sleep(3);
	sleep(3);
	sleep(3);
	return 0;
}