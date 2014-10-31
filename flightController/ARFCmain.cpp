#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include "errorMap.hpp"
#include "subSystem.hpp"
#include "spiWorker.hpp"
#include "arfcDefines.hpp"
#include "imuWorker.hpp"
#include "pidCtrl.hpp"
#include "relay.hpp"
#include "dataTypes.hpp"

#define MAIN_DEBUG
#ifdef MAIN_DEBUG
#endif
using namespace std; 

int main(int argc, char const *argv[]) {
	cout << endl;
	SPIworker *spi = new SPIworker();
	IMUworker *imu = new IMUworker();
	PIDctrl   *pid = new PIDctrl();
	Relay     *relay = new Relay();
	Motorgroup motors;

	try {
		#ifdef MAIN_DEBUG
			cout << "> Initializing..." << endl;
		#endif

		Potential_t   gyro
					, accel
					, steering;

		spi->Open().Start().Detach();
		imu->Prepare().Start().Detach();
		motors.PID_ratio(Def::PID_RATIO).Zero();
		relay->Start().Detach();
		#ifdef MAIN_DEBUG
			cout << "> Entering open control" << endl;
		#endif
		/* wait for all threads to run */
		sleep(1);
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
		throw UNREACHABLE;
	}
	catch(exception& e) {
		cout << e.what() << ":: safely disposing objects and exiting..." << endl;
	}
	delete imu;
	delete spi;
	delete pid;
	delete relay;
	cout << "> Exit(1)::\n" << endl;
	exit(1);
	return 0;
}