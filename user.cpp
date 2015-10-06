/*
 * user.cpp
 *
 *  Created on: 2015/06/14
 *      Author: Koizumi
 */

#include"user.h"

void error() {
	motor::sleep_motor();		//motorを止める
	while ((SWITCH_RIGHT == OFF) && (SWITCH_LEFT == OFF)) {	//スイッチが押されてなければ待機
		my7seg::light_error();
		wait_ms(100);
		my7seg::turn_off();
		wait_ms(100);
	}
}

float degree(float radian) {
	return (radian * 180 / PI());
}

float radian(float degree) {
	return (degree * PI() / 180);
}

void wait_ms(const unsigned int wait_ms_count) {
	set_count_ms(0);	//カウントをリセット
	while (1) {
		if (get_count_ms() == wait_ms_count) {	//目標の時間まで待ったら
			return;
		}
	}
}

void set_count_ms(const unsigned long set_ms_value) {
	wait_package(0, set_ms_value);	//値を代入
}

unsigned long get_count_ms() {
	return wait_package(1, 0);	//値を取得
}

unsigned long wait_package(const unsigned char wait_mode,
		const unsigned long set_ms_value) {

	static unsigned long count_ms = 0;
	if (wait_mode == 0) {	//setのとき
		count_ms = set_ms_value;	//代入
	} else if (wait_mode == 1) {	//getのとき
		//何もせず
	}
	return count_ms;
}

unsigned char mode::select_mode(const unsigned char mode_number) {
	unsigned char select = 0;

	while (1) {

		while ((SWITCH_RIGHT == OFF) && (SWITCH_LEFT == OFF)) {	//押されてなければ待機
			if (photo::check_wall(MUKI_RIGHT)) {
				my7seg::blink(select, 200, 1);
			} else {
				my7seg::light(select);
			}
		}
		wait_ms(100);	//チャタリング対策
		if (photo::check_wall(MUKI_RIGHT)) {
			break;
		}

		my7seg::turn_off();

		select++;
		if (select >= mode_number) {	//上限に達したら、下限に飛ばす
			select = 0;
		}

		while ((SWITCH_RIGHT == ON) || (SWITCH_LEFT == ON)) {	//押されていれば待機
		}
		wait_ms(100);	//チャタリング対策

	}

	while ((SWITCH_RIGHT == OFF) && (SWITCH_LEFT == OFF)) {	//押されてなければ待機
		my7seg::blink(select, 200, 1);
	}
	wait_ms(100);	//チャタリング対策

	return select;
}

mode::mode() {
}

mode::~mode() {

}

signed int int_log::log_case[2][INT_LOG_COUNT];
unsigned int int_log::log_counter;

void int_log::reset_log() {
	log_counter = LOG_COUNT;		//リセット中の書き込み対策。
	for (unsigned int i = 0; i < LOG_COUNT; i++) {
		log_case[0][i] = init_number;
		log_case[1][i] = init_number;
	}
	log_counter = 0;	//カウントを初期化
}

void int_log::put_log(const int put_number) {
	if (log_counter >= LOG_COUNT) {		//LOG_COUNT以上の個数は弾く。それ以上の配列が用意されてないから
		//何もしない
	} else {
		log_case[0][log_counter] = put_number;
		log_counter++;
	}
}

void int_log::put_log(const int put_number, const int put_number2) {
	if (log_counter >= LOG_COUNT) {		//LOG_COUNT以上の個数は弾く。それ以上の配列が用意されてないから
		//何もしない
	} else {
		log_case[0][log_counter] = put_number;
		log_case[1][log_counter] = put_number;
		log_counter++;
	}
}

int int_log::get_log(const unsigned int get_number) {
	if (get_number >= LOG_COUNT) {		//LOG_COUNT以上の個数は弾く。それ以上の配列が用意されてないから
		return init_number;
	} else {
		return log_case[0][get_number];
	}
}

int int_log::get_log(const unsigned int ordinal_number,
		const unsigned int get_number) {
	if (get_number >= LOG_COUNT) {		//LOG_COUNT以上の個数は弾く。それ以上の配列が用意されてないから
		return init_number;
	} else {
		return log_case[ordinal_number][get_number];
	}
}

int_log::int_log() {
	init_number = 0;
	log_counter = 0;
	reset_log();
}

int_log::int_log(int initialize_number) {
	init_number = initialize_number;
	log_counter = 0;
	reset_log();
}

int_log::~int_log() {
}

float float_log::log_case[LOG_COUNT];
unsigned int float_log::log_counter;

void float_log::reset_log() {
	log_counter = LOG_COUNT;		//リセット中の書き込み対策。
	for (unsigned int i = 0; i < LOG_COUNT; i++) {
		log_case[i] = init_number;
	}
	log_counter = 0;	//カウントを初期化
}

void float_log::put_log(const float put_number) {
	if (log_counter >= LOG_COUNT) {		//LOG_COUNT以上の個数は弾く。それ以上の配列が用意されてないから
		//何もしない
	} else {
		log_case[log_counter] = put_number;
		log_counter++;
	}
}

float float_log::get_log(const unsigned int get_number) {
	if (get_number >= LOG_COUNT) {		//LOG_COUNT以上の個数は弾く。それ以上の配列が用意されてないから
		return init_number;
	} else {
		return log_case[get_number];
	}
}

float_log::float_log() {
	init_number = 0;
	log_counter = 0;
	reset_log();
}

float_log::float_log(float initialize_number) {
	init_number = initialize_number;
	log_counter = 0;
	reset_log();
}

float_log::~float_log() {
}

