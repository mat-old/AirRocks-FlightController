from 	interface_lib import *
import definition_lib as dlib
import setup 
import datetime

ERR_FLAG_ = False

def bus( GPIO, SMBUS ):
	rev = GPIO.RPI_REVISION
	if rev == 2 or rev == 3:
		return SMBUS.SMBus(1) 
	else:
		return SMBUS.SMBus(0)


def TEST(cb):
	a = datetime.datetime.now()
	cb()
	b = datetime.datetime.now()
	c = b - a
	print c.total_seconds(), "s"


def US_TEST( TIME, US_DELAY, BUS, GADDRESS, US_READ, sensors ):
	global ERR_FLAG_
	for pin in sensors:
		x = SensorRead( TIME, US_DELAY, BUS, GADDRESS, US_READ, sensors[pin] )
		if x<=0:
			ERR_FLAG_ = True
		print "Testing US_", pin, "[ OK ]" if x>0 else " [ FAIL ] ", x


		