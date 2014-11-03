#ifndef JWRITER 
#define JWRITER
#include <time.h>   // rand
#include <cstdarg>
#include <sstream>
#include <stdlib.h> // rand
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "../Includes.hpp"

using boost::property_tree::write_json;
using boost::property_tree::ptree;

class JWriter {
private:
	std::ostringstream s;
	bool pretty;
	ptree t;
protected:
	void Write() {
		write_json(s,t,pretty);
		std::cout << s.str() << std::flush;
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
	template <typename T>
	void put(const std::string name, T val){
		t.put(name,val);
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
/* i name things opposites a lot... */
class GenericWriter : public JWriter {
public:
	void operator()(std::string data) {
		JWriter::operator () ("status",data);
	}
	void operator()(Command& cmd) {
		this->put("processed",(cmd.processed?"true":"false"));
		ptree p;
		p.add("name",cmd.name);
		p.add("val",cmd.getValue());
		JWriter::operator () ("cmd",p);
	}
	void operator()(Motorgroup& mg) {
		JWriter::operator () ("motors",array("dddd",mg.A(),mg.B(),mg.C(),mg.D()));
	}
	void operator()(PID_t& pt){
		ptree p;
		this->put("name"  , pt.name);
		this->put("input" , pt.last_input);
		this->put("output", pt.output);
		p.add_child("labels",array("sss", "p", "i", "d"));
		p.add_child("values",array("fff", pt.kp, pt.ki, pt.kd));
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
	/* where, number of extra args, argv... */
	void err(const char* what, const int argc, ...) {
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

	void cmd(std::string data, bool processed) {
		this->put("processed",(processed?"true":"false"));
		JWriter::operator () ("cmd",data);
	}

};

void test() {
	srand(time(NULL));
	GenericWriter writer;
	writer.Pretty(false);
	int start = 0;
	int end   = 11;
	for( int k = 0; k<2000; k++ )
	for (int i = start; i < end; ++i)
	switch(i){
		case 0:{
			Command cmd1("magic:54444.1");
			writer(cmd1);
		}break;
		case 1:{
			Motorgroup mg;
			mg.PID_ratio(Defines::PID_RATIO).Zero();
			//mg.All(0.8);
			float basev = ((float)(rand()%101) /100.0f);
			mg.A( basev+((float)(rand()%16) /100.0f) );
			mg.B( basev+((float)(rand()%16) /100.0f) );
			mg.C( basev+((float)(rand()%16) /100.0f) );
			mg.D( basev+((float)(rand()%16) /100.0f) );
			writer(mg);
		}break;
		case 2:{
			PID_t pt;
			pt.name = "pitch";
			pt.set_point = 0.024f;
			pt.SetTime(20);
			pt.SetPID(0.16,3.02,0.025);
			pt.Compute(0.092f);
			usleep(20000);
			pt.Compute( ((float)(rand()%70+1) /100.0f) );

			PID_t r;
			r.name = "roll";
			r.set_point = 0.024f;
			r.SetTime(20);
			r.SetPID(0.16,3.02,0.025);
			r.Compute(0.092f);
			usleep(20000);
			r.Compute( ((float)(rand()%70+1) /100.0f) );

			PID_t y;
			y.name = "yaw";
			y.set_point = 0.024f;
			y.SetTime(20);
			y.SetPID(0.16,3.02,0.025);
			y.Compute(0.092f);
			usleep(20000);
			y.Compute( 1.0f - (abs(pt.last_input) + abs(r.last_input)) );

			writer(r);
			writer(y);
			writer(pt);
		}break;
		case 3:{
			Potential_t pot("gyro");
			pot.x = 9.9;
			pot.y = 8.8;
			pot.z = 1.1;
			writer(pot);
		}break;
		case 4: {
			Throttle_t t("A");
			t.setReserveRatio(0.2);
			t.setPower(0.5);
			writer(t);
		}break;
		case 5: {
			/* no support for timers */
		}break;
		case 6: {
			writer.err("Global.test.JWriter",3,"super","error","OMG!");
		}break;
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