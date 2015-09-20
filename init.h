/*
 * init.h
 *
 *  Created on: 2015/05/19
 *      Author: Koizumi
 */

//初期設定

#ifndef INIT_H_
#define INIT_H_

//全ての初期設定（これより下に書いてあるすべて）を実行
void Init_ALL(void);

//mtu割り込みの初期設定
void Init_MTU ( void );

//cmt割り込みの初期設定
void Init_CMT0 ( void );
void Init_CMT1 ( void );

//SCIの初期設定
void Init_SCI(void);

//SPIの初期設定
void Init_SPI(void);

//SYSTEM関連の初期設定
void Init_SYSTEM(void);

//AD変換の初期設定
void Init_AD(void);

//ピンの入出力設定
void Init_pfc(void);

#endif /* INIT_H_ */
