#ifndef SPIWORKER
#define SPIWORKER
#include "../Defines.hpp"
#include "../Cores/AsyncWorker.hpp"
#include "../Types/Motorgroup.hpp"

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
	volatile uint8_t Tx[ioLength];
public:
	SPIworker() ;
	~SPIworker() ;
	SPIworker& Open() ;
	SPIworker& Zero()  ;
	void Update( Motorgroup& m ) ;

private:
	void updateTx(uint8_t * destination, volatile uint8_t * source);
	void updateTx(volatile uint8_t * destination, const uint8_t * source) ;
	virtual void *worker_run() ;

	inline void Send(int fd);
};
#endif