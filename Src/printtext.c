/*
 * printtext.c
 *
 *  Created on: 13.08.2018
 *      Author: drzac
 */

#include <string.h>
#include <ili9341_LCD.h>

// xbm generated fonts
#include "xbm_font.h"

extern unsigned char bgi8x8font[8 *256];
extern unsigned char bgi8x16font[16 *12];
extern unsigned char bgi16x32font[32*2 *12];

extern unsigned short currentColor;
extern unsigned short currentBGColor;


void setCurrentColor(unsigned short color)
{
	currentColor = color;
}

void setCurrentBGColor(unsigned short color)
{
	currentBGColor = color;
}


void outcharxy(unsigned short x, unsigned short y, unsigned char c)
{
	unsigned char charline;


	for(int i = 0; i < 8; i++)
	{
		// Current line being printed
		charline = bgi8x8font[8*c+i];

		for(int j = 0; j < 8; j++)
		{
			if((charline << j) & 0x80)
			{
				ILI9341_Draw_Pixel(x*8 + j, y*8 + i, currentColor);
			}
			else
			{
				ILI9341_Draw_Pixel(x*8 + j, y*8 + i, currentBGColor);
			}
		}
	}
}

void outtextxy(unsigned short x, unsigned short y, unsigned char* str)
{
	short len = strlen(str);

	for(short i = 0; i < len; i++)
	{
		outcharxy(x+i, y, str[i]);
	}
}



short get8x16index(unsigned char c)
{
	switch(c)
	{
		case '0':
			return(0);
			break;
		case '1':
			return(1);
			break;
		case '2':
			return(2);
			break;
		case '3':
			return(3);
			break;
		case '4':
			return(4);
			break;
		case '5':
			return(5);
			break;
		case '6':
			return(6);
			break;
		case '7':
			return(7);
			break;
		case '8':
			return(8);
			break;
		case '9':
			return(9);
			break;
		case ':':
			return(10);
			break;
		case '.':
			return(11);
			break;
		default:
			return(11);
			break;
	}
}




void outcharxy16(unsigned short x, unsigned short y, unsigned char c)
{
	unsigned char charline;
	unsigned char index = get8x16index(c);

	for(int i = 0; i < 16; i++)
	{
		// Current line being printed
		charline = bgi8x16font[16*index+i];

		for(int j = 0; j < 8; j++)
		{
			if((charline >> j) & 0x01)
			{
				ILI9341_Draw_Pixel(x*8 + j, y*16 + i, currentColor);
			}
			else
			{
				ILI9341_Draw_Pixel(x*8 + j, y*16 + i, currentBGColor);
			}
		}
	}
}

void outtextxy16(unsigned short x, unsigned short y, unsigned char* str)
{
	short len = strlen(str);

	for(short i = 0; i < len; i++)
	{
		outcharxy16(x+i, y, str[i]);
	}
}


void outcharxy32(unsigned short x, unsigned short y, unsigned char c)
{
	unsigned short charline;
	unsigned char index = get8x16index(c);

	for(int i = 0; i < 32; i++)
	{
		// Current line being printed
		memcpy(&charline, bgi16x32font+(64*index+2*i), 2);

		for(int j = 0; j < 16; j++)
		{
			if((charline >> j) & 0x01)
			{
				ILI9341_Draw_Pixel(x*16 + j, y*32 + i, currentColor);
			}
			else
			{
				ILI9341_Draw_Pixel(x*16 + j, y*32 + i, currentBGColor);
			}
		}
	}
}

void outtextxy32(unsigned short x, unsigned short y, unsigned char* str)
{
	short len = strlen(str);

	for(short i = 0; i < len; i++)
	{
		outcharxy32(x+i, y, str[i]);
	}
}




void outcharxy32x(unsigned short x, unsigned short y, unsigned char c, unsigned char offset)
{
	unsigned short charline;
	unsigned char index = c - 32;
	//get8x16index(c);

	for(int i = 0; i < 32; i++)
	{
		// Current line being printed
		//memcpy(&charline, bgi16x32font+(64*index+2*i), 2);
		memcpy(&charline, xbmFont[index]+2*i, 2);

		for(int j = 0; j < 16; j++)
		{
			if((charline >> j) & 0x01)
			{
				ILI9341_Draw_Pixel(x*16 + j, y*32 + i + offset, currentColor);
			}
			else
			{
				ILI9341_Draw_Pixel(x*16 + j, y*32 + i + offset, currentBGColor);
			}
		}
	}
}

void outtextxy32x(unsigned short x, unsigned short y, unsigned char* str, unsigned char offset)
{
	short len = strlen(str);

	for(short i = 0; i < len; i++)
	{
		outcharxy32x(x+i, y, str[i], offset);
	}
}


void marquetextxy32x(unsigned short x, unsigned short y, unsigned char* str)
{
	static char bigOffset = 0;
	static char smallOffset = 0;
	static char direction = 0;

	if(bigOffset > 20)
	{
		direction = 1;
	}

	if(bigOffset < 0)
	{
		bigOffset = 0;
		direction = 0;
	}

	short len = strlen(str);

	for(short i = 0; i < len; i++)
	{
		marquecharxy32x(x+i+bigOffset, y, str[i]);
	}

	if(!direction)
		bigOffset++;
	else
		bigOffset--;
}

void marquecharxy32x(unsigned short x, unsigned short y, unsigned char c)
{
	unsigned short charline;
	unsigned char index = c - 32;
	//get8x16index(c);

	for(int i = 0; i < 32; i++)
	{
		// Current line being printed
		//memcpy(&charline, bgi16x32font+(64*index+2*i), 2);
		memcpy(&charline, xbmFont[index]+2*i, 2);

		for(int j = 0; j < 16; j++)
		{
			if((charline >> j) & 0x01)
			{
				ILI9341_Draw_Pixel(x*16 + j, y*32 + i, currentColor);
			}
			else
			{
				ILI9341_Draw_Pixel(x*16 + j, y*32 + i, currentBGColor);
			}
		}
	}
}








