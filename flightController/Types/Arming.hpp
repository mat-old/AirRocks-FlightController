#ifndef ARMING_FLAG_TYPE
#define ARMING_FLAG_TYPE
class Arming {
private:
	static bool ARM_FLAG;
	static bool RESET_FLAG;
	static bool RESET_HARD_FLAG;
public:
	Arming();
	static void ARM();
	static void DISARM();
	static bool ARMED();

	static void RESET();
	static void UNSET();
	static bool RESETTING();

	static void RESET_HARD();
	static void UNSET_HARD();
	static bool RESETTING_HARD();

	static void clear();
};
#endif