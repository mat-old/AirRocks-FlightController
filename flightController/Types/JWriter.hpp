#ifndef JWRITER 
#define JWRITER
#include <iostream>
#include <cstdarg>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "../Includes.hpp"

using boost::property_tree::ptree;
using boost::property_tree::write_json;

class JWriter {
private:
	ptree t;
	std::ostringstream s;
	bool pretty;
protected:
	void Write() {
		write_json(s,t,pretty);
		std::cout << s.str();
		s.str("");
		s.clear();
		t.clear();
	}
	ptree array(const char* fmt...) {
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

public:
	JWriter()  {	pretty = false;	}
	JWriter(bool p)  {	Pretty(p);	}
	~JWriter() {}
	template <typename T>
	void operator()(std::string const type, T data) {
		t.put("type",type);
		t.put("data",data);
		Write();
	}
	void operator()(std::string const type, ptree child) {
		t.put("type",type);
		t.add_child("data",child);
		Write();
	}
	void Pretty(bool p) {
		pretty = p;
	}
};

class GenericWriter : public JWriter {
public:
	void operator()(std::string data) {
		JWriter::operator () ("status",data);
	}
	void operator()(Command& cmd) {
		ptree p;
		p.add("name",cmd.name);
		p.add("val",cmd.getValue());
		JWriter::operator () ("cmd",p);
	}
	void operator()(Motorgroup& mg) {
		JWriter::operator () ("motors",array("ffff",mg.A(),mg.B(),mg.C(),mg.D()));
	}
	void operator()(PID_t& pt){
		ptree p;
		p.add_child("labels",array("ssss", "output", "p", "i", "d"));
		p.add_child("values",array("ffff", pt.output, pt.kp, pt.ki, pt.kd));
		p.add("name",pt.name);
		JWriter::operator () ("PID",p);
	}
	void operator()(Potential_t& pot) {
		ptree p;
		p.add("name", pot.name);
		p.add_child("values",array("fff", pot.x, pot.y, pot.z ));
		JWriter::operator () ("potential",p);
	}
	void operator()(Throttle_t& t) {
		ptree p;
		p.add("name",t.name);
		p.add("speed",t.Throttle());
		JWriter::operator () ("throttle",p);
	}
	void err( int argc, ...) {
		ptree ary, ret;

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
};

void test() {
	GenericWriter writer;
	writer.Pretty(true);
	int start = 4;
	for (int i = start; i < 12; ++i)
	switch(i){
		case 0:{
			Command cmd1("magic:54444.1");
			writer(cmd1);
		}
		case 1:{
			Motorgroup mg;
			mg.All(0.8);
			writer(mg);
		}
		case 2:{
			PID_t pt;
			pt.name = "cool axis";
			pt.set_point = 9.9;
			pt.SetPID(11.1,8.2,2.2);
			pt.Compute(7.6);
			writer(pt);
		}
		case 3:{
			Potential_t pot("gyro");
			pot.x = 9.9;
			pot.y = 8.8;
			pot.z = 1.1;
			writer(pot);
		}
		case 4: {
			Throttle_t t("wrong wing");
			t.setReserveRatio(0.2);
			t.setPower(0.5);
			writer(t);
		}
		case 5: {
			/* no support for timers */
		}
		case 6: {

		}
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		break;
		default:
		break;
	}
}

#endif