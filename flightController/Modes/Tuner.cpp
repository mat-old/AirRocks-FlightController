#include "../Defines.hpp"
void UAV() {

	SPIworker *spi = new SPIworker();
	IMUworker *imu = new IMUworker();
	PIDctrl   *pid = new PIDctrl();
	Relay     *relay = new Relay();
	Motorgroup motors;

	try {
		emit("initializing... (1)");

		Potential_t   gyro("gyro")
					, accel("accel")
					, steering("steering");

		relay->Start().Detach();
		spi->Open().Zero();

		emit("awaiting motor ARM command");
		while( relay->Disarmed() ) { 
			relay->Transact   (motors, pid, gyro, accel);
			sleep(1);
		}

		spi->Start().Detach();
		imu->Prepare().Start().Detach();
		motors.PID_ratio(Defines::PID_RATIO).Zero();

		/* wait for all threads to run */
		sleep(1);
		emit("Flight Controller ready for use");
		while(imu->Active()) {
			/* fetch data from IMU, transported in Potential_t gyro & accel */
			imu->  Update     (gyro, accel);
			/* calculate the adjustments to the motors, factor in steering from IMU data */
			pid->  Calculate  (motors, steering, gyro, accel);
			/* send the motor speeds over the SPI */
			spi->  Update     (motors);
			/* process new commands, send feedback to base station */
			relay->Transact   (motors, pid, gyro, accel);
		}
	}
	catch(std::exception& e) {
		//cout << e.what() << ":: safely disposing objects and exiting..." << endl;
		emit.err("ARFC.main.flightController",2,e.what(),":: safely disposing objects and exiting...");
	}
	delete imu;
	delete spi;
	delete pid;
	delete relay;
	//cout << "> Exit(1)::\n" << endl;
}
