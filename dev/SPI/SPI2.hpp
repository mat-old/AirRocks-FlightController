#include <fcntl.h>				// for file descriptors since the pins are mem-maps
#include <unistd.h>				// usleep and such
#include <sys/ioctl.h>			// actual IO control
#include <linux/types.h>		// more types
#include <linux/spi/spidev.h>	// SPI interface
#include <string>
//#include <thread>
#include <mutex>
#include <iostream>
#include <pthread.h>

#ifndef SPI_IFACE2_H
#define SPI_IFACE2_H

#define MOTOR_ZERO_LEVEL 125
#define MOTOR_ARM_START  140
#define MOTOR_MAX_LEVEL  254
#define BAUD_RATE        500000 // .5 MHZ transmission


void *transmission(void*);
void Init(void);

volatile uint8_t speedA=0, speedB=0, speedC=0, speedD=0;

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

//std::string device;

volatile struct spi_ioc_transfer transmit;

pthread_t transmitter;

void Init(void)
{
	std::cout << "Init::" << std::endl << std::flush;
	max_level   = MOTOR_MAX_LEVEL;
	zero_level  = MOTOR_ZERO_LEVEL;
	armed_level = MOTOR_ARM_START;
	speed_hz    = BAUD_RATE;
	delay       = 0x0;

	static const char *device = "/dev/spidev0.1";
	begin_flag  = 0x1;
	end_flag    = 0x0;

	rx = new uint8_t[8];
	tx = new uint8_t[8];

	int ret = 0;
	int fd  = open(device,O_RDWR);
	if( fd < 0 ) std::cout << "Error cannot read IO device." << std::endl;
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if( ret == -1 ) std::cout << "Failed to set SPI mode."   << std::endl;
	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if( ret == -1 ) std::cout << "Failed to get SPI mode."   << std::endl;
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bit_pr_tx);
	if(ret == -1) std::cout << "Failed to set bits per word."<< std::endl;
	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bit_pr_tx);
	if(ret == -1) std::cout << "Failed to get bits per word."<< std::endl;
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed_hz);
	if(ret == -1) std::cout << "Failed to set Max Speed hz"  << std::endl;
	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed_hz);
	if(ret == -1) std::cout << "Failed to get Max Speed hz"  << std::endl;
		

	int rc = pthread_create(&transmitter, NULL, transmission, (void*)fd);
	if( rc ) std::cout << "failed to start transmitter" << std::endl << std::flush;
}


void reset()
{

}

uint8_t sratio(int s) {
	int min = MOTOR_ZERO_LEVEL;
	int max = MOTOR_MAX_LEVEL;
	double span = (double)max - (double)min;
	double dif = s /100.0f;
	double n = span * dif + (double)min;
	std::cout << n << std::endl;
	return (uint8_t)n;
}

uint8_t goodspeed(uint8_t s) {
	if(s < MOTOR_ZERO_LEVEL)
		s = MOTOR_ZERO_LEVEL;
	if( s > MOTOR_MAX_LEVEL)
		s = MOTOR_MAX_LEVEL;
	return s;
}

void *transmission(void *f) {
	int dt = (int)f;
	std::cout << dt << "SPI OPEN" << std::endl << std::flush;
	while(true)
	{
		
		tx[0] = 0xA;	
		tx[1] = goodspeed(speedA);
		tx[2] = goodspeed(speedB);
		tx[3] = goodspeed(speedC);
		tx[4] = goodspeed(speedD);
		tx[5] = '\0';

		transmit.tx_buf 			= (unsigned long)tx;
		transmit.rx_buf 			= (unsigned long)rx;
		transmit.len 				= 8;
		transmit.delay_usecs 		= delay;
		transmit.speed_hz 			= speed_hz;
		transmit.bits_per_word 		= bit_pr_tx;

		int ret = ioctl(dt, SPI_IOC_MESSAGE(1), &transmit);
		//std::cout << rx << speedA << " " << speedB << " " << std::endl;
		usleep(BAUD_RATE);
	}
	close(dt);
}

#endif