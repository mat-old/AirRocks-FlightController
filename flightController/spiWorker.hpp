#include "arfcDefines.hpp"

#include <thread>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#ifndef SPIWORKER
#define SPIWORKER
class SPIworker : public SubSystem {
public:
	SPIworker() : SubSystem() {}
	~SPIworker() {
		dispose = true;
		/* wait and catch thread */
		usleep(Def::ioBAUD_RATE*0x2);
		close(dev);
		delete daemon;
	}
	SPIworker& Open() {
		try {
			dev = open(Def::spi_device,O_RDWR);
			if( dev < 0 )												throw 0;
			if(ioctl(dev, SPI_IOC_WR_MODE, &mode) == -1 )				throw 1;
			if(ioctl(dev, SPI_IOC_RD_MODE, &mode) == -1 )				throw 2;
			if(ioctl(dev, SPI_IOC_WR_BITS_PER_WORD, &io.bits_per_word) == -1)	throw 3;
			if(ioctl(dev, SPI_IOC_RD_BITS_PER_WORD, &io.bits_per_word) == -1)	throw 4;
			if(ioctl(dev, SPI_IOC_WR_MAX_SPEED_HZ, &io.speed_hz) == -1)	throw 5;
			if(ioctl(dev, SPI_IOC_RD_MAX_SPEED_HZ, &io.speed_hz) == -1)	throw 6;
		} catch( int e ) {
			err.Response(e);
		}
		return *this;
	}
	/* start thread */
	SPIworker& Start() {
		daemon = new std::thread(&SPIworker::transmit_SPId, this);
		return *this;
	}
	/* make daemon, "just saves resources, dont do if switch has too much over head" */
	SPIworker& Detach() {
		daemon->detach();
	}
	SPIworker& Dispose() {
		dispose = true;
	}
	uint64_t testUp() {
		uint8_t s[4] = {0xF,0xF,0xF,0xF};
		return updateTx(Def::ioMsg_Offset, Def::ioMsg_Length, s);
	}
private:
	/* ioctrl file descriptor */
	int dev;
	/* SPI constants */
	uint8_t   mode;
	uint64_t  Tx, Rx;
	struct spi_ioc_transfer io;
	/* SPIworker daemon members */
	volatile long motor_Tx_bytes;
	volatile bool dispose = false;
	std::thread * daemon;

	uint64_t updateTx(const size_t offset, const size_t length, uint8_t * d) {
		uint64_t ntx = 0;
		int i=0;
		for(; i<offset; ++i)
			ntx = ntx << 8u | Def::ioFlag_Start;
		for(; i<length+offset; ++i, ++d)
			ntx = ntx << 8u | *d;
		return ntx << 8u*(Def::ioLength-i) | Def::ioFlag_End;
	}

	void transmit_SPId() {
		while(true) {
			

			if( dispose ) return;
		}
	}
};
#endif