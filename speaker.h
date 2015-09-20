/*
 * speaker.h
 *
 *  Created on: 2015/06/03
 *      Author: Koizumi
 */

#ifndef SPEAKER_H_
#define SPEAKER_H_

#include"iodefine.h"
#include"define.h"
#include"extern_c.h"

//圧電ブザーを鳴らすためのファイル
//小泉　太嘉志　( takashi19940314@gmail.com )
//著作権フリー


#define BZ_DUTY 50				//ブザーのDuty　[％] 5の倍数のみ

#define BZ_HIGH()	PORT7.DR.BIT.B0 = 1		//bzのピン出力
#define BZ_LOW()	PORT7.DR.BIT.B0 = 0		//bzのピン出力

//音階
#define FREQ_REST    0                      //休符
#define FREQ_LA0     440                    //ラ（A）の周波数
#define FREQ_SI0     494                    //シ（B）の周波数
#define FREQ_DO      523                    //ド（C）の周波数
#define FREQ_RE      587                    //レ（D）の周波数
#define FREQ_MI      659                    //ミ（E）の周波数
#define FREQ_FA      698                    //ファ（F）の周波数
#define FREQ_SO      784                    //ソ（G）の周波数
#define FREQ_LA      880                    //ラ（A）の周波数
#define FREQ_SI      988                    //シ（B）の周波数
#define FREQ_DO2     1047                   //ド（C）の周波数
#define FREQ_RE2     1174                   //レ（D）の周波数
#define FREQ_MI2     1318                   //ミ（E）の周波数
#define FREQ_FA2     1396                   //ファ（F）の周波数
#define FREQ_SO2     1568                   //ソ（G）の周波数
#define FREQ_LA2     1760                   //ラ（A）の周波数
#define FREQ_SI2     1976                   //シ（B）の周波数
#define FREQ_DO3     2094                   //ド（C）の周波数

#define BZ_STOP()	CMT.CMSTR0.BIT.STR1=0	//ブザーストップ
#define BZ_START()	CMT.CMSTR0.BIT.STR1=1	//ブザースタート



#define MY_DO 1
#define MY_RE 2
#define MY_MI 3
#define MY_FA 4
#define MY_SO 5
#define MY_LA 6
#define MY_SI 7
#define MY_NO 0		//音なし

#define QUATER_NOTE 0	//4分音符
#define HALF_NOTE 	1	//2分音符
#define EIGHTH_NOTE 2	//8分音符
#define SIXTEEN_NOTE 3	//16分音符

#define SHARP 1
#define FLAT 2
#define MELODY_END	3



//割り込み関数で実行
void interrupt_speaker();


/********************************************************************/
/*	関数説明														*/
/*	鳴らしたい周波数とduty[%](5の倍数のみ)を与えると、その音を鳴らすための変数を計算する	*/
/*	ただし、周波数を0にすると割り込みが止まる(鳴らなくなる)			*/
/*------------------------------------------------------------------*/
/*	引数	Hz:鳴らしたい周波数[Hz]									*/
/*			bz_duty:ブザーのduty[％]								*/
/*																	*/
/*	返り値	なし													*/
/********************************************************************/
void input_bz_cycle(unsigned int Hz, unsigned char bz_duty );




/********************************************************************/
/*	関数説明														*/
/*	鳴らしたい周波数と時間を与えると、その時間だけその音を鳴らす	*/
/*------------------------------------------------------------------*/
/*	引数	Hz:鳴らしたい周波数[Hz]									*/
/*			bz_miri_second:ブザーを鳴らす時間[ms]					*/
/*																	*/
/*	返り値	なし													*/
/********************************************************************/
void sound_bz(unsigned int Hz, unsigned int bz_miri_second);



/********************************************************************/
/*	関数説明														*/
/*	typedefされているMELODY型の配列とオクターブと曲のテンポを渡すと */
/*	中心からオクターブを指定しただけずらしてメロディーを奏でる		*/
/*------------------------------------------------------------------*/
/*	引数	*melody : MELODY型の配列の先頭ポインタ					*/
/*			octave	: char型のオクターブを中心からどれだけずれるか	*/
/*			beat_tempo: unsigned int型で、曲のテンポを表す			*/
/*																	*/
/*	返り値	なし													*/
/********************************************************************/
void sound_melody(MELODY *melody, signed char octave, unsigned int beat_tempo);


#endif /* SPEAKER_H_ */
