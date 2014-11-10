#ifndef LSM303_h
#define LSM303_h
#include "../Defines.hpp"
#include "includes.hpp"
#include <stdint.h>
#include "I2CBus.h"

// register addresses
#define LSM303_CRA_REG_M         0x00 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_CRB_REG_M         0x01 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_MR_REG_M          0x02 // LSM303DLH, LSM303DLM, LSM303DLHC

#define LSM303_TEMP_OUT_L        0x05 // LSM303D
#define LSM303_TEMP_OUT_H        0x06 // LSM303D
#define LSM303_STATUS_M          0x07 // LSM303D

#define LSM303_SR_REG_M          0x09 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_IRA_REG_M         0x0A // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_IRB_REG_M         0x0B // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_IRC_REG_M         0x0C // LSM303DLH, LSM303DLM, LSM303DLHC

#define LSM303_WHO_AM_I_M        0x0F // LSM303DLM
#define LSM303_WHO_AM_I          0x0F // LSM303D

#define LSM303_INT_CTRL_M        0x12 // LSM303D
#define LSM303_INT_SRC_M         0x13 // LSM303D
#define LSM303_INT_THS_L_M       0x14 // LSM303D
#define LSM303_INT_THS_H_M       0x15 // LSM303D
#define LSM303_OFFSET_X_L_M      0x16 // LSM303D
#define LSM303_OFFSET_X_H_M      0x17 // LSM303D
#define LSM303_OFFSET_Y_L_M      0x18 // LSM303D
#define LSM303_OFFSET_Y_H_M      0x19 // LSM303D
#define LSM303_OFFSET_Z_L_M      0x1A // LSM303D
#define LSM303_OFFSET_Z_H_M      0x1B // LSM303D

#define LSM303_REFERENCE_X       0x1C // LSM303D
#define LSM303_REFERENCE_Y       0x1D // LSM303D
#define LSM303_REFERENCE_Z       0x1E // LSM303D

#define LSM303_CTRL0             0x1F // LSM303D
#define LSM303_CTRL_REG1_A       0x20 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_CTRL1             0x20 // LSM303D
#define LSM303_CTRL_REG2_A       0x21 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_CTRL2             0x21 // LSM303D
#define LSM303_CTRL_REG3_A       0x22 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_CTRL3             0x22 // LSM303D
#define LSM303_CTRL_REG4_A       0x23 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_CTRL4             0x23 // LSM303D
#define LSM303_CTRL_REG5_A       0x24 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_CTRL5             0x24 // LSM303D
#define LSM303_CTRL_REG6_A       0x25 // LSM303DLHC
#define LSM303_CTRL6             0x25 // LSM303D
#define LSM303_HP_FILTER_RESET_A 0x25 // LSM303DLH, LSM303DLM
#define LSM303_REFERENCE_A       0x26 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_CTRL7             0x26 // LSM303D
#define LSM303_STATUS_REG_A      0x27 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_STATUS_A          0x27 // LSM303D

#define LSM303_OUT_X_L_A         0x28 // LSM303DLH, LSM303DLM, LSM303DLHC, LSM303D
#define LSM303_OUT_X_H_A         0x29 // LSM303DLH, LSM303DLM, LSM303DLHC, LSM303D
#define LSM303_OUT_Y_L_A         0x2A // LSM303DLH, LSM303DLM, LSM303DLHC, LSM303D
#define LSM303_OUT_Y_H_A         0x2B // LSM303DLH, LSM303DLM, LSM303DLHC, LSM303D
#define LSM303_OUT_Z_L_A         0x2C // LSM303DLH, LSM303DLM, LSM303DLHC, LSM303D
#define LSM303_OUT_Z_H_A         0x2D // LSM303DLH, LSM303DLM, LSM303DLHC, LSM303D

#define LSM303_FIFO_CTRL_REG_A   0x2E // LSM303DLHC
#define LSM303_FIFO_SRC_REG_A    0x2F // LSM303DLHC

#define LSM303_INT1_CFG_A        0x30 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_IG_CFG1           0x30 // LSM303D
#define LSM303_INT1_SRC_A        0x31 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_IG_SRC1           0x31 // LSM303D
#define LSM303_INT1_THS_A        0x32 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_IG_THS1           0x32 // LSM303D
#define LSM303_INT1_DURATION_A   0x33 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_IG_DUR1           0x33 // LSM303D
#define LSM303_INT2_CFG_A        0x34 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_IG_CFG2           0x34 // LSM303D
#define LSM303_INT2_SRC_A        0x35 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_IG_SRC2           0x35 // LSM303D
#define LSM303_INT2_THS_A        0x36 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_IG_THS2           0x36 // LSM303D
#define LSM303_INT2_DURATION_A   0x37 // LSM303DLH, LSM303DLM, LSM303DLHC
#define LSM303_IG_DUR2           0x37 // LSM303D

#define LSM303_CLICK_CFG_A       0x38 // LSM303DLHC
#define LSM303_CLICK_CFG         0x38 // LSM303D
#define LSM303_CLICK_SRC_A       0x39 // LSM303DLHC
#define LSM303_CLICK_SRC         0x39 // LSM303D
#define LSM303_CLICK_THS_A       0x3A // LSM303DLHC
#define LSM303_CLICK_THS         0x3A // LSM303D
#define LSM303_TIME_LIMIT_A      0x3B // LSM303DLHC
#define LSM303_TIME_LIMIT        0x3B // LSM303D
#define LSM303_TIME_LATENCY_A    0x3C // LSM303DLHC
#define LSM303_TIME_LATENCY      0x3C // LSM303D
#define LSM303_TIME_WINDOW_A     0x3D // LSM303DLHC
#define LSM303_TIME_WINDOW       0x3D // LSM303D
#define LSM303_ACT_THS           0x3E // LSM303D
#define LSM303_ACT_DUR           0x3F // LSM303D

#define LSM303_TEMP_OUT_H_M      0x31 // LSM303DLHC
#define LSM303_TEMP_OUT_L_M      0x32 // LSM303DLHC

// Dummy addresses for registers that have inconsistent addresses.
#define LSM303_OUT_X_H_M         -1
#define LSM303_OUT_X_L_M         -2
#define LSM303_OUT_Y_H_M         -3
#define LSM303_OUT_Y_L_M         -4
#define LSM303_OUT_Z_H_M         -5
#define LSM303_OUT_Z_L_M         -6

// Specific addresses for the dummy addresses above:
#define LSM303DLH_OUT_X_H_M      0x03
#define LSM303DLH_OUT_X_L_M      0x04
#define LSM303DLH_OUT_Y_H_M      0x05
#define LSM303DLH_OUT_Y_L_M      0x06
#define LSM303DLH_OUT_Z_H_M      0x07
#define LSM303DLH_OUT_Z_L_M      0x08

#define LSM303DLM_OUT_X_H_M      0x03
#define LSM303DLM_OUT_X_L_M      0x04
#define LSM303DLM_OUT_Z_H_M      0x05
#define LSM303DLM_OUT_Z_L_M      0x06
#define LSM303DLM_OUT_Y_H_M      0x07
#define LSM303DLM_OUT_Y_L_M      0x08

#define LSM303DLHC_OUT_X_H_M     0x03
#define LSM303DLHC_OUT_X_L_M     0x04
#define LSM303DLHC_OUT_Z_H_M     0x05
#define LSM303DLHC_OUT_Z_L_M     0x06
#define LSM303DLHC_OUT_Y_H_M     0x07
#define LSM303DLHC_OUT_Y_L_M     0x08

#define LSM303D_OUT_X_L_M        0x08
#define LSM303D_OUT_X_H_M        0x09
#define LSM303D_OUT_Y_L_M        0x0A
#define LSM303D_OUT_Y_H_M        0x0B
#define LSM303D_OUT_Z_L_M        0x0C
#define LSM303D_OUT_Z_H_M        0x0D
/*
Relevant Pololu products:

#1250  LSM303DLH                  SA0_A pulled to GND, accessible via.
#1264  LSM303DLH + L3G4200D (v0)  SA0_A pulled to GND, accessible thru-hole.
#1265  LSM303DLM + L3G4200D (v1)  SA0_A pulled to GND, accessible thru-hole.
#1268  LSM303DLHC + L3GD20  (v2)  (chip has no SA0 line)
#1273  LSM303DLM                  SA0_A pulled to GND, accessible via.
#2124  LSM303DLHC                 (chip has no SA0 line)
#2127  LSM303D                    SA0 pulled to VDD, accessible thru-hole.
#2468  LSM303D + L3GD20H (v3)     SA0 pulled to VDD, accessible thru-hole
*/

// LSM303D addresses:
#define D_SA0_HIGH_ADDRESS              0b0011101
#define D_SA0_LOW_ADDRESS               0b0011110

// LSM303DLH, LSM303DLM, LSM303DLHC addresses:
#define NON_D_MAG_ADDRESS               0b0011110
#define NON_D_ACC_SA0_LOW_ADDRESS       0b0011000
#define NON_D_ACC_SA0_HIGH_ADDRESS      0b0011001

#define D_WHO_ID    0x49
#define DLM_WHO_ID  0x3C

class LSM303
{
 public:
	int a[3];  // accelerometer readings
	int m[3];  // magnetometer readings



	LSM303(const char * i2cDeviceName) : i2c_mag(i2cDeviceName), i2c_acc(i2cDeviceName)
	{
		I2CBus i2c(i2cDeviceName);
		bool sa0;

		if (i2c.tryReadByte(D_SA0_HIGH_ADDRESS, LSM303_WHO_AM_I) == D_WHO_ID)
		{
			// Detected LSM303D with SA0 high.
			device = Device::LSM303D;
			sa0 = true;
		}
		else if (i2c.tryReadByte(D_SA0_LOW_ADDRESS, LSM303_WHO_AM_I) == D_WHO_ID)
		{
			// device responds to address 0011110 with D ID; it's a D with SA0 low
			device = Device::LSM303D;
			sa0 = false;
		}
		// Remaining possibilities: LSM303DLHC, LSM303DLM, or LSM303DLH.
		// LSM303DLHC seems to respond to WHO_AM_I request the same way as DLM, even
		// though this register isn't documented in its datasheet.
		// Instead of reading the WHO_AM_I register, guess if it's an LSM303DLHC based on
		// accelerometer address, because Pololu boards with the LSM303DLM or LSM303DLH
		// pull SA0 low.  The LSM303DLHC doesn't have SA0 but uses same accelerometer address
		// as LSM303DLH/LSM303DLM with SA0 high).
		else if (i2c.tryReadByte(NON_D_ACC_SA0_HIGH_ADDRESS, LSM303_CTRL_REG1_A) >= 0)
		{
			// Guess that it's an LSM303DLHC.
			device = Device::LSM303DLHC;
			sa0 = true;
		}
		// Remaining possibilities: LSM303DLM or LSM303DLH.
		// Check accelerometer with SA0 low address to make sure it's responsive.
		else if (i2c.tryReadByte(NON_D_ACC_SA0_LOW_ADDRESS, LSM303_CTRL_REG1_A) >= 0)
		{
			sa0 = false;

			if (i2c.tryReadByte(NON_D_MAG_ADDRESS, LSM303_WHO_AM_I_M) == DLM_WHO_ID)
			{
				// Detected LSM303DLM with SA0 low.
				device = Device::LSM303DLM;
			}
			else
			{
				// Guess that it's an LSM303DLH with SA0 low.
				device = Device::LSM303DLH;
			}
		}
		else
		{
			/*i get thrown when?*/
			//throw std::runtime_error("Could not detect accelerometer.");
			throw IMU_BAD_CONNECT;
		}

		// Set the I2C addresses.
		if (device == Device::LSM303D)
		{
			uint8_t address = sa0 ? D_SA0_HIGH_ADDRESS : D_SA0_LOW_ADDRESS;
			i2c_acc.addressSet(address);
			i2c_mag.addressSet(address);
		}
		else
		{
			i2c_acc.addressSet(sa0 ? NON_D_ACC_SA0_HIGH_ADDRESS : NON_D_ACC_SA0_LOW_ADDRESS);
			i2c_mag.addressSet(NON_D_MAG_ADDRESS);
		}

		// Make sure we can actually read an accelerometer control register.
		// (This throws an exception if there is a problem.)
		readAccReg(LSM303_CTRL_REG1_A);

		// For non-LSM303 devices, the magnetometer has a different address so
		// we should make sure it is actually there.
		if (device != Device::LSM303D && readMagReg(LSM303_WHO_AM_I_M) != 0x3C)
		{
			throw std::runtime_error("Error getting \"Who Am I\" register for magnetometer.\n");
		}
	}

	uint8_t readMagReg(uint8_t reg)
	{
		return i2c_mag.readByte(reg);
	}

	uint8_t readAccReg(uint8_t reg)
	{
		return i2c_acc.readByte(reg);
	}

	void writeMagReg(uint8_t reg, uint8_t value)
	{
		i2c_mag.writeByte(reg, value);
	}

	void writeAccReg(uint8_t reg, uint8_t value)
	{
		i2c_acc.writeByte(reg, value);
	}

	// Turns on the LSM303's accelerometer and magnetometers and places them in normal
	// mode.
	void enable(void)
	{
		if (device == Device::LSM303D)
		{
			//// LSM303D Accelerometer

			// AODR = 0101 (50 Hz ODR)
			// AZEN = AYEN = AXEN = 1 (all axes enabled)
			writeAccReg(LSM303_CTRL1, 0b01010111);

			// AFS = 011 (8 g full scale)
			writeAccReg(LSM303_CTRL2, 0b00011000);

			//// LSM303D Magnetometer
			// M_RES = 11 (high resolution mode)
			// M_ODR = 001 (6.25 Hz ODR)
			writeMagReg(LSM303_CTRL5, 0b01100100);

			// MFS = 01 (4 gauss full scale)
			writeMagReg(LSM303_CTRL6, 0b00100000);

			// MLP = 0 (low power mode off)
			// MD = 00 (continuous-conversion mode)
			writeMagReg(LSM303_CTRL7, 0b00000000);
		}
		else if (device == Device::LSM303DLHC)
		{
			//// LSM303DLHC Accelerometer

			// ODR = 0100 (50 Hz ODR)
			// LPen = 0 (normal mode)
			// Zen = Yen = Xen = 1 (all axes enabled)
			writeAccReg(LSM303_CTRL_REG1_A, 0b01000111);

			// FS = 10 (8 g full scale)
			// HR = 1 (high resolution enable)
			writeAccReg(LSM303_CTRL_REG4_A, 0b00101000);

			//// LSM303DLHC Magnetometer

			// DO = 011 (7.5 Hz ODR)
			writeMagReg(LSM303_CRA_REG_M, 0b00001100);

			// GN = 001 (+/- 1.3 gauss full scale)
			writeMagReg(LSM303_CRB_REG_M, 0b00100000);

			// MD = 00 (continuous-conversion mode)
			writeMagReg(LSM303_MR_REG_M, 0b00000000);
		}
		else
		{
			//// LSM303DLM or LSM303DLH Accelerometer

			// FS = 11 (8 g full scale)
			writeAccReg(LSM303_CTRL_REG4_A, 0b00110000);

			// PM = 001 (normal mode)
			// DR = 00 (50 Hz ODR)
			// Zen = Yen = Xen = 1 (all axes enabled)
			writeAccReg(LSM303_CTRL_REG1_A, 0b00100111);

			//// LSM303DLM or LSM303DLH Magnetometer

			// DO = 011 (7.5 Hz ODR)
			writeMagReg(LSM303_CRA_REG_M, 0b00001100);

			// GN = 001 (+/- 1.3 gauss full scale)
			writeMagReg(LSM303_CRB_REG_M, 0b00100000);

			 // MD = 00 (continuous-conversion mode)
			writeMagReg(LSM303_MR_REG_M, 0b00000000);
		}
	}

	void readAcc(void)
	{
		uint8_t block[6];
		i2c_acc.readBlock(0x80 | LSM303_OUT_X_L_A, sizeof(block), block);
		a[0] = (int16_t)(block[0] | block[1] << 8);
		a[1] = (int16_t)(block[2] | block[3] << 8);
		a[2] = (int16_t)(block[4] | block[5] << 8);
	}

	void readMag(void)
	{
		uint8_t block[6];

		if (device == Device::LSM303D)
		{
			// LSM303D: XYZ order, little endian
			i2c_mag.readBlock(0x80 | LSM303D_OUT_X_L_M, sizeof(block), block);
			m[0] = (int16_t)(block[0] | block[1] << 8);
			m[1] = (int16_t)(block[2] | block[3] << 8);
			m[2] = (int16_t)(block[4] | block[5] << 8);
		}
		else if (device == Device::LSM303DLH)
		{
			// LSM303DLH: XYZ order, big endian
			i2c_mag.readBlock(0x80 | LSM303DLH_OUT_X_H_M, sizeof(block), block);
			m[0] = (int16_t)(block[1] | block[0] << 8);
			m[1] = (int16_t)(block[3] | block[2] << 8);
			m[2] = (int16_t)(block[5] | block[4] << 8);
		}
		else
		{
			// LSM303DLM, LSM303DLHC: XZY order, big endian (and same addresses)
			i2c_mag.readBlock(0x80 | LSM303DLM_OUT_X_H_M, sizeof(block), block);
			m[0] = (int16_t)(block[1] | block[0] << 8);
			m[1] = (int16_t)(block[5] | block[4] << 8);
			m[2] = (int16_t)(block[3] | block[2] << 8);
		}
	}

	void read(void)
	{
		readAcc();
		readMag();
	}


private:
	I2CBus i2c_mag, i2c_acc;
	enum class Device {
		LSM303DLH,
		LSM303DLM,
		LSM303DLHC,
		LSM303D,
	} device;
};

#endif
