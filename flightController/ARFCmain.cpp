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
		#ifdef MAIN_DEBUG
			cout << "> Entering open control" << endl;
		#endif
		/* wait for all threads to run */
		sleep(1);
		while(imu->Active()) {
			imu->Update     (gyro, accel);
			pid->Calculate  (motors, steering, gyro, accel);
			//spi->Update     (motors);
			relay->Update   (motors, pid);

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