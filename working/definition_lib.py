# Address of Grove
GADDRESS = 0x4
# Grove command codes
dRead_cmd 		= [1]
dWrite_cmd 		= [2]
aRead_cmd 		= [3]
aWrite_cmd 		= [4]
pMode_cmd 		= [5]
uRead_cmd 		= [7]
acc_xyz_cmd 	= [20]
rtc_getTime_cmd = [30]

uDelay_32bit	= .15

# I2C address for each sensor
def US_sensor_pins():
	return US_Sensors.values()
US_Sensors = {
	'rL': 8,
	'rR': 7,
	'fL': 5,
	'fR': 6
	}

US_Reading = {
	'rL': 0,
	'rR': 0,
	'fL': 0,
	'fR': 0
	}

