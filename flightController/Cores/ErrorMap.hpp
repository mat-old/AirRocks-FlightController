#ifndef SYSEX_EXCEPTION
#define SYSEX_EXCEPTION
/*class ControlledException: public std::exception {
public:
	const char* what() const throw()
	{
		return "Controlled Exception";
	}
	friend std::ostream& operator<< (std::ostream& out, const ControlledException& s) {
		//out << "CtrlEx";
		return out;
	}
} sysEx;*/
#endif
#ifndef ERRORMAP
#define ERRORMAP
#include <map>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <exception>

#include "../Defines.hpp"
#include "../Types/JWriter.hpp"

class ErrorMap {
	static JWriter emit;
	typedef std::map<int, const char *> errCode;
public:
	errCode code;
	ErrorMap();
	void Response(int r);
	void sleepThrowWhere(int s, int w);
	void infloop();
};
#endif
