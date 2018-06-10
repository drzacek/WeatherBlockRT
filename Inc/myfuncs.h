#ifndef MYFUNCS_H
#define MYFUNCS_H

// This file contains function definitions written by me, AKA useful code, non generated
// Jacek Mainczyk, 02.06.2018

#include <stdint.h>

// Two functions to read and write from/to USB Serial console
int readUSBSerial(unsigned char* receiveText);
int writeUSBSerial(unsigned char* sendText);

// Read Information from DHT22
int readDHT22Data();

uint32_t DHT22_GetReadings(void);
uint16_t DHT22_DecodeReadings(void);
uint16_t DHT22_GetHumidity(void);
uint16_t DHT22_GetTemperature(void);

// Read/write time from real time clock
int getCurrentTime();
int setCurrentTime(unsigned char hour, unsigned char min, unsigned char sec);

int getCurrentDate();
int setCurrentDate(int weekDay, int day, int month, int year);
int DatetoBIN(uint32_t* bin, unsigned int weekDay, unsigned int day, unsigned int month, unsigned int year);

unsigned char SecToBIN(unsigned char sec);
unsigned char MinToBIN(unsigned char min);
unsigned char HourToBIN(unsigned char hour);

unsigned char BINtoSec(unsigned char bin);
unsigned char BINtoMin(unsigned char bin);
unsigned char BINtoHour(unsigned char bin);




// Show information on the LCD screen
int paintInterface();


typedef struct
{
	// TIME
	char dt_seconds;
	char dt_minutes;
	char dt_hours;

	// DATE
	char dt_weekday;
	char dt_day;
	char dt_month;
	short dt_year;
} DATETIME_STRUCT;


#define BIT0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
#define BIT1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define BIT2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define BIT3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define BIT4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define BIT5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define BIT6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define BIT7                 ((uint16_t)0x0080)  /* Pin 7 selected    */


extern char* weekDayNames[7];

#endif
