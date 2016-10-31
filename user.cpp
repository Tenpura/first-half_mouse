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

unsigned char mode::select_mode(const unsigned char mode_number,
		const unsigned char muki) {
	unsigned char select = 0;

	while (1) {

		while ((SWITCH_RIGHT == OFF) && (SWITCH_LEFT == OFF)) {	//押されてなければ待機
			if (photo::check_wall(muki)) {
				my7seg::blink(select, 200, 1);
			} else {
				my7seg::light(select);
			}
		}
		wait_ms(100);	//チャタリング対策
		if (photo::check_wall(muki)) {
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

bool mode::search_mode() {
	signed char direction_x, direction_y;

	switch (select_mode(7, MUKI_RIGHT)) {
	case 0:		//0はメニューに戻る
		break;

	case 1:		//1は普通の足立法
		mouse::set_position(0, 0);
		mouse::set_direction(MUKI_UP);
		adachi::adachi_method_spin(GOAL_x, GOAL_y);
		break;

	case 2:		//2は帰りもあるよ
		mouse::set_position(0, 0);
		mouse::set_direction(MUKI_UP);
		if (adachi::adachi_method_spin(GOAL_x, GOAL_y)) {	//足立法が成功したら
			wait_ms(100);
			mouse::set_position(GOAL_x, GOAL_y);
			run::spin_turn(180);
			mouse::turn_direction(MUKI_RIGHT);
			mouse::turn_direction(MUKI_RIGHT);
			wait_ms(100);

			adachi::adachi_method_spin(0, 0);
		}

		break;

	case 3:
		mouse::set_position(0, 0);
		mouse::set_direction(MUKI_UP);
		if (adachi::adachi_method(GOAL_x, GOAL_y)) {	//足立法が成功したら
			wait_ms(100);
			mouse::set_position(GOAL_x, GOAL_y);
			run::spin_turn(180);
			mouse::turn_direction(MUKI_RIGHT);
			mouse::turn_direction(MUKI_RIGHT);
			wait_ms(100);

			adachi::adachi_method(0, 0);
		}
		break;

	case 4:
		mouse::set_position(0, 0);
		mouse::set_direction(MUKI_UP);
		if (adachi::adachi_method(GOAL_x, GOAL_y)) {	//足立法が成功したら
		}
		break;

	case 5:
		mouse::set_position(0, 0);
		mouse::set_direction(MUKI_UP);
		if (adachi::adachi_method_verF(GOAL_x, GOAL_y, false)) {//古川式足立法が成功したら
			wait_ms(100);
			mouse::set_position(GOAL_x, GOAL_y);
			run::spin_turn(180);
			mouse::turn_direction(MUKI_RIGHT);
			mouse::turn_direction(MUKI_RIGHT);
			wait_ms(100);

			adachi::adachi_method_verF(0, 0, false);
		}
		break;

	case 6:
		mouse::set_position(0, 0);
		mouse::set_direction(MUKI_UP);
		if (adachi::adachi_method_verF(GOAL_x, GOAL_y, true)) {	//古川式足立法（櫛無視）が成功したら
			wait_ms(100);
			mouse::set_position(GOAL_x, GOAL_y);
			run::spin_turn(180);
			mouse::turn_direction(MUKI_RIGHT);
			mouse::turn_direction(MUKI_RIGHT);
			wait_ms(100);

			adachi::adachi_method_verF(0, 0, true);
		}
		break;
		break;

	}

	return false;

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

float float_log::log_case[LOG_DIMENSION][LOG_COUNT];
unsigned int float_log::log_counter;
float float_log::init_number;

void float_log::reset_log() {
	log_counter = LOG_COUNT;		//リセット中の書き込み対策。
	for (unsigned int i = 0; i < LOG_COUNT; i++) {
		for (unsigned int j = 0; j < LOG_DIMENSION; j++) {
			log_case[j][i] = init_number;
		}
	}
	log_counter = 0;	//カウントを初期化
}

void float_log::put_log(const float put_number_a, const float put_number_b) {
	if (log_counter >= LOG_COUNT) {		//LOG_COUNT以上の個数は弾く。それ以上の配列が用意されてないから
		//何もしない
	} else {
		switch (LOG_DIMENSION) {
		case 1:
			log_case[0][log_counter] = put_number_a;
			break;

		case 2:
			log_case[0][log_counter] = put_number_a;
			log_case[1][log_counter] = put_number_b;
			break;

		}
		log_counter++;
	}
}

float float_log::get_log(const unsigned char get_dimension,
		const unsigned int get_number) {
	if (get_dimension >= LOG_DIMENSION) {	//LOG_DIMENSION以上の次元なんてねぇよ！
		return init_number;
	}
	if (get_number >= LOG_COUNT) {		//LOG_COUNT以上の個数は弾く。それ以上の配列が用意されてないから
		return init_number;
	}

	return log_case[get_dimension][get_number];

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

const float my_math::sin_table[92] = { 0, 0.017452406, 0.034899497, 0.052335956,
		0.069756474, 0.087155743, 0.104528463, 0.121869343, 0.139173101,
		0.156434465, 0.173648178, 0.190808995, 0.207911691, 0.224951054,
		0.241921896, 0.258819045, 0.275637356, 0.292371705, 0.309016994,
		0.325568154, 0.342020143, 0.35836795, 0.374606593, 0.390731128,
		0.406736643, 0.422618262, 0.438371147, 0.4539905, 0.469471563,
		0.48480962, 0.5, 0.515038075, 0.529919264, 0.544639035, 0.559192903,
		0.573576436, 0.587785252, 0.601815023, 0.615661475, 0.629320391,
		0.64278761, 0.656059029, 0.669130606, 0.68199836, 0.69465837,
		0.707106781, 0.7193398, 0.731353702, 0.743144825, 0.75470958,
		0.766044443, 0.777145961, 0.788010754, 0.79863551, 0.809016994,
		0.819152044, 0.829037573, 0.838670568, 0.848048096, 0.857167301,
		0.866025404, 0.874619707, 0.882947593, 0.891006524, 0.898794046,
		0.906307787, 0.913545458, 0.920504853, 0.927183855, 0.933580426,
		0.939692621, 0.945518576, 0.951056516, 0.956304756, 0.961261696,
		0.965925826, 0.970295726, 0.974370065, 0.978147601, 0.981627183,
		0.984807753, 0.987688341, 0.990268069, 0.992546152, 0.994521895,
		0.996194698, 0.99756405, 0.998629535, 0.999390827, 0.999847695, 1,
		0.999847695 };

float my_math::sin(const float radian) {
	float temp_radian = radian;
	float temp_degree;
	unsigned char degree_index;
	float answer = 0, answer_sign = 1;

	//-π ~ π の間に落とし込む
	while (ABS(temp_radian) > PI()) {
		temp_radian -= (SIGN(radian) * PI());
	}

	temp_degree = degree(temp_radian);
	if (temp_degree < 0) {
		//角度を反転し、符号を覚えておく
		temp_degree = -temp_degree;
		answer_sign = -1;
	}

	if (temp_degree > 90) {
		temp_degree = 180 - temp_degree;

	}
	degree_index = (char) temp_degree;		//charになる時に小数点以下は切り捨てられるはず
	answer = sin_table[degree_index];	//テーブルの内、目的の値に近い奴を入れる
	//線形近似
	answer += (temp_degree - degree_index)
			* (sin_table[degree_index + 1] - sin_table[degree_index]);

	return (answer * answer_sign);
}

float my_math::cos(const float radian) {
	return sin(radian + PI() / 2);
}
