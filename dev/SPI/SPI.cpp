/* RPI: g++ */
/*
	C FLAGS 	none
	I BOOST 	no
	V FINAL 	no

	the initial SPI interface
	- mat

	TODO: make this into a usable interface
		  create a debug mode / flag
*/

#define MOTOR_ZERO_LEVEL  125
#define MOTOR_ARM_START  140
#define MOTOR_MAX_LEVEL  254


#include <stdint.h>				// extra types
#include <unistd.h>				// std linux stuff
#include <stdio.h>				// printf and such 			** try omit 
#include <stdlib.h>				// std 						** try omit 
#include <getopt.h>				// for command line args 	** try omit 
#include <fcntl.h>				// for file descriptors since the pins are mem-maps
#include <sys/ioctl.h>			// actual IO control
#include <linux/types.h>		// more types
#include <linux/spi/spidev.h>	// SPI interface

#include <iostream>				// 							** try omit 

using namespace std;

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define BAUD_RATE 10000

static void pabort(const char *s)
{
	perror(s);
	abort();
}

static const char *device = "/dev/spidev0.1";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = BAUD_RATE;
static uint16_t delay;
static uint32_t cycle = 0;


void copy(uint32_t n, uint8_t * r, int i)
{
	do
	{
		r[i] = n % 10 + '0';
		n /= 10;
		i--;
	}
	while(n > 0);
}

string toS(uint8_t n)
{
	string s = "   ";
	int i = 2;
	do
	{
		s[i] = n % 10 + '0';
		n /= 10;
		i--;
	}
	while(n > 0);

	return s;
}

string decode(uint8_t * msg)
{
	string s = "";
	s += "[" + toS(msg[1]) + "]";
	s += "[" + toS(msg[2]) + "]";
	s += "[" + toS(msg[3]) + "]";
	s += "[" + toS(msg[4]) + "]";

	return s;
}


static void transfer(int fd, int a, int b, int c, int d)
{
	int ret;
	int size = 7;
	cycle++;
	uint8_t tx[size];

	tx[0] = 'S';	
	tx[1] = a;
	tx[2] = b;
	tx[3] = c;
	tx[4] = d;
	tx[5] = 'E';
	tx[6] = '\0';


	uint8_t rx[7];

	struct spi_ioc_transfer tr;
		tr.tx_buf 			= (unsigned long)tx;
		tr.rx_buf 			= (unsigned long)rx;
		tr.len 				= size;//ARRAY_SIZE(tx);
		tr.delay_usecs 		= delay;
		tr.speed_hz 		= speed;
		tr.bits_per_word 	= bits;
	

	//printf("%s\n",tx );

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);

	//cout << '"' <<decode(rx)<< "\"   (" <<decode(tx)<< ") "<<ret;
	cout << decode( tx ) ;
	if (ret < 1)
		pabort("can't send spi message");

    //cout << "(" << ARRAY_SIZE(tx) << ")" << tx << endl;
	/*for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {
		cout << rx[ret];
	}*/
	//cout << endl;
    //
}


int main(int argc, char *argv[])
{
	int ret = 0;
	int fd;

	//parse_opts(argc, argv);

	fd = open(device, O_RDWR);
	if (fd < 0)
		pabort("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

	for (int i = 10; i >= 0; --i)
	{
		cout << "starting in " << i << "  \r" << flush;
		if( i != 0 ) usleep(50000);
	}
	cout << endl;

	//for( int i = 0; i < 100000; i++ )
	//{

		int rs = MOTOR_ARM_START + 70;
		int rz = MOTOR_ZERO_LEVEL;

		cout << "::INIT::"  << "\t: ";
		for (int i = 0; i < 10; ++i)
		{
			transfer(fd, rz, rz, rz, rz );
			usleep(BAUD_RATE);
			cout << endl;
		}
		cout << endl << flush;	

		cout << 1  << "\t: ";
		for (int i = 0; i < 10; ++i)
		{
			transfer(fd, rs, rz, rz, rz );		
			usleep(BAUD_RATE);
			cout << endl;
		}
		cout << endl << flush;	

		cout << 2  << "\t: ";
		for (int i = 0; i < 10; ++i)
		{
			transfer(fd, rz, rs, rz, rz );		
			usleep(BAUD_RATE);
			cout << endl;
		}
		cout << endl << flush;	

		cout << 3  << "\t: ";
		for (int i = 0; i < 10; ++i)
		{
			transfer(fd, rz, rz, rs, rz );		
			usleep(BAUD_RATE);
			cout << endl;
		}
		cout << endl << flush;	


		cout << "::HALT::"  << "\t: ";
		for (int i = 0; i < 10; ++i)
		{
			transfer(fd, rz, rz, rz, rz );		
			usleep(BAUD_RATE);
			cout << endl;
		}
		cout << endl << flush;	


	close(fd);

	return ret;
}










/* Acknowledgments go to MEYERS, http://mitchtech.net/raspberry-pi-arduino-spi/ */
/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */