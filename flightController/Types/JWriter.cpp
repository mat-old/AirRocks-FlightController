#include "JWriter.hpp"

	bool JWriter::connected;
	int JWriter::soc;
	/* host information */
	struct hostent *JWriter::hp;
	/* server address */
	struct sockaddr_in JWriter::address;

	JWriter::JWriter() {
		pretty = false;
	}
	JWriter::JWriter(bool p) {
		Pretty(p);
	}
	JWriter::~JWriter() {}

	void JWriter::Connect(ConnectPack cp) {
		Connect( cp.getAddress(), cp.getPort() );
	}

	void JWriter::Connect(const char * host, ushort port) {
		soc = socket(AF_INET, SOCK_DGRAM, 0);

		std::memset((char*)&address, 0, sizeof(address));
		address.sin_family = AF_INET;
		address.sin_port   = htons(port);
		
		hp = gethostbyname(host);
		if (!hp)
			throw HOST_ADDRESS_ERR;
		
		std::memcpy((void *)&address.sin_addr, hp->h_addr_list[0], hp->h_length);

		this->connected = true;
	}
	void JWriter::Pretty(bool p) {
		pretty = p;
	}


	void JWriter::Write(std::string s) {
		std::cout << s << std::endl;
		if (sendto(soc, s.data(), s.length(), 0, (struct sockaddr *)&address, sizeof(address)) < 0)
			throw JWRITER_FAILED_SEND;
	}

	template <typename T>
	std::string JWriter::pair( std::string const name, T value) {
		std::ostringstream ss;
		ss << dq << name << dq << ':' << dq << value << dq;
		return ss.str();
	}
	template <typename T>
	std::string JWriter::pair( std::string const name, T value, char fmt) {
		std::ostringstream ss;
		switch( fmt ) {
			case 'F':
			case 'f':
				ss << dq << name << dq << ':' << FLOAT_FORMAT << dq << value << dq;
			break;
			default:
				ss << dq << name << dq << ':' << dq << value << dq;
		}
		return ss.str();
	}
	/* no quote on second arg */
	template <typename T>
	std::string JWriter::pairNQ( std::string const name, T value) {
		std::ostringstream ss;
		ss << dq << name << dq << ':' << value;
		return ss.str();
	}

	template <typename T>
	void JWriter::operator()(std::string const type, T data) {
		std::ostringstream ss;
		ss << jBeg << pair("type", type) << ',' << pair("data",data) << jEnd;
		Write(ss.str());
	}

	void JWriter::operator()(std::string data) {
		JWriter::operator () ("status",data);
	}
	//void operator()(JCommand& cmd) {
		//this->put("processed",(cmd.processed?"true":"false"));
		//ptree p;
		//p.add("name",cmd.name);
		//p.add("val",cmd.getValue());
		//JWriter::operator () ("cmd",p);
	//}
	template <typename T>
	std::string JWriter::array_4T( T v1, T v2, T v3, T v4 ) {
		std::ostringstream ss;
		ss <<'['<<dq<<v1<<dq<<','<<dq<<v2<<dq<<','<<dq<<v3<<dq<<','<<dq<<v4<<dq <<']';
		return ss.str();
	}
	template <typename T>
	std::string JWriter::array_3T( T v1, T v2, T v3 ) {
		std::ostringstream ss;
		ss <<'['<<dq<<v1<<dq<<','<<dq<<v2<<dq<<','<<dq<<v3<<dq<<']';
		return ss.str();
	}

	void JWriter::operator()(Motorgroup& mg) {
		std::ostringstream ss;
		ss
		<< jBeg
		<< pair("type", "motors")
		<< ','
		<< pairNQ("data", array_4T( (int)mg.A(), (int)mg.B(), (int)mg.C(), (int)mg.D()) )
		<< jEnd;
		Write(ss.str());
	}
	void JWriter::operator()(PID_t& pt){
		std::ostringstream ss;
		ss
		<< jBeg
		<< pair("type", "PID")
		<< ','
		<< pair("name", pt.name)
		<< ','
		<< pair("input", pt.last_input, 'f')
		<< ','
		<< pairNQ("data", array_3T( pt.kp, pt.ki, pt.kd ))
		<< jEnd;
		Write(ss.str());
		//ptree p;
		//this->put("name"  , pt.name);
		//this->put("input" , pt.last_input);
		//this->put("output", pt.output);
		//p.add_child("labels",array("sss", "p", "i", "d"));
		//p.add_child("values",array("fff", pt.kp, pt.ki, pt.kd));
		//JWriter::operator () ("PID",p);
	}
	void JWriter::operator()(Potential_t& pot) {
		//ptree p;
		//p.add("name", pot.name);
		//p.add_child("values",array("fff", pot.x, pot.y, pot.z ));
		//JWriter::operator () ("potential",p);
	}
	void JWriter::operator()(Throttle_t& t) {
		//ptree p;
		//p.add("name",t.name);
		//p.add("speed",t.Throttle());
		//JWriter::operator () ("throttle",p);
	}
	/* where, number of extra args, argv... */
	void JWriter::err(const char* what, const int argc, ...) {
		//ptree ary, ret;
		//this->put("what",what);
		//va_list args;
		//va_start(args,argc);
		//for (int i = 0; i < argc; ++i) {
		//	ptree tmp;
		//	ary.push_back( std::make_pair("",tmp.put("",va_arg(args,char*))) );
		//}
		//va_end(args);
		//ret.add_child("list",ary);
		//JWriter::operator () ("error",ret);
	}

	void JWriter::cmd(std::string data, bool processed) {
		//this->put("processed",(processed?"true":"false"));
		//JWriter::operator () ("cmd",data);
	}

	void JWriter::log(std::string s) {
		std::cout << s << std::endl << std::flush;
	}

	void JWriter::log(std::string s, var_float_t t) {
		std::cout << s << " " << t << std::endl << std::flush;
	}
