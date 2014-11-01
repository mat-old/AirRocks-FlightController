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
protected:
	void Write() {
		write_json(s,t,false);
		std::cout << s.str();
		s.str("");
		s.clear();
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
			}
			++fmt;
			ary.push_back(std::make_pair("",tmp));
		}
		va_end(args);
		return ary;
	}
public:
	JWriter()  {}
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
};

void test() {
	GenericWriter writer;
	Command cmd1("magic:54444.1");

	writer(cmd1);

}

#endif