#ifndef UDP_DATAGRAM_INTERFACE
#define UDP_DATAGRAM_INTERFACE

#include "../Defines.hpp"

#include "../Cores/AsyncWorker.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>
#include <cstring>
/*
	this listen - reserves the socket
	bool trypeek- checks if the socket is free
	this connect- waits for a UDP message
*/
class DGRAMinterface : public AsyncWorker {
private:
	int soc, data_length;
	struct sockaddr_in address;
protected:
	DGRAMinterface();
	DGRAMinterface& Listen(char * buffer);
	DGRAMinterface& Connect();
	ConnectPack& getHandshake();
	bool good();
public:

};
#endif