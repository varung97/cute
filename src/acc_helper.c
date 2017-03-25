/*
 * acc_helper.c
 *
 *  Created on: Mar 5, 2017
 *      Author: Varun
 */

#include "acc_helper.h"

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/
#define I2CDEV LPC_I2C2

#define ACC_I2C_ADDR    (0x1D)

#define ACC_ADDR_XOUTL  0x00
#define ACC_ADDR_XOUTH  0x01
#define ACC_ADDR_YOUTL  0x02
#define ACC_ADDR_YOUTX  0x03
#define ACC_ADDR_ZOUTL  0x04
#define ACC_ADDR_ZOUTH  0x05
#define ACC_ADDR_XOUT8  0x06
#define ACC_ADDR_YOUT8  0x07
#define ACC_ADDR_ZOUT8  0x08
#define ACC_ADDR_STATUS 0x09
#define ACC_ADDR_DETSRC 0x0A
#define ACC_ADDR_TOUT   0x0B
#define ACC_ADDR_I2CAD  0x0D
#define ACC_ADDR_USRINF 0x0E
#define ACC_ADDR_WHOAMI 0x0F
#define ACC_ADDR_XOFFL  0x10
#define ACC_ADDR_XOFFH  0x11
#define ACC_ADDR_YOFFL  0x12
#define ACC_ADDR_YOFFH  0x13
#define ACC_ADDR_ZOFFL  0x14
#define ACC_ADDR_ZOFFH  0x15
#define ACC_ADDR_MCTL   0x16
#define ACC_ADDR_INTRST 0x17
#define ACC_ADDR_CTL1   0x18
#define ACC_ADDR_CTL2   0x19
#define ACC_ADDR_LDTH   0x1A
#define ACC_ADDR_PDTH   0x1B
#define ACC_ADDR_PW     0x1C
#define ACC_ADDR_LT     0x1D
#define ACC_ADDR_TW     0x1E

#define ACC_MCTL_MODE(m) ((m) << 0)
#define ACC_MCTL_GLVL(g) ((g) << 2)


#define ACC_STATUS_DRDY 0x01
#define ACC_STATUS_DOVR 0x02
#define ACC_STATUS_PERR 0x04

#define ACC_BITMASK0 0x1
#define ACC_BITMASK6 0x2
#define ACC_BITMASK6 0x6

static int I2CRead(uint8_t addr, uint8_t* buf, uint32_t len)
{
	I2C_M_SETUP_Type rxsetup;

	rxsetup.sl_addr7bit = addr;
	rxsetup.tx_data = NULL;	// Get address to read at writing address
	rxsetup.tx_length = 0;
	rxsetup.rx_data = buf;
	rxsetup.rx_length = len;
	rxsetup.retransmissions_max = 3;

	if (I2C_MasterTransferData(I2CDEV, &rxsetup, I2C_TRANSFER_POLLING) == SUCCESS){
		return (0);
	} else {
		return (-1);
	}
}

static int I2CWrite(uint8_t addr, uint8_t* buf, uint32_t len)
{
	I2C_M_SETUP_Type txsetup;

	txsetup.sl_addr7bit = addr;
	txsetup.tx_data = buf;
	txsetup.tx_length = len;
	txsetup.rx_data = NULL;
	txsetup.rx_length = 0;
	txsetup.retransmissions_max = 3;

	if (I2C_MasterTransferData(I2CDEV, &txsetup, I2C_TRANSFER_POLLING) == SUCCESS){
		return (0);
	} else {
		return (-1);
	}
}

void acc_enable_interrupt() {
	acc_setMode(ACC_MODE_LEVEL);
	gpio_interrupt_enable(0, 3);

	uint8_t buf[3];
	// Motion Detection
	buf[0] = ACC_ADDR_CTL2 & ~ACC_BITMASK0;
	I2CWrite(ACC_I2C_ADDR, buf, 1);

	// Setting threshold to absolute value
	buf[0] = ACC_ADDR_CTL1 & ~ACC_BITMASK6;
	I2CWrite(ACC_I2C_ADDR, buf, 1);

	// Setting threshold value
	buf[0] = ACC_ADDR_LDTH | 20;
	I2CWrite(ACC_I2C_ADDR, buf, 1);

	// Setting interrupt setting
	buf = ACC_ADDR_CTL1;
	buf[2] = 0;
	buf[1] = 1;
	buf[0] = 1;
	I2CWrite(ACC_I2C_ADDR, buf, 3);
}

void acc_interrupt_clear() {
	uint8_t buf[2];

	// Clear interrupt
	buf[2] = ACC_ADDR_INTRST | (1 << 1);
	I2CWrite(ACC_I2C_ADDR, buf, 2);

	// Enable interrupt again to trigger next detection
	buf[2] = ACC_ADDR_INTRST & ~ACC_BITMASK2;
	I2CWrite(ACC_I2C_ADDR, buf, 2);
}
