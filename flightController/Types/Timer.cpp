	#include "Timer.hpp"
	Timer_t::Timer_t(int del, const int tdiv) {
		this->del = del;
		this->tdiv = tdiv;
		last_call = clock()/( CLOCKS_PER_SEC / tdiv );
	}

	bool Timer_t::Allow(){
		clock_t this_call = clock()/( CLOCKS_PER_SEC / tdiv );
		if( this_call - last_call >= del ) {
			last_call = this_call;
			return true;
		}
		return false;
	}

	void Timer_t::setDelay(int del) {
		this->del = del;
	}