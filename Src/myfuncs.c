// This file contains function definitions written by me, AKA useful code, non generated
// Jacek Mainczyk, 02.06.2018


#include <stdint.h>
#include <string.h>
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal.h"
#include "usbd_cdc_if.h"
#include "myfuncs.h"

#include "ds3231_i2c.h"

char newUSBCommandReceived;
unsigned char userRxBuf[256];

DATETIME_STRUCT dateTime;


// DHT22 Related shit - copied from https://github.com/LonelyWolf/stm32/blob/master/am2302/dht22.c
// Cause yeah - it was there and I took it. What'you gonna do about, huh?


extern volatile short microsecondCounter;
extern I2C_HandleTypeDef hi2c1;


#define MICROSECCONSTANT 72


void Delay_us(short us)
{
//	microsecondCounter = 0;
//
//	while(microsecondCounter < us)
//	{
//
//	}
	for(short i = 0; i < us; i++)
	{
		for(int j = 0; j< MICROSECCONSTANT; j++)
		{
			asm volatile("nop");
		}
	}
}



	/* Port and pin with DHT22 sensor*/
	#define DHT22_GPIO_PORT            GPIOB
	#define DHT22_GPIO_CLOCK           RCC_APB2Periph_GPIOB
	#define DHT22_GPIO_PIN             GPIO_PIN_12

	/* DHT22_GetReadings response codes */
	#define DHT22_RCV_OK               0 // Return with no error
	#define DHT22_RCV_NO_RESPONSE      1 // No response from sensor
	#define DHT22_RCV_BAD_ACK1         2 // Bad first half length of ACK impulse
	#define DHT22_RCV_BAD_ACK2         3 // Bad second half length of ACK impulse
	#define DHT22_RCV_RCV_TIMEOUT      4 // It was timeout while receiving bits




	uint8_t  hMSB = 0;
	uint8_t  hLSB = 0;
	uint8_t  tMSB = 0;
	uint8_t  tLSB = 0;
	uint8_t  parity_rcv = 0;
	uint16_t bits[40];
	static GPIO_InitTypeDef PORT;



uint16_t DHT22_GetHumidity(void)
{
	return (hMSB << 8) + hLSB;
}

uint16_t DHT22_GetTemperature(void)
{
	return (tMSB << 8) + tLSB;
}


// Read Information from DHT22
int readDHT22Data()
{
	uint32_t wait;
	uint8_t i;


	// Switch pin to input with Pull-Up
	PORT.Mode = GPIO_MODE_OUTPUT_PP;
	//PORT.Pull = GPIO_NOPULL;
	PORT.Pin = DHT22_GPIO_PIN;
	PORT.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DHT22_GPIO_PORT,&PORT);



	// Generate start impulse for sensor
	DHT22_GPIO_PORT->BRR = DHT22_GPIO_PIN; // Pull down SDA (Bit_SET)
	HAL_Delay(2); // Host start signal at least 800us
	DHT22_GPIO_PORT->BSRR = DHT22_GPIO_PIN; // Release SDA (Bit_RESET)

	//Delay_us(50);
	// Switch pin to input with Pull-Up
	PORT.Mode = GPIO_MODE_INPUT;
	PORT.Pull = GPIO_NOPULL;
	PORT.Pin = DHT22_GPIO_PIN;
	HAL_GPIO_Init(DHT22_GPIO_PORT,&PORT);

	// Wait for AM2302 to start communicate
	wait = 0;
	while ((DHT22_GPIO_PORT->IDR & DHT22_GPIO_PIN) && (wait++ < 200)) Delay_us(2);
	if (wait > 50) return DHT22_RCV_NO_RESPONSE;

	// Check ACK strobe from sensor
	wait = 0;
	while (!(DHT22_GPIO_PORT->IDR & DHT22_GPIO_PIN) && (wait++ < 100)) Delay_us(1);
	if ((wait < 8) || (wait > 15)) return DHT22_RCV_BAD_ACK1;

	wait = 0;
	while ((DHT22_GPIO_PORT->IDR & DHT22_GPIO_PIN) && (wait++ < 100)) Delay_us(1);
	if ((wait < 8) || (wait > 15)) return DHT22_RCV_BAD_ACK2;

	// ACK strobe received --> receive 40 bits
	i = 0;
	while (i < 40) {
		// Measure bit start impulse (T_low = 50us)
		wait = 0;
		while (!(DHT22_GPIO_PORT->IDR & DHT22_GPIO_PIN) && (wait++ < 20)) Delay_us(1);
		if (wait > 16) {
			// invalid bit start impulse length
			bits[i] = 0xffff;
			while ((DHT22_GPIO_PORT->IDR & DHT22_GPIO_PIN) && (wait++ < 20)) Delay_us(1);
		} else {
			// Measure bit impulse length (T_h0 = 25us, T_h1 = 70us)
			wait = 0;
			while ((DHT22_GPIO_PORT->IDR & DHT22_GPIO_PIN) && (wait++ < 20)) Delay_us(1);
			bits[i] = (wait < 16) ? wait : 0xffff;
		}

		i++;
	}

	for (i = 0; i < 40; i++) if (bits[i] == 0xffff) return DHT22_RCV_RCV_TIMEOUT;

	return DHT22_RCV_OK;
}

uint16_t DHT22_DecodeReadings(void)
{
	uint8_t parity;
	uint8_t  i = 0;

	hMSB = 0;
	for (; i < 8; i++) {
		hMSB <<= 1;
		if (bits[i] > 7) hMSB |= 1;
	}
	hLSB = 0;
	for (; i < 16; i++) {
		hLSB <<= 1;
		if (bits[i] > 7) hLSB |= 1;
	}
	tMSB = 0;
	for (; i < 24; i++) {
		tMSB <<= 1;
		if (bits[i] > 7) tMSB |= 1;
	}
	tLSB = 0;
	for (; i < 32; i++) {
		tLSB <<= 1;
		if (bits[i] > 7) tLSB |= 1;
	}
	for (; i < 40; i++) {
		parity_rcv <<= 1;
		if (bits[i] > 7) parity_rcv |= 1;
	}

	parity  = hMSB + hLSB + tMSB + tLSB;

	return (parity_rcv << 8) | parity;
}




// Two functions to read and write from/to USB Serial console
int readUSBSerial(unsigned char* receiveText)
{

	if(newUSBCommandReceived)
	{
		// Copy the USB Receive buffer to receiveText
		strcpy(receiveText, userRxBuf);
		newUSBCommandReceived = 0;
	}
	else
	{
		// Nothing to receive, walk away
		return(1);
	}
	//CDC_Receive_FS(receiveText, 255);

	return(0);
}

int writeUSBSerial(unsigned char* sendText)
{
	CDC_Transmit_FS(sendText, strlen(sendText));

	return(0);
}




// Read/write time from real time clock
int getCurrentTime()
{
	// How the I2C communication with DS3231 works:
	//	1. MASTER (stm32 in this case), sends address of a register it wants to receive
	//		- START I2C
	//		- Send 7bit address + WRITE BIT
	//		- Receive ACK from DS3231
	//		- Transmit a BYTE containing address of the register(registers?) we want to read
	//		- STOP
	//	2. MASTER initiates DS3231 transmit
	//		- START I2C
	//		- Send 7bit address + READ BIT
	//		- Receive ACK from DS3231
	//		- Receive a byte from the register specified earlier
	//		- STOP

	int errCheck = 0;

	int addr = DS3231_ADDRESS<<1;
	unsigned char data[4];
	memset(data, 0, 4);

	// All
	data[0] = 0x00;
	errCheck = HAL_I2C_Master_Transmit(&hi2c1, addr, data, 1, 100);
	HAL_Delay(20);
	errCheck = HAL_I2C_Master_Receive(&hi2c1, addr, data, 4, 100);
	HAL_Delay(20);

	dateTime.dt_hours = 	BINtoHour(data[2]);
	dateTime.dt_minutes =	BINtoMin(data[1]);
	dateTime.dt_seconds = 	BINtoSec(data[0]);

	return(errCheck);
}

int setCurrentTime(unsigned char hour, unsigned char min, unsigned char sec)
{
	int errCheck = 0;

	int addr = DS3231_ADDRESS<<1;
	unsigned char data[4];
	memset(data, 0, 4);

	data[0] = 0x00;

	data[1] = SecToBIN(sec);
	data[2] = MinToBIN(min);
	data[3] = HourToBIN(hour);

	errCheck = HAL_I2C_Master_Transmit(&hi2c1, addr, data, 4, 100);
	HAL_Delay(20);

	return(0);
}


int getCurrentDate()
{
	int errCheck = 0;

	int addr = DS3231_ADDRESS<<1;
	unsigned char data[4];
	memset(data, 0, 4);

	// All
	data[0] = 0x03;
	errCheck = HAL_I2C_Master_Transmit(&hi2c1, addr, data, 1, 100);
	HAL_Delay(20);
	errCheck = HAL_I2C_Master_Receive(&hi2c1, addr, data, 4, 100);
	HAL_Delay(20);

	dateTime.dt_weekday = data[0] & 0x7;	// Make sure we only use first 3 bits, rest SHOULD be 0, but you never know
	dateTime.dt_day		= (data[1] & 0xf) + ((data[1] & 0x30) >> 4) *10;
	dateTime.dt_month	= (data[2] & 0xf) + ((data[2] & 0x10) >> 4) *10;
	dateTime.dt_year	= (data[3] & 0xf) + ((data[3] & 0xf0) >> 4) *10 + ((data[2] & BIT7)>>7)*100 + 2000;

	return(errCheck);
}





int setCurrentDate(int weekDay, int day, int month, int year)
{
	int errCheck = 0;

	uint32_t bin;
	int addr = DS3231_ADDRESS<<1;
	unsigned char data[4];
	memset(data, 0, 4);

	data[0] = 0x00;

//	data[1] = SecToBIN(sec);
//	data[2] = MinToBIN(min);
//	data[3] = HourToBIN(hour);


	DatetoBIN(&bin, weekDay, day, month, year);


	errCheck = HAL_I2C_Master_Transmit(&hi2c1, addr, data, 4, 100);
	HAL_Delay(20);

		return(0);
	return(0);
}



unsigned char SecToBIN(unsigned char sec)
{
	unsigned char bin;
	bin = (sec%10) + (sec/10<<4);
	return(bin);
}

unsigned char MinToBIN(unsigned char min)
{
	unsigned char bin;
	bin = (min % 10) + (min/10 << 4);
	return(bin);
}

unsigned char HourToBIN(unsigned char hour)
{
	unsigned char bin;
	// ASSUME 24 HOUR MODE
	bin = hour > 19 ? (hour % 10) + BIT5 : (hour % 10) + BIT4 * (hour > 9);
	return(bin);
}


// Convert readings from DS3231 to human format
unsigned char BINtoSec(unsigned char bin)
{
	unsigned char sec;
	sec = (bin & 0xf) + ((bin>>4)*10);
	return(sec);
}

unsigned char BINtoMin(unsigned char bin)
{
	unsigned char min;
	min = (bin & 0xf) + ((bin>>4)*10);
	return(min);
}

unsigned char BINtoHour(unsigned char bin)
{
	unsigned char hour;
	// hour = if BIT6 set - 12HOUR MODE, else 24HOUR MODDE, BIT5 set - PM, else 20s +
	hour = (bin & 0x40) ? (bin & 0xf) + ((bin & 0x20) >> 5)*12 + ((bin & 0x10) >> 4)*10 : (bin & 0xf) + ((bin & 0x20) >> 5)*20 + ((bin & 0x10) >> 4)*10 ;
	return(hour);
}


int BINtoDate(DATETIME_STRUCT* datetime, unsigned char bin[4])
{
	dateTime.dt_weekday 	= bin[0] & 0x7;	// Make sure we only use first 3 bits, rest SHOULD be 0, but you never know
	dateTime.dt_day			= (bin[1] & 0xf) + ((bin[1] & 0x30) >> 4) *10;
	dateTime.dt_month		= (bin[2] & 0xf) + ((bin[2] & 0x10) >> 4) *10;
	dateTime.dt_year		= (bin[3] & 0xf) + ((bin[3] & 0xf0) >> 4) *10 + ((bin[2] & BIT7)>>7)*100 + 2000;

	return(0);
}

int DatetoBIN(uint32_t* bin, unsigned int weekDay, unsigned int day, unsigned int month, unsigned int year)
{
	char data[4];



	data[0] = weekDay & 0x7;
	data[1] = (day%10 & 0xf) + ((day/10 << 4) & 0x30);
	data[2] = (month%10 & 0xf) + ((month/10 << 4) & 0x10);

	if(year > 2000)
		year = year - 2000;

	data[3] = (year%10 & 0xf) + ((year/10 << 4) & 0xf0);

	memcpy(bin, data, 4);
	return(*bin);
}


// Show information on the LCD screen
int paintInterface()
{

	return(0);
}
