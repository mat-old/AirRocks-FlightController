#include "Defines.hpp"
#include "Modes/Modes.hpp"
#include "Types/JWriter.hpp"
#include <algorithm>


bool Exists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}


int main(int argc, char *argv[]) {
	GenericWriter emit;
	Mode Drone;


	if( Exists( argv, argv+argc, "test" ) ) {
		Drone.Drone_mode = TEST_MODE;
	} else
	if( Exists( argv, argv+argc, "tuner" ) ) {
		Drone.Drone_mode = TUNE_MODE;
	} else
	if( Exists( argv, argv+argc, "UAV" ) ) {
		Drone.Drone_mode = UAV_MODE;
	}



	if( Drone.Drone_mode == TEST_MODE ) {
		//test();
		emit( "true" );
	} else
	if( Drone.Drone_mode == UAV_MODE ) {
		while(!Drone.SHUTDOWN_FLAG) {
			Drone.UAV();
		}
	} else 
	if( Drone.Drone_mode == TUNE_MODE) {
		while(!Drone.SHUTDOWN_FLAG) {
			Drone.Tuner();
		}
	}else emit("Nothing to do...");
	return 0;
}