
class SPIworker : public SubSystem {
public:
	/* ioctrl file descriptor */
	int dev;
	SPIworker() : SubSystem() {}
	~SPIworker(){}
	SPIworker& Open() {
		try {
			dev = open(Def::spi_device,O_RDWR);
			if( dev < 0 )												throw 0;
			if(ioctl(dev, SPI_IOC_WR_MODE, &mode) == -1 )				throw 1;
			if(ioctl(dev, SPI_IOC_RD_MODE, &mode) == -1 )				throw 2;
			if(ioctl(dev, SPI_IOC_WR_BITS_PER_WORD, &bit_pr_tx) == -1)	throw 3;
			if(ioctl(dev, SPI_IOC_RD_BITS_PER_WORD, &bit_pr_tx) == -1)	throw 4;
			if(ioctl(dev, SPI_IOC_WR_MAX_SPEED_HZ, &speed_hz) == -1)	throw 5;
			if(ioctl(dev, SPI_IOC_RD_MAX_SPEED_HZ, &speed_hz) == -1)	throw 6;
		} catch( int e ) {
			err.Response(e);
		}
		return *this;
	}
	SPIworker& Start(){

		return *this;
	}
private:
	/* motor levels */
	uint8_t   max_level
			, zero_level
			, armed_level;
	/* SPI constants */
	uint16_t  delay;

	uint32_t  speed_hz
			, length;

	uint8_t  *tx
			,*rx
			, mode
			, size
			, bit_pr_tx
			, begin_flag
			, end_flag;
};