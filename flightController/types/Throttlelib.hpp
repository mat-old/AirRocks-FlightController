class Throttle_t {
public:
	uint8_t control_amount
		  , desired_amount
		  , final_limit;

	Throttle_t(uint8_t climit, uint8_t dlimit) {
		control_amount = climit;
		desired_amount = dlimit;
		final_limit    = climit + dlimit;
	}

	void Control( pid_t ratio ) {
		pid_t throttle = control_amount * 


	}


};