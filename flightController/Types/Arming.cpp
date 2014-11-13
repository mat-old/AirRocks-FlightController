#include "Arming.hpp"
	bool Arming::ARM_FLAG;
	bool Arming::RESET_FLAG;
	bool Arming::RESET_HARD_FLAG;

	Arming::Arming()     {
		clear();
	}
	void Arming::ARM()   { ARM_FLAG = true;  }
	void Arming::DISARM(){ ARM_FLAG = false;  }
	bool Arming::ARMED() { return ARM_FLAG;  }

	void Arming::RESET()      { RESET_FLAG = true; }
	void Arming::UNSET()      { RESET_FLAG = false;  }
	bool Arming::RESETTING()  { return RESET_FLAG;  }

	void Arming::RESET_HARD()     { RESET_HARD_FLAG = true;  }
	void Arming::UNSET_HARD()     { RESET_HARD_FLAG = false; }
	bool Arming::RESETTING_HARD() { return RESET_HARD_FLAG;  }

	void Arming::clear() {
		ARM_FLAG = false;
		RESET_FLAG = false;
		RESET_HARD_FLAG = false;
	}