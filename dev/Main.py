#!/usr/bin/python

import math as Math
import Numpy

# attitude is all IMU data
# vector is the current physical vector, last, and future goal
class Moment:
	attitude = False
	path     = False
	reward   = 0
	def __init__():
		attitude = Attitude()
		path     = Path()
		reward   = 0

	def start():
		return self.path[0]

	def current():
		return self.path[1]

	def goal():
		return self.path[2]


class Coordinate:
	x = 0
	y = 0
	def __init__(self, x,y):
		self.x = x
		self.y = y

	def __eq__(self, other):
		return ( isinstance( other, self.__class__ ) and self.x == other.x and self.y == other.y )

	# BOOLEAN  <--  self.isNear( *other-oridnal, *max-distance )
	def isNear( b, threshold ):
		return threshold <= getDistance( self, b )

	# VALUE_TYPE  <-- self.distranceFrom( *Other-ordinal )
	def distanceFrom(self, b ):
		return self.getDistance( self, b )

	# VALUE_TYPE  <-- self.distranceFromOrigin( *Other-ordinal )
	def distanceFromOrigin(self):
		return Math.sqrt( self.x**2 + self.y**2 )

	# VALUE_TYPE  <--  getDistance( *ordinalA, *ordinalB )
	def getDistance(self, a, b ):
		return Math.sqrt( (a.x - b.x)**2 + (a.y - b.y)**2 )

	def __str__(self):
		return "(" + str(self.x) + "," + str(self.y) + ")"



class Path:
	vector = []
	distance = False
	def __init__(self):
		pass

	def append(self, x,y ):
		self.vector.append( Coordinate(x,y) )

	def getDistance(self):
		if self.distance != False:
			return self.distance
		else:
			self.distance = self.vector[0].distanceFromOrigin()
			for i in range( len(self.vector) - 1 ):
				self.distance += self.vector[0].distanceFrom( self.vector[i+1] )
			return self.distance

	def getArc(self):
		pass

	def __str__(self):
		s = ""
		for point in self.vector:
			s+= str(point)
		return s




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


p = Path()

p.append(0,1)
p.append(1,2)
p.append(1,3)


print p
print p.getDistance()