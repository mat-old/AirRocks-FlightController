#ifndef SPIWORKER
#define SPIWORKER
#include "../Defines.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

class SPIworker : public AsyncWorker {
private:
	/* ioctrl file descriptor */
	int dev;
	uint32_t  iospeed_hz;
	uint8_t   iobits_per_word
			, iomode
			, mode;
	/* SPIworker worker members */
	volatile uint8_t Tx[Defines::ioLength];
public:
	SPIworker() : AsyncWorker() {
		iospeed_hz       = Defines::ioBAUD_RATE;
		iobits_per_word  = Defines::ioBits;
		iomode           = 0x0;
		dev              = -1;
		updateTx(Tx, Defines::InitialMotorState);
		if( !SPI_ENABLED ) Disable();
	}
	~SPIworker() {
		Dispose();
		close(dev);
	}
	SPIworker& Open() {
		if( Disabled() ) return *this;
		try {
			dev = open(Defines::spi_device,O_RDWR);
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
			if( iobits_per_word != Defines::ioBits )				throw FAIL_GET_BIT;
			ret = ioctl(dev, SPI_IOC_WR_MAX_SPEED_HZ, &iospeed_hz);
			if( ret == -1)										throw FAIL_SET_SPEED;
			ret = ioctl(dev, SPI_IOC_RD_MAX_SPEED_HZ, &iospeed_hz);
			if( ret == -1)										throw FAIL_GET_SPEED;
			if( iospeed_hz != Defines::ioBAUD_RATE )				throw FAIL_GET_SPEED;
		} catch( int e ) {
			Set_Fail(true);
			err.Response(e);
		}
		return *this;
	}
	SPIworker& Zero()  {
		for (int i = 1; i < 5; ++i)
			Tx[i] = Defines::MOTOR_ZERO_LEVEL;
		Tx[0] = Defines::ioFlag_Start;
		Tx[7] = Defines::ioFlag_End;
		Send(dev);
	}
	void Update( Motorgroup& m ) {
		Tx[0] = Defines::ioFlag_Start;
		Tx[1] = m.A();
		Tx[2] = m.B();
		Tx[3] = m.C();
		Tx[4] = m.D();
		Tx[7] = Defines::ioFlag_End;
	}

private:
	void updateTx(uint8_t * destination, volatile uint8_t * source) {
		int i = 0;
		destination[i++] = Defines::ioFlag_Start;
		for (; i < Defines::ioMsg_Length + Defines::ioMsg_Offset; ++i)
			destination[i] = MOTOR_SAFE_SPEED(source[i]);
		destination[i] = Defines::ioFlag_End;
	}
	void updateTx(volatile uint8_t * destination, const uint8_t * source) {
		int i = 0;
		destination[i++] = Defines::ioFlag_Start;
		for (; i < Defines::ioMsg_Length + Defines::ioMsg_Offset; ++i)
			destination[i] = MOTOR_SAFE_SPEED(source[i]);
		destination[i] = Defines::ioFlag_End;
	}
	virtual void *worker_run() {
		int fd  = dup(dev);

		emit("SPI is about to start");

		while(true) {
			if( Disposed() ) return 0;
			Send(fd);
			usleep(Defines::ioBAUD_RATE);
		}
		return 0;
	}

	inline void Send(int fd){
		struct spi_ioc_transfer ioc;
		uint8_t tx[Defines::ioLength] = {0,};
		uint8_t rx[Defines::ioLength] = {0,};

		updateTx(tx,Tx);

		ioc.tx_buf 			= (uint64_t)tx;
		ioc.rx_buf 			= (uint64_t)rx;
		ioc.len 			= Defines::ioLength;
		ioc.delay_usecs 	= Defines::ioDelay;
		ioc.speed_hz 		= Defines::ioBAUD_RATE;
		ioc.bits_per_word 	= Defines::ioBits;
		if( ioctl(fd, SPI_IOC_MESSAGE(1), &ioc) < 1 ) {
			emit.err(1,"SPI failed to transmit");
		}
	}
};
#endif