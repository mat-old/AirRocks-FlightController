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

	spi->Start().Detach();

	sleep(4);
	
	cout << endl;
	try {
		uint8_t speedBuf[Def::ioMsg_Length] = {0u,};
		spi->Open().Start().Detach();	


		while(true) {
			uint8_t n = 1;
			cin >> n;
			if( n == 0 ) throw 0xFF;
			speedBuf[0] = n;
			speedBuf[1] = n;
			speedBuf[2] = n;
			speedBuf[3] = n;
			spi->Update(speedBuf);
		}	
	}
	catch(exception& e) {
		cout << e.what() << " occured disposing objects and exiting..." << endl;

		delete spi;
		exit(1);
	}
	return 0;
}