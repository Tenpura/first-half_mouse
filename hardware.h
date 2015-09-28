/*
 * hardware.h
 *
 *  Created on: 2015/06/14
 *      Author: Koizumi
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include"iodefine.h"
#include"define.h"
#include"extern_c.h"
#include"parameter.h"
#include"ad_convert.h"
#include"user.h"
#include"run.h"

//自作7セグ関連
class my7seg {
private:
public:
	static void light(const unsigned char number);		//つける
	static void turn_off();//消す

	static void light_error();//Eと光らせる

	//repeat_count回だけ点滅する。消えてる状態で終わる
	static void blink(const unsigned char number, const unsigned short blink_cycle_ms, const unsigned short repeat_count);

	//start_numberから0までカウントダウン
	static void count_down(const unsigned char start_number, const unsigned short cycle_ms);

};

//gyro関連
//時計回りが正
class gyro {
private:
	static unsigned short gyro_value;
	static float default_angle;				//最小二乗法で補正をかけてない角度
	static float angle,before_angle,gyro_ref;
	static float angular_velocity;

	static float get_gyro_ref();
	static void set_least_square_slope();//最小二乗法をまわして補正項を計算

	gyro();

public:
	static float least_square_slope;//補正項の傾き

	static void set_gyro();
	static unsigned short get_gyro();
	static void set_gyro_ref();

	static void reset_angle();
	static void cal_angle();//角度計算[°]
	static float get_angle();

	static void cal_angular_velocity();//角速度計算[rad/s]
	static float get_angular_velocity();

	~gyro();

};

//encoder関連

class encoder {
private:
	static float right_velocity,left_velocity,velocity;
	encoder();

public:
	static void cal_encoder();		//モーターのEncoderの値計算

	static float get_velocity();//左右の平均(重心速度)のEncoder取得[m/s]　 移動平均取ってることに注意！

	~encoder();
};

//motor関連

class motor {
private:
	static signed char right_duty,left_duty;

	static signed short get_duty_left();	//左モーターのDuty取得
	static signed short get_duty_right();//右モーターのDuty取得

	motor();

public:
	static void set_duty_left(const signed char set_duty);//左モーターのDuty決定
	static void set_duty_right(const signed char set_duty);//右モーターのDuty決定

	static void sleep_motor();//モータードライバをスリープ状態に
	static void stanby_motor();//モータードライバをスタンバイ状態に

	static void mtu_set();//MTUのパルスを決定

	~motor();
};

//光学センサー関連



class photo {
private:
	static signed int right_ad, left_ad, front_right_ad, front_left_ad;
	static signed int right_ref, left_ref, front_right_ref, front_left_ref;
	static void switch_led(PHOTO_TYPE sensor_type, unsigned char one_or_zero);
	static bool light_flag;		//赤外線LEDを光らせてセンサー値を読むかどうかのフラグ

	photo();

public:

	static void light(PHOTO_TYPE sensor_type);
	static void turn_off(PHOTO_TYPE sensor_type);
	static void turn_off_all();		//すべて消す

	//TODO boolで引数にしなくても、関数内で判別してもいいかも
	//消えてれば基準値として、光っていればその差分を記録する
	static void set_ad(PHOTO_TYPE sensor_type, bool on_light);

	static unsigned int get_ad(PHOTO_TYPE sensor_type);

	//TODO この関数はマウスclassにあるべきかも
	static bool check_wall(unsigned char muki);


	~photo();

};

//それぞれのセンサから制御をかけるクラス

//PIDのゲインと偏差を管理する用の構造体
typedef struct {
	float P;
	float I;
	float D;
} PID;
//センサータイプを表す列挙型
enum SEN_TYPE {
	sen_gyro = 0, sen_encoder = 1, sen_photo = 2
};

class control {
private:
	//TODO D項はいらないらしいハセシュン曰く
	static float cross_delta_gain(SEN_TYPE sensor);//P_GAIN*P_DELTA+・・・を行う

	static bool wall_control_flag;//壁制御をかけてればtrue、切ってればfalse。
	static bool control_phase;//姿勢制御をかけてるか否か。かけていればtrue

	static float control_velocity();//速度に関するPID制御(エンコーダーのみ)。戻り値はDuty？
	static float control_angular_velocity();//速度に関するPID制御(エンコーダーのみ)。戻り値はDuty？

	static float get_feedback(const signed char right_or_left);//FBを掛けた後のDutyを返す。PID制御。
	static float get_feedforward(const signed char right_or_left);//FFを掛けた後のDutyを返す。

	control();

public:
	//デバック時に参照できるようpublicに置いてある
	static PID gyro_delta,photo_delta,encoder_delta;//各種Δ

	static void cal_delta();//割り込み関数内で、偏差を計算する

	static void posture_control();//FF,FB制御をかける。set_dutyまで行う。

	static void start_wall_control();//壁制御をかける
	static void stop_wall_control();//壁制御を切る

	static void start_control();//制御をかける
	static void stop_control();//制御をとめる

	static bool get_control_phase();//姿勢制御がかかっているかを取得。かかっていればtrue

	static void reset_delta();

	static void fail_safe();//Iゲインが一定以上いったらモーターを止める

	~control();
};

#endif /* HARDWARE_H_ */
