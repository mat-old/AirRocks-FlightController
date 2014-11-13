#include "relay.hpp"
	Relay::Relay()  {
		action["set"]            = AC_set;
		action["reset"]          = AC_reset; 
		action["reset-hard"]     = AC_reset_HARD;
		action["handshake"]      = AC_handshake;
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
		delayTimer = new TimerMS(delayTimerMS);
		/*for initial disarmed state*/
		Set_Active(true);
		AC_tuneing_state = AC_inactive;
		/* TODO: update this for performance */
		//setDelay(10);
	}
	Relay::~Relay() {
		Dispose();
	}

	void Relay::Process(Motorgroup& m
				, PID_t& Pitch
				, PID_t& Roll
				, PID_t& Yaw
				, Potential_t& g
				, Potential_t& a ) {
		// process always 
		if( !post_parse.empty() )
			Update(m, Pitch, Roll, Yaw);
		// feedback sometimes 

		

		if( Data_Valid() || delayTimer->Allow() ) {
			Set_Data_Valid(false);
			emit(m);
			emit(Pitch);
			emit(Roll);
			emit(Yaw);
			//emit(g);
			//emit(a);
		}
	}


	void Relay::Update(Motorgroup& motors, PID_t& Pitch, PID_t& Roll, PID_t& Yaw ) {
		if( access.try_lock() ) {
			//emit( "in lock"  );

			/*TODO, test the reliability of this code, remove try*catch() */
			/*TODO, test performance of .begin vs algorithm's begin(std::alloc*) */
			const std::vector<JCommand>::iterator
				start = post_parse.begin()
			  , end   = post_parse.end();
			 
			std::vector<JCommand>::iterator cursor = start;

			
			var_float_t val;
			try {
				for(;cursor != end; ++cursor) {
					//emit( "updating " + cursor->Action()  );
					//if( !jco.tryParse( *cursor ) ) continue;
					switch( getActionCode( cursor->Action() ) ) {
						case AC_set :
							val = cursor->Value();
							if( val < 0 )
								break;
							switch( getActionCode( cursor->Name() ) ) {
								case AC_pitch_p : Pitch.setP(val); break;
								case AC_pitch_i : Pitch.setI(val); break;
								case AC_pitch_d : Pitch.setD(val); break;
								case AC_roll_p : Roll.setP(val); break;
								case AC_roll_i : Roll.setI(val); break;
								case AC_roll_d : Roll.setD(val); break;
								case AC_yaw_p : Yaw.setP(val); break;
								case AC_yaw_i : Yaw.setI(val); break;
								case AC_yaw_d : Yaw.setD(val); break;
								case AC_throttle_torque :
									if( safety.ARMED() ) {
										//emit.log( "throttle", val );
										motors.All( val / 100.0f );
									}
									break;
								default:
								break;
							}
							break;
						case AC_reset_HARD:
							emit.log(" HARD Reset");
							safety.RESET_HARD();
						case AC_reset:
							emit.log(" Reset");
							safety.RESET();
							safety.DISARM();
							break;
						case AC_throttle_arm :
							safety.ARM();
							break;
						case AC_throttle_start :
							//setActiveTuner( AC_tuneing_state, motors );
							motors.All(true);
							break;
						case AC_throttle_stop :
							powerDown(motors);
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
							//emit.err("Tuner",1,"Unrecognized");
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
		//bool unset = tuner == AC_tuneing_state;
		if( AC_tuneing_state != tuner ) {
			switch( tuner ) {
				case AC_pitch_activate : 
						emit("Pitch-activated");
						motors.PitchOnly();
					break;
				case AC_roll_active : 
						emit("Roll-activated");
						motors.RollOnly();
					break;
				case AC_yaw_active : 
						emit("Yaw-activated");
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
				emit.log("Relay:: Failed to get coms...");
			}
		}		
	}

	void Relay::waitForARM() {
		lockIfDark();

		emit("Waiting for arm signal");
		waitFor( AC_throttle_arm );
		safety.ARM();
		emit("ARMED");
	}

	int Relay::waitFor( AC_action_codes code ) {
		lockIfDark();
		/* while not armed */
		while(true) {

			if( !delayTimer->Allow() ) {
				usleep(100);
				continue;
			}

			if( post_parse.empty() ) continue;

			access.lock();
			std::vector<JCommand>::iterator
				start = post_parse.begin()
			  , end   = post_parse.end();
			 
			std::vector<JCommand>::iterator cursor = start;

			for( ;end != cursor; ++cursor ) {
				if ( getActionCode( cursor->Action() ) == code ) {
					// if found, copy, clear, unlock, return
					int v = cursor->Value();
					post_parse.clear();
					access.unlock();
					return v;
				}
			}
			/* if not found clean and unlock */
			post_parse.clear();
			access.unlock();
		}
		return 0;
	}

	void Relay::ListenForHandshake() {
		lockIfDark();
		JCommand jco; 
		/* 
			waiting for a object with
			name = client IP
			value = port
			{ action:"handshake", name:"0.0.0.0", value:5000 }
		*/
		while( true ) {
			emit.log("waiting for handshake...");
			Listen();
			if( !jco.tryParse( data ) ) continue;
			if( getActionCode( jco.Action() ) != AC_handshake ) continue;

			ConnectPack cp;
			cp.setAddress( jco.Name() );
			cp.setPort( (ushort)jco.Value('u') );

			emit.log( cp.getAddress() );
			emit.log( std::to_string( cp.getPort() ) ) ;

			emit.Connect( cp );
			try {
				emit("Connected");
				return;
			} catch( ERR_CODES e ) {
				err.Response( e );
			}
		}
	}


	void *Relay::worker_run() {
		JCommand jco; 
		while(true) {
			Listen();
			//emit.log( data );
			if( jco.tryParse( data ) ) {
				access.lock();
				post_parse.push_back( jco );
				emit.log( data );			
				//emit("command accepted " + std::to_string(post_parse.size()));
				access.unlock();
			}
			if( Disposed() ) break;
		}
		return 0;
	}