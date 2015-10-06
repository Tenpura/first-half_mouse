/*
 * parameter.cpp
 *
 *  Created on: 2015/09/09
 *      Author: Koizumi
 */

#include"parameter.h"
//ideal_photo[x][y]	xは0がクラシック,1がハーフ	yが光学センサの向きに対応。
//right left front_right front_left
const unsigned int parameter::ideal_photo[2][4] = { { 3300, 2800, 1815, 1100 },
		{ 100, 100, 100, 100 } };
const unsigned int parameter::min_wall_photo[2][4] = { { 2000, 1700, 0, 0 }, {
		0, 0, 0, 0 } };

//0番目は探索用
const TRAPEZOID parameter::straight_run[RUN_MODE_NUMBER] = { { 2.0,
SEARCH_VELOCITY, 2.0 }, { 2.0, 1.0, 2.0 }, { 2.0, 1.5, 2.0 }, { 2.0, 2.0, 2.0 },
		{ 3.0, 2.0, 3.0 } };

//velocity,target_angle,clotoid_angle,before,after,angular_accel,time
const INIT_SLALOM parameter::right_slalom[slalom_type_count][RUN_MODE_NUMBER] =
		{ { { 0 } },	//none
				{ { 0.5, 90, 30, 0, 0, 3, 0 } },	//small

				{ { 0 }, { 0.7, 90, 30, 0.12, 0.04, 36.5, 0 }, { 0.7, 90, 30,
						0.12, 0.04, 36.5, 0 }, { 0.7, 90, 30, 0.12, 0.04, 36.5,
						0 }, { 0.7, 90, 30, 0.12, 0.04, 36.5, 0 }	//big_90

				} };
const INIT_SLALOM parameter::left_slalom[slalom_type_count][RUN_MODE_NUMBER] = {
		0 };

float parameter::get_run_acceleration(const unsigned char select_mode) {
	if (select_mode >= RUN_MODE_NUMBER) {			//存在しないモードを選択したらエラー
		error();
		myprintf("存在しない走行モード[%d]の加速度を選択しています!!", select_mode);
		return 0;
	}
	return straight_run[select_mode].acceleration;
}

unsigned int parameter::get_ideal_photo(PHOTO_TYPE photo_type) {
	switch (photo_type) {
	case right:
		return ideal_photo[MOUSE_MODE - 1][right];
		break;

	case left:
		return ideal_photo[MOUSE_MODE - 1][left];
		break;

	case front_right:
		return ideal_photo[MOUSE_MODE - 1][front_right];
		break;

	case front_left:
		return ideal_photo[MOUSE_MODE - 1][front_left];
		break;
	}

	return 0;
}

unsigned int parameter::get_min_wall_photo(PHOTO_TYPE photo_type) {
	switch (photo_type) {
	case right:
		return min_wall_photo[MOUSE_MODE - 1][right];
		break;

	case left:
		return min_wall_photo[MOUSE_MODE - 1][left];
		break;

	case front_right:
		return min_wall_photo[MOUSE_MODE - 1][front_right];
		break;

	case front_left:
		return min_wall_photo[MOUSE_MODE - 1][front_left];
		break;
	}

	return 0;
}

float parameter::get_run_max_velocity(const unsigned char select_mode) {
	if (select_mode >= RUN_MODE_NUMBER) {			//存在しないモードを選択したらエラー
		error();
		myprintf("存在しない走行モード[%d]の最高速度を選択しています!!", select_mode);
		return 0;
	}
	return straight_run[select_mode].max_velocity;
}

float parameter::get_run_de_acceleration(const unsigned char select_mode) {
	if (select_mode >= RUN_MODE_NUMBER) {			//存在しないモードを選択したらエラー
		error();
		myprintf("存在しない走行モード[%d]の減速度を選択しています!!", select_mode);
		return 0;
	}
	return straight_run[select_mode].de_acceleration;
}

float parameter::get_slalom(const SLALOM_TYPE slalom_type,
		const SLALOM_ELEMENT slalom_element, const signed char right_or_left,
		const unsigned char select_mode) {

	if (select_mode >= RUN_MODE_NUMBER) {			//存在しないモードを選択したらエラー
		error();
		myprintf("存在しない走行モード[%d]のスラローム要素を参照しています!!", select_mode);
		return 0;
	}

	switch (slalom_element) {
	case velocity:
		if (right_or_left == MUKI_RIGHT) {
			return right_slalom[slalom_type][select_mode].velocity;
		} else {
			return left_slalom[slalom_type][select_mode].velocity;
		}
		break;

	case target_angle:
		if (right_or_left == MUKI_RIGHT) {
			return right_slalom[slalom_type][select_mode].target_angle;
		} else {
			return left_slalom[slalom_type][select_mode].target_angle;
		}
		break;

	case clothoid_angle:
		if (right_or_left == MUKI_RIGHT) {
			return right_slalom[slalom_type][select_mode].clothoid_angle;
		} else {
			return left_slalom[slalom_type][select_mode].clothoid_angle;
		}
		break;

	case before_distance:
		if (right_or_left == MUKI_RIGHT) {
			return right_slalom[slalom_type][select_mode].before_distance;
		} else {
			return left_slalom[slalom_type][select_mode].before_distance;
		}
		break;

	case after_distance:
		if (right_or_left == MUKI_RIGHT) {
			return right_slalom[slalom_type][select_mode].after_distance;
		} else {
			return left_slalom[slalom_type][select_mode].after_distance;
		}
		break;

	case angular_accel:
		if (right_or_left == MUKI_RIGHT) {
			return right_slalom[slalom_type][select_mode].angular_accel;
		} else {
			return left_slalom[slalom_type][select_mode].angular_accel;
		}
		break;

	case time:
		if (right_or_left == MUKI_RIGHT) {
			return right_slalom[slalom_type][select_mode].time;
		} else {
			return left_slalom[slalom_type][select_mode].time;
		}
		break;
	}

	return 0;
}

