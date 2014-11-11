#include "dgramInterface.hpp"



DGRAMinterface& DGRAMinterface::Listen( char * data ) {
	struct sockaddr_in client;
	socklen_t addrlen = sizeof(client); 
	int length = recvfrom(soc, data, UDP_BUF_SIZE, 0, (struct sockaddr *)&client, &addrlen);
	return *this;
}
DGRAMinterface& DGRAMinterface::Connect() {
	if ((soc = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		throw NOENT_SOCKET_ERR;
		Set_Data_Valid(false);
	} 

	memset((char *)&address, 0, sizeof(address));
	address.sin_family      = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY); // htn long
	address.sin_port        = htons(PORT);       // htn short

	if (bind(soc, (struct sockaddr *)&address, sizeof(address)) < 0) {
		throw( BIND_SOCKET_ERR );	
		Set_Data_Valid(false);
	}

	Set_Active(true);
	Set_Data_Valid(true);

	emit( std::string(inet_ntoa( address.sin_addr )) +":"+ std::to_string( htons(address.sin_port) ) );
	//cout << inet_ntoa( address.sin_addr ) << ":" << htons(address.sin_port)<<" "<< address.sin_port << endl;
 
	return *this;
}
bool DGRAMinterface::good() {
	return Active() && Data_Valid() ;
}