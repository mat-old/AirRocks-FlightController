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
	Timer_t(int del, const int tdiv);
	bool Allow();
	void setDelay(int del);
};


class TimerMS : public Timer_t {
public:
	TimerMS(int ms) : Timer_t(ms,1000) {}
};
#endif