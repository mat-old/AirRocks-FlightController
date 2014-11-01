#ifndef  DATATYPES
#define  DATATYPES
	
	/*std*/
	#include <stdint.h>
	#include <unistd.h>


	/*ARHS*/
	#include "../ARHS/vector.h"
	
	/* no coupling */
	#include "Potential.hpp"
	#include "Timer.hpp"

	/* just defines */
	#include "Throttle.hpp"
	#include "PID.hpp"

	/* requires other types */
	#include "Motorgroup.hpp"
	#include "Command.hpp"
	/*requires all types*/
	#include "JWriter.hpp"	

#endif