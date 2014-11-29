#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <unistd.h>

using namespace std;
class test
{
public:
	volatile long c = 0;
	volatile bool dispose_flag = false;
	mutex lock;
    ~test() {
    	dispose_flag = true;
    	usleep(1000000);
    }

    void start() {
		t = thread(&test::test_func, this);
		t.detach();
		while(true) {
			//lock.lock();
			c++;
			if( c > 30000 ) dispose_flag = true;
			//lock.unlock();
			usleep(9);
		}
    }
    void detach(){
    	t.detach();
    }
    void test_func() {
    	while(true) {
    		if( true){//lock.try_lock() ) {
    			cout << " " << c << flush;
				//lock.unlock();
				usleep(500000);
    		}
    		else
    			cout << "!";
    		if( dispose_flag ) {
    			cout << "ending thread" << endl;
    			return;
    		}
		} 
	}
    thread t;
};


int main(int argc, char const *argv[])
{
	uint8_t r[4] = {0xA,0xB,0xC,0xD};
	test t;
	t.start();
	t.detach();
	sleep(2);



	return 0;
}