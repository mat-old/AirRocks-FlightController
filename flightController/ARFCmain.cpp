#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include "errorMap.hpp"
#include "subSystem.hpp"
#include "spiWorker.hpp"
#include "arfcDefines.hpp"
using namespace std;

int main(int argc, char const *argv[])
{
	SPIworker *spi = new SPIworker();
	PIDctrl   *pid = new PIDctrl();

	cout << endl;
	try {
		uint8_t speedBuf[Def::ioMsg_Length] = {Def::MOTOR_ZERO_LEVEL,};
		spi->Open().Start().Detach().Update(speedBuf);	

		while(true) {
			spi->Update(speedBuf);
			sleep(1);
			char i;
			cin >> i;
		}	
	}
	catch(exception& e) {
		cout << e.what() << " occured disposing objects and exiting..." << endl;
	}
	delete spi;
	delete pid;
	exit(1);
	return 0;
}