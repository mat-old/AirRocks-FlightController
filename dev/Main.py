#!/usr/bin/python

# attitude is all IMU data
# vector is the current physical vector, last, and future goal
class Moment:
	attitude = False
	vector   = False
	reward   = 0
	def __init__():
		attitude = Attitude()
		vector   = Vector()
		reward   = 0

	def start:
		return self.vector[0]

	def current:
		return self.vector[1]

	def goal:
		return self.vector[2]


class ord:
	x = 0
	y = 0
	def __init__(x,y):
		self.x = x
		self.y = y


class Vector:
	points = []
	def __init__():
		pass

	def append( x,y ):
		self.points.append( ord(x,y) )

class Attitude:
	gyro          = False
	magnetometer  = False
	accelerometer = False

	def __init__(gyro, mag, accel):
		self.gyro = gyro
		self.magnetometer = mag
		self.accelerometer = accel

class Reading:
	x = 0
	y = 0
	z = 0
	def __init__(x,y,z):
		self.x = x
		self.y = y
		self.z = z

# class Flight


print "okay"