/*
 * serial.c
 *
 *  Created on: 2015/06/01
 *      Author: Koizumi
 */

#include"serial.h"

//1バイト送信
void put1byte(char c) {
	while (SCI1.SSR.BIT.TDRE == 0)
		;       //データ送信が終了するまで待機
	SCI1.SSR.BIT.TDRE = 0;       	   //データ送信命令
	SCI1.TDR = c;       	               	               	   //送信データ書き込み
}

//nバイト送信
void putnbyte(char *buf, int len) {
	int c;
	for (c = 0; c < len; c++) {
		put1byte(buf[c]);
	}
}

//文字出力関数　myprintf　の定義
int myprintf(const char *fmt, ...) {
	static char buffer[100];
	int len;  	               	               	    	  	//送信データの大きさ
	va_list ap;               	               	    	  	//可変長引数処理に使用する変数定義
	va_start(ap, fmt); 	               	     		//可変長引数アクセスのための初期処理
	len = vsprintf(buffer, fmt, ap);		// 可変長引数データの書き込み
	putnbyte(buffer, len);            	               		//文字データ送信
	va_end(ap);            	               	               //可変長引数アクセス後の終了処理
	return len;
}

