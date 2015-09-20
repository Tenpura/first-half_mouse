/*
 * init.c
 *
 *  Created on: 2015/05/19
 *      Author: Koizumi
 */

#include"init.h"
#include"iodefine.h"

void Init_ALL(void) {
	Init_SYSTEM();	//一番最初に

	Init_MTU();
	Init_CMT0();
	Init_CMT1();
	Init_SCI();
	Init_AD();
	Init_pfc();
}

//mtu割り込みの初期設定
void Init_MTU(void) {
	MSTP( MTU ) = 0;			// 　MTUスタンバイ解除(全部)

	MTU0.TMDR1.BIT.MD = 0x02;			//PWMモード1に設定

	MTU0.TCR.BIT.CCLR = 6;				// TGRDコンペアマッチでTCNTクリア
	//MTU0.TIER.BIT.TGIEC = 0;			// TGRCの割り込み許可、0で禁止
	MTU0.TIER.BIT.TGIED = 1;			// TGRDの割り込み許可、0で禁止
	MTU0.TIORH.BIT.IOA = 1;				// 初期出力はLow　コンペアマッチでLow
	MTU0.TIORH.BIT.IOB = 2;				// 初期出力はLow　コンペアマッチでHigh
	MTU0.TIORL.BIT.IOC = 1;				// 初期出力はLow　コンペアマッチでLow
	MTU0.TIORL.BIT.IOD = 2;				// 初期出力はLow　コンペアマッチでHigh
	MTU0.TGRA = 100;					//
	MTU0.TGRB = 1000;					// 100000 kHz / 1000 = 100 kHz
	MTU0.TGRC = 100;					//
	MTU0.TGRD = 1000;					// 100000 kHz / 1000 = 100 kHz

	MTU0.TCR.BIT.TPSC = 0;				//ICLK/1でカウント
	MTU0.TCR.BIT.CKEG = 0;				//立ち上がりでカウント

	MTU0.TCNT = 0;		//カウント

	//encoder

	MTU.TSTRA.BIT.CST1 = 0;				//MTU1カウントストップ
	MTU.TSTRA.BIT.CST2 = 0;				//MTU2カウントストップ

	MTU1.TMDR1.BIT.MD = 0x04;			//位相計数モード1に設定
	MTU2.TMDR1.BIT.MD = 0x04;			//位相計数モード1に設定

	IOPORT.PFCMTU.BIT.TCLKS = 0;		//ピンをMTCLKn-A端子に設定(n=A~D)
	PORT3.ICR.BIT.B0 = 1;				//入力バッファを有効にする
	PORT3.ICR.BIT.B1 = 1;				//入力バッファを有効にする
	PORT3.ICR.BIT.B2 = 1;				//入力バッファを有効にする
	PORT3.ICR.BIT.B3 = 1;				//入力バッファを有効にする

	PORT3.DDR.BIT.B0 = 0;			//入力に設定
	PORT3.DDR.BIT.B1 = 0;			//入力に設定
	PORT3.DDR.BIT.B2 = 0;			//入力に設定
	PORT3.DDR.BIT.B3 = 0;			//入力に設定

	//割り込み許可
	IEN(MTU0,TGID0)= 1;					//割り込み許可
	IPR(MTU0,TGID0)= 14;				//割り込み優先度設定

	MTU.TSTRA.BIT.CST0 = 1;				//MTU0カウントスタート
	MTU.TSTRA.BIT.CST1 = 1;				//MTU1カウントスタート
	MTU.TSTRA.BIT.CST2 = 1;				//MTU2カウントスタート
}

//cmt割り込みの初期設定
void Init_CMT0(void) {
	CMT.CMSTR0.BIT.STR0 = 0;		//カウント停止

	MSTP( CMT0 ) = 0;		// 　CMT0スタンバイ解除
	IEN( CMT0, CMI0 )= 1;		// 　CMT割り込み許可
	IPR( CMT0, CMI0 )= 11;		//　 割り込み優先度設定
	CMT0.CMCR.BIT.CKS = 0;		// 　クロック選択 PCLK　/　8
	CMT0.CMCR.BIT.CMIE = 1;		// 　コンペアマッチ割り込み許可
	CMT0.CMCOR = 6250 - 1;		//　 割り込み周期設定 1ms

	CMT.CMSTR0.BIT.STR0 = 1;		//カウント開始
}

void Init_CMT1(void) {
	CMT.CMSTR0.BIT.STR1 = 0;		//カウント停止

	MSTP( CMT1 ) = 0;		// 　CMT0スタンバイ解除
	IEN( CMT1, CMI1 )= 1;		// 　CMT割り込み許可
	IPR( CMT1, CMI1 )= 10;		//　 割り込み優先度設定
	CMT1.CMCR.BIT.CKS = 0;		// 　クロック選択 PCLK　/　8
	CMT1.CMCR.BIT.CMIE = 1;		// 　コンペアマッチ割り込み許可
	CMT1.CMCOR = 6250 - 1;		//　 割り込み周期設定 1ms

	CMT.CMSTR0.BIT.STR1 = 1;		//カウント開始
}

//SCIの初期設定
void Init_SCI(void) {
	MSTP(SCI1) = 0;                 //シリアルスタンバイ解除
	SCI1.SMR.BYTE = 0;
	SCI1.SCR.BYTE = 0;
	SCI1.BRR = 12;                  // ビットレート設定：115200bps
	SCI1.SCR.BIT.RIE = 1;           // 受信割込み：許可(1)/禁止(0)
	SCI1.SCR.BIT.TE = 1;                           	// 送信許可
	SCI1.SCR.BIT.RE = 1;                           	// 受信許可
	PORTD.DDR.BIT.B3 = 0;                           	// PD5をTXD1に設定
	PORTD.DDR.BIT.B5 = 0;                           	// PD5をRXD1に設定
	PORTD.ICR.BIT.B5 = 1;                           	// 入力バッファ設定
}

//SPIの初期設定
void Init_SPI(void) {
	MSTP(RSPI0);		//RSPI0スタンバイ解除

	RSPI0.SPCR.BIT.SPMS = 0;	//RSPIモード選択	SPI動作(4線式)
	RSPI0.SPCR.BIT.TXMD = 0;	//通信動作モード選択
	RSPI0.SPCR.BIT.MODFEN = 0;	//モードフォルトエラー検出
	RSPI0.SPCR.BIT.MSTR = 1;	//RSPIマスタ/スレーブ選択
	RSPI0.SPCR.BIT.SPEIE = 0;	//RSPIエラー割り込み許可（禁止）
	RSPI0.SPCR.BIT.SPTIE = 0;	//RSPI送信割り込み許可（禁止）
	//RSPI0.SSLP.BIT.SSL1P = 0//0アクティブ

	RSPI0.SPBR = 12;	//ビットレート0.96MHz

	RSPI0.SPCR.BIT.SPE = 1;	//RSPI機能有効ビット
	RSPI0.SPCR.BIT.SPRIE = 0;	//RSPI受信割り込み許可
}

//SYSTEM関連の初期設定
void Init_SYSTEM(void) {
	SYSTEM.SCKCR.BIT.PCK = 1;	//周辺モジュールクロック(PCLK)　入力(12.5MHz)×4倍に設定
	SYSTEM.SCKCR.BIT.ICK = 0;	//システムクロック(ICLK)　入力(12.5MHz)×8倍に設定	100MHz
}

//AD変換の初期設定
void Init_AD(void) {
	MSTP( S12AD0 ) = 0;			//AD変換スタンバイ解除
	MSTP( S12AD1 ) = 0;			//AD変換スタンバイ解除

	S12AD0.ADCSR.BIT.ADST = 0;		//ADスタート　0で停止，1で開始
	S12AD0.ADCSR.BIT.EXTRG = 0;		//トリガ選択　外部トリガ禁止
	S12AD0.ADCSR.BIT.CKS = 2;		//クロック選択　0-PCLK/8，1-PCLK/4，2-PCLK/2，3-PCLK
	S12AD0.ADCSR.BIT.ADCS = 0;	//AD変換モード選択　0-シングル，1-1サイクル，2-連続スキャン，3-2チャネルスキャン
	S12AD0.ADCSR.BIT.ADIE = 0;		//AD変換割り込み許可　禁止
	S12AD0.ADCER.BIT.ADPRC = 0;		//AD変換の精度決め　12bit
	S12AD0.ADCER.BIT.ADRFMT = 0;	//AD変換の格納　右詰

	S12AD1.ADCSR.BIT.ADST = 0;		//ADスタート　0で停止，1で開始
	S12AD1.ADCSR.BIT.EXTRG = 0;		//トリガ選択　外部トリガ禁止
	S12AD1.ADCSR.BIT.CKS = 2;		//クロック選択　0-PCLK/8，1-PCLK/4，2-PCLK/2，3-PCLK
	S12AD1.ADCSR.BIT.ADCS = 0;	//AD変換モード選択　0-シングル，1-1サイクル，2-連続スキャン，3-2チャネルスキャン
	S12AD1.ADCSR.BIT.ADIE = 0;		//AD変換割り込み許可　禁止
	S12AD1.ADCER.BIT.ADPRC = 0;		//AD変換の精度決め　12bit
	S12AD1.ADCER.BIT.ADRFMT = 0;	//AD変換の格納　右詰

}

//ピンの入出力設定
void Init_pfc(void) {

	//自作7セグ
	PORT7.DDR.BIT.B1 = 1;	//LED5
	PORT7.DDR.BIT.B2 = 1;	//LED4
	PORT7.DDR.BIT.B3 = 1;	//LED2
	PORT7.DDR.BIT.B4 = 1;	//LED6
	PORT2.DDR.BIT.B2 = 1;	//LED1
	PORTA.DDR.BIT.B3 = 1;	//LED3
	PORTA.DDR.BIT.B4 = 1;	//LED8
	PORTB.DDR.BIT.B2 = 1;	//LED7

	//光学センサー類
	PORT1.DDR.BIT.B0 = 1;	//LED_F_RIGHT
	PORTA.DDR.BIT.B5 = 1;	//LED_RIGHT
	PORT9.DDR.BIT.B1 = 1;	//LED_LEFT
	PORT9.DDR.BIT.B2 = 1;	//LED_F_LEFT

	PORT7.DDR.BIT.B0 = 1;	//SPEAKER
	PORT7.DDR.BIT.B5 = 0;	//SWITCH1
	PORT7.DDR.BIT.B6 = 0;	//SWITCH2

	//モーター
	PORTB.DDR.BIT.B0 = 1;	//スタンバイ
	PORTB.DDR.BIT.B4 = 1;	//モーターA
	PORTB.DDR.BIT.B5 = 1;	//モーターA	はんだ不良
	PORT9.DDR.BIT.B4 = 1;		//もーたーA
	PORTB.DDR.BIT.B6 = 1;	//モーターB
	PORTB.DDR.BIT.B7 = 1;	//モーターB

}

