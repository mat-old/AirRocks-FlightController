#include "ConnectPackage.hpp"


	ConnectPack& ConnectPack::set( struct sockaddr_in addr) {
		setAddress(addr);
		setPort(addr);
		return *this;
	}
	ConnectPack& ConnectPack::setAddress( struct sockaddr_in addr ){
		address = inet_ntoa( addr.sin_addr );
		return *this;
	}
	ConnectPack& ConnectPack::setAddress( std::string addr ){
		address = addr;
		return *this;
	}
	ConnectPack& ConnectPack::setPort( unsigned short p ) {
		port = p;
		return *this;
	}
	ConnectPack& ConnectPack::setPort( struct sockaddr_in addr ) {
		port = htons( addr.sin_port );
		return *this;
	}
	const char * ConnectPack::getAddress() {
		return address.data();
	}
	unsigned short ConnectPack::getPort() {
		return port;
	}