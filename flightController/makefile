CC=g++
CFLAGS= -std=c++0x -pthread
LFLAGS=-lpthread
SOURCES= ARFCmain.cpp arfcDefines.hpp dataTypes.hpp errorMap.hpp subSystem.hpp imuInterface.hpp imuWorker.hpp pidCtrl.hpp spiWorker.hpp
OBJECTS=$(SOURCES:.hpp=.o)
EXEC=ARFCapp.o

#all: $(SOURCES) $(EXEC)

all: ARFCmain.cpp
	$(CC) $(CFLAGS) ARFCmain.cpp -o $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

.hpp.o:
	$(CC) $(CFLAGS) $< -o $@ 


