# Define basic IO methods and routines
# pass everything in by value, NO GLOBALS
def SensorRead( time, delay, bus, address, command, sensor_pin ):
	write_i2c_block( bus, address, command+[ sensor_pin, 0, 0] )
	time.sleep(delay)
	read_i2c_byte( bus, address )
	Bytes = read_i2c_block( bus, address ) 
	return ( Bytes[1]*256 + Bytes[2])


def write_i2c_block( bus, address, block ):
	try:
		return bus.write_i2c_block_data( address, 1, block)
	except IOError:
		print "IOError 0x1"
		return [-1,-1,-1]

#Read I2C byte
def read_i2c_byte( bus, address ):
	try:
		return bus.read_byte( address )
	except IOError:
		print "IOError 0x2"
		return [-1,-1,-1]


#Read I2C block
def read_i2c_block( bus, address ):
	try:
		return bus.read_i2c_block_data( address, 1)
	except IOError:
		print "IOError 0x3"
		return [-1,-1,-1]
