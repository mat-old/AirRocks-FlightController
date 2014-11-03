#include "Includes.hpp"
#include <algorithm>

using namespace Defines;

bool Exists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}

bool systemsTest() {
	return true;
}

int main(int argc, char *argv[]) {

	if( Exists( argv, argv+argc, "-test" ) ) {
		Drone_mode = TEST_MODE;
	} else
	if( Exists( argv, argv+argc, "-tune" ) ) {
		Drone_mode = TUNE_MODE;
	} else
	if( Exists( argv, argv+argc, "-UAV" ) ) {
		Drone_mode = UAV_MODE;
	}



	if( Drone_mode == TEST_MODE ) {
		//test();
		emit( systemsTest?"true":"false" );
	} else
	if( Drone_mode == UAV_MODE ) {
		while(!SHUTDOWN_FLAG) {
			UAV();
		}
	} else emit("Nothing to do...");
	return 0;
}