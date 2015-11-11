/*
 * parameter.h
 *
 *  Created on: 2015/09/09
 *      Author: Koizumi
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include"define.h"

//パラメータ
#define TIRE_R	11.9		//タイヤの半径[mm]
#define TREAD_W	44		//トレッド幅[mm]

#define WALL_BRAKE 10	//1msでこれだけセンサー値が変化したら壁の切れ目
#define WALL_OFF_DISTANCE_RIGHT 0.083		//壁切れ検知からどれだけ進むか[m]
#define WALL_OFF_DISTANCE_LEFT 0.08		//壁切れ検知からどれだけ進むか[m]

#define MOVING_AVERAGE		3						//encoderでいくつの移動平均をとるか

#define LEAST_SQUARE_TIME	2000		//ジャイロの最小二乗法を何ms出かけるか

#define LOG_DIMENSION	2	//何次元配列を用意するか
#define LOG_COUNT 750	//LOGをいくつとるか。最低1、0にはしないこと

#define INT_LOG_COUNT	1	//Int型のLogをいくつとるか　最低1
#define MAX_DUTY	70	//motorのduty制限[%]

#define FB_SWITCH	ON	//フィードバック制御を入れるか入れないか
#define FF_SWITCH	OFF	//フィードフォワードを入れるか入れないか

#define MOUSE_MODE	1	//1がクラシック,2がハーフ

#define SEARCH_VELOCITY	0.5	//探索の速度[m/s]

#include"user.h"
#include"serial.h"

#define RUN_MODE_NUMBER	5	//走行モードを何種類用意するか

class parameter {
private:
	const static signed int ideal_photo[2][4]; //ideal_photo[クラシックorハーフ][光学センサの向き]
	const static signed int min_wall_photo[2][4];//壁がある時の最小値[クラシックorハーフ][光学センサの向き]

	//クラシック用パラメータ
	const static TRAPEZOID straight_run[RUN_MODE_NUMBER];
	const static INIT_SLALOM right_slalom[slalom_type_count][RUN_MODE_NUMBER], left_slalom[slalom_type_count][RUN_MODE_NUMBER];

	//ハーフ用パラメータ
	const static TRAPEZOID straight_run_half[RUN_MODE_NUMBER];
	const static INIT_SLALOM right_slalom_half[slalom_type_count][RUN_MODE_NUMBER], left_slalom_half[slalom_type_count][RUN_MODE_NUMBER];

public:
	static signed int get_ideal_photo(const PHOTO_TYPE photo_type);
	static signed int get_min_wall_photo(const PHOTO_TYPE photo_type);

	static float get_run_acceleration(const unsigned char select_mode);
	static float get_run_max_velocity(const unsigned char select_mode);
	static float get_run_de_acceleration(const unsigned char select_mode);

	static float get_slalom(const SLALOM_TYPE slalom_type, const SLALOM_ELEMENT slalom_element,
			const signed char right_or_left, const unsigned char select_mode);

};

#endif /* PARAMETER_H_ */
