/*
 * parameter.cpp
 *
 *  Created on: 2015/09/09
 *      Author: Koizumi
 */

#include"parameter.h"
//ideal_photo[x][y]	xは0がクラシック,1がハーフ	yが光学センサの向きに対応。
//right left front_right front_left
const signed int parameter::ideal_photo[2][4] = { { 3100, 3200, 3000, 1100 }, {
		2600, 2500, 3000, 1100 } };
const signed int parameter::min_wall_photo[2][4] = {
		{ 2300, 2500, 1300, 40000 }, { 3100, 2400, 2000, 400000 } };

//0番目は探索用
const TRAPEZOID parameter::straight_run[RUN_MODE_NUMBER] = { { 2.0,
SEARCH_VELOCITY, 2.0 }, { 2.0, 1.0, 2.0 }, { 2.0, 2.0, 2.0 }, { 3.0, 2.0, 3.0 },
		{ 5.0, 3.0, 5.0 } };

//velocity,target_angle,clotoid_angle,before,after,angular_accel,max_angular_velocity,time
const INIT_SLALOM parameter::right_slalom[slalom_type_count][RUN_MODE_NUMBER] =
		{ { { 0 } },	//none
				{ { SEARCH_VELOCITY, 90, 30, 0.02, 0.025, 121.389, 11.274, 0 },
						{ SEARCH_VELOCITY, 90, 30, 0.02, 0.025, 121.389, 11.274,
								0 }, { SEARCH_VELOCITY, 90, 30, 0.02, 0.025,
								121.389, 11.274, 0 }, { SEARCH_VELOCITY, 90, 30,
								0.02, 0.025, 121.389, 11.274, 0 }, {
								SEARCH_VELOCITY, 90, 30, 0.02, 0.025, 121.389,
								11.274, 0 } },	//small
				{ { 0.85, 90, 30, 0.02, 0.02, 67.148, 8.3855, 0 }, { 0 } },	//big_90
				{ { 0.85, 180, 60, 0.02, 0.02, 52.1571, 10.4517, 0 } } //big_180
		};
const INIT_SLALOM parameter::left_slalom[slalom_type_count][RUN_MODE_NUMBER] = {
		{ { 0 } },	//none
		{ { SEARCH_VELOCITY, 90, 30, 0.02, 0.035, 121.389, 11.274, 0 }, {
				SEARCH_VELOCITY, 90, 30, 0.02, 0.035, 121.389, 11.274, 0 }, {
				SEARCH_VELOCITY, 90, 30, 0.02, 0.035, 121.389, 11.274, 0 }, {
				SEARCH_VELOCITY, 90, 30, 0.02, 0.035, 121.389, 11.274, 0 }, {
				SEARCH_VELOCITY, 90, 30, 0.02, 0.035, 121.389, 11.274, 0 } },//small
		{ { 0.85, 90, 30, 0.02, 0.02, 67.148, 8.3855, 0 } },	//big_90
		{ { 0.85, 180, 60, 0.02, 0.02, 52.1571, 10.4517, 0 } } //big_180
};

//ハーフパラメーター
//0番目は探索用
const TRAPEZOID parameter::straight_run_half[RUN_MODE_NUMBER] = { { 2.0,
SEARCH_VELOCITY / 2, 2.0 }, { 2.0, 2.0, 2.0 }, { 2.0, 1.5, 2.0 }, { 2.0, 2.0,
		2.0 }, { 3.0, 2.0, 3.0 } };

//velocity,target_angle,clotoid_angle,before,after,angular_accel,time
const INIT_SLALOM parameter::right_slalom_half[slalom_type_count][RUN_MODE_NUMBER] =
		{ { { 0 } },	//none
				{ { SEARCH_VELOCITY / 2, 90, 30, 0.005, 0.01, 92.386, 0 } },//small
				{ { 0 }	//big_90
				} };
const INIT_SLALOM parameter::left_slalom_half[slalom_type_count][RUN_MODE_NUMBER] =
		{ { { 0 } },	//none
				{ { SEARCH_VELOCITY / 2, 90, 30, 0.005, 0.01, 92.386, 0 } },//small
		};

float parameter::get_run_acceleration(const unsigned char select_mode) {
	if (select_mode >= RUN_MODE_NUMBER) {			//存在しないモードを選択したらエラー
		error();
		myprintf("存在しない走行モード[%d]の加速度を選択しています!!", select_mode);
		return 0;
	}

	if (MOUSE_MODE == 1) {	//クラシックなら
		return straight_run[select_mode].acceleration;

	} else {				//ハーフなら
		return straight_run_half[select_mode].acceleration;
	}
}

signed int parameter::get_ideal_photo(PHOTO_TYPE photo_type) {
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

signed int parameter::get_min_wall_photo(PHOTO_TYPE photo_type) {
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
	if (MOUSE_MODE == 1) {	//クラシックなら
		return straight_run[select_mode].max_velocity;

	} else {				//ハーフなら
		return straight_run_half[select_mode].max_velocity;
	}
}

float parameter::get_run_de_acceleration(const unsigned char select_mode) {
	if (select_mode >= RUN_MODE_NUMBER) {			//存在しないモードを選択したらエラー
		error();
		myprintf("存在しない走行モード[%d]の減速度を選択しています!!", select_mode);
		return 0;
	}
	if (MOUSE_MODE == 1) {	//クラシックなら
		return straight_run[select_mode].de_acceleration;

	} else {				//ハーフなら
		return straight_run_half[select_mode].de_acceleration;
	}
}

float parameter::get_slalom(const SLALOM_TYPE slalom_type,
		const SLALOM_ELEMENT slalom_element, const signed char right_or_left,
		const unsigned char select_mode) {

	if (select_mode >= RUN_MODE_NUMBER) {			//存在しないモードを選択したらエラー
		error();
		myprintf("存在しない走行モード[%d]のスラローム要素を参照しています!!", select_mode);
		return 0;
	}

//クラシックなら
	if (MOUSE_MODE == 1) {
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

		case max_angular_velocity:
			if (right_or_left == MUKI_RIGHT) {
				return right_slalom[slalom_type][select_mode].max_angular_velocity;
			} else {
				return left_slalom[slalom_type][select_mode].max_angular_velocity;
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

		//ハーフなら
	} else {
		switch (slalom_element) {
		case velocity:
			if (right_or_left == MUKI_RIGHT) {
				return right_slalom_half[slalom_type][select_mode].velocity;
			} else {
				return left_slalom_half[slalom_type][select_mode].velocity;
			}
			break;

		case target_angle:
			if (right_or_left == MUKI_RIGHT) {
				return right_slalom_half[slalom_type][select_mode].target_angle;
			} else {
				return left_slalom_half[slalom_type][select_mode].target_angle;
			}
			break;

		case clothoid_angle:
			if (right_or_left == MUKI_RIGHT) {
				return right_slalom_half[slalom_type][select_mode].clothoid_angle;
			} else {
				return left_slalom_half[slalom_type][select_mode].clothoid_angle;
			}
			break;

		case before_distance:
			if (right_or_left == MUKI_RIGHT) {
				return right_slalom_half[slalom_type][select_mode].before_distance;
			} else {
				return left_slalom_half[slalom_type][select_mode].before_distance;
			}
			break;

		case after_distance:
			if (right_or_left == MUKI_RIGHT) {
				return right_slalom_half[slalom_type][select_mode].after_distance;
			} else {
				return left_slalom_half[slalom_type][select_mode].after_distance;
			}
			break;

		case angular_accel:
			if (right_or_left == MUKI_RIGHT) {
				return right_slalom_half[slalom_type][select_mode].angular_accel;
			} else {
				return left_slalom_half[slalom_type][select_mode].angular_accel;
			}
			break;

		case max_angular_velocity:
			if (right_or_left == MUKI_RIGHT) {
				return right_slalom_half[slalom_type][select_mode].max_angular_velocity;
			} else {
				return left_slalom_half[slalom_type][select_mode].max_angular_velocity;
			}
			break;

		case time:
			if (right_or_left == MUKI_RIGHT) {
				return right_slalom_half[slalom_type][select_mode].time;
			} else {
				return left_slalom_half[slalom_type][select_mode].time;
			}
			break;
		}

	}

	return 0;
}

