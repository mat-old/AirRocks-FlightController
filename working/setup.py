from 	interface_lib import *
import definition_lib as dlib
import setup 
import datetime
import sys

ERR_FLAG_ = False

class colors:
    accent = '\033[95m'
    blue = '\033[94m'
    green = '\033[92m'
    yellow = '\033[93m'
    red = '\033[91m'
    text = '\033[0m'

def bus( GPIO, SMBUS ):
	rev = GPIO.RPI_REVISION
	if rev == 2 or rev == 3:
		return SMBUS.SMBus(1) 
	else:
		return SMBUS.SMBus(0)

def TEST(cb):
	sys.stdout.write( "\r" )
	sys.stdout.flush()
	a = datetime.datetime.now()
	s = cb()
	b = datetime.datetime.now()
	c = b - a
	
	sys.stdout.write( s + " " + str(c.total_seconds()) + "      " + '\033[20C' )
	#sys.stdout.flush()

def US_TEST( TIME, US_DELAY, BUS, GADDRESS, US_READ, sensors ):
	global ERR_FLAG_
	s = ""
	for pin in sensors:
		x = SensorRead( TIME, US_DELAY, BUS, GADDRESS, US_READ, sensors[pin] )
		if x<=0:
			ERR_FLAG_ = True
		s += (colors.green if x>0 else colors.red) + "["+pin+"]"

	return s+colors.text