#include "Arming.hpp"
	Arming::Arming()     { ARM_FLAG = false; }
	void Arming::ARM()   { ARM_FLAG = true;  }
	void Arming::DISARM(){ ARM_FLAG = false; }
	bool Arming::ARMED() { return ARM_FLAG;  }