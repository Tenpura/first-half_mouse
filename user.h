/*
 * user.h
 *
 *  Created on: 2015/06/14
 *      Author: Koizumi
 */

#ifndef USER_H_
#define USER_H_

#include"extern_c.h"
#include"define.h"
#include"hardware.h"

//Errorのときはこの関数に飛ばす
void error();

//角度変換関数
float degree(float radian);		//ラジアン	->	度数		-π≦θ≦π
float radian(float degree);		//度数	->	ラジアン	-180≦θ≦180

//wait関数
void wait_ms(const unsigned int wait_ms_count);				//ms(ミリ秒)単位で待つ関数。
void set_count_ms(const unsigned long set_ms_value);		//msをカウントする変数に代入。
unsigned long get_count_ms();			//msのカウントを返す。
unsigned long wait_package(const unsigned char, const unsigned long);//上3つの関数の実態はココ。表からは見えない

//mode選択クラス
class mode {
private:
	mode();

public:
	//今はスイッチで切替、muki方向のセンサに反応させながらスイッチが決定
	static unsigned char select_mode(const unsigned char mode_number, const unsigned char muki);//mode選択し、選んだモード番号を返す

	static bool search_mode();		//探索モード
	static bool shortest_mode();	//最短モード

	~mode();
};

//logクラス	いつかはtemplateで任意の型に対応させたい…!

class int_log {
private:
	static signed int log_case[2][INT_LOG_COUNT];
	static unsigned int log_counter;
	int init_number;

public:
	void reset_log();		//logの中身を消去。全て0にする。

	static void put_log(const int put_number);//logの最後尾に引数を格納。既に埋まっていれば何もしない
	static void put_log(const int put_number,const int put_number2);//上と同じ。ただし2つ保存可

	int get_log(const unsigned int get_number);//get_number番目に格納されてるlogを取り出す
	int get_log(const unsigned int ordinal_number,const unsigned int get_number);//上と同じ。ただし2つ保存可

	int_log();
	int_log(int initialize_number);//もし初期化する数を0以外にしたかったら、ここで変更してね
	~int_log();
};

class float_log {
private:
	static float log_case[2][LOG_COUNT];
	static unsigned int log_counter;
	static float init_number;

public:
	static void reset_log();		//logの中身を消去。全て0にする。

	//logの最後尾に引数を格納。既に埋まっていれば何もしない
	//a,bは多次元配列用。1次元の場合はbは無視される。
	static void put_log(const float put_number_a,const float put_number_b);

	//get_dimensionの次元のget_number番目に格納されてるlogを取り出す
	float get_log(const unsigned char get_dimension, const unsigned int get_number);

	float_log();
	float_log(float initialize_number);//もし初期化する数を0以外にしたかったら、ここで変更してね
	~float_log();
};


//数学クラス
class my_math{
private:
	const static float sin_table[92];		//[]の要素がdegreeに対応。念のため92個

public:
	static float sin(const float radian);
	static float cos(const float radian);


};
#endif /* USER_H_ */
