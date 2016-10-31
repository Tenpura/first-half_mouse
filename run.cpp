/*
 * run.cpp
 *
 *  Created on: 2015/09/07
 *      Author: Koizumi
 */

#include"run.h"

float mouse::ideal_acceleration, mouse::ideal_angular_acceleration;
float mouse::ideal_velocity, mouse::ideal_angular_velocity;
float mouse::run_distance;
unsigned long mouse::mouse_count_ms;

bool mouse::back_run_flag = false;
bool mouse::slalom_flag = false;

MAP_DATA mouse::now_map;

POSITION mouse::position;
unsigned char mouse::mouse_direction;

bool mouse::fail_flag = false;

void mouse::reset_count() {
	mouse_count_ms = 0;
}

void mouse::add_one_count_ms() {
	mouse_count_ms = mouse_count_ms + 1;
}

unsigned long mouse::get_count_ms() {
	return mouse_count_ms;
}

void mouse::set_acceleration(const float set_value_m_ss) {
	ideal_acceleration = set_value_m_ss;
}

float mouse::get_acceleration() {
	return ideal_acceleration;	//m/s^2
}

void mouse::set_angular_acceleration(const float set_value_m_ss) {
	ideal_angular_acceleration = set_value_m_ss;
}

float mouse::get_angular_acceleration() {
	return ideal_angular_acceleration;	//m/s^2
}

void mouse::set_ideal_velocity(const float set_value_m_s) {
	ideal_velocity = set_value_m_s;
}

float mouse::get_ideal_velocity() {
	return ideal_velocity;
}

void mouse::set_ideal_angular_velocity(const float set_value_rad_s) {
	ideal_angular_velocity = set_value_rad_s;
}

float mouse::get_ideal_angular_velocity() {
	return ideal_angular_velocity;
}

void mouse::reset_angle() {
	control::reset_delta(sen_gyro);
	gyro::reset_angle();
	mouse::reset_count();		//最小二乗法の補正ためカウントをリセット
}

float mouse::get_angle_degree() {
	return gyro::get_angle();
}

float mouse::get_distance_m() {
	return run_distance;
}

void mouse::set_distance_m(const float set_value_m) {
	run_distance = set_value_m;
}

unsigned char mouse::get_x_position() {
	return position.bit.x;
}

unsigned char mouse::get_y_position() {
	return position.bit.y;
}

void mouse::set_position(const unsigned char x, const unsigned char y) {
	position.bit.x = x;
	position.bit.y = y;
}

void mouse::turn_direction(const unsigned char right_or_left) {
	//マウスの向きを90°回転

	if (right_or_left == MUKI_RIGHT) {
		switch (mouse_direction) {
		case MUKI_UP:
			mouse_direction = MUKI_RIGHT;
			break;

		case MUKI_DOWN:
			mouse_direction = MUKI_LEFT;
			break;

		case MUKI_RIGHT:
			mouse_direction = MUKI_DOWN;
			break;

		case MUKI_LEFT:
			mouse_direction = MUKI_UP;
			break;
		}

	} else {
		switch (mouse_direction) {
		case MUKI_UP:
			mouse_direction = MUKI_LEFT;
			break;

		case MUKI_DOWN:
			mouse_direction = MUKI_RIGHT;
			break;

		case MUKI_RIGHT:
			mouse_direction = MUKI_UP;
			break;

		case MUKI_LEFT:
			mouse_direction = MUKI_DOWN;
			break;
		}
	}
}

unsigned char mouse::get_direction() {
	return mouse_direction;
}

void mouse::get_direction(signed char *direction_x, signed char *direction_y) {
	switch (mouse_direction) {
	case MUKI_RIGHT:
		*direction_x = 1;
		*direction_y = 0;
		return;
		break;

	case MUKI_LEFT:
		*direction_x = -1;
		*direction_y = 0;
		return;
		break;

	case MUKI_UP:
		*direction_x = 0;
		*direction_y = 1;
		return;
		break;

	case MUKI_DOWN:
		*direction_x = 0;
		*direction_y = -1;
		return;
		break;
	}
}

void mouse::set_direction(const unsigned char muki) {
	mouse_direction = muki;
}

void mouse::set_direction(const signed char direction_x,
		const signed char direction_y) {
	switch (direction_x) {
	case 1:
		mouse_direction = MUKI_RIGHT;
		break;

	case -1:
		mouse_direction = MUKI_LEFT;
		break;

	case 0:
		if (direction_y == 1) {
			mouse_direction = MUKI_UP;
		} else {
			mouse_direction = MUKI_DOWN;
		}
		break;
	}
}

bool mouse::get_fail_flag() {
	//フェイルセーフがかかったからtrue
	return fail_flag;
}

void mouse::set_fail_flag(bool set_flag) {
	fail_flag = set_flag;
}

void mouse::cal_accel() {
	//（速度+=加速度）を制御にぶち込む
	set_ideal_velocity(
			get_ideal_velocity() + (get_acceleration() * CONTROL_PERIOD));
	//（角速度+=加角速度）を制御にぶち込む
	set_ideal_angular_velocity(
			get_ideal_angular_velocity()
					+ (get_angular_acceleration() * CONTROL_PERIOD));
}

void mouse::cal_distance() {
	//TODO 距離計算どっちがいいのか、わからない
	//run_distance += mouse::get_ideal_velocity() * CONTROL_PERIOD;
	run_distance += encoder::get_velocity() * CONTROL_PERIOD;
}

void mouse::look_wall(bool comb_ignore) {
	//壁を見て、壁が存在するなら壁を作り、ないなら壊す。見たことも記録
	//マウスの向きや座標も内部できちんと考える

	signed char direction_x, direction_y;
	mouse::get_direction(&direction_x, &direction_y);

	switch (mouse_direction) {
	case MUKI_UP:
		//壁情報を更新
		//マウスから見て右
		map::remember_exist(position.bit.x, position.bit.y, MUKI_RIGHT);//壁を見たことを記録
		if (photo::check_wall(MUKI_RIGHT)) {
			map::create_wall(position.bit.x, position.bit.y, MUKI_RIGHT);
		} else {
			map::destroy_wall(position.bit.x, position.bit.y, MUKI_RIGHT);

			//櫛無視するなら
			if (comb_ignore) {
				//既に見ていて壁がないなら
				if (map::check_exist(position.bit.x - direction_x,
						position.bit.y - direction_y,
						MUKI_RIGHT) == TRUE) {
					if (map::get_wall(position.bit.x - direction_x,
							position.bit.y - direction_y,
							MUKI_RIGHT) == FALSE) {
						//ルール的に壁があるはず
						map::create_wall(position.bit.x + 1, position.bit.y,
						MUKI_DOWN);
						map::remember_exist(position.bit.x + 1, position.bit.y,
						MUKI_DOWN);		//壁を見たことを記録
					}
				}
			}
		}
		//マウスから見て前
		map::remember_exist(position.bit.x, position.bit.y, MUKI_UP); //壁を見たことを記録
		if (photo::check_wall(MUKI_UP)) {
			map::create_wall(position.bit.x, position.bit.y, MUKI_UP);
		} else {
			map::destroy_wall(position.bit.x, position.bit.y, MUKI_UP);
		}
		//マウスから見て左
		map::remember_exist(position.bit.x, position.bit.y, MUKI_LEFT);	//壁を見たことを記録
		if (photo::check_wall(MUKI_LEFT)) {
			map::create_wall(position.bit.x, position.bit.y, MUKI_LEFT);
		} else {
			map::destroy_wall(position.bit.x, position.bit.y, MUKI_LEFT);

			//櫛無視するなら
			if (comb_ignore) {
				//既に見ていて壁がないなら
				if (map::check_exist(position.bit.x - direction_x,
						position.bit.y - direction_y,
						MUKI_LEFT) == TRUE) {
					if (map::get_wall(position.bit.x - direction_x,
							position.bit.y - direction_y,
							MUKI_LEFT) == FALSE) {
						//ルール的に壁があるはず
						map::create_wall(position.bit.x - 1, position.bit.y,
						MUKI_DOWN);
						map::remember_exist(position.bit.x - 1, position.bit.y,
						MUKI_DOWN);	//壁を見たことを記録
					}
				}
			}

		}

		break;

	case MUKI_RIGHT:
		//壁情報を更新
		//マウスから見て右
		map::remember_exist(position.bit.x, position.bit.y, MUKI_DOWN);	//壁を見たことを記録
		if (photo::check_wall(MUKI_RIGHT)) {
			map::create_wall(position.bit.x, position.bit.y, MUKI_DOWN);
		} else {
			map::destroy_wall(position.bit.x, position.bit.y, MUKI_DOWN);

			//櫛無視するなら
			if (comb_ignore) {
				//既に見ていて壁がないなら
				if (map::check_exist(position.bit.x - direction_x,
						position.bit.y - direction_y,
						MUKI_DOWN) == TRUE) {
					if (map::get_wall(position.bit.x - direction_x,
							position.bit.y - direction_y,
							MUKI_DOWN) == FALSE) {
						//ルール的に壁があるはず
						map::create_wall(position.bit.x, position.bit.y - 1,
						MUKI_LEFT);
						map::remember_exist(position.bit.x, position.bit.y - 1,
						MUKI_LEFT);	//壁を見たことを記録
					}
				}
			}

		}
		//マウスから見て前
		map::remember_exist(position.bit.x, position.bit.y, MUKI_RIGHT);//壁を見たことを記録
		if (photo::check_wall(MUKI_UP)) {
			map::create_wall(position.bit.x, position.bit.y, MUKI_RIGHT);
		} else {
			map::destroy_wall(position.bit.x, position.bit.y, MUKI_RIGHT);
		}
		//マウスから見て左
		map::remember_exist(position.bit.x, position.bit.y, MUKI_UP);//壁を見たことを記録
		if (photo::check_wall(MUKI_LEFT)) {
			map::create_wall(position.bit.x, position.bit.y, MUKI_UP);
		} else {
			map::destroy_wall(position.bit.x, position.bit.y, MUKI_UP);
			//櫛無視するなら
			if (comb_ignore) {
				//既に見ていて壁がないなら
				if (map::check_exist(position.bit.x - direction_x,
						position.bit.y - direction_y,
						MUKI_UP) == TRUE) {
					if (map::get_wall(position.bit.x - direction_x,
							position.bit.y - direction_y,
							MUKI_UP) == FALSE) {
						//ルール的に壁があるはず
						map::create_wall(position.bit.x, position.bit.y + 1,
						MUKI_LEFT);
						map::remember_exist(position.bit.x, position.bit.y + 1,
						MUKI_LEFT);	//壁を見たことを記録
					}
				}
			}

		}

		break;

	case MUKI_LEFT:
		//壁情報を更新
		//マウスから見て右
		map::remember_exist(position.bit.x, position.bit.y, MUKI_UP);//壁を見たことを記録
		if (photo::check_wall(MUKI_RIGHT)) {
			map::create_wall(position.bit.x, position.bit.y, MUKI_UP);
		} else {
			map::destroy_wall(position.bit.x, position.bit.y, MUKI_UP);

			//櫛無視するなら
			if (comb_ignore) {
				//既に見ていて壁がないなら
				if (map::check_exist(position.bit.x - direction_x,
						position.bit.y - direction_y,
						MUKI_UP) == TRUE) {
					if (map::get_wall(position.bit.x - direction_x,
							position.bit.y - direction_y,
							MUKI_UP) == FALSE) {
						//ルール的に壁があるはず
						map::create_wall(position.bit.x, position.bit.y + 1,
						MUKI_RIGHT);
						map::remember_exist(position.bit.x, position.bit.y + 1,
						MUKI_RIGHT);	//壁を見たことを記録
					}
				}
			}

		}
		//マウスから見て前
		map::remember_exist(position.bit.x, position.bit.y, MUKI_LEFT);	//壁を見たことを記録
		if (photo::check_wall(MUKI_UP)) {
			map::create_wall(position.bit.x, position.bit.y, MUKI_LEFT);
		} else {
			map::destroy_wall(position.bit.x, position.bit.y, MUKI_LEFT);
		}
		//マウスから見て左
		map::remember_exist(position.bit.x, position.bit.y, MUKI_DOWN);	//壁を見たことを記録
		if (photo::check_wall(MUKI_LEFT)) {
			map::create_wall(position.bit.x, position.bit.y, MUKI_DOWN);
		} else {
			map::destroy_wall(position.bit.x, position.bit.y, MUKI_DOWN);

			//櫛無視するなら
			if (comb_ignore) {
				//既に見ていて壁がないなら
				if (map::check_exist(position.bit.x - direction_x,
						position.bit.y - direction_y,
						MUKI_DOWN) == TRUE) {
					if (map::get_wall(position.bit.x - direction_x,
							position.bit.y - direction_y,
							MUKI_DOWN) == FALSE) {
						//ルール的に壁があるはず
						map::create_wall(position.bit.x, position.bit.y - 1,
						MUKI_RIGHT);
						map::remember_exist(position.bit.x, position.bit.y - 1,
						MUKI_RIGHT);	//壁を見たことを記録
					}
				}
			}

		}

		break;

	case MUKI_DOWN:
		//壁情報を更新
		//マウスから見て右
		map::remember_exist(position.bit.x, position.bit.y, MUKI_LEFT);	//壁を見たことを記録
		if (photo::check_wall(MUKI_RIGHT)) {
			map::create_wall(position.bit.x, position.bit.y, MUKI_LEFT);
		} else {
			map::destroy_wall(position.bit.x, position.bit.y, MUKI_LEFT);

			//櫛無視するなら
			if (comb_ignore) {
				//既に見ていて壁がないなら
				if (map::check_exist(position.bit.x - direction_x,
						position.bit.y - direction_y,
						MUKI_LEFT) == TRUE) {
					if (map::get_wall(position.bit.x - direction_x,
							position.bit.y - direction_y,
							MUKI_LEFT) == FALSE) {
						//ルール的に壁があるはず
						map::create_wall(position.bit.x - 1, position.bit.y,
						MUKI_UP);
						map::remember_exist(position.bit.x - 1, position.bit.y,
						MUKI_UP);	//壁を見たことを記録
					}
				}
			}

		}
		//マウスから見て前
		map::remember_exist(position.bit.x, position.bit.y, MUKI_DOWN);	//壁を見たことを記録
		if (photo::check_wall(MUKI_UP)) {
			map::create_wall(position.bit.x, position.bit.y, MUKI_DOWN);
		} else {
			map::destroy_wall(position.bit.x, position.bit.y, MUKI_DOWN);
		}
		//マウスから見て左
		map::remember_exist(position.bit.x, position.bit.y, MUKI_RIGHT);//壁を見たことを記録
		if (photo::check_wall(MUKI_LEFT)) {
			map::create_wall(position.bit.x, position.bit.y, MUKI_RIGHT);
		} else {
			map::destroy_wall(position.bit.x, position.bit.y, MUKI_RIGHT);

			//櫛無視するなら
			if (comb_ignore) {
				//既に見ていて壁がないなら
				if (map::check_exist(position.bit.x - direction_x,
						position.bit.y - direction_y,
						MUKI_RIGHT) == TRUE) {
					if (map::get_wall(position.bit.x - direction_x,
							position.bit.y - direction_y,
							MUKI_RIGHT) == FALSE) {
						//ルール的に壁があるはず
						map::create_wall(position.bit.x + 1, position.bit.y,
						MUKI_UP);
						map::remember_exist(position.bit.x + 1, position.bit.y,
						MUKI_UP);	//壁を見たことを記録
					}
				}
			}
		}

		break;

	}

}

mouse::mouse() {
}

mouse::~mouse() {

}

void run::fit_stop(const float distance_m, const float select_mode,
		const float fit_velocity) {
	float de_accel_value = ABS(parameter::get_run_de_acceleration(select_mode));

	//減速
	mouse::set_acceleration(-de_accel_value);
	while (mouse::get_ideal_velocity() > fit_velocity) {
		//速度より先に距離がなくなったら抜ける
		if (distance_m < mouse::get_distance_m()) {
			break;
		}
		//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			return;
		}
	}

	mouse::set_acceleration(0);
	mouse::set_ideal_velocity(fit_velocity);

	while (photo::get_ad(front_right) < parameter::get_ideal_photo(front_right)) {
		//速度より先に距離がなくなったら抜ける
		if (distance_m < mouse::get_distance_m()) {
			break;
		}

		//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			return;
		}
	}

	//減速
	mouse::set_acceleration(-de_accel_value);
	while (mouse::get_ideal_velocity() > 0) {
		//速度より先に距離がなくなったら抜ける
		if (distance_m < mouse::get_distance_m()) {
			break;
		}
		//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			return;
		}
	}

	mouse::set_acceleration(0);
	mouse::set_ideal_velocity(0);

}

void run::accel_run(const float distance_m, const float end_velocity,
		const unsigned char select_mode) {

	float max_velocity = parameter::get_run_max_velocity(select_mode);
	float accel_value = parameter::get_run_acceleration(select_mode);
	float de_accel_value = ABS(parameter::get_run_de_acceleration(select_mode));

	float de_accel_distance = ABS(
			mouse::get_ideal_velocity() * mouse::get_ideal_velocity()
			- end_velocity * end_velocity) / (2 * de_accel_value);	//減速に必要な距離

//加速
	mouse::set_acceleration(ABS(parameter::get_run_acceleration(select_mode)));
	while (mouse::get_ideal_velocity() < max_velocity) {
		//現在速度から減速にかかる距離を計算
		de_accel_distance = ABS(
				mouse::get_ideal_velocity() * mouse::get_ideal_velocity()
				- end_velocity * end_velocity) / (2 * de_accel_value);

		//減速に距離が必要な距離が足りなくなったら抜ける
		if (de_accel_distance >= distance_m - mouse::get_distance_m()) {
			break;
		}

		//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			return;
		}

	}

//等速
	mouse::set_acceleration(0);
	while (1) {
		//現在速度から減速にかかる距離を計算
		de_accel_distance = ABS(
				mouse::get_ideal_velocity() * mouse::get_ideal_velocity()
				- end_velocity * end_velocity) / (2 * de_accel_value);

		//減速に距離が必要な距離が足りなくなったら抜ける
		if (de_accel_distance >= (distance_m - mouse::get_distance_m())) {
			break;
		}

		//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			return;
		}
	}

//減速
	if (mouse::get_ideal_velocity() >= end_velocity) {
		mouse::set_acceleration(-de_accel_value);
		while (mouse::get_ideal_velocity() > end_velocity) {
			//速度より先に距離がなくなったら抜ける
			if (distance_m < mouse::get_distance_m()) {
				break;
			}
			//フェイルセーフが掛かっていればそこで抜ける
			if (mouse::get_fail_flag()) {
				return;
			}
		}

	} else {
		mouse::set_acceleration(ABS(de_accel_value));
		while (mouse::get_ideal_velocity() < end_velocity) {
			//速度より先に距離がなくなったら抜ける
			if (distance_m < mouse::get_distance_m()) {
				break;
			}
			//フェイルセーフが掛かっていればそこで抜ける
			if (mouse::get_fail_flag()) {
				return;
			}
		}
	}

	mouse::set_acceleration(0);
	mouse::set_ideal_velocity(end_velocity);

//速度0だとここに閉じ込められてしまう
	if (end_velocity > 0) {
		//もし速度が先に無くなっても、最後まで走りきるよう
		while (distance_m > mouse::get_distance_m()) {
		}
	}

	mouse::set_distance_m(0);
}

void run::back_run(const float distance_m, const float end_velocity,
		const unsigned char select_mode) {

	float max_velocity = 0.3;	//parameter::get_run_max_velocity(select_mode);
	float accel_value = parameter::get_run_acceleration(select_mode);
	float de_accel_value = -ABS(
			parameter::get_run_de_acceleration(select_mode));

	float de_accel_distance = ABS(
			mouse::get_ideal_velocity() * mouse::get_ideal_velocity()
			- end_velocity * end_velocity) / (2 * de_accel_value);	//減速に必要な距離

	long temp_count;	//一時的に保存しとくよう
	static const long back_count = 300;	//等速でバックしてる時間[ms]

	if (distance_m < 0) {		//バックする時
		accel_value = -ABS(accel_value);
		max_velocity = -ABS(max_velocity);
		de_accel_value = -ABS(de_accel_value);
		mouse::back_run_flag = true;
	}

//加速
	mouse::set_acceleration(accel_value);
	while (ABS(mouse::get_ideal_velocity()) < ABS(max_velocity)) {
		//現在速度から減速にかかる距離を計算

		/*
		 de_accel_distance = (
		 mouse::get_ideal_velocity() * mouse::get_ideal_velocity()
		 - end_velocity * end_velocity / (2 * de_accel_value));

		 //減速に距離が必要な距離が足りなくなったら抜ける
		 if (de_accel_distance >= ABS(distance_m - mouse::get_distance_m())) {
		 break;
		 }
		 */
		//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			return;
		}

	}

//等速
	mouse::set_acceleration(0);
	temp_count = mouse::get_count_ms();
	while (1) {
		//back_countだけ時間が経ったら強制的に打ち切る
		if ((mouse::get_count_ms() - temp_count) > back_count) {
			break;
		}

		//現在速度から減速にかかる距離を計算
		de_accel_distance = ABS(
				(mouse::get_ideal_velocity() * mouse::get_ideal_velocity()
						- end_velocity * end_velocity) / (2 * de_accel_value));

		//減速に距離が必要な距離が足りなくなったら抜ける
		if (de_accel_distance >= ABS(distance_m - mouse::get_distance_m())) {
			break;
		}

		//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			return;
		}
	}

//減速
	if (mouse::get_ideal_velocity() >= end_velocity) {
		mouse::set_acceleration(-ABS(de_accel_value));
		while (mouse::get_ideal_velocity() > end_velocity) {
			//速度より先に距離がなくなったら抜ける
			if (ABS(distance_m) < ABS(mouse::get_distance_m())) {
				break;
			}
			//フェイルセーフが掛かっていればそこで抜ける
			if (mouse::get_fail_flag()) {
				return;
			}
		}

	} else {
		mouse::set_acceleration(ABS(de_accel_value));
		while (mouse::get_ideal_velocity() < end_velocity) {
			//速度より先に距離がなくなったら抜ける
			if (ABS(distance_m) < ABS(mouse::get_distance_m())) {
				break;
			}
			//フェイルセーフが掛かっていればそこで抜ける
			if (mouse::get_fail_flag()) {
				return;
			}
		}
	}

	mouse::set_acceleration(0);
	mouse::set_ideal_velocity(end_velocity);

//速度0だとここに閉じ込められてしまう
	if (end_velocity != 0) {
		//もし速度が先に無くなっても、最後まで走りきるよう
		while (ABS(distance_m) > ABS(mouse::get_distance_m())) {
		}
	}

	mouse::back_run_flag = false;
	control::reset_delta(sen_all);
	mouse::set_distance_m(0);
}

void run::accel_run_wall_off(const float distance_m, const float end_velocity,
		const unsigned char select_mode, const float check_distance) {

	float target_distance = check_distance;
	bool wall_off_flag = true;
	bool wall_flag_right = false, wall_flag_left = false;	//壁があるかないかフラグ

	//チェック距離までは普通に走る
	accel_run(distance_m - check_distance, end_velocity, select_mode);

	while (mouse::get_distance_m() < target_distance) {
		//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			return;
		}

		if (wall_off_flag) {

			if (wall_flag_right == false) {
				if (photo::check_wall(MUKI_RIGHT)) {
					wall_flag_right = true;
				}
			} else {
				if (photo::get_ad(right)
						< parameter::get_min_wall_photo(right)) {
					//壁切れ発動
					mouse::set_distance_m(0);
					target_distance = WALL_OFF_DISTANCE_RIGHT;
					wall_off_flag = false;
				}
			}

			if (wall_flag_left == false) {
				if (photo::check_wall(MUKI_LEFT)) {
					wall_flag_left = true;
				}
			} else {
				if (photo::get_ad(left) < parameter::get_min_wall_photo(left)) {
					//壁切れ発動
					mouse::set_distance_m(0);
					target_distance = WALL_OFF_DISTANCE_LEFT;
					wall_off_flag = false;
				}

			}
		} else {
			my7seg::light(7);
		}

	}
	if (wall_off_flag == false) {
		my7seg::turn_off();
	}
	mouse::set_distance_m(0);

}

void run::slalom(const SLALOM_TYPE slalom_type, const signed char right_or_left,
		const unsigned char select_mode) {
	float distance = parameter::get_slalom(slalom_type, before_distance,
			right_or_left, select_mode);
	float slalom_velocity = parameter::get_slalom(slalom_type, velocity,
			right_or_left, select_mode);
	float angular_acceleration = parameter::get_slalom(slalom_type,
			angular_accel, right_or_left, select_mode);
	float target_angle_degree = parameter::get_slalom(slalom_type, target_angle,
			right_or_left, select_mode);
	float clothoid_angle_degree = parameter::get_slalom(slalom_type,
			clothoid_angle, right_or_left, select_mode);
	float max_angular = parameter::get_slalom(slalom_type, max_angular_velocity,
			right_or_left, select_mode);
	float de_accel_angle = 0;

	if (slalom_type == none) {
		return;
	}

	gyro::reset_angle();
	control::reset_delta(sen_gyro);

//前距離の分走る
	control::stop_wall_control();

	mouse::set_distance_m(0);
	accel_run(distance, slalom_velocity, select_mode);

	mouse::slalom_flag = true;

//時計回りが正
	if (right_or_left == MUKI_LEFT) {
		angular_acceleration = -ABS(angular_acceleration);
		max_angular = -ABS(max_angular);
	}
	mouse::set_ideal_angular_velocity(0);

//角加速区間
	mouse::set_angular_acceleration(angular_acceleration);
	while (ABS(gyro::get_angle()) < clothoid_angle_degree) {
//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			return;
		}
		//最大角速度に達したら終了
		if (ABS(mouse::get_ideal_angular_velocity()) > ABS(max_angular)) {
			mouse::set_ideal_angular_velocity(max_angular);
			break;
		}
	}

//等角速度
	mouse::set_angular_acceleration(0);

	while (ABS(gyro::get_angle())
			< (target_angle_degree - clothoid_angle_degree)) {
//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			return;
		}

		de_accel_angle = degree(
				(gyro::get_angular_velocity() * gyro::get_angular_velocity())
				//		(mouse::get_ideal_angular_velocity() * mouse::get_ideal_angular_velocity())
						/ (2 * angular_acceleration));

//減速に必要な角度が残ってなければ抜ける
		if (ABS(de_accel_angle)
				>= (target_angle_degree - ABS(mouse::get_angle_degree()))) {
			break;
		}

	}

//角減速区間
	mouse::set_angular_acceleration(-angular_acceleration);
	while (ABS(gyro::get_angle()) < target_angle_degree) {
		if (ABS(mouse::get_ideal_angular_velocity()) < 0.2) {
			mouse::set_angular_acceleration(0);

			if (right_or_left == MUKI_RIGHT) {
				mouse::set_ideal_angular_velocity(0.2);
			} else {
				mouse::set_ideal_angular_velocity(-0.2);
			}
			break;
		}

//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			return;
		}
	}

	mouse::set_angular_acceleration(0);
	mouse::set_ideal_angular_velocity(0);

//後ろ距離分走る
	mouse::set_distance_m(0);
	distance = parameter::get_slalom((slalom_type), after_distance,
			right_or_left, select_mode);
	accel_run(distance, slalom_velocity, select_mode);

	mouse::slalom_flag = false;
	//gyro::reset_angle();
	control::reset_delta(sen_gyro);

}

void run::slalom_by_sin(const SLALOM_TYPE slalom_type,
		const signed char right_or_left, const unsigned char select_mode) {
	float distance = parameter::get_slalom(slalom_type, before_distance,
			right_or_left, select_mode);
	float slalom_velocity = parameter::get_slalom(slalom_type, velocity,
			right_or_left, select_mode);
	float angular_acceleration;
	float target_angle_degree = parameter::get_slalom(slalom_type, target_angle,
			right_or_left, select_mode);
	float clothoid_angle_degree = parameter::get_slalom(slalom_type,
			clothoid_angle, right_or_left, select_mode);
	float max_angular = parameter::get_slalom(slalom_type, max_angular_velocity,
			right_or_left, select_mode);	//シュミレータ上のパラメータＡ
	float ramda = max_angular * 90 / clothoid_angle_degree;	//シュミレータ上のパラメータＢ
	float temp_t;	//時間の変数
	long ref_count_ms;	//時間を管理する変数
	signed char accel_sign = 1;	//符号を管理

	mouse::slalom_flag = true;

//前距離の分走る
	mouse::set_distance_m(0);
	accel_run(distance, slalom_velocity, select_mode);

	control::stop_wall_control();
//時計回りが正
	if (right_or_left == MUKI_LEFT) {
		accel_sign = -1;
	}
	gyro::reset_angle();
	mouse::set_ideal_angular_velocity(0);

//角加速区間
	ref_count_ms = mouse::get_count_ms();	//この時間を基準に
	temp_t = 0;	//こっちが時間の変数
	while (ABS(gyro::get_angle()) < clothoid_angle_degree) {
//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			return;
		}

//角加速度は (1/2)*ABsin(Bt)
		angular_acceleration = accel_sign
				* (max_angular * ramda * my_math::sin(ramda * temp_t) / 2);
//cmtが割り込むまで待機
		mouse::set_angular_acceleration(angular_acceleration);
		ref_count_ms = mouse::get_count_ms();
		while (ref_count_ms == mouse::get_count_ms()) {
		}
		temp_t += CONTROL_PERIOD;	//時間の変数を進める

		if ((temp_t * ramda) > PI()) {
			temp_t = PI() / ramda;
		}
	}
	temp_t = PI() / ramda;

//等角速度
	mouse::set_angular_acceleration(0);
	mouse::set_ideal_angular_velocity(max_angular * accel_sign);
	while (ABS(gyro::get_angle())
			< (target_angle_degree - clothoid_angle_degree)) {
//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			return;
		}
	}

//角減速区間
	accel_sign = -accel_sign;	//符号を逆に
	ref_count_ms = mouse::get_count_ms();	//この時間を基準に
	while (ABS(gyro::get_angle()) < target_angle_degree) {
//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			return;
		}

//角速度が減速しきったにも関わらず角度が足りないときは、ここで稼ぐ
		if (ABS(gyro::get_angular_velocity()) < 0.1) {
			mouse::set_angular_acceleration(0);
			if (right_or_left == MUKI_RIGHT) {
				mouse::set_ideal_angular_velocity(0.1);
			} else {
				mouse::set_ideal_angular_velocity(-0.1);
			}

		} else {
			//角加速度は (1/2)*ABsin(Bt)
			angular_acceleration = accel_sign
					* (max_angular * ramda * my_math::sin(ramda * temp_t) / 2);

			//cmtが割り込むまで待機
			mouse::set_angular_acceleration(angular_acceleration);
			ref_count_ms = mouse::get_count_ms();
			while (ref_count_ms == mouse::get_count_ms()) {
			}
			temp_t -= CONTROL_PERIOD;	//時間の変数を進める(ただし減速なので減らしていく)
			//XXX 本当に周期関数なら増やしていい気がする

			if (temp_t < 0) {
				temp_t = 0;
			}
		}

	}

	mouse::set_angular_acceleration(0);
	mouse::set_ideal_angular_velocity(0);
	mouse::reset_angle();

//後ろ距離分走る
	control::start_wall_control();
	mouse::set_distance_m(0);
	distance = parameter::get_slalom(slalom_type, after_distance, right_or_left,
			select_mode);
	accel_run(distance, slalom_velocity, select_mode);

	mouse::slalom_flag = false;

}

void run::spin_turn(const float target_degree) {
	const static float max_angular_velocity = 20.0;	//rad/s
	float angular_acceleration = 20.0;	//rad/s^2
	float angle_degree = 0;

	control::stop_wall_control();
	wait_ms(100);

//時計回りが正
	if (target_degree < 0) {
		angular_acceleration = -1 * ABS(angular_acceleration);
	}

	mouse::reset_angle();
	mouse::set_ideal_velocity(0);
	mouse::set_ideal_angular_velocity(0);
	control::reset_delta(sen_all);

//角加速区間
	mouse::set_angular_acceleration(angular_acceleration);

	while (ABS(gyro::get_angle()) < ABS(target_degree / 3)) {
//最大角速度に達したら
		if (ABS(mouse::get_ideal_angular_velocity()) > max_angular_velocity) {
			mouse::set_angular_acceleration(0);
			//時計回りが正
			if (target_degree < 0) {
				mouse::set_ideal_angular_velocity(-max_angular_velocity);
			} else {
				mouse::set_ideal_angular_velocity(max_angular_velocity);
			}
		}
	}

//等角速度区間
	mouse::set_angular_acceleration(0);
	angle_degree = target_degree - angle_degree;
	while (ABS(mouse::get_angle_degree()) < ABS(target_degree * 2 / 3)) {
//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			return;
		}
//減速に必要な角度を計算
		angle_degree = degree(
				(gyro::get_angular_velocity() * gyro::get_angular_velocity())
						/ (2 * angular_acceleration));
//減速に必要な角度が残ってなければ抜ける
		if (ABS(angle_degree) >= ABS(target_degree - mouse::get_angle_degree())) {
			break;
		}
	}

//角減速区間
	mouse::set_angular_acceleration(-angular_acceleration);
	while (ABS(mouse::get_angle_degree()) < ABS(target_degree)) {

//この条件付けないと、先に角速度が0になった場合いつまでたってもループを抜けない
		if (ABS(gyro::get_angular_velocity()) < 1) {
			mouse::set_angular_acceleration(0);
			if (target_degree > 0) {
				mouse::set_ideal_angular_velocity(1);
			} else {
				mouse::set_ideal_angular_velocity(-1);
			}
		}
	}

	mouse::set_angular_acceleration(0);
	mouse::set_ideal_angular_velocity(0);

	wait_ms(100);

	control::stop_control();

	mouse::reset_angle();
	control::reset_delta(sen_all);

	mouse::set_distance_m(0);

	control::start_control();
	control::start_wall_control();
}

void run::path(const float finish_velocity, const unsigned char run_mode) {
	float next_velocity = 0;
	bool naname_flag = false;
	SLALOM_TYPE slalom_type;
	unsigned char slalom_muki;

	control::stop_wall_control();
	control::stop_control();
	mouse::set_fail_flag(false);

	motor::stanby_motor();

	wait_ms(1000);

	mouse::reset_angle();
	mouse::set_ideal_velocity(0);
	mouse::set_ideal_angular_velocity(0);
	gyro::reset_angle();
	control::reset_delta(sen_all);
	wait_ms(100);

	control::start_control();

	my7seg::count_down(3, 500);

	mouse::set_distance_m(0);
	control::start_wall_control();

	for (int path_count = 0; path::get_path_flag(path_count); path_count++) {

		//配列外にでるなら
		if (path_count >= PATH_MAX) {
			break;
		}

		if (mouse::get_fail_flag()) {		//フェイルセーフが掛かっていたら終了
			break;
		}

		mouse::set_distance_m(0);

		my7seg::turn_off();

//直線の処理
		if (path::get_path_straight(path_count) == 0) {			//直線がない場合
			//既に終わってた場合
			if ((path::get_path_flag(path_count)) == false) {
				next_velocity = finish_velocity;
				//例外処理なので最後の直線を走って終わり
				run::accel_run(0.09, next_velocity, run_mode);
				break;

			}
		} else {
			//次のパスで終了する場合
			if ((path::get_path_flag(path_count + 1)) == false) {
				if (path::get_path_turn_type(path_count) == none) {	//この直線で最後
					next_velocity = finish_velocity;
					//例外処理なので最後の直線を走って終わり
					run::accel_run(path::get_path_straight(path_count),
							next_velocity, run_mode);
					break;

				} else {
					//次のターン速度に合わせる
					next_velocity = parameter::get_slalom(
							path::get_path_turn_type(path_count), velocity,
							path::get_path_turn_muki(path_count), run_mode);

				}

			} else {
				//次のターン速度に合わせる
				next_velocity = parameter::get_slalom(
						path::get_path_turn_type(path_count), velocity,
						path::get_path_turn_muki(path_count), run_mode);
			}

			if (naname_flag) {	//ナナメ走行中
				run::accel_run(path::get_path_straight(path_count),
						next_velocity, run_mode);

			} else {				//普通の直進
				my7seg::light(1);

				control::start_wall_control();

				run::accel_run_wall_off(path::get_path_straight(path_count),
						next_velocity, run_mode, 0.12);
			}

		}

		if (mouse::get_fail_flag()) {		//フェイルセーフが掛かっていたら終了
			break;
		}

		my7seg::light(2);

//ターンの処理
		slalom_type = path::get_path_turn_type(path_count);
		slalom_muki = path::get_path_turn_muki(path_count);

		run::slalom(slalom_type, slalom_muki, run_mode);

		//FIXME
		/*
		 switch (path::get_path_turn_type(path_count)) {
		 //ナナメに入るなら
		 case begin_45:
		 case begin_135:
		 naname_flag = true;
		 break;
		 //ナナメから出るなら
		 case end_45:
		 case end_135:
		 naname_flag = false;
		 break;

		 default:
		 break;

		 }
		 */
	}

	wait_ms(100);
	motor::sleep_motor();

}

run::run() {
}

run::~run() {
}

unsigned char carcuit::run_mode;

void carcuit::set_run_mode(unsigned char select_mode) {
	if (select_mode > RUN_MODE_NUMBER) {
		error();
		myprintf(
				"You choice unknow parameter mode %d in carcuit class, set_run_mode function \n\r, ",
				select_mode);
	} else {
		run_mode = select_mode;
	}
}

void carcuit::run_carcuit(const unsigned char maze_x_size,
		const unsigned char maze_y_size, const unsigned char cycle_count,
		bool wall_off) {
	float slalom_velocity;

	my7seg::turn_off();

	motor::stanby_motor();

	wait_ms(1000);

	control::start_control();
	mouse::set_ideal_velocity(0);
	mouse::set_ideal_angular_velocity(0);

	my7seg::count_down(3, 500);

	control::reset_delta(sen_all);
	mouse::set_distance_m(0);
	control::start_wall_control();

	if (run_mode == 0) {		//0のときは超信地でまわる

		for (int i = 0; i < cycle_count; i++) {
			run::accel_run(0.18 * (maze_y_size - 1), 0, run_mode);
			run::spin_turn(90);

			run::accel_run(0.18 * (maze_x_size - 1), 0, run_mode);
			run::spin_turn(90);

			run::accel_run(0.18 * (maze_y_size - 1), 0, run_mode);
			run::spin_turn(90);

			run::accel_run(0.18 * (maze_x_size - 1), 0, run_mode);
			run::spin_turn(90);
		}
		run::accel_run(0.18 * 3, 0, run_mode);
		motor::sleep_motor();

	} else {
		slalom_velocity = parameter::get_slalom(small, velocity, MUKI_RIGHT,
				run_mode);

//とりあえず一周する

//最初の直線は1区間長い
		run::accel_run(0.18 * (maze_y_size - 2) + 0.09, slalom_velocity,
				run_mode);
		run::slalom(small, MUKI_RIGHT, run_mode);

		run::accel_run(0.18 * (maze_x_size - 2), slalom_velocity, run_mode);
		run::slalom(small, MUKI_RIGHT, run_mode);

		run::accel_run(0.18 * (maze_y_size - 2), slalom_velocity, run_mode);
		run::slalom(small, MUKI_RIGHT, run_mode);

		run::accel_run(0.18 * (maze_x_size - 2), slalom_velocity, run_mode);
		run::slalom(small, MUKI_RIGHT, run_mode);

		for (int i = 1; i < cycle_count; i++) {
			run::accel_run(0.18 * (maze_y_size - 2), slalom_velocity, run_mode);
			run::slalom(small, MUKI_RIGHT, run_mode);

			run::accel_run(0.18 * (maze_x_size - 2), slalom_velocity, run_mode);
			run::slalom(small, MUKI_RIGHT, run_mode);

			run::accel_run(0.18 * (maze_y_size - 2), slalom_velocity, run_mode);
			run::slalom(small, MUKI_RIGHT, run_mode);

			run::accel_run(0.18 * (maze_x_size - 2), slalom_velocity, run_mode);
			run::slalom(small, MUKI_RIGHT, run_mode);
		}

		run::accel_run(0.18 * 2, slalom_velocity, run_mode);
		motor::sleep_motor();
	}
}

carcuit::carcuit() {

}

carcuit::~carcuit() {

}

void left_hand::run_next_action(ACTION_TYPE next_action) {

	switch (next_action) {
	case go_straight:
//1区間直進
		wait_ms(500);
		run::accel_run((0.18 / MOUSE_MODE), SEARCH_VELOCITY, 0);
		return;
		break;

	case turn_right:
//半区間⇒超信地⇒半区間
		run::accel_run((0.09 / MOUSE_MODE), 0, 0);
		run::spin_turn(90);
		run::accel_run((0.09 / MOUSE_MODE), SEARCH_VELOCITY, 0);
		return;
		break;

	case turn_left:
//半区間⇒超信地⇒半区間
		run::accel_run((0.09 / MOUSE_MODE), 0, 0);
		run::spin_turn(270);
		run::accel_run((0.09 / MOUSE_MODE), SEARCH_VELOCITY, 0);
		return;
		break;

	case back:
//半区間直進して180°ターンして半区間直進
		run::accel_run((0.09 / MOUSE_MODE), 0, 0);
		run::spin_turn(180);
		run::accel_run((0.09 / MOUSE_MODE), SEARCH_VELOCITY, 0);
		return;
		break;

	case stop:
//半区間進んでストップ
		run::accel_run((0.09 / MOUSE_MODE), 0, 0);		//半区間直進
		return;
		break;
	}
}

bool left_hand::run2(const unsigned char target_x,
		const unsigned char target_y) {
	signed char direction_x, direction_y;
	unsigned char now_x, now_y;	//座標一時保存用。見易さのため
	bool left_hand_flag = true;

	my7seg::turn_off();

//向きを取得
	mouse::get_direction(&direction_x, &direction_y);

//初期化
	control::stop_wall_control();
	mouse::set_fail_flag(false);

	motor::stanby_motor();

	wait_ms(1000);

	mouse::reset_angle();
	mouse::set_ideal_velocity(0);
	mouse::set_ideal_angular_velocity(0);
	control::reset_delta(sen_all);

	control::start_control();

	my7seg::count_down(3, 500);

	mouse::set_distance_m(0);
	control::start_wall_control();

	float_log::reset_log();

	run::accel_run((0.09 / MOUSE_MODE), SEARCH_VELOCITY, 0);

	mouse::get_direction(&direction_x, &direction_y);

	mouse::set_position(0, 0);
	while (left_hand_flag) {
//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			left_hand_flag = false;
			break;
		}

//向きをセット
		mouse::set_direction(direction_x, direction_y);

//座標を更新
		now_x = (mouse::get_x_position() + direction_x);
		now_y = (mouse::get_y_position() + direction_y);
		mouse::set_position(now_x, now_y);

//壁情報を更新
		mouse::look_wall(false);

//ゴールなら抜ける
		if (mouse::get_x_position() == target_x) {
			if (mouse::get_y_position() == target_y) {
				break;
			}
		}

		my7seg::light(mouse::get_x_position());

//壁チェック
		if (photo::check_wall(MUKI_LEFT)) {
			if (photo::check_wall(MUKI_UP)) {
				if (photo::check_wall(MUKI_RIGHT)) {
					run::accel_run((0.09 / MOUSE_MODE), 0, 0);
					run::spin_turn(180);
					direction_turn(&direction_x, &direction_y, MUKI_LEFT);//向きを90°変える
					direction_turn(&direction_x, &direction_y, MUKI_LEFT);//向きを90°変える
					run::accel_run((0.09 / MOUSE_MODE), SEARCH_VELOCITY, 0);

				} else {

					run::slalom(small, MUKI_RIGHT, 0);

					//run::accel_run((0.09 / MOUSE_MODE), 0, 0);
					//run::spin_turn(90);
					//run::accel_run((0.09 / MOUSE_MODE), SEARCH_VELOCITY, 0);

					direction_turn(&direction_x, &direction_y, MUKI_RIGHT);	//向きを90°変える

				}
			} else {

				run::accel_run((0.18 / MOUSE_MODE), SEARCH_VELOCITY, 0);

			}
		} else {

			run::slalom(small, MUKI_LEFT, 0);
			direction_turn(&direction_x, &direction_y, MUKI_LEFT);	//向きを90°変える

			//run::accel_run((0.09 / MOUSE_MODE), 0, 0);
			//run::spin_turn(-90);
			//run::accel_run((0.09 / MOUSE_MODE), SEARCH_VELOCITY, 0);

		}

	}

	if (left_hand_flag) {
		run_next_action(stop);
		return true;
	} else {
		motor::sleep_motor();
		return false;
	}

}

left_hand::left_hand() {

}

left_hand::~left_hand() {

}

signed char adachi::direction_x, adachi::direction_y;

void adachi::set_direction() {
	unsigned char direction = mouse::get_direction();

	switch (direction) {
	case MUKI_RIGHT:
		direction_x = 1;
		direction_y = 0;
		break;

	case MUKI_LEFT:
		direction_x = -1;
		direction_y = 0;
		break;

	case MUKI_UP:
		direction_x = 0;
		direction_y = 1;
		break;

	case MUKI_DOWN:
		direction_x = 0;
		direction_y = -1;
		break;
	}

}

bool adachi::check_move_by_step(unsigned char target_x, unsigned char target_y,
		unsigned char muki) {
	signed char muki_x = 0, muki_y = 0;

	switch (muki) {
	case MUKI_RIGHT:
//迷路外ならfalseを返す
		if ((target_x + 1) >= MAZE_SIZE) {
			return false;
		}
		muki_x = 1;
		muki_y = 0;
		break;

	case MUKI_LEFT:
//迷路外ならfalseを返す
		if ((target_x - 1) < 0) {
			return false;
		}
		muki_x = -1;
		muki_y = 0;
		break;

	case MUKI_UP:
//迷路外ならfalseを返す
		if ((target_y + 1) >= MAZE_SIZE) {
			return false;
		}
		muki_x = 0;
		muki_y = 1;
		break;

	case MUKI_DOWN:
//迷路外ならfalseを返す
		if ((target_y - 1) < 0) {
			return false;
		}
		muki_x = 0;
		muki_y = -1;
		break;
	}

	if (step::get_step((target_x + muki_x), (target_y + muki_y))
			== (step::get_step(target_x, target_y) - 1)) {	//歩数の小さいほうへ
		if ((map::get_wall(target_x, target_y, muki) == FALSE)) {	//壁がないなら
			return true;
		}

	}

	return false;
}

unsigned int adachi::count_unknown_wall(unsigned char target_x,
		unsigned char target_y) {
	unsigned int unknown_count = 0;

//各方向について探索済みか調べる.未探索ならカウントアップ
	if ((map::check_exist(target_x, target_y, MUKI_RIGHT)) == FALSE) {
		unknown_count++;
	}
	if ((map::check_exist(target_x, target_y, MUKI_LEFT)) == FALSE) {
		unknown_count++;
	}
	if ((map::check_exist(target_x, target_y, MUKI_UP)) == FALSE) {
		unknown_count++;
	}
	if ((map::check_exist(target_x, target_y, MUKI_DOWN)) == FALSE) {
		unknown_count++;
	}

	return unknown_count;
}

void adachi::run_next_action(ACTION_TYPE next_action, bool wall_off) {
	bool front_wall_flag = false, right_wall_flag = false, left_wall_flag =
			false;
	static const short back_break_time = 100;	//尻当てするときの待機時間

	switch (next_action) {
	case go_straight:
//1区間直進
		if (wall_off) {
			run::accel_run_wall_off((0.18 / MOUSE_MODE), SEARCH_VELOCITY, 0,
					(0.12 / MOUSE_MODE));
		} else {
			run::accel_run((0.18 / MOUSE_MODE), SEARCH_VELOCITY, 0);
		}
		break;

	case turn_right:
//半区間⇒超信地⇒半区間

		run::accel_run((0.09 / MOUSE_MODE), 0, 0);
		run::spin_turn(90);
		run::accel_run((0.09 / MOUSE_MODE), SEARCH_VELOCITY, 0);

		//run::slalom(small, MUKI_RIGHT, 0);
		direction_turn(&direction_x, &direction_y, MUKI_RIGHT);	//向きを90°変える

		break;

	case turn_left:
//半区間⇒超信地⇒半区間

		run::accel_run((0.09 / MOUSE_MODE), 0, 0);
		run::spin_turn(-90);
		run::accel_run((0.09 / MOUSE_MODE), SEARCH_VELOCITY, 0);

		//run::slalom(small, MUKI_LEFT, 0);
		direction_turn(&direction_x, &direction_y, MUKI_LEFT);	//向きを90°変える
		break;

	case back:
//半区間進んで180°ターンして半区間直進

		//前壁があればそこで尻当てするためにフラグをたてる
		if (photo::check_wall(MUKI_UP)) {
			front_wall_flag = true;
		}
		if (photo::check_wall(MUKI_RIGHT)) {
			right_wall_flag = true;
		} else if (photo::check_wall(MUKI_LEFT)) {
			left_wall_flag = true;
		}

		control::start_wall_control();
		run::fit_stop(0.09, 0, 0.1);
		direction_turn(&direction_x, &direction_y, MUKI_RIGHT);	//向きを90°変える
		direction_turn(&direction_x, &direction_y, MUKI_RIGHT);	//向きを90°変える
		control::stop_wall_control();

		//尻当てする
		if (right_wall_flag) {

			run::spin_turn(270);
			wait_ms(back_break_time);
			run::back_run(-0.18, 0, 0);
			wait_ms(back_break_time);
			run::accel_run(0.06, 0, 0);
			wait_ms(back_break_time);
			run::spin_turn(-90);

		} else if (left_wall_flag) {
			run::spin_turn(90);
			wait_ms(back_break_time);
			run::back_run(-0.18, 0, 0);
			wait_ms(back_break_time);
			run::accel_run(0.06, 0, 0);
			wait_ms(back_break_time);
			run::spin_turn(90);

		} else {
			run::spin_turn(180);

		}

		wait_ms(back_break_time);
		control::stop_wall_control();

		if (front_wall_flag) {
			run::back_run(-0.18, 0, 0);
			wait_ms(back_break_time);
			control::stop_wall_control();
			run::accel_run_wall_off(0.06 + 0.09, SEARCH_VELOCITY, 0, 0.11);
		} else {
			run::accel_run((0.09 / MOUSE_MODE), SEARCH_VELOCITY, 0);	//半区間直進

		}
		gyro::reset_angle();
		control::start_wall_control();
		break;

	case stop:
//半区間進んでストップ
		run::accel_run((0.09 / MOUSE_MODE), 0, 0);	//半区間直進
		break;
	}

	return;
}

void adachi::run_next_action_slalom(ACTION_TYPE next_action, bool wall_off) {
	bool front_wall_flag = false, right_wall_flag = false, left_wall_flag =
			false;
	my7seg::turn_off();
	static const short back_break_time = 100;	//尻当てするときの待機時間
	static bool angle_miss_flag = false;	//スラロームの角度がずれたらtrueに

	switch (next_action) {
	case go_straight:
//1区間直進
		control::start_wall_control();
		if (wall_off) {
			run::accel_run_wall_off((0.18 / MOUSE_MODE), SEARCH_VELOCITY, 0,
					0.12);
		} else {
			run::accel_run((0.18 / MOUSE_MODE), SEARCH_VELOCITY, 0);
		}
		angle_miss_flag = false;

		break;

	case turn_right:
//半区間⇒超信地⇒半区間
		control::stop_wall_control();

		//スラロームがずれてなければ
		if (angle_miss_flag == false) {

			run::slalom(small, MUKI_RIGHT, 0);
			if (ABS(
					gyro::get_angle()-90) > 3) {	//5°以上ずれてたら
				my7seg::light(4);
				angle_miss_flag = true;

			}

		} else {
			//ずれてれば尻当てする

			//壁があればそこで尻当てするためにフラグをたてる
			if (photo::check_wall(MUKI_UP)) {
				front_wall_flag = true;
			}
			if (photo::check_wall(MUKI_LEFT)) {
				left_wall_flag = true;
			}

			control::start_wall_control();
			run::accel_run(0.09, 0, 0);
			control::stop_wall_control();

			//尻当てする
			if (front_wall_flag) {
				run::spin_turn(180);
				run::back_run(-0.18, 0, 0);
				wait_ms(back_break_time);
				run::accel_run(0.06, 0, 0);
				wait_ms(back_break_time);
				run::spin_turn(-90);
			} else {
				wait_ms(back_break_time);
				run::spin_turn(90);
			}

			wait_ms(back_break_time);

			if (left_wall_flag) {
				run::back_run(-0.18, 0, 0);
				wait_ms(back_break_time);
				run::accel_run(0.06, SEARCH_VELOCITY, 0);
			}

			angle_miss_flag = false;

			run::accel_run(0.06, SEARCH_VELOCITY, 0);

		}

		direction_turn(&direction_x, &direction_y, MUKI_RIGHT);	//向きを90°変える
		gyro::reset_angle();
		control::start_wall_control();

		break;

	case turn_left:
//半区間⇒超信地⇒半区間
		control::stop_wall_control();

		//スラロームがずれてなければ
		if (angle_miss_flag == false) {
			run::slalom(small, MUKI_LEFT, 0);

			if (ABS(ABS(gyro::get_angle())-90) > 3) {	//5°以上ずれてたら
				my7seg::light(4);
				angle_miss_flag = true;

			}
		} else {
			//ずれてれば尻当てする

			//壁があればそこで尻当てするためにフラグをたてる
			if (photo::check_wall(MUKI_UP)) {
				front_wall_flag = true;
			}
			if (photo::check_wall(MUKI_RIGHT)) {
				right_wall_flag = true;
			}

			control::start_wall_control();
			run::accel_run(0.09, 0, 0);
			control::stop_wall_control();

			//尻当てする
			if (front_wall_flag) {
				run::spin_turn(180);
				run::back_run(-0.18, 0, 0);
				wait_ms(back_break_time);
				run::accel_run(0.06, 0, 0);
				wait_ms(back_break_time);
				run::spin_turn(90);
			} else {
				wait_ms(back_break_time);
				run::spin_turn(-90);
			}

			wait_ms(back_break_time);

			if (right_wall_flag) {
				run::back_run(-0.18, 0, 0);
				wait_ms(back_break_time);
				run::accel_run(0.06, SEARCH_VELOCITY, 0);
			}

			angle_miss_flag = false;

			run::accel_run(0.06, SEARCH_VELOCITY, 0);

		}

		gyro::reset_angle();

		direction_turn(&direction_x, &direction_y, MUKI_LEFT);	//向きを90°変える
		control::start_wall_control();

		break;

	case back:
//半区間進んで180°ターンして半区間直進

		//前壁があればそこで尻当てするためにフラグをたてる
		if (photo::check_wall(MUKI_UP)) {
			front_wall_flag = true;
		}
		if (photo::check_wall(MUKI_RIGHT)) {
			right_wall_flag = true;
		} else if (photo::check_wall(MUKI_LEFT)) {
			left_wall_flag = true;
		}

		control::start_wall_control();
		run::fit_stop(0.09, 0, 0.1);
		direction_turn(&direction_x, &direction_y, MUKI_RIGHT);	//向きを90°変える
		direction_turn(&direction_x, &direction_y, MUKI_RIGHT);	//向きを90°変える
		control::stop_wall_control();

		//尻当てする
		if (right_wall_flag) {

			run::spin_turn(270);
			wait_ms(back_break_time);
			run::back_run(-0.18, 0, 0);
			wait_ms(back_break_time);
			run::accel_run(0.06, 0, 0);
			wait_ms(back_break_time);
			run::spin_turn(-90);

		} else if (left_wall_flag) {
			run::spin_turn(90);
			wait_ms(back_break_time);
			run::back_run(-0.18, 0, 0);
			wait_ms(back_break_time);
			run::accel_run(0.06, 0, 0);
			wait_ms(back_break_time);
			run::spin_turn(90);

		} else {
			run::spin_turn(180);

		}

		wait_ms(back_break_time);
		control::stop_wall_control();

		if (front_wall_flag) {
			run::back_run(-0.18, 0, 0);
			wait_ms(back_break_time);
			control::stop_wall_control();
			run::accel_run_wall_off(0.06 + 0.09, SEARCH_VELOCITY, 0, 0.11);
		} else {
			run::accel_run((0.09 / MOUSE_MODE), SEARCH_VELOCITY, 0);	//半区間直進

		}
		angle_miss_flag = false;
		gyro::reset_angle();
		control::start_wall_control();
		break;

	case stop:
//半区間進んでストップ
		run::accel_run((0.09 / MOUSE_MODE), 0, 0);	//半区間直進
		break;
	}

	return;
}

ACTION_TYPE adachi::get_next_action(DIRECTION next_direction) {
	switch (mouse::get_direction()) {
	case MUKI_RIGHT:
//真っ直ぐ行けるなら、それを最優先
		if (next_direction.element.right == 1) {
			return go_straight;
		}
//右のターンを次に優先
		if (next_direction.element.down == 1) {
			return turn_right;
		}
//左のターンを次に優先
		if (next_direction.element.up == 1) {
			return turn_left;
		}
//Uターンは優先度最低
		if (next_direction.element.left == 1) {
			return back;
		}
		break;

//やってることは以下同様なのでコメントは省く
	case MUKI_LEFT:
		if (next_direction.element.left == 1) {
			return go_straight;
		}
		if (next_direction.element.up == 1) {
			return turn_right;
		}
		if (next_direction.element.down == 1) {
			return turn_left;
		}
		if (next_direction.element.right == 1) {
			return back;
		}
		break;

	case MUKI_UP:
		if (next_direction.element.up == 1) {
			return go_straight;
		}
		if (next_direction.element.right == 1) {
			return turn_right;
		}
		if (next_direction.element.left == 1) {
			return turn_left;
		}
		if (next_direction.element.down == 1) {
			return back;
		}
		break;

	case MUKI_DOWN:
		if (next_direction.element.down == 1) {
			return go_straight;
		}
		if (next_direction.element.left == 1) {
			return turn_right;
		}
		if (next_direction.element.right == 1) {
			return turn_left;
		}
		if (next_direction.element.up == 1) {
			return back;
		}
		break;
	}

//ここにたどり着くのは、次行く方向がないか、予期せぬ例外なので、マウスを止める。
	return stop;
}

bool adachi::adachi_method_spin(unsigned char target_x,
		unsigned char target_y) {
	bool adachi_flag = true;	//途中でミスがあったらfalseに
	unsigned char now_x, now_y;	//座標一時保存用。見易さのため
	DIRECTION next_direction, priority_direction;	//次に行く方向を管理
	unsigned char max_unknown_count, target_unknown_count;	//未探索の壁の数を管理
	ACTION_TYPE next_action;	//次の行動を管理

	my7seg::turn_off();

	//保存していたマップを読みだす
	map::input_map_data(&mouse::now_map);

//向きを取得
	mouse::get_direction(&direction_x, &direction_y);

	control::stop_wall_control();
	control::start_control();
	mouse::set_fail_flag(false);


	wait_ms(1000);

	mouse::reset_angle();
	mouse::set_ideal_velocity(0);
	mouse::set_ideal_angular_velocity(0);
	control::reset_delta(sen_all);

	motor::stanby_motor();

	control::start_control();

	my7seg::count_down(3, 500);

	mouse::set_distance_m(0);
	control::start_wall_control();

	run::accel_run((0.09 / MOUSE_MODE), SEARCH_VELOCITY, 0);

	while (adachi_flag) {
//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			adachi_flag = false;
			break;
		}

//座標を更新
		now_x = mouse::get_x_position() + direction_x;
		now_y = mouse::get_y_position() + direction_y;
		mouse::set_position(now_x, now_y);
//向きも
		mouse::set_direction(direction_x, direction_y);

//壁情報更新
		mouse::look_wall(false);

//目標の座標にたどり着いたら終了
		if ((now_x == target_x) && (now_y == target_y)) {
			run_next_action(stop, false);
			break;
		}

//歩数マップ作製
		step::set_step(target_x, target_y);

//方向と未探索の壁の数をリセット
		next_direction.all = 0;
		priority_direction.all = 0;
		max_unknown_count = 1;	//0をはじくため

//歩数的に前後左右のマスへ行けるか判別.
//行ければ次行く方向の候補に入れる
//更に見てない壁の数が多ければpriority_directionの方にも候補として追加
		if (check_move_by_step(now_x, now_y, MUKI_RIGHT)) {			//右
			next_direction.element.right = 1;

			target_unknown_count = count_unknown_wall((now_x + 1), now_y);
			if (target_unknown_count == max_unknown_count) {
				priority_direction.element.right = 1;
			} else if (target_unknown_count >= max_unknown_count) {
				max_unknown_count = target_unknown_count;
				priority_direction.all = 0;				//他の方向はいらないのでリセット
				priority_direction.element.right = 1;
			}

		}
		if (check_move_by_step(now_x, now_y, MUKI_LEFT)) {			//左
			next_direction.element.left = 1;

			target_unknown_count = count_unknown_wall((now_x - 1), now_y);
			if (target_unknown_count == max_unknown_count) {
				priority_direction.element.left = 1;
			} else if (target_unknown_count >= max_unknown_count) {
				max_unknown_count = target_unknown_count;
				priority_direction.all = 0;				//他の方向はいらないのでリセット
				priority_direction.element.left = 1;
			}

		}
		if (check_move_by_step(now_x, now_y, MUKI_UP)) {			//上
			next_direction.element.up = 1;

			target_unknown_count = count_unknown_wall(now_x, (now_y + 1));
			if (target_unknown_count == max_unknown_count) {
				priority_direction.element.up = 1;
			} else if (target_unknown_count >= max_unknown_count) {
				max_unknown_count = target_unknown_count;
				priority_direction.all = 0;				//他の方向はいらないのでリセット
				priority_direction.element.up = 1;
			}

		}
		if (check_move_by_step(now_x, now_y, MUKI_DOWN)) {			//下
			next_direction.element.down = 1;

			target_unknown_count = count_unknown_wall(now_x, (now_y - 1));
			if (target_unknown_count == max_unknown_count) {
				priority_direction.element.down = 1;
			} else if (target_unknown_count >= max_unknown_count) {
				max_unknown_count = target_unknown_count;
				priority_direction.all = 0;				//他の方向はいらないのでリセット
				priority_direction.element.down = 1;
			}

		}

//未探索区間が候補の中にあるなら、次に行く方向はその中から選ぶ
		if (priority_direction.all != 0) {
			next_direction.all = priority_direction.all;
		}

//next_dirrctionから次行く方向を選び、行動する
		next_action = get_next_action(next_direction);
		if (MOUSE_MODE == 1) {
			run_next_action(next_action, true);
		} else {
			run_next_action(next_action, false);

		}
//もし止まるべきと出たならココで足立法をやめる
		if (next_action == stop) {
			adachi_flag = false;
		}

//方向更新
		mouse::set_direction(direction_x, direction_y);
	}

	if (adachi_flag) {
		//足立法成功なのでマップを保存する
		map::output_map_data(&mouse::now_map);

		return true;				//足立法完了!!

	} else {
//ここに来るということは足立法が失敗してる
		motor::sleep_motor();
//TODO わかりやすい何かが欲しい
		error();
		myprintf("Adachi method failed!\n\r");
		if (mouse::get_fail_flag()) {
			myprintf("fail safe!\n\r");
		}

		myprintf("now -> (%d,%d)\n\r", mouse::get_x_position(),
				mouse::get_y_position());
	}

	return false;
}

bool adachi::adachi_method(unsigned char target_x, unsigned char target_y) {
	bool adachi_flag = true;	//途中でミスがあったらfalseに
	unsigned char now_x, now_y;	//座標一時保存用。見易さのため
	DIRECTION next_direction, priority_direction;	//次に行く方向を管理
	unsigned char max_unknown_count, target_unknown_count;	//未探索の壁の数を管理
	ACTION_TYPE next_action;	//次の行動を管理

	my7seg::turn_off();

	map::input_map_data(&mouse::now_map);

//向きを取得
	mouse::get_direction(&direction_x, &direction_y);

	control::stop_wall_control();
	mouse::set_fail_flag(false);

	motor::stanby_motor();

	wait_ms(1000);

	mouse::reset_angle();
	mouse::set_ideal_velocity(0);
	mouse::set_ideal_angular_velocity(0);
	control::reset_delta(sen_all);

	control::start_control();

	my7seg::count_down(3, 500);

	mouse::set_distance_m(0);
	control::start_wall_control();

	run::accel_run((0.09 / MOUSE_MODE), SEARCH_VELOCITY, 0);

	while (adachi_flag) {
//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			adachi_flag = false;
			break;
		}

//座標を更新
		now_x = mouse::get_x_position() + direction_x;
		now_y = mouse::get_y_position() + direction_y;
		mouse::set_position(now_x, now_y);
//向きも
		mouse::set_direction(direction_x, direction_y);

//壁情報更新
		mouse::look_wall(false);

//目標の座標にたどり着いたら終了
		if ((now_x == target_x) && (now_y == target_y)) {
			run_next_action(stop, true);
			break;
		}

//歩数マップ作製
		step::set_step(target_x, target_y);

//方向と未探索の壁の数をリセット
		next_direction.all = 0;
		priority_direction.all = 0;
		max_unknown_count = 1;	//0をはじくため

//歩数的に前後左右のマスへ行けるか判別.
//行ければ次行く方向の候補に入れる
//更に見てない壁の数が多ければpriority_directionの方にも候補として追加
		if (check_move_by_step(now_x, now_y, MUKI_RIGHT)) {			//右
			next_direction.element.right = 1;

			target_unknown_count = count_unknown_wall((now_x + 1), now_y);
			if (target_unknown_count == max_unknown_count) {
				priority_direction.element.right = 1;
			} else if (target_unknown_count >= max_unknown_count) {
				max_unknown_count = target_unknown_count;
				priority_direction.all = 0;				//他の方向はいらないのでリセット
				priority_direction.element.right = 1;
			}

		}
		if (check_move_by_step(now_x, now_y, MUKI_LEFT)) {			//左
			next_direction.element.left = 1;

			target_unknown_count = count_unknown_wall((now_x - 1), now_y);
			if (target_unknown_count == max_unknown_count) {
				priority_direction.element.left = 1;
			} else if (target_unknown_count >= max_unknown_count) {
				max_unknown_count = target_unknown_count;
				priority_direction.all = 0;				//他の方向はいらないのでリセット
				priority_direction.element.left = 1;
			}

		}
		if (check_move_by_step(now_x, now_y, MUKI_UP)) {			//上
			next_direction.element.up = 1;

			target_unknown_count = count_unknown_wall(now_x, (now_y + 1));
			if (target_unknown_count == max_unknown_count) {
				priority_direction.element.up = 1;
			} else if (target_unknown_count >= max_unknown_count) {
				max_unknown_count = target_unknown_count;
				priority_direction.all = 0;				//他の方向はいらないのでリセット
				priority_direction.element.up = 1;
			}

		}
		if (check_move_by_step(now_x, now_y, MUKI_DOWN)) {			//下
			next_direction.element.down = 1;

			target_unknown_count = count_unknown_wall(now_x, (now_y - 1));
			if (target_unknown_count == max_unknown_count) {
				priority_direction.element.down = 1;
			} else if (target_unknown_count >= max_unknown_count) {
				max_unknown_count = target_unknown_count;
				priority_direction.all = 0;				//他の方向はいらないのでリセット
				priority_direction.element.down = 1;
			}

		}
//未探索区間が候補の中にあるなら、次に行く方向はその中から選ぶ
		if (priority_direction.all != 0) {
			next_direction.all = priority_direction.all;
		}

//next_dirrctionから次行く方向を選び、行動する
		next_action = get_next_action(next_direction);
		run_next_action_slalom(next_action, true);

//もし止まるべきと出たならココで足立法をやめる
		if (next_action == stop) {
			adachi_flag = false;
		}

//方向更新
		mouse::set_direction(direction_x, direction_y);
	}

	if (adachi_flag) {
		//足立法が成功したのでマップを保存する
		map::output_map_data(&mouse::now_map);
		return true;				//足立法完了!!

	} else {
//ここに来るということは足立法が失敗してる
		motor::sleep_motor();
//TODO わかりやすい何かが欲しい
		error();
		myprintf("Adachi method failed!\n\r");
		if (mouse::get_fail_flag()) {
			myprintf("fail safe!\n\r");
		}

		myprintf("now -> (%d,%d)\n\r", mouse::get_x_position(),
				mouse::get_y_position());
	}

	return false;
}

bool adachi::adachi_method_verF(unsigned char target_x,
		unsigned char target_y, const bool comb_ignore) {
	bool adachi_flag = true;	//途中でミスがあったらfalseに
	unsigned char now_x, now_y;	//座標一時保存用。見易さのため
	DIRECTION next_direction, priority_direction;	//次に行く方向を管理
	unsigned char max_unknown_count, target_unknown_count;	//未探索の壁の数を管理
	ACTION_TYPE next_action;	//次の行動を管理

	my7seg::turn_off();

	map::input_map_data(&mouse::now_map);

//向きを取得
	mouse::get_direction(&direction_x, &direction_y);

	control::stop_wall_control();
	mouse::set_fail_flag(false);

	motor::stanby_motor();

	wait_ms(1000);

	mouse::reset_angle();
	mouse::set_ideal_velocity(0);
	mouse::set_ideal_angular_velocity(0);
	control::reset_delta(sen_all);

	control::start_control();

	my7seg::count_down(3, 500);

	mouse::set_distance_m(0);
	control::start_wall_control();

	run::accel_run((0.09 / MOUSE_MODE), SEARCH_VELOCITY, 0);

	while (adachi_flag) {
//フェイルセーフが掛かっていればそこで抜ける
		if (mouse::get_fail_flag()) {
			adachi_flag = false;
			break;
		}

//座標を更新
		now_x = mouse::get_x_position() + direction_x;
		now_y = mouse::get_y_position() + direction_y;
		mouse::set_position(now_x, now_y);
//向きも更新
		mouse::set_direction(direction_x, direction_y);

//壁情報更新
		mouse::look_wall(comb_ignore);

//目標の座標にたどり着いたら終了
		if ((now_x == target_x) && (now_y == target_y)) {
			run_next_action(stop, true);
			break;
		}

//歩数マップ作製
		step::set_step(target_x, target_y);

//方向と未探索の壁の数をリセット
		next_direction.all = 0;
		priority_direction.all = 0;
		max_unknown_count = 1;	//0をはじくため

//歩数的に前後左右のマスへ行けるか判別.
//行ければ次行く方向の候補に入れる
//更に見てない壁の数が多ければpriority_directionの方にも候補として追加

		if (check_move_by_step(now_x, now_y, MUKI_RIGHT)) {			//右
			next_direction.element.right = 1;

			target_unknown_count = count_unknown_wall((now_x + 1), now_y);
			if (target_unknown_count == max_unknown_count) {
				priority_direction.element.right = 1;
			} else if (target_unknown_count >= max_unknown_count) {
				max_unknown_count = target_unknown_count;
				priority_direction.all = 0;				//他の方向はいらないのでリセット
				priority_direction.element.right = 1;
			}

		}
		if (check_move_by_step(now_x, now_y, MUKI_LEFT)) {			//左
			next_direction.element.left = 1;

			target_unknown_count = count_unknown_wall((now_x - 1), now_y);
			if (target_unknown_count == max_unknown_count) {
				priority_direction.element.left = 1;
			} else if (target_unknown_count >= max_unknown_count) {
				max_unknown_count = target_unknown_count;
				priority_direction.all = 0;				//他の方向はいらないのでリセット
				priority_direction.element.left = 1;
			}

		}
		if (check_move_by_step(now_x, now_y, MUKI_UP)) {			//上
			next_direction.element.up = 1;

			target_unknown_count = count_unknown_wall(now_x, (now_y + 1));
			if (target_unknown_count == max_unknown_count) {
				priority_direction.element.up = 1;
			} else if (target_unknown_count >= max_unknown_count) {
				max_unknown_count = target_unknown_count;
				priority_direction.all = 0;				//他の方向はいらないのでリセット
				priority_direction.element.up = 1;
			}

		}
		if (check_move_by_step(now_x, now_y, MUKI_DOWN)) {			//下
			next_direction.element.down = 1;

			target_unknown_count = count_unknown_wall(now_x, (now_y - 1));
			if (target_unknown_count == max_unknown_count) {
				priority_direction.element.down = 1;
			} else if (target_unknown_count >= max_unknown_count) {
				max_unknown_count = target_unknown_count;
				priority_direction.all = 0;				//他の方向はいらないのでリセット
				priority_direction.element.down = 1;
			}

		}

//未探索区間が候補の中にあるなら、次に行く方向はその中から選ぶ
		if (priority_direction.all != 0) {
			next_direction.all = priority_direction.all;
		}

//next_dirrctionから次行く方向を選び、行動する
		next_action = get_next_action(next_direction);

		//古川法
		if (photo::check_wall(MUKI_UP) == false) {		//前に壁がなければ
			//前方に見てない壁があれば
			if (count_unknown_wall((now_x + direction_x), (now_y + direction_y))
					> 0) {
				next_action = go_straight;
			}
		}

		run_next_action_slalom(next_action, true);

//もし止まるべきと出たならココで足立法をやめる
		if (next_action == stop) {
			adachi_flag = false;
		}

//方向更新
		mouse::set_direction(direction_x, direction_y);
	}

	if (adachi_flag) {
		//足立法が成功したのでマップを保存する
		map::output_map_data(&mouse::now_map);
		return true;				//足立法完了!!

	} else {
//ここに来るということは足立法が失敗してる
		motor::sleep_motor();
//TODO わかりやすい何かが欲しい
		error();
		myprintf("Adachi method failed!\n\r");
		if (mouse::get_fail_flag()) {
			myprintf("fail safe!\n\r");
		}

		myprintf("now -> (%d,%d)\n\r", mouse::get_x_position(),
				mouse::get_y_position());
	}

	return false;
}

adachi::adachi() {
}

adachi::~adachi() {

}

