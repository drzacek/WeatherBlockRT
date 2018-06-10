#include "usbcommands.h"
#include "myfuncs.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f1xx_hal.h"

// External variables
extern DATETIME_STRUCT dateTime;

extern int temp;
extern int humi;
extern unsigned char USBtext[256];



// Decodes a command received from USB Virtual Com port
//		Available commands are:
//			ALL 		- does everything
//			DATE 		- prints out date
//			TIME 		- prints out time
//			SETTIME		- sets TIME - requires arguments in format HH:MM:SS
//			SETDATE		- sets DATE - requires arguments in format WD DD.MM.YYYY
//
//	Returns 0 on failure (no known command) or positive int with command code
int DecodeCommand(char* comString)
{
	char command[128];

	memset (command, 0, 128);

	// Make sure no overflow happens
	if(strlen(comString) > 127)
	{
		comString[127] = 0;
	}

	GetFirstWord(command, comString);

	char teststring[128] = "";

	HAL_Delay(5);
	sprintf(teststring, "Command is %s\n", command);
	writeUSBSerial(teststring);
	HAL_Delay(5);

	if(!strcmp(command, "ALL"))
	{
		// Do everything!
		return(COMMAND_ALL);
	}

	if(!strcmp(command, "TIME"))
	{
		// Print TIME
		return(COMMAND_TIME);
	}

	if(!strcmp(command, "DATE"))
	{
		// Print DATE
		return(COMMAND_DATE);
	}

	if(!strcmp(command, "SETTIME"))
	{
		// Set TIME
		return(COMMAND_SETTIME);
	}

	if(!strcmp(command, "SETDATE"))
	{
		// Set DATE
		return(COMMAND_SETDATE);
	}

	if(!strcmp(command, "SENSOR"))
	{
		// Set DATE
		return(COMMAND_SENSOR);
	}

	//if(!strcmp(command, ""))
	{
		// No known command?
		return(0);
	}

}

void DoCommand(int command)
{

	unsigned char text[256] = "";

	if((command == COMMAND_ALL) || (command == COMMAND_TIME))
	{
		HAL_Delay(5);
		sprintf(text, "%02d:%02d:%04d\n", dateTime.dt_hours, dateTime.dt_minutes, dateTime.dt_seconds);
		writeUSBSerial(text);
		HAL_Delay(5);
	}

	if((command == COMMAND_ALL) || (command == COMMAND_DATE))
	{
		HAL_Delay(5);
		sprintf(text, "%02d.%02d.%04d\n", dateTime.dt_day, dateTime.dt_month, dateTime.dt_year);
		writeUSBSerial(text);
		HAL_Delay(5);
	}

	if((command == COMMAND_SETTIME))
	{
		HAL_Delay(5);
		unsigned char hour, min, sec;
		sscanf(USBtext, "SETTIME %d:%d:%d", &hour, &min, &sec);
		setCurrentTime(hour, min, sec);
		HAL_Delay(5);
	}

	if((command == COMMAND_SETDATE))
	{
		HAL_Delay(5);
		HAL_Delay(5);
	}

	if((command == COMMAND_ALL) || (command == COMMAND_SENSOR))
	{
		HAL_Delay(5);
		sprintf(text, "Temperature is %d.%d", temp/10, temp%10);
		writeUSBSerial(text);
		HAL_Delay(5);

		sprintf(text, ", humidity is %d.%d\n", humi/10, humi%10);
		writeUSBSerial(text);
		HAL_Delay(5);
	}


}


void GetFirstWord(char* dest, char* src)
{
	// This function will try to get first WORD out of char*
	// Make sure both are equally long to no overflow happen
	if(dest == NULL)
		return;

	if(src == NULL)
		return;

	uint8_t len = 0;

	for(uint8_t i = 0; i < strlen(src); i++)
	{
		if((src[i] == ' ') || (src[i] == '\n') || (src[i] == '\r'))
		{
			// Found first space! We have a word!
			len = i;
			break;
		}
	}


	if(len == 0)
		len = strlen(src) + 1;

	for(uint8_t i = 0; i < len; i++)
	{
		dest[i] = src[i];
	}

	dest[len] = 0;
}














