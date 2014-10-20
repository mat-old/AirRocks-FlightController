#ifndef _L3G_h
#define _L3G_h

#include "I2CBus.h"
#include <stdexcept>

#define L3G_WHO_AM_I      0x0F

#define L3G_CTRL_REG1     0x20
#define L3G_CTRL_REG2     0x21
#define L3G_CTRL_REG3     0x22
#define L3G_CTRL_REG4     0x23
#define L3G_CTRL_REG5     0x24
#define L3G_REFERENCE     0x25
#define L3G_OUT_TEMP      0x26
#define L3G_STATUS_REG    0x27

#define L3G_OUT_X_L       0x28
#define L3G_OUT_X_H       0x29
#define L3G_OUT_Y_L       0x2A
#define L3G_OUT_Y_H       0x2B
#define L3G_OUT_Z_L       0x2C
#define L3G_OUT_Z_H       0x2D

#define L3G_FIFO_CTRL_REG 0x2E
#define L3G_FIFO_SRC_REG  0x2F

#define L3G_INT1_CFG      0x30
#define L3G_INT1_SRC      0x31
#define L3G_INT1_THS_XH   0x32
#define L3G_INT1_THS_XL   0x33
#define L3G_INT1_THS_YH   0x34
#define L3G_INT1_THS_YL   0x35
#define L3G_INT1_THS_ZH   0x36
#define L3G_INT1_THS_ZL   0x37
#define L3G_INT1_DURATION 0x38
#define L3G_LOW_ODR       0x39

#define L3G4200D_ADDRESS_SA0_LOW  (0xD0 >> 1)
#define L3G4200D_ADDRESS_SA0_HIGH (0xD2 >> 1)
#define L3GD20_ADDRESS_SA0_LOW    (0xD4 >> 1)
#define L3GD20_ADDRESS_SA0_HIGH   (0xD6 >> 1)

class L3G
{
public:
	// gyro angular velocity readings
	int g[3];

	L3G(const char * i2cDeviceName) : i2c(i2cDeviceName)
	{
		detectAddress();
	}

	void detectAddress()
	{
		int whoami;

		i2c.addressSet(L3G4200D_ADDRESS_SA0_LOW);
		if (i2c.tryReadByte(L3G_WHO_AM_I) == 0xD3)
		{
			// Detected L3G4200D with the SA0 pin low.
			return;
		}

		i2c.addressSet(L3G4200D_ADDRESS_SA0_HIGH);
		if (i2c.tryReadByte(L3G_WHO_AM_I) == 0xD3)
		{
			// Detected L3G4200D with the SA0 pin high.
			return;
		}

		i2c.addressSet(L3GD20_ADDRESS_SA0_LOW);
		whoami = i2c.tryReadByte(L3G_WHO_AM_I);
		if (whoami == 0xD4)
		{
			// Detected L3GD20 with the SA0 pin low.
			return;
		}
		if (whoami == 0xD7)
		{
			// Detected L3GD20H with the SA0 pin low.
			return;
		}

		i2c.addressSet(L3GD20_ADDRESS_SA0_HIGH);
		whoami = i2c.tryReadByte(L3G_WHO_AM_I);
		if (whoami == 0xD4)
		{
			// Detected L3GD20 with the SA0 pin high.
			return;
		}
		if (whoami == 0xD7)
		{
			// Detected L3GD20H with the SA0 pin high.
			return;
		}

		throw std::runtime_error("Could not detect gyro.");
	}

	// Turns on the gyro and places it in normal mode.
	void enable()
	{
		writeReg(L3G_CTRL_REG1, 0b00001111); // Normal power mode, all axes enabled
		writeReg(L3G_CTRL_REG4, 0b00100000); // 2000 dps full scale
	}

	void writeReg(uint8_t reg, uint8_t value)
	{
		i2c.writeByte(reg, value);
	}

	uint8_t readReg(uint8_t reg)
	{
		return i2c.readByte(reg);
	}

	void read()
	{
		uint8_t block[6];
		i2c.readBlock(0x80 | L3G_OUT_X_L, sizeof(block), block);

		g[0] = (int16_t)(block[1] << 8 | block[0]);
		g[1] = (int16_t)(block[3] << 8 | block[2]);
		g[2] = (int16_t)(block[5] << 8 | block[4]);
	}


private:
	I2CBus i2c;
};

#endif
