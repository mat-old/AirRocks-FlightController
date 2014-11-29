all: testing

test:
	g++  SPIworkertest.cpp -std=c++11 -pthread

testing: test.o spi.o errmap.o arfcdefines.o subsystem.o
	g++ test.o spi.o errmap.o arfcdefines.o subsystem.o   -o run.o 

test.o:
	g++ -c SPIworkertest.cpp -std=c++11

spi.o:
	g++ -c spiWorker.hpp -pthread -std=c++11

errmap.o:
	g++ -c errorMap.hpp 

arfcdefines.o:
	g++ -c arfcDefines.hpp

subsystem.o:
	g++ -c subSystem.hpp 

clean: 
	rm -rf *.o 