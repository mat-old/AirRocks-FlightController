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
	Mode Drone;

	Relay rel;
	Arming safety;

	rel.Connect();
	rel.Start().Detach();


	while( true ) {
		Drone.emit( "waiting for mode select..." );
		Drone.setMode( rel.waitFor( AC_mode_select ) );

		switch( Drone.mode ) {
			case TEST_MODE:

			break;
			case TUNE_MODE: 
				rel.waitForARM(safety);
				while( safety.ARMED() )
					Drone.Tuner(rel,safety);  /* relative safety... get it!? */
			break;
			case UAV_MODE: 

			break;
			case NO_MODE: 
			default:
			break;
		}

		if( Drone.Shutdown() ) return 0;
	}


	return 0;
}