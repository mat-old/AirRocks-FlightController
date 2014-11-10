#ifndef ARFC_INCLUDES_Eigen
#define ARFC_INCLUDES_Eigen

/* hardware drivers */
#include "LSM303.h"
#include "IMU.h"
#include "L3G.h"
#include "I2CBus.h"

/* math */
#include "exceptions.h"
#include "vector.h"

/* coms and SMBUS headers */
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

/* normal headers */
#include <sys/time.h>			/* time funcs 			*/
#include <wordexp.h>			/* ?? 					*/
#include <time.h>				/* time funcs 			*/
#include <stdint.h>				/* value_type std 		*/
#include <unistd.h>				/* unix std 			*/
#include <stdio.h>				/* c style printf 		*/
#include <fstream>				/* fileIO 				*/
#include <iomanip>				/* std::cout format		*/
#include <iostream>				/* std::cout in 		*/

#endif