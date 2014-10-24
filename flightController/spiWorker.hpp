#include "arfcDefines.hpp"

#define SPIDEBUG
#ifdef SPIDEBUG
	#include <stdio.h>
#endif

#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#ifndef SPIWORKER
#define SPIWORKER
class SPIworker : public AsyncWorker {
private:
	/* ioctrl file descriptor */
	int dev;
	uint32_t  iospeed_hz;
	uint8_t   iobits_per_word
			, iomode
			, mode;
	/* SPIworker worker members */
	volatile uint8_t Tx[Def::ioLength];
public:
	SPIworker() : AsyncWorker() {
		iospeed_hz       = Def::ioBAUD_RATE;
		iobits_per_word  = Def::ioBits;
		iomode           = 0x0;
		dev              = -1;
		updateTx(Tx, Def::InitialMotorState);
		if( !SPI_ENABLED ) Disable();
	}
	~SPIworker() {
		Dispose();
		close(dev);
	}
	SPIworker& Open() {
		try {
			dev = open(Def::spi_device,O_RDWR);
			int ret = 0;
			if( dev < 0 )										throw BAD_IO;
			ret = ioctl(dev, SPI_IOC_WR_MODE, &iomode);
			if( ret == -1 )										throw FAIL_SET_SPI;
			ret = ioctl(dev, SPI_IOC_RD_MODE, &iomode);
			if( ret == -1 )										throw FAIL_GET_SPI;
			ret = ioctl(dev, SPI_IOC_WR_BITS_PER_WORD, &iobits_per_word);
			if( ret == -1)										throw FAIL_SET_BIT;
			ret = ioctl(dev, SPI_IOC_RD_BITS_PER_WORD, &iobits_per_word);
			if( ret == -1)										throw FAIL_GET_BIT;
			if( iobits_per_word != Def::ioBits )				throw FAIL_GET_BIT;
			ret = ioctl(dev, SPI_IOC_WR_MAX_SPEED_HZ, &iospeed_hz);
			if( ret == -1)										throw FAIL_SET_SPEED;
			ret = ioctl(dev, SPI_IOC_RD_MAX_SPEED_HZ, &iospeed_hz);
			if( ret == -1)										throw FAIL_GET_SPEED;
			if( iospeed_hz != Def::ioBAUD_RATE )				throw FAIL_GET_SPEED;
		} catch( int e ) {
			Set_Fail(true);
			err.Response(e);
		}
		return *this;
	}
	SPIworker& Update(uint8_t speeds[Def::ioMsg_Length]) {
		Tx[0] = Def::ioFlag_Start;
		Tx[1] = speeds[0];
		Tx[2] = speeds[1];
		Tx[3] = speeds[2];
		Tx[4] = speeds[3];
		Tx[7] = Def::ioFlag_End;
	}

private:
	void updateTx(uint8_t * destination, volatile uint8_t * source) {
		int i = 0;
		destination[i++] = Def::ioFlag_Start;
		for (; i < Def::ioMsg_Length + Def::ioMsg_Offset; ++i)
			destination[i] = Def::MOTOR_SAFE_SPEED(source[i]);
		destination[i] = Def::ioFlag_End;
	}
	void updateTx(volatile uint8_t * destination, const uint8_t * source) {
		int i = 0;
		destination[i++] = Def::ioFlag_Start;
		for (; i < Def::ioMsg_Length + Def::ioMsg_Offset; ++i)
			destination[i] = Def::MOTOR_SAFE_SPEED(source[i]);
		destination[i] = Def::ioFlag_End;
	}
	virtual void *worker_run() {
		int fd  = dup(dev);
		#ifdef SPIDEBUG
			std::cout << "spi mode      " << (int)mode << "\n"
					  << "bits per word " << (int)iobits_per_word << "\n"
					  << "max speed     " << iospeed_hz << "Hz\n" << std::flush; 
		#endif
		while(true) {
			/* timing proves better to do local variables */
			struct spi_ioc_transfer ioc;
			uint8_t tx[Def::ioLength] = {0,};
			uint8_t rx[Def::ioLength] = {0,};

			updateTx(tx,Tx);

			ioc.tx_buf 			= (uint64_t)tx;
			ioc.rx_buf 			= (uint64_t)rx;
			ioc.len 			= Def::ioLength;
			ioc.delay_usecs 	= Def::ioDelay;
			ioc.speed_hz 		= Def::ioBAUD_RATE;
			ioc.bits_per_word 	= Def::ioBits;
			if( Disposed() ) return 0;
			if( ioctl(fd, SPI_IOC_MESSAGE(1), &ioc) < 1 ) {
				std::cout<<"IOERROR SPI WORKER failed to transmit ";
				#ifdef SPIDEBUG
					for (int i = 0; i < Def::ioLength; ++i) 
						printf("%x", (uint8_t)(ioc.tx_buf >> (i*8u)));
					std::cout << "\n" << std::flush;
				#endif
			}
			
			usleep(Def::ioBAUD_RATE);
		}
		return 0;
	}
};
#endif