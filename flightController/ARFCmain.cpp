#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include "errorMap.hpp"
#include "subSystem.hpp"
#include "spiWorker.hpp"
#include "arfcDefines.hpp"
#include "pidCtrl.hpp"
#include "imuWorker.hpp"
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

	try {
		#ifdef MAIN_DEBUG
			cout << "> Initializing..." << endl;
		#endif

		Potential gyro
				, accel
				, steering;
		uint8_t   speedBuf[Def::ioMsg_Length] = {Def::MOTOR_ZERO_LEVEL,}
				, throttle = Def::MOTOR_ZERO_LEVEL;

		spi->Open().Start().Detach();
		imu->Prepare().Start().Detach();
		pid->Use(speedBuf);
		#ifdef MAIN_DEBUG
			cout << "> Entering open control" << endl;
		#endif
		/* wait for all threads to run */
		sleep(1);
		while(imu->Active()) {

			unsigned long loop = Def::millis();

			imu->Update     (gyro, accel);
			pid->Calculate  (throttle, steering, gyro, accel);
			//spi->Update     (speedBuf);


				// << "  " << (Def::millis() - loop) << endl;
				// the loop is roughly an entire milli second WOO!



		}
		throw UNREACHABLE;
	}
	catch(exception& e) {
		cout << e.what() << ":: safely disposing objects and exiting..." << endl;
	}
	delete imu;
	delete spi;
	delete pid;
	cout << "> Exit(1)::\n" << endl;
	exit(1);
	return 0;
}