#include <map>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <exception>
#ifndef ERRORMAP
#define ERRORMAP

class SystemException: public std::exception {
	virtual const char* what() const throw()
	{
		return "SystemException";
	}
	friend std::ostream& operator<< (std::ostream& out, const SystemException& s) {
		out << "sysEx";
		return out;
	}
} sysEx;

class ErrorMap {
	typedef std::map<int, const char *> errCode;
public:
	errCode code;
	ErrorMap() {
		code[0] = "Error cannot read IO device";
		code[1] = "Failed to set SPI mode";
		code[2] = "Failed to get SPI mode";
		code[3] = "Failed to set bits per word";
		code[4] = "Failed to get bits per word";
		code[5] = "Failed to set Max Speed hz";
		code[6] = "Failed to get Max Speed hz";
		code[7] = "Could not start SPIdaemon thread";
		code[8] = "Error flag was set early";
	}
	void Response(int r) {
		std::cout << code[r] << std::endl;
		switch(r) {
			/* AnyError */
			case 0xFFFF:
				std::cout << "Any error occured" << std::endl;
				sleepThrowWhere(3,sysEx,r);
				return;
			/* SPI subsystem */
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				std::cout << "No permission" << std::endl;
				sleepThrowWhere(3,sysEx,r);
				return;
			/* SPIworker daemon */
			case 7:
			case 8:
				std::cout << "Cannot control motors" << std::endl;
				sleepThrowWhere(3,sysEx,r);
				return;
			default:
			std::cout << "Unknown ErrorMap.Response" << r << std::endl;
		}
	}

	void sleepThrowWhere(int s, SystemException t, int w) {
		while(s--) {
			std::cout << "\rCaught("<<w<<")\tThrow("<<t<<") in "<<s<<"s"<<std::flush; 
			sleep(1);
		}
		std::cout<<std::endl;
		throw t;
	}

	void infloop() {
		while(true) {
			sleep(2);
		}
	}
};
#endif