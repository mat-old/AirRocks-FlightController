/*
*  Inertia Measurement provider 
*  this class provides the required higher level data for the PIDctrl
*/
#include "arfcDefines.hpp"
#include "imuInterface.hpp"
#ifndef IMPROVIDER
#define IMPROVIDER 
class IMprovider : public IMUinterface {
private:
public:
	IMprovider() : IMUinterface() {}
	~IMprovider() {}

	IMprovider& Open() {
		IMUinterface::Open();
	}

};

#endif