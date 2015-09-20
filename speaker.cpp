/*
 * speaker.cpp
 *
 *  Created on: 2015/06/03
 *      Author: Koizumi
 */

#include"speaker.h"

unsigned int bz_t=50;		//bzを何usオンにするか
unsigned int bz_cycle=100;	//bzのdutyを操作するとき、ひとつのパルスの幅を何十usにするか



void interrupt_speaker() {
	static unsigned int bz_count = 0;


	//duty管理
	if (bz_count < bz_t ) {		//bz_tの間Highにする。bz_tはinput_bz_cycleで計算
		BZ_HIGH();
	} else {					//スピーカーがLowのとき
		BZ_LOW();
	}

	//カウント増加
	bz_count++;
	//上限に達したらカウントをリセット
	if (bz_count >= bz_cycle) {		//鳴らしたい音の周期までカウントしたらリセット（bz_tは周期/2）
		bz_count = 0;
	}
}

void input_bz_cycle(unsigned int Hz, unsigned char bz_duty) {

	if (Hz < 20) {	//可聴域以下のときは
		BZ_STOP();			//ブザーストップ

	} else {
		BZ_STOP();			//ブザーストップ

		bz_cycle= (int) (1.0 / Hz * 100000 );			//目的のHzは、何us周期でONとOFFを繰り返せばよいか計算
		bz_t = (int) (bz_cycle * bz_duty / 100);		//ブザーをHighにする時間を計算

		BZ_START();			//ブザースタート

	}

}

void sound_bz(unsigned int Hz, unsigned int bz_miri_second) {

	input_bz_cycle(Hz, BZ_DUTY);

	wait_ms(bz_miri_second);//bz_miri_second);		//bz_miri_secondだけ待機する

	BZ_STOP();			//ブザーストップ

	wait_ms(10);

}

/*
 unsigned char	semiton	:2;			//半音あげるか下げるかを表す
 unsigned char	octave	:1;			//オクターブを1つあげるかどうか
 unsigned char	beat	:2;			//拍数を表す
 unsigned char	scale	:3;			//音階を表す　0←ドレミファソラシ休→7
 */
void sound_melody(MELODY *melody, signed char octave, unsigned int beat_tempo) {
	float melody_octave = 0;			//オクターブを管理
	float melody_time = 0;			//音を鳴らす時間を保存[ms]
	unsigned int melody_hz = 0;		//鳴らす音の周波数を保存[Hz]
	unsigned int melody_scale[9] = { 0, FREQ_DO, FREQ_RE, FREQ_MI, FREQ_FA,
	FREQ_SO, FREQ_LA, FREQ_SI, FREQ_DO2 };	//音階の周波数を配列に保存

	for (int temp_count = 0; melody[temp_count].bit.semiton != MELODY_END;
			temp_count++) {

		//拍数から音の長さを計算する
		switch (melody[temp_count].bit.beat) {
		case QUATER_NOTE:	//4分音符
			melody_time = (60.0 / beat_tempo * 1000);
			break;

		case HALF_NOTE:		//2分音符
			melody_time = (60.0 / beat_tempo * 2 * 1000);
			break;

		case EIGHTH_NOTE:	//8分音符
			melody_time = (60.0 / beat_tempo / 2 * 1000);
			break;

		case SIXTEEN_NOTE:	//16分音符
			melody_time = (60.0 / beat_tempo / 4 * 1000);
			break;
		}

		//鳴らす周波数と半音の上げ下げを同時に行う
		//同時に行うのはFLATでmelody_scale配列の[-1]を参照するとやばいから
		if (melody[temp_count].bit.scale == MY_NO) {		//鳴らさない場合は０Hz
			melody_hz = 0;

		} else {												//鳴らす場合は
			melody_hz = melody_scale[melody[temp_count].bit.scale];	//周波数を配列の要素から代入

			//半音あげたり下げたり
			if (melody[temp_count].bit.semiton == FLAT) {			//半音下げる
				melody_hz = (melody_hz
						+ melody_scale[melody[temp_count].bit.scale - 1]) / 2;

			} else if (melody[temp_count].bit.semiton == SHARP) {	//半音あげる
				melody_hz = (melody_hz
						+ melody_scale[melody[temp_count].bit.scale + 1]) / 2;
			}
		}

		if (octave == 0) {		//中心からずれない
			melody_octave = 1.0;

		} else if (octave > 0) {	//オクターブを高くするなら
			melody_octave = 1;
			for (int i = 0; i < octave; i++) {
				melody_octave *= 2;
			}
		} else {					//オクターブを低くするなら
			melody_octave = 1.0;
			for (int i = 0; i < (-octave); i++) {
				melody_octave /= 2;
			}
		}

		//いざ音を鳴らす
		if (melody[temp_count].bit.octave == 0) {		//通常オクターブ
			sound_bz(melody_hz * melody_octave, (int) melody_time);
		} else {											//1オクターブ高い
			sound_bz(melody_hz * melody_octave * 2, (int) melody_time);
		}

	}

}

