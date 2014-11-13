	#include "Modes.hpp"

	Mode::Mode() {
		mode = NO_MODE;
		SHUTDOWN_FLAG = false;
	}

	void Mode::setMode(int i) {
		switch(i) {
			case TEST_MODE:
				emit( "test mode slected" );
				mode = TEST_MODE;
			break;
			case TUNE_MODE:
				emit( "tuner mode slected" );
				mode = TUNE_MODE;
			break;
			case UAV_MODE:
				emit( "UAV mode slected" );
				mode = UAV_MODE;
			break;
			case NO_MODE:
			default:
				mode = NO_MODE;
			break;
		}
	}

	bool Mode::Shutdown() {
		if( SHUTDOWN_FLAG ) {
			ShutdownSequence();
			emit("Shutting down...");
		}
		return SHUTDOWN_FLAG;
	}

	void Mode::ShutdownSequence() {
		emit("Shutdown");
		sleep(3);
	}

	void Mode::UAV(Relay& rel) {


	}
	/* Tuner is only accessable when ARMED */
	void Mode::Tuner(Relay& rel, SPIworker& spi, IMUworker& imu) {
		if( !safety.ARMED() ) {
			emit.log( "Entered Tuner, NOT ARMED" );
			return;
		}
		/*SPIworker  spi;
		IMUworker  imu;*/
		PIDctrl    pid;
		Motorgroup motors;


		try {

			Potential_t   gyro("gyro")
						, accel("accel")
						, steering("steering");

			motors.PID_ratio(PID_RATIO).All(true).Zero();

			/* wait for all threads to run */
			sleep(1);
			emit("READY");
			while(safety.ARMED()) {
				/* fetch data from IMU, transported in Potential_t gyro & accel */
				imu.Update     (gyro, accel);
				/* calculate the adjustments to the motors, factor in steering from IMU data */
				pid.Calculate  (motors, steering, gyro, accel);
				/* send the motor speeds over the SPI */
				spi.Update     (motors);
				/* process new commands, send feedback to base station */
				rel.Process    (motors, pid.getPitch(), pid.getRoll(), pid.getYaw(), gyro, accel);

				if( safety.RESETTING() ) { 
					emit.log( "RESETTING" );
					safety.DISARM();
				}
			}

		}
		catch(std::exception& e) {
			//cout << e.what() << ":: safely disposing objects and exiting..." << endl;
			emit.err("ARFC.main.flightController",2,e.what(),":: safely disposing objects and exiting...");
		}
		return;
	}
