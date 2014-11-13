#ifndef JWRITER 
#define JWRITER
//#include <cstdlib>   // rand
#include <cstdarg>
#include <sstream>
#include <cstring>
//#include <stdlib.h> // rand
#include <iostream>
//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/json_parser.hpp>

#include "../Defines.hpp"
#include "Motorgroup.hpp"
#include "Throttle.hpp"
#include "PID.hpp"
#include "Potential.hpp"
#include "ConnectPackage.hpp"


#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <iomanip>
#define FLOAT_FORMAT std::fixed << std::setprecision(3) << std::setw(6)
#define jBeg '{'
#define jEnd '}'
#define dq   '"'


/*
	do to the swp{b} symbol, at runtime the ret_ptr was corrupted
	boost::write_json cannot be used on arm-V5+
 */
//using boost::property_tree::write_json;
//using boost::property_tree::ptree;

class JWriter {
private:
	//std::ostringstream s;
	bool pretty; /* FALSE in production */
	static bool connected;
	static int soc;
	/* host information */
	static struct hostent *hp;
	/* server address */
	static struct sockaddr_in address;
protected:
	void Write(std::string s);

	template <typename T>	
	std::string array_4T( T, T, T, T );
	template <typename T>	
	std::string array_3T( T, T, T );
	//template <typename T>
	//void put(const std::string name, T val);

	/* no quote on second arg */
	template <typename T>
	std::string pairNQ( std::string const name, T value);

	template <typename T>
	std::string pair(std::string name, T value);

	template <typename T>
	std::string pair( std::string const name, T value, char fmt);
public:
	/* acquires socket */
	/* host must be a string like "192.168.10.13" */
	void Connect(const char * host, ushort port);
	void Connect(ConnectPack clientPack);
	JWriter();
	JWriter(bool p);
	~JWriter();
	void Pretty(bool p); 

	/*  { type:name , data: simple }  */
	template <typename T>
	void operator()(std::string const type, T data);

	/*  { type:name : data { complex } }  */
	//void operator()(std::string const type, ptree child);

	/*  { type:status, data:"s" }  */
	void operator()(std::string data);

	/*  { type:motorgroup, data: [ f, f, f, f ] }  */
	void operator()(Motorgroup& mg);

	/*  { type:PID, name:'s', input:'f', output:'f', data: { labels:[ s,s,s ],values:[ f, f, f ] }  }  */
	void operator()(PID_t& pt);

	/*  { type:'potential', data:{ name:'', values:[ float, float, float ] } }  */
	void operator()(Potential_t& pot);

	/*  { type:'throttle', data:{ name:'',speed:'' } }  */
	void operator()(Throttle_t& t);

	/* title, format, args... */
	void err(const char* what, const int argc, ...);
	/* use as promise? */
	void cmd(std::string data, bool processed);

	void log(std::string s);

	void log(std::string s, var_float_t t);
};
#endif