/*
 * ad_convert.h
 *
 *  Created on: 2015/05/19
 *      Author: Koizumi
 */

//AD変換まわりの関数のまとめ

#include"iodefine.h"
#include"define.h"

#ifndef AD_CONVERT_H_
#define AD_CONVERT_H_

//ad_convert_anXXX()
//XXXのAD変換結果をunsigned int型で返す
unsigned int ad_convert_an000();
unsigned int ad_convert_an001();
unsigned int ad_convert_an002();
unsigned int ad_convert_an003();
unsigned int ad_convert_an100();
unsigned int ad_convert_an101();
unsigned int ad_convert_an102();
unsigned int ad_convert_an103();


//バッテリー電圧を読んで、float型で返す関数
float get_battery();

//gyroの値を読んで、それをintで返す関数
unsigned int ad_convert_gyro();
unsigned int ad_convert_gyro_ref();

#endif /* AD_CONVERT_H_ */
