#ifndef _I2CBus_h
#define _I2CBus_h

#include "includes.hpp"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include "exceptions.h"

#include "../errorMap.hpp"

class I2CBus
{
public:
	I2CBus(const char * deviceName) {
		fd = open(deviceName, O_RDWR);
		if (fd == -1)
			throw FAIL_I2C_PERM;
			//throw posix_error("Failed to open I2C device.");
	}

	~I2CBus() {
		close(fd);
	}

	void addressSet(uint8_t address) {
		int result = ioctl(fd, I2C_SLAVE, address);
		if (result == -1)
			throw FAIL_I2C_DEV;
			//throw posix_error("Failed to set address.");
	}

	void writeByte(uint8_t command, uint8_t data) {
		int result = i2c_smbus_write_byte_data(fd, command, data);
		if (result == -1)
			throw FAIL_I2C_WRITE;
			//throw posix_error("Failed to write byte to I2C.");
	}

	uint8_t readByte(uint8_t command) {
		int result = i2c_smbus_read_byte_data(fd, command);
		if (result == -1)
			throw FAIL_I2C_READ;
			//throw posix_error("Failed to read byte from I2C.");
		return result;
	}

	int tryReadByte(uint8_t command) {
		return i2c_smbus_read_byte_data(fd, command);
	}

	void readBlock(uint8_t command, uint8_t size, uint8_t * data) {
		int result = i2c_smbus_read_i2c_block_data(fd, command, size, data);
		if (result != size)
			throw FAIL_I2C_BLOCK;
			//throw posix_error("Failed to read block from I2C.");
	}


	int tryReadByte(uint8_t address, uint8_t command) {
		addressSet(address);
		return tryReadByte(command);
	}

private:
	int fd;
};

#endif
