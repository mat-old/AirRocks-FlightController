
#ifndef MODE_CLASS
#define MODE_CLASS

#include "../Defines.hpp"

#include "../Types/Potential.hpp"
#include "../Types/Motorgroup.hpp"
#include "../Types/Arming.hpp"
#include "../Types/JWriter.hpp"


#include "../Systems/pidCtrl.hpp"
#include "../Systems/spiWorker.hpp"
#include "../Systems/imuWorker.hpp"
#include "../Systems/relay.hpp"     

class Mode {
public:
	DRONE_MODES Drone_mode;
	ATOMIC SHUTDOWN_FLAG;

	Mode();

	void UAV() ;

	void Tuner();


};
#endif