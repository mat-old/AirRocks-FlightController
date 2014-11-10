#ifndef ARMING_FLAG_TYPE
#define ARMING_FLAG_TYPE
class Arming {
private:
	bool ARM_FLAG;
public:
	Arming();
	void ARM();
	void DISARM();
	bool ARMED();
};
#endif