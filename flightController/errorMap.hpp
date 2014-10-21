#include <map>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <exception>
#ifndef ERRORMAP
#define ERRORMAP

typedef enum {
	  BAD_IO
	, FAIL_SET_SPI
	, FAIL_GET_SPI
	, FAIL_SET_BIT
	, FAIL_GET_BIT
	, FAIL_SET_SPEED
	, FAIL_GET_SPEED
	, FAIL_START_DAEMON
	, FAIL_FLAG_SET
	, FAIL_I2C_PERM
	, FAIL_I2C_DEV
	, FAIL_I2C_WRITE
	, FAIL_I2C_READ
	, FAIL_I2C_BLOCK
	, FAIL_I2C_CAL_OPEN
	, FAIL_I2C_CAL_READ
	, ERR_ANY = 0xFFFF  /* catch all code */
} errCodes;

class ControlledException: public std::exception {
	virtual const char* what() const throw()
	{
		return "Controlled Exception";
	}
	friend std::ostream& operator<< (std::ostream& out, const ControlledException& s) {
		out << "CtrlEx";
		return out;
	}
} sysEx;

class ErrorMap {
	typedef std::map<int, const char *> errCode;
public:
	errCode code;
	ErrorMap() {
		/* SPI subsystem */
		code[BAD_IO]         = "Error cannot read IO device";
		code[FAIL_SET_SPI]   = "Failed to set SPI mode";
		code[FAIL_GET_SPI]   = "Failed to get SPI mode";
		code[FAIL_SET_BIT]   = "Failed to set bits per word";
		code[FAIL_GET_BIT]   = "Failed to get bits per word";
		code[FAIL_SET_SPEED] = "Failed to set Max Speed hz";
		code[FAIL_GET_SPEED] = "Failed to get Max Speed hz";
		code[FAIL_START_DAEMON]  = "Could not start SPIdaemon thread";
		code[FAIL_FLAG_SET]  = "Error flag was set early";
		/* IMU interface subsystem */
		code[FAIL_I2C_DEV]   = "Invalid I2C device";
		code[FAIL_I2C_PERM]  = "Failed to open I2C device";
		code[FAIL_I2C_WRITE] = "Failed to write byte to I2C";
		code[FAIL_I2C_READ]  = "Failed to read byte from I2C";
		code[FAIL_I2C_BLOCK] = "Failed to read block from I2C";
		code[FAIL_I2C_CAL_OPEN] = "Failed to open IMU calibration";
		code[FAIL_I2C_CAL_READ] = "Failed to read IMU calibration";
	}
	void Response(int r) {
		std::cout << code[r] << std::endl;
		switch(r) {
			/* AnyError */
			case ERR_ANY:
				std::cout << "Any error occured" << std::endl;
				sleepThrowWhere(3,sysEx,r);
				return;
			case FAIL_I2C_WRITE:
			case FAIL_I2C_READ:
			case FAIL_I2C_BLOCK:
			case FAIL_I2C_CAL_OPEN:
			case FAIL_I2C_CAL_READ:
				std::cout << "IMU threw a critical error" << std::endl;
				sleepThrowWhere(3,sysEx,r);
				return;
			case FAIL_I2C_DEV:		// IMU
			case FAIL_I2C_PERM:		// IMU 
				std::cout << "Cannot start IMU" << std::endl;
				sleepThrowWhere(3,sysEx,r);
				return;
			case BAD_IO:			// SPI
			case FAIL_SET_SPI:		// SPI
			case FAIL_GET_SPI:		// SPI
			case FAIL_SET_BIT:		// SPI
			case FAIL_GET_BIT:		// SPI
			case FAIL_SET_SPEED:	// SPI
			case FAIL_GET_SPEED:	// SPI
				std::cout << "No permission" << std::endl;
				sleepThrowWhere(3,sysEx,r);
				return;
			/* SPIworker daemon */
			case FAIL_FLAG_SET:
			case FAIL_START_DAEMON:
				std::cout << "Cannot control motors" << std::endl;
				sleepThrowWhere(3,sysEx,r);
				return;
			default:
			std::cout << "Unknown ErrorMap.Response" << r << std::endl;
		}
	}

	void sleepThrowWhere(int s, ControlledException t, int w) {
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