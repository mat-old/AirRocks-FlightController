#! /usr/bin/python
# Initialize.py - setup all pins and check sensors & pre-flight tasks
# Main program
import RPi.GPIO as GPIO
import smbus 	as SMBUS
import time 	as TIME
import math

from 	interface_lib import *
import definition_lib as dlib
import setup 

import signal
import sys





# localize EVERYTHING
def main(argv = 0):
	print "Initializing"
	BUS     	= setup.bus( GPIO, SMBUS )
	GADDRESS 	= dlib.GADDRESS
	US_READ  	= dlib.uRead_cmd
	US_DELAY 	= dlib.uDelay_32bit
	rl,rr,fl,fr = dlib.US_sensor_pins()

	while not setup.ERR_FLAG_:
		setup.TEST( lambda: setup.US_TEST( TIME, US_DELAY, BUS, GADDRESS, US_READ, dlib.US_Sensors ) ) 
		print setup.ERR_FLAG_






def signal_handler(signal, frame):
	#GPIO.cleanup() #for SPI bus only
	print '...'
	sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)
main()