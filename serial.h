/*
 * serial.h
 *
 *  Created on: 2015/03/01
 *      Author: Koizumi
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdio.h>
#include <stdarg.h>
#include"iodefine.h"

//1バイト送信
void put1byte(char c);

//nバイト送信
void putnbyte(char *buf, int len);

//文字出力関数　myprintf　の定義
int myprintf(const char *fmt, ...);


#endif /* SERIAL_H_ */
