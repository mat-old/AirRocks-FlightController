/* provides one method that returns true only when the amount of time / tdiv has passed */
#ifndef TIMER_T
#define TIMER_T
#include <ctime>
class Timer_t {
private:
	clock_t last_call;
	int     del;
	int     tdiv;
public:
	Timer_t(int del, const int tdiv) {
		this->del = del;
		this->tdiv = tdiv;
		last_call = clock()/( CLOCKS_PER_SEC / tdiv );
	}

	bool Allow(){
		clock_t this_call = clock()/( CLOCKS_PER_SEC / tdiv );
		if( this_call - last_call >= del ) {
			last_call = this_call;
			return true;
		}
		return false;
	}

	void setDelay(int del) {
		this->del = del;
	}

};


class TimerMS : public Timer_t {
public:
	TimerMS(int ms) : Timer_t(ms,1000) {}
};
#endif