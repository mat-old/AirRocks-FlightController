#ifndef CONNECTPACK_CLASS
#define CONNECTPACK_CLASS
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
class ConnectPack {
	unsigned short port;
	std::string address; /* 123.456.789.012\0 */
public:
	ConnectPack& set(struct sockaddr_in address);
	ConnectPack& setAddress( struct sockaddr_in address );
	ConnectPack& setPort( struct sockaddr_in address );
	ConnectPack& setAddress( std::string addr );
	ConnectPack& setPort( unsigned short p );
	const char * getAddress();
	unsigned short getPort();
};
#endif