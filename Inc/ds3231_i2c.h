#ifndef DS3231_H
#define DS3231_H


// BITS and ADDRESSES for I2C communication with DS3231 Real time Clock

#define I2C_WRITE_BIT 	0
#define I2C_READ_BIT	1


// Address of the DS3231 is 1101000 + READ/WRITE BIT
#define DS3231_ADDRESS 0x68


#endif
