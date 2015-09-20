/*
 * ad_convert.c
 *
 *  Created on: 2015/05/19
 *      Author: Koizumi
 */

#include"ad_convert.h"

unsigned int ad_convert_an000() {
	S12AD0.ADCSR.BIT.ADST = 0;	//ADスタート　0で停止，1で開始
	S12AD0.ADANS.BIT.CH = 0;	//AN000
	S12AD0.ADCSR.BIT.ADST = 1;	//ADスタート　0で停止，1で開始
	while (1) {
		if (S12AD0.ADCSR.BIT.ADST == 0) {		//AD変換が終了したら0になる → ループを抜ける
			break;
		}
	};
	return S12AD0.ADDR0A;	//AD変換結果
}
unsigned int ad_convert_an001() {
	S12AD0.ADCSR.BIT.ADST = 0;	//ADスタート　0で停止，1で開始
	S12AD0.ADANS.BIT.CH = 1;	//AN001
	S12AD0.ADCSR.BIT.ADST = 1;	//ADスタート　0で停止，1で開始
	while (1) {
		if (S12AD0.ADCSR.BIT.ADST == 0) {		//AD変換が終了したら0になる → ループを抜ける
			break;
		}
	};
	return S12AD0.ADDR1;	//AD変換結果
}
unsigned int ad_convert_an002() {
	S12AD0.ADCSR.BIT.ADST = 0;	//ADスタート　0で停止，1で開始
	S12AD0.ADANS.BIT.CH = 2;	//AN002
	S12AD0.ADCSR.BIT.ADST = 1;	//ADスタート　0で停止，1で開始
	while (1) {
		if (S12AD0.ADCSR.BIT.ADST == 0) {		//AD変換が終了したら0になる → ループを抜ける
			break;
		}
	};
	return S12AD0.ADDR2;	//AD変換結果
}
unsigned int ad_convert_an003() {
	S12AD0.ADCSR.BIT.ADST = 0;	//ADスタート　0で停止，1で開始
	S12AD0.ADANS.BIT.CH = 3;	//AN003
	S12AD0.ADCSR.BIT.ADST = 1;	//ADスタート　0で停止，1で開始
	while (1) {
		if (S12AD0.ADCSR.BIT.ADST == 0) {		//AD変換が終了したら0になる → ループを抜ける
			break;
		}
	};
	return S12AD0.ADDR3;	//AD変換結果
}

unsigned int ad_convert_an100() {
	S12AD1.ADCSR.BIT.ADST = 0;	//ADスタート　0で停止，1で開始
	S12AD1.ADANS.BIT.CH = 0;	//AN100
	S12AD1.ADCSR.BIT.ADST = 1;	//ADスタート　0で停止，1で開始
	while (1) {
		if (S12AD1.ADCSR.BIT.ADST == 0) {		//AD変換が終了したら0になる → ループを抜ける
			break;
		}
	};
	return S12AD1.ADDR0A;	//AD変換結果
}
unsigned int ad_convert_an101() {
	S12AD1.ADCSR.BIT.ADST = 0;	//ADスタート　0で停止，1で開始
	S12AD1.ADANS.BIT.CH = 1;	//AN101
	S12AD1.ADCSR.BIT.ADST = 1;	//ADスタート　0で停止，1で開始
	while (1) {
		if (S12AD1.ADCSR.BIT.ADST == 0) {		//AD変換が終了したら0になる → ループを抜ける
			break;
		}
	};
	return S12AD1.ADDR1;	//AD変換結果
}
unsigned int ad_convert_an102() {
	S12AD1.ADCSR.BIT.ADST = 0;	//ADスタート　0で停止，1で開始
	S12AD1.ADANS.BIT.CH = 2;	//AN102
	S12AD1.ADCSR.BIT.ADST = 1;	//ADスタート　0で停止，1で開始
	while (1) {
		if (S12AD1.ADCSR.BIT.ADST == 0) {		//AD変換が終了したら0になる → ループを抜ける
			break;
		}
	};
	return S12AD1.ADDR2;	//AD変換結果
}
unsigned int ad_convert_an103() {
	S12AD1.ADCSR.BIT.ADST = 0;	//ADスタート　0で停止，1で開始
	S12AD1.ADANS.BIT.CH = 3;	//AN103
	S12AD1.ADCSR.BIT.ADST = 1;	//ADスタート　0で停止，1で開始
	while (1) {
		if (S12AD1.ADCSR.BIT.ADST == 0) {		//AD変換が終了したら0になる → ループを抜ける
			break;
		}
	};
	return S12AD1.ADDR3;	//AD変換結果

}

float get_battery() {

	float temp_battery = 0;
	unsigned int ad_accurate;	//AD変換の精度格納用

	switch (S12AD0.ADCER.BIT.ADPRC) {	//AD変換の精度を確定
	case 0:	//12bit
		ad_accurate = 4096;
		break;
	case 1:	//10bit
		ad_accurate = 1024;
		break;
	case 2:	//8bit
		ad_accurate = 256;
		break;
	}

	temp_battery = (float) LOGIK_V * ad_convert_an000() / ad_accurate / 1 * 6.1;

	return temp_battery;
}

unsigned int ad_convert_gyro(){
	return ad_convert_an101();
}
unsigned int ad_convert_gyro_ref(){
	return ad_convert_an002();
}

