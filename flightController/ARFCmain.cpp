#include "Includes.hpp"

using namespace Defines;

void flightController() {

	SPIworker *spi = new SPIworker();
	IMUworker *imu = new IMUworker();
	PIDctrl   *pid = new PIDctrl();
	Relay     *relay = new Relay();
	Motorgroup motors;

	try {
		emit("initializing...");

		Potential_t   gyro
					, accel
					, steering;

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
	}
	delete imu;
	delete spi;
	delete pid;
	delete relay;
	//cout << "> Exit(1)::\n" << endl;
}

int main(int argc, char const *argv[]) {

	test(); /*test emit for each type, make sure it can be de-serialized*/

	return 0;

	while(true) {
		flightController();
	}

	exit(1);
	return 0;
}