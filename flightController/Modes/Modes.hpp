/*
	this class represents the highest level of the drone's software scope
*/
#ifndef MODE_CLASS
#define MODE_CLASS

#include "../Defines.hpp"

#include "../Cores/SubSystem.hpp"

#include "../Types/Potential.hpp"
#include "../Types/Motorgroup.hpp"
#include "../Types/Arming.hpp"

#include "../Systems/pidCtrl.hpp"
#include "../Systems/spiWorker.hpp"
#include "../Systems/imuWorker.hpp"
#include "../Systems/relay.hpp"     

class Mode : public SubSystem {
public:
	DRONE_MODES mode;
	ATOMIC SHUTDOWN_FLAG;

	Mode();

	void UAV(Relay& rel) ;

	void Tuner(Relay& rel, SPIworker& spi, IMUworker& imu);

	bool Shutdown();
	void ShutdownSequence();
	void setMode(int i);

};
#endif