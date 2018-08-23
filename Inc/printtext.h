/*
 * printtext.h
 *
 *  Created on: 15.08.2018
 *      Author: drzac
 */

#ifndef PRINTTEXT_H_
#define PRINTTEXT_H_


void outtextxy32(unsigned short x, unsigned short y, unsigned char* str);
void outcharxy32(unsigned short x, unsigned short y, unsigned char c);
void outtextxy32x(unsigned short x, unsigned short y, unsigned char* str, unsigned char offset);
void outcharxy32x(unsigned short x, unsigned short y, unsigned char c, unsigned char offset);

void marquetextxy32x(unsigned short x, unsigned short y, unsigned char* str);
void marquecharxy32x(unsigned short x, unsigned short y, unsigned char c);

void outtextxy16(unsigned short x, unsigned short y, unsigned char* str);
void outcharxy16(unsigned short x, unsigned short y, unsigned char c);
void outtextxy(unsigned short x, unsigned short y, unsigned char* str);
void outcharxy(unsigned short x, unsigned short y, unsigned char c);
void setCurrentBGColor(unsigned short color);
void setCurrentColor(unsigned short color);

#endif /* PRINTTEXT_H_ */
