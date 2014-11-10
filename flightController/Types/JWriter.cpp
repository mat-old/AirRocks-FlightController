#include "JWriter.hpp"

	void JWriter::Write() {
		write_json(s,t,pretty);
		std::cout << s.str() << std::endl << std::flush;
		s.str("");
		s.clear();
		t.clear();
	}
	ptree JWriter::array(const char* fmt...) {
		ptree ary;
		va_list args;
		va_start(args, fmt);
		while (*fmt != '\0') {
			ptree tmp;
			if (*fmt == 'd') {
				int i = va_arg(args, int);
				tmp.put<int>("",i);
			} else if (*fmt == 'c') {
				int c = va_arg(args, int);
				tmp.put<char>("",static_cast<char>(c));
			} else if (*fmt == 'f') {
				double d = va_arg(args, double);
				tmp.put<double>("",d);
			} else if (*fmt == 's') {
				tmp.put("",va_arg(args,char*));
			}
			++fmt;
			ary.push_back(std::make_pair("",tmp));
		}
		va_end(args);
		return ary;
	}
	template <typename T>
	void JWriter::put(const std::string name, T val){
		t.put(name,val);
	}
	JWriter::JWriter()  {	pretty = false;	}
	JWriter::JWriter(bool p)  {	Pretty(p);	}
	JWriter::~JWriter() {}
	template <typename T>
	void JWriter::operator()(std::string const type, T data) {
		t.put("type",type);
		t.put("data",data);
		Write();
	}
	void JWriter::operator()(std::string const type, ptree child) {
		t.put("type",type);
		t.add_child("data",child);
		Write();
	}
	void JWriter::Pretty(bool p) {
		pretty = p;
	}


/* GENERIC WRITER */
/* GENERIC WRITER */
/* GENERIC WRITER */
/* GENERIC WRITER */
/* GENERIC WRITER */
	void GenericWriter::operator()(std::string data) {
		JWriter::operator () ("status",data);
	}
	//void operator()(JCommand& cmd) {
		//this->put("processed",(cmd.processed?"true":"false"));
		//ptree p;
		//p.add("name",cmd.name);
		//p.add("val",cmd.getValue());
		//JWriter::operator () ("cmd",p);
	//}
	void GenericWriter::operator()(Motorgroup& mg) {
		JWriter::operator () ("motors",array("dddd",mg.A(),mg.B(),mg.C(),mg.D()));
	}
	void GenericWriter::operator()(PID_t& pt){
		ptree p;
		this->put("name"  , pt.name);
		this->put("input" , pt.last_input);
		this->put("output", pt.output);
		p.add_child("labels",array("sss", "p", "i", "d"));
		p.add_child("values",array("fff", pt.kp, pt.ki, pt.kd));
		JWriter::operator () ("PID",p);
	}
	void GenericWriter::operator()(Potential_t& pot) {
		ptree p;
		p.add("name", pot.name);
		p.add_child("values",array("fff", pot.x, pot.y, pot.z ));
		JWriter::operator () ("potential",p);
	}
	void GenericWriter::operator()(Throttle_t& t) {
		ptree p;
		p.add("name",t.name);
		p.add("speed",t.Throttle());
		JWriter::operator () ("throttle",p);
	}
	/* where, number of extra args, argv... */
	void GenericWriter::err(const char* what, const int argc, ...) {
		ptree ary, ret;
		this->put("what",what);
		va_list args;
		va_start(args,argc);
		for (int i = 0; i < argc; ++i) {
			ptree tmp;
			ary.push_back( std::make_pair("",tmp.put("",va_arg(args,char*))) );
		}
		va_end(args);
		ret.add_child("list",ary);
		JWriter::operator () ("error",ret);
	}

	void GenericWriter::cmd(std::string data, bool processed) {
		this->put("processed",(processed?"true":"false"));
		JWriter::operator () ("cmd",data);
	}
