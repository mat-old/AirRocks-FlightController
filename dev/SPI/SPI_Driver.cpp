/* SPI subsystem 
* Purpose, check for data in the /var/local/SPI_arfc.dat 
* and send it over SPI when appropriate. 
*/

#define SPI_DATA "/home/arfc/SPI_arfc.dat"
#define MOTOR_ZERO_LEVEL 125
#define MOTOR_ARM_START  140
#define MOTOR_MAX_LEVEL  254
#define BAUD_RATE        500000 // .5 MHZ transmission

#define DEBUG

#ifdef DEBUG
#include <iostream>
bool BLINK = false;
#endif 

#include <fstream>
#include <unistd.h>
#include <stdint.h>

static uint8_t A, B, C, D;
static char reg[4];
inline bool read(std::ifstream &data) {
	data.read(reg,4);
	data.seekg (0, data.beg);
	#ifdef DEBUG
		std::cout << '\r' << (BLINK?">":" ") << (int)reg[0] << " " << (int)reg[1] << " " << (int)reg[2] << " " << (int)reg[3] << std::flush;
		BLINK = !BLINK;
		usleep(BAUD_RATE);
	#endif
	return A && B && C && D;
}

inline void initData() {
	std::fstream init;
	init.open(SPI_DATA, std::fstream::out | std::fstream::trunc | std::fstream::binary);
	#ifdef DEBUG
		std::cout << (init.is_open()?" Write OK":" Write Fail") << std::endl;
	#endif
	char blank[4] = {0,0,0,0};
	for (int i = 0; i < 4; ++i)
		init.write(0x0,1);
	init.close();
}

inline void verify() {
	if(reg[0]>MOTOR_MAX_LEVEL) reg[0] = MOTOR_MAX_LEVEL;
	else if(reg[0]<MOTOR_ZERO_LEVEL) reg[0] = MOTOR_ZERO_LEVEL;
	if(reg[1]>MOTOR_MAX_LEVEL) reg[1] = MOTOR_MAX_LEVEL;
	else if(reg[1]<MOTOR_ZERO_LEVEL) reg[1] = MOTOR_ZERO_LEVEL;
	if(reg[2]>MOTOR_MAX_LEVEL) reg[2] = MOTOR_MAX_LEVEL;
	else if(reg[2]<MOTOR_ZERO_LEVEL) reg[2] = MOTOR_ZERO_LEVEL;
	if(reg[3]>MOTOR_MAX_LEVEL) reg[3] = MOTOR_MAX_LEVEL;
	else if(reg[3]<MOTOR_ZERO_LEVEL) reg[3] = MOTOR_ZERO_LEVEL;
}

inline void update() {
	usleep(BAUD_RATE);
}

int main(int argc, char const *argv[]) {
	std::ifstream data(SPI_DATA, std::ifstream::in | std::fstream::binary);
	if( !data.is_open() ) {
		#ifdef DEBUG
			std::cout << " Could not open... creating \n";
		#endif
		initData();
		sleep(1);
		data.open(SPI_DATA, std::ifstream::in | std::fstream::binary);
		sleep(1);
	}
	#ifdef DEBUG
		std::cout << " Starting... \n";
	#endif
	while( data.good() ) {
		/* read the data */
		if( read(data) ) {
			/* if its good send it */
			verify();
			update();
		}
		/* else sleep for 3 seconds */
		else {
			sleep(1);
			#ifdef DEBUG
				std::cout << '\r' << (BLINK?" WAITING...          ":"           WAITING... ") << std::flush;
				BLINK = !BLINK;
			#endif
		}
	}
	data.close();
	return 0;
}