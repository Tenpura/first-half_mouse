/*
 * run.h
 *
 *  Created on: 2015/09/07
 *      Author: Koizumi
 */

#ifndef RUN_H_
#define RUN_H_

#include"define.h"
#include"hardware.h"
#include"parameter.h"
#include"map.h"
#include"user.h"

//position
typedef union {
	unsigned char all;
	struct {
		unsigned char x :4;
		unsigned char y :4;
	} bit;
} POSITION;

class mouse {
private:
	static float ideal_acceleration;			//マウスの加速度[m/s^2]
	static float ideal_angular_acceleration;//マウスの加角速度[rad/s^2]

	static float ideal_velocity;//マウスの目標速度[m/s]
	static float ideal_angular_velocity;//マウスの目標角速度[rad/s]

	static float run_distance;//マウスの走行距離[m]

	static unsigned long mouse_count_ms;//マウスの時間[msec].基本リセットしない

	static bool fail_flag;//フェイルセーフがかかるとこのフラグがTRUEに

	static POSITION position;//マウスの位置（座標）
	static unsigned char mouse_direction;//マウスの向き

	mouse();

public:

	static bool slalom_flag;
	static MAP_DATA now_map;

	static void reset_count();
	static void add_one_count_ms();//カウントに1加算。割り込み内のみで呼び出す
	static unsigned long get_count_ms();//カウント取得。単位はms

	static float get_acceleration();
	static void set_acceleration(const float set_value_m_ss);

	static float get_angular_acceleration();
	static void set_angular_acceleration(const float set_value_m_ss);

	static float get_ideal_velocity();
	static void set_ideal_velocity(const float set_value_m_s);

	static float get_ideal_angular_velocity();
	static void set_ideal_angular_velocity(const float set_value_rad_s);

	static float get_angle_degree();
	static void reset_angle();

	static float get_distance_m();//距離を取得
	static void set_distance_m(const float set_value_m);//距離をセット

	static unsigned char get_x_position();
	static unsigned char get_y_position();
	static void set_position(const unsigned char x, const unsigned char y);

	static void turn_direction(const unsigned char right_or_left);
	static unsigned char get_direction();//defineされたMUKI_??で返す
	static void get_direction(signed char *direction_x, signed char *direction_y);//引数の値をポインタ経由で変更する
	static void set_direction(const unsigned char muki);
	static void set_direction(const signed char direction_x, const signed char direction_y);

	static bool get_fail_flag();//フェイルセーフがかかったかどうかの判別用
	static void set_fail_flag(bool set_flag);

	static void cal_accel();//加速を行う。速度の加減算
	static void cal_distance();//距離計算

	//壁を見て、壁が存在するなら壁を作り、ないなら壊す。見たことも記録
	//マウスの向きや座標も内部できちんと考える
	static void look_wall(bool comb_ignore);//壁を見る関数

	~mouse();

};

class run {
private:
	run();

public:

	static void fit_stop(const float distance_m, const float select_mode, const float fit_velocity);	//袋小路でちょうど半分で止まるための関数

	static void back_run(const float distance_m, const float end_velocity, const unsigned char select_mode);

	static void accel_run(const float distance_m, const float end_velocity, const unsigned char select_mode);
	//壁切れを目標距離-check_distanceの距離から見始める
	static void accel_run_wall_off(const float distance_m, const float end_velocity, const unsigned char select_mode, const float check_distance);

	static void slalom(const SLALOM_TYPE slalom_type, const signed char right_or_left, const unsigned char select_mode);
	static void spin_turn(const float target_degree);
	static void slalom_by_sin(const SLALOM_TYPE slalom_type, const signed char right_or_left, const unsigned char select_mode);

	static void path(const float finish_velocity, const unsigned char run_mode);

	~run();

};

//ACTION_TYPE 足立法のとき次に取る行動の種類
typedef enum {
	go_straight, turn_right, turn_left, back, stop
} ACTION_TYPE;

class left_hand {
private:

public:

	//引数に応じて次の行動をマウスが実行する（実際に動く部分）区画の中心から中心へ
	static void run_next_action(ACTION_TYPE next_action);

	static bool run(const unsigned char target_x, const unsigned char target_y);
	static bool run2(const unsigned char target_x, const unsigned char target_y);

	left_hand();
	~left_hand();

};

class adachi {
private:
	static signed char direction_x, direction_y;

	static void set_direction();

	//現在の座標と向きを与えると歩数的にそのマスに進めるかを判別
	static bool check_move_by_step(unsigned char target_x, unsigned char target_y, unsigned char muki);

	//目標のマスのまだ見てない壁の数を数え上げる
	static unsigned int count_unknown_wall(unsigned char target_x, unsigned char target_y);

	//次行く方向を与えると、次に取る行動を返す。優先度は、直進、右ターン、左ターン、バックの順
	static ACTION_TYPE get_next_action(DIRECTION next_direction);

	//引数に応じて次の行動をマウスが実行する（実際に動く部分）
	static void run_next_action(ACTION_TYPE next_action, bool wall_off);
	static void run_next_action_slalom(ACTION_TYPE next_action, bool wall_off);

	adachi();

public:

	static bool adachi_method(const unsigned char target_x, const unsigned char target_y);
	static bool adachi_method_wall_off(const unsigned char target_x, const unsigned char target_y);
	~adachi();

};

class carcuit {
private:
	carcuit();

public:
	static unsigned char run_mode;
	static void set_run_mode(const unsigned char select_mode);		//パラメータ0は超信地
	static void run_carcuit(const unsigned char maze_x_size,const unsigned char maze_y_sizeconst, const unsigned char cycle_count,bool wall_off);

	~carcuit();
};

#endif /* RUN_H_ */
