	#include "Modes.hpp"

	Mode::Mode() {
		Drone_mode = NO_MODE;
		SHUTDOWN_FLAG = false;
	}

	void Mode::UAV() {


	}

	void Mode::Tuner() {
		GenericWriter emit;
		SPIworker *spi = new SPIworker();
		IMUworker *imu = new IMUworker();
		Relay     *relay = new Relay();
		PIDctrl    pid;
		Arming     safety;
		Motorgroup motors;

		try {
			emit("initializing... (1)");

			Potential_t   gyro("gyro")
						, accel("accel")
						, steering("steering");

			relay->Start().Detach();
			spi->Open().Zero(); // only line to write to SPI above like 30

			emit("awaiting motor 'ARM' command");
			while( !safety.ARMED() ) { 
				relay->armPending(safety);
				usleep(20000);
			}
			emit("Starting UAV");
			emit("Standby...");

			/* there MUST be no above code to write to the SPI except line 19 */
			spi->Start().Detach();
			imu->Prepare().Start().Detach();
			motors.PID_ratio(PID_RATIO).Zero();

			/* wait for all threads to run */
			sleep(1);
			emit("Flight Controller ready for use");
			while(imu->Active()) {
				/* fetch data from IMU, transported in Potential_t gyro & accel */
				imu->  Update     (gyro, accel);
				/* calculate the adjustments to the motors, factor in steering from IMU data */
				pid.   Calculate  (motors, steering, gyro, accel);
				/* send the motor speeds over the SPI */
				spi->  Update     (motors);
				/* process new commands, send feedback to base station */
				relay->Process    (motors, pid, gyro, accel, safety);
			}
		}
		catch(std::exception& e) {
			//cout << e.what() << ":: safely disposing objects and exiting..." << endl;
			emit.err("ARFC.main.flightController",2,e.what(),":: safely disposing objects and exiting...");
		}
		delete imu;
		delete spi;
		delete relay;
		//cout << "> Exit(1)::\n" << endl;
	}
