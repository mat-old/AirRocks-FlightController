

#include <iostream>
#include <unistd.h>

#include "errorMap.hpp"
#include "subSystem.hpp"
#include "spiWorker.hpp"
#include "arfcDefines.hpp"
using namespace std;


int main(int argc, char const *argv[])
{
	SPIworker *spi = new SPIworker();

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