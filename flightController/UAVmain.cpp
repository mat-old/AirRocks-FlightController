#include "Defines.hpp"
#include "Modes/Modes.hpp"

#include "Types/Arming.hpp"
#include "Systems/relay.hpp"

//#include <algorithm>


/*bool Exists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}
*/


int main(int argc, char *argv[]) {
	Mode      Drone;
	Relay     rel;
	SPIworker spi;
	IMUworker imu;

	try {
		rel.Connect();
		rel.ListenForHandshake();
		rel.Start().Detach();
		spi.Open().Zero(); 
		spi.Start().Detach();
		imu.Prepare().Start().Detach();
	}
	catch( ERR_CODES e ) {
		Drone.err.Response(e);
		exit( EXIT_FAILURE ); 
	}


	while( true ) {
		try {
			sleep(1);
			Drone.emit( "waiting for mode select..." );
			Drone.setMode( rel.waitFor( AC_mode_select ) );
			Drone.emit.log( "mode selected" );
		} catch( ERR_CODES e ) {
			Drone.err.Response(e);
			exit( EXIT_FAILURE ); 
		}

		switch( Drone.mode ) {
			case TEST_MODE:

			break;
			case TUNE_MODE: 

				do {
					Drone.safety.clear();
					
					rel.waitForARM();

					Drone.Tuner(rel, spi, imu);  /* relative safety... get it!? */
					
					if( Drone.safety.RESETTING_HARD() ) {
						Drone.setMode(NO_MODE);
						Drone.Shutdown();
					}
					if( Drone.safety.RESETTING() ) {
						Drone.safety.DISARM();	
					}
				}
				while( Drone.safety.RESETTING() ); // loop if resetting

			break;
			case UAV_MODE: 

			break;
			case NO_MODE: 
			default:
			break;
		}

		if( Drone.Shutdown() ) {
			Drone.emit.log("Shutdown flag 0x0");
			return 0;
		}
	}

	Drone.emit.log("How did I get here?");
	return 0;
}