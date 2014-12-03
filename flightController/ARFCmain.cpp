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
	//PIDctrl   *pid = new PIDctrl();

	try {
		#ifdef MAIN_DEBUG
			cout << "> Initializing..." << endl;
		#endif
		uint8_t speedBuf[Def::ioMsg_Length] = {Def::MOTOR_ZERO_LEVEL,};
		Potential gyro, accel;
		spi->Open().Start().Detach();
		imu->Prepare().Start().Detach();
		#ifdef MAIN_DEBUG
			cout << "> Entering open control" << endl;
		#endif
		/* wait for all threads to run */
		sleep(1);
		while(imu->Active()) {
			spi->Update(speedBuf);
			imu->Update(gyro, accel);


			cout <<'\r'<<accel<<"          "<<flush;

			//sleep(1);
			//char i;
			//cin >> i;
		}
		throw UNREACHABLE;
	}
	catch(exception& e) {
		cout << e.what() << ":: safely disposing objects and exiting..." << endl;
	}
	delete imu;
	delete spi;
	//delete pid;
	cout << "> Exit(1)::\n" << endl;
	exit(1);
	return 0;
}