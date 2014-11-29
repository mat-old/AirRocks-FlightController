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


	uint64_t x = spi->testUp();

	for (int i = 0; i < 8; ++i)
	{
		uint8_t t = x >> (i*8u);
		printf("%x",t);
	}
	cout << endl;
	try {
		spi->Open();		
	}
	catch(exception& e) {
		cout << e.what() << " occured disposing objects and exiting..." << endl;

		delete spi;
		exit(1);
	}
	return 0;
}