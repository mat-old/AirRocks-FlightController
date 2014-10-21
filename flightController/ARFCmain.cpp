#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include "errorMap.hpp"
#include "subSystem.hpp"
#include "spiWorker.hpp"
#include "arfcDefines.hpp"
#include "pidCtrl.hpp"
#include "imProvider.hpp"
using namespace std; 

int main(int argc, char const *argv[]) {
	cout << endl;
	//SPIworker *spi = new SPIworker();
	IMprovider*imp = new IMprovider();
	//PIDctrl   *pid = new PIDctrl();

	try {
		uint8_t speedBuf[Def::ioMsg_Length] = {Def::MOTOR_ZERO_LEVEL,};
		//spi->Open().Start().Detach().Update(speedBuf);
		imp->Open();

		while(true) {
			//spi->Update(speedBuf);
			sleep(1);
			char i;
			cin >> i;
		}	
	}
	catch(exception& e) {
		cout << e.what() << ":: safely disposing objects and exiting..." << endl;
	}
	delete imp;
	//delete spi;
	//delete pid;
	exit(1);
	return 0;
}