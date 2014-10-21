#include "arfcDefines.hpp"

#define DEBUG
#ifdef DEBUG
	#include <stdio.h>
#endif

#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#ifndef SPIWORKER
#define SPIWORKER
class SPIworker : public SubSystem {
private:
	/* ioctrl file descriptor */
	int dev;
	uint32_t  iospeed_hz;
	uint8_t   iobits_per_word
			, iomode
			, mode;
	/* SPIworker daemon members */
	volatile uint8_t Tx[Def::ioLength];
	pthread_t daemon;

public:
	SPIworker() : SubSystem() {
		iospeed_hz       = Def::ioBAUD_RATE;
		iobits_per_word  = Def::ioBits;
		iomode           = 0x0;
		dev              = -1;
		updateTx(Tx, Def::InitialMotorState);
	}
	~SPIworker() {
		Dispose();
		/* wait and catch thread */
		usleep(iospeed_hz*0x2);
		close(dev);
	}
	SPIworker& Open() {
		try {
			dev = open(Def::spi_device,O_RDWR);
			int ret = 0;
			if( dev < 0 )										throw 0;
			ret = ioctl(dev, SPI_IOC_WR_MODE, &iomode);
			if( ret == -1 )										throw 1;
			ret = ioctl(dev, SPI_IOC_RD_MODE, &iomode);
			if( ret == -1 )										throw 2;
			ret = ioctl(dev, SPI_IOC_WR_BITS_PER_WORD, &iobits_per_word);
			if( ret == -1)										throw 3;
			ret = ioctl(dev, SPI_IOC_RD_BITS_PER_WORD, &iobits_per_word);
			if( ret == -1)										throw 4;
			if( iobits_per_word != Def::ioBits )				throw 4;
			ret = ioctl(dev, SPI_IOC_WR_MAX_SPEED_HZ, &iospeed_hz);
			if( ret == -1)										throw 5;
			ret = ioctl(dev, SPI_IOC_RD_MAX_SPEED_HZ, &iospeed_hz);
			if( ret == -1)										throw 6;
			if( iospeed_hz != Def::ioBAUD_RATE )				throw 6;
		} catch( int e ) {
			fail_flag = true;
			err.Response(e);
		}
		return *this;
	}
	/* start thread */
	SPIworker& Start() {
		if( !fail_flag ) {
			int ret = pthread_create(&daemon, NULL, &SPIworker::daemon_helper, this);
			if( ret != 0 ) throw 7;
		} else throw 8;
		return *this;
	}
	/* make daemon, "just saves resources, dont do if switch has too much over head" */
	SPIworker& Detach() {
		pthread_detach(daemon);
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

	static void *daemon_helper(void *context) {
		return ((SPIworker *)context)->transmit_SPId();
	}

	void *transmit_SPId() {
		int fd  = dup(dev);
		#ifdef DEBUG
			std::cout << "spi mode      " << (int)mode << "\n"
					  << "bits per word " << (int)iobits_per_word << "\n"
					  << "max speed     " << iospeed_hz << "Hz\n" << std::flush; 
		#endif
		while(true) {
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

			if( ioctl(fd, SPI_IOC_MESSAGE(1), &ioc) < 1 ) {
				std::cout<<"IOERROR SPI WORKER failed to transmit ";
				#ifdef DEBUG
					/* msg will appear reversed, MOTOR_A is closest to 'a' */
					for (int i = 0; i < Def::ioLength; ++i) {
						uint8_t x = ioc.tx_buf >> (i*8u);
						printf("%x", x);
					}
					std::cout << "\n" << std::flush;
				#endif
			}
			
			usleep(Def::ioBAUD_RATE);
			if( dispose ) return 0;
		}
		return 0;
	}
};
#endif