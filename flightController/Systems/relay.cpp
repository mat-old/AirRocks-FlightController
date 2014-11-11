#include "relay.hpp"
	Relay::Relay()  {
		action["set"]            = AC_set;
		action["Mode-select"]    = AC_mode_select;
		action["Throttle-arm"]   = AC_throttle_arm;
		action["Throttle-start"] = AC_throttle_start;
		action["Throttle-stop"]  = AC_throttle_stop;
		action["Throttle-Torque"]= AC_throttle_torque;
		action["Pitch-activate"] = AC_pitch_activate;
		action["Pitch-reset"]    = AC_pitch_reset;
		action["Pitch-save"]     = AC_pitch_save;
		action["Pitch-P"]        = AC_pitch_p;
		action["Pitch-I"]        = AC_pitch_i;
		action["Pitch-P"]        = AC_pitch_d;
		action["Roll-activate"]  = AC_roll_active;
		action["Roll-reset"]     = AC_roll_reset;
		action["Roll-save"]      = AC_roll_save;
		action["Roll-P"]         = AC_roll_p;
		action["Roll-I"]         = AC_roll_i;
		action["Roll-D"]         = AC_roll_d;
		action["Yaw-activate"]   = AC_yaw_active;
		action["Yaw-reset"]      = AC_yaw_reset;
		action["Yaw-save"]       = AC_yaw_save;
		action["Yaw-P"]          = AC_yaw_p;
		action["Yaw-I"]          = AC_yaw_i;
		action["Yaw-D"]          = AC_yaw_d;
		action["err"]            = AC_err;

		AC_start = action.begin();
		AC_end   = action.end();
		/*for initial disarmed state*/
		Set_Active(false);
		AC_tuneing_state = AC_inactive;
		/* TODO: update this for performance */
		//setDelay(10);
	}
	Relay::~Relay() {}

	void Relay::Process(Motorgroup  & m
				, PIDctrl    & p
				, Potential_t& g
				, Potential_t& a 
				, Arming     & s) {
		// process always 
		if( !post_parse.empty() )
			Update(m, p, s);
		// feedback sometimes 
		if( !timer->Allow() ) return;

		if( Data_Valid() ) {
			Set_Data_Valid(false);
			emit(m);
			emit(p.getPitch());
			emit(p.getRoll());
			emit(p.getYaw());
			emit(g);
			emit(a);
		}
	}


	void Relay::Update(Motorgroup& motors, PIDctrl& P, Arming& safety ) {
		if( access.try_lock() ) {
			/*TODO, test the reliability of this code, remove try*catch() */
			/*TODO, test performance of .begin vs algorithm's begin(std::alloc*) */
			const std::vector<JCommand>::iterator
				start = post_parse.begin()
			  , end   = post_parse.end();
			 
			std::vector<JCommand>::iterator cursor = start;

			
			var_float_t val;
			try {
				for(;cursor != end; ++cursor) {
					//if( !jco.tryParse( *cursor ) ) continue;
					switch( getActionCode( cursor->Action() ) ) {
						case AC_set :
							val = cursor->Value();
							if( val < 0 )
								break;
							switch( getActionCode( cursor->Name() ) ) {
								case AC_pitch_p : P.getPitch().setP(val); break;
								case AC_pitch_i : P.getPitch().setI(val); break;
								case AC_pitch_d : P.getPitch().setD(val); break;
								case AC_roll_p : P.getRoll().setP(val); break;
								case AC_roll_i : P.getRoll().setI(val); break;
								case AC_roll_d : P.getRoll().setD(val); break;
								case AC_yaw_p : P.getYaw().setP(val); break;
								case AC_yaw_i : P.getYaw().setI(val); break;
								case AC_yaw_d : P.getYaw().setD(val); break;
								default:
								break;
							}
							break;
						case AC_throttle_arm :
							safety.ARM();
							break;
						case AC_throttle_start :
							setActiveTuner( AC_tuneing_state, motors );
							break;
						case AC_throttle_stop :
							powerDown(motors);
							break;
						case AC_throttle_torque :
							if( safety.ARMED() )
								motors.All( val );
							break;
						case AC_pitch_activate :
							setActiveTuner( AC_pitch_activate, motors );
							break;
						case AC_pitch_reset :
							break;
						case AC_pitch_save :
							break;
						case AC_roll_active :
							setActiveTuner( AC_roll_active, motors );
							break;
						case AC_roll_reset :
							break;
						case AC_roll_save :
							break;
						case AC_yaw_active :
							setActiveTuner( AC_yaw_active, motors );
							break;
						case AC_yaw_reset :
							break;
						case AC_yaw_save :
							break;
						case AC_err :
							emit.err("Tuner",1,"Unrecognized");
						default:
						break;
					}
				}
			} catch(ERR_CODES e) {
				err.Response(e);
			}
			post_parse.erase(start,cursor);
			access.unlock();
			Set_Data_Valid(true);
		}
	}

	void Relay::setActiveTuner( AC_action_codes tuner, Motorgroup& motors) {
		powerDown(motors);
		bool unset = tuner == AC_tuneing_state;
		if( AC_tuneing_state != tuner ) {
			switch( tuner ) {
				case AC_pitch_activate : 
						motors.PitchOnly();
					break;
				case AC_roll_active : 
						motors.RollOnly();
					break;
				case AC_yaw_active : 
						motors.YawOnly();
					break;
			}
			AC_tuneing_state = tuner;
		}
		else AC_tuneing_state = AC_inactive;
	}

	bool Relay::inactive() {
		return AC_tuneing_state == AC_inactive;
	}

	void Relay::powerDown(Motorgroup& motors) {
		emit("Throttle-Torque-reset");
		motors.Zero();
		motors.All(false);
	}

	AC_action_codes Relay::getActionCode(std::string s) {
		std::map<std::string, AC_action_codes>::iterator cursor = action.find( s );
		return  cursor==AC_end?AC_err:cursor->second;
	}

	void Relay::lockIfDark() {
		if( !good() ) {
			while(true) {
				sleep(2);
				emit.err("Relay",1,"Failed to get coms...");
			}
		}		
	}

	void Relay::waitForARM( Arming& safety ) {
		lockIfDark();
		
		JCommand jco; 
		/* while not armed */
		while(!safety.ARMED()) {
			/* listen to the socket */   
			Listen();
			if( !jco.tryParse( data )  ) continue;
			/* if its the arm signal, arm the UAV's safety */
			if( getActionCode( jco.Action() ) == AC_throttle_arm ) {
				emit("'ARM' signal received");
				safety.ARM();
			}
		}
		emit("ARMED");
	}

	int Relay::waitFor( AC_action_codes code ) {
		lockIfDark();

		JCommand jco; 
		/* while not armed */
		while(true) {
			/* listen to the socket */   
			Listen();
			if( !jco.tryParse( data )  ) continue;
			/* if its the arm signal, arm the UAV's safety */
			if( getActionCode( jco.Action() ) == code ) {
				return jco.Value();
			}
		}
	}

	void *Relay::worker_run() {
		JCommand jco; 
		while(true) {
			//emit("processing...");
			Listen();
			if( jco.tryParse( data ) ) {
				access.lock();
				post_parse.push_back( jco );			
				access.unlock();
			}
			if( Disposed() ) break;
		}
		return 0;
	}