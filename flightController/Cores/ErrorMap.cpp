#include "ErrorMap.hpp"
	JWriter ErrorMap::emit;
	ErrorMap::ErrorMap() {
		/* SPI subsystem */
		code[BAD_IO]         = "Error cannot read IO device";
		code[FAIL_SET_SPI]   = "Failed to set SPI mode";
		code[FAIL_GET_SPI]   = "Failed to get SPI mode";
		code[FAIL_SET_BIT]   = "Failed to set bits per word";
		code[FAIL_GET_BIT]   = "Failed to get bits per word";
		code[FAIL_SET_SPEED] = "Failed to set Max Speed Hz";
		code[FAIL_GET_SPEED] = "Failed to get Max Speed Hz";
		code[FAIL_START_WORKER]  = "Could not start SPIdaemon thread";
		code[FAIL_FLAG_SET]  = "Error flag was set early";
		/* IMU interface subsystem */
		code[FAIL_I2C_DEV]   = "Invalid I2C device";
		code[FAIL_I2C_PERM]  = "Failed to open I2C device";
		code[FAIL_I2C_WRITE] = "Failed to write byte to I2C";
		code[FAIL_I2C_READ]  = "Failed to read byte from I2C";
		code[FAIL_I2C_BLOCK] = "Failed to read block from I2C";
		code[FAIL_I2C_CAL_OPEN] = "Failed to open IMU calibration";
		code[FAIL_I2C_CAL_READ] = "Failed to read IMU calibration";
		code[UNREACHABLE]    = "Unreachable state was detected";
		code[SHUTDOWN]       = "Shutting down";
		code[IMU_BAD_CONNECT]= "Could not detect accelerometer";
		code[NOENT_SOCKET_ERR] = "Failed to reserve socket, no permission.";
		code[BIND_SOCKET_ERR] = "Socket is in use";
	}
	void ErrorMap::Response(int r) {
		std::cout << code[r] << std::endl;
		switch(r) {
			/* AnyError */
			case ERR_ANY:
				//std::cout << "Any error occurred" << std::endl;
				emit.err("Anything",1,"Any error occurred","D:");
				sleepThrowWhere(3,r);
				return;
			case NOENT_SOCKET_ERR:
			case BIND_SOCKET_ERR:
				emit.err("dgram.interface.system",1,code[r]);
				sleepThrowWhere(3,r);
				return;
			case FAIL_I2C_WRITE:
			case FAIL_I2C_READ:
			case FAIL_I2C_BLOCK:
			case FAIL_I2C_CAL_OPEN:
			case FAIL_I2C_CAL_READ:
				//std::cout << "IMU threw a critical error" << std::endl;
				emit.err("AHRS.LSM303",1,"IMU threw a critical error","Failed to start");
				sleepThrowWhere(3,r);
				return;
			case FAIL_I2C_DEV:		// IMU
			case FAIL_I2C_PERM:		// IMU 
			case IMU_BAD_CONNECT:
				//std::cout << "Cannot start IMU" << std::endl;
				emit.err("ARFC.IMUWorker.IMUInterface.SMBUS",1,"Cannot start IMU");
				sleepThrowWhere(3,r);
				return;
			case BAD_IO:			// SPI
			case FAIL_SET_SPI:		// SPI
			case FAIL_GET_SPI:		// SPI
			case FAIL_SET_BIT:		// SPI
			case FAIL_GET_BIT:		// SPI
			case FAIL_SET_SPEED:	// SPI
			case FAIL_GET_SPEED:	// SPI
				emit.err("ARFC.SPIworker.SMBUS", 1,"No permission");
				sleepThrowWhere(3,r);
				return;
			/* SPIworker daemon */
			case FAIL_FLAG_SET:
			case FAIL_START_WORKER:
				emit.err("ARFC.SPIWorker",1,"Cannot control motors");
				sleepThrowWhere(3,r);
				return;
			/* FATAL */
			case UNREACHABLE:
				emit.err( "SYSTEM", 1,"FATAL ERROR");
				sleepThrowWhere(10,r);
				return;
			case SHUTDOWN:
				emit.err("SYSTEM",1,"I sure hope you're not flying... ");
				sleepThrowWhere(0,r);
				return;
			default:
			emit.err("Unknown", 1, "Unknown ErrorMap.Response");
		}
	}

	void ErrorMap::sleepThrowWhere(int s,  int w) {
			//emit.err(t.what(), 1, "Error countdown" );
		while(s--) {
			//std::cout << "\rCaught("<<w<<")\tThrow("<<t<<") in "<<s<<"s"<<std::flush; 
			//std::string s = "Caught(" + stoi(w) + ")";
			sleep(1);
		}
		std::cout<<std::endl;
		//throw t;
	}

	void ErrorMap::infloop() {
		while(true) {
			sleep(2);
		}
	}