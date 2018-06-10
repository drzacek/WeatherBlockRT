#ifndef USBCOMMANDS_H
#define USBCOMMANDS_H



// Command LIST
#define COMMAND_ALL			1
#define COMMAND_TIME		2
#define COMMAND_DATE		3
#define COMMAND_SETTIME		4
#define COMMAND_SETDATE		5
#define COMMAND_SENSOR		6









int DecodeCommand(char* comString);

void DoCommand(int command);

void GetFirstWord(char* dest, char* src);



#endif
