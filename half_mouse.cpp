/***************************************************************/
/*                                                             */
/*      PROJECT NAME :  half_mouse                             */
/*      FILE         :  half_mouse.cpp                         */
/*      DESCRIPTION  :  Main Program                           */
/*      CPU SERIES   :  RX600                                  */
/*      CPU TYPE     :  RX62T                                  */
/*                                                             */
/*      This file is generated by e2 studio.                   */
/*                                                             */
/***************************************************************/

/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c or Main.cpp                                    */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

#include"iodefine.h"
#include"extern_c.h"
#include"define.h"
#include"init.h"
#include"ad_convert.h"
#include"serial.h"
#include"speaker.h"
#include"hardware.h"
#include"user.h"
#include"run.h"
#include"map.h"

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

// 半音		オクターブ		拍数			音階
MELODY dorakue_noroi[] = { { 0, 0, QUATER_NOTE, MY_SI }, { 0, 1, QUATER_NOTE,
MY_SI }, { FLAT, 0, QUATER_NOTE, MY_SI }, { FLAT, 1, QUATER_NOTE, MY_SI }, { 0,
		0, QUATER_NOTE, MY_SI }, { 0, 1,
QUATER_NOTE, MY_SI }, { FLAT, 0, QUATER_NOTE, MY_SI }, { FLAT, 1, QUATER_NOTE,
MY_SI }, { 0, 0, QUATER_NOTE, MY_SI }, { 0, 1,
QUATER_NOTE, MY_SI }, { FLAT, 0, QUATER_NOTE, MY_SI }, { FLAT, 1, QUATER_NOTE,
MY_SI }, { 0, 0, QUATER_NOTE, MY_SI }, { 0, 1,
QUATER_NOTE, MY_SI }, { FLAT, 0, QUATER_NOTE, MY_SI }, { FLAT, 1, QUATER_NOTE,
MY_SI }, { 0, 1, QUATER_NOTE, MY_MI }, { 0, 1,
HALF_NOTE, MY_MI }, { 0, 1, SIXTEEN_NOTE, MY_SO }, { FLAT, 1,
SIXTEEN_NOTE, MY_LA }, { FLAT, 0, QUATER_NOTE, MY_SI }, {
MELODY_END, 0, 0, 0 } };

void interrupt_cmt0();	//割り込み
void interrupt_cmt1();	//1ms、センサー用

void interrupt_mtu0_tgid0();	//mtu割り込み(モーター)

void main(void) {
	Init_ALL();
	mouse::reset_count();

	my7seg::blink(8, 250, 5);
	float_log float_log;

//	sound_melody(dorakue_noroi,-2,300);

	myprintf("\n\n\n");
	myprintf("Hello!!\n\r");
	myprintf("V_dd = %f\n\r", get_battery());
	myprintf("\n\n\n");

	if (get_battery() < 7.6) {
		error();
	}

	my7seg::light(1);
	gyro::set_gyro_ref();
	my7seg::turn_off();

	map::reset_wall();

	/*
	 motor::stanby_motor();

	 mouse::reset_angle();
	 mouse::set_ideal_velocity(0);
	 mouse::set_ideal_angular_velocity(0);
	 control::reset_delta(sen_all);

	 control::start_control();

	 */

	while ((SWITCH_RIGHT == OFF) && (SWITCH_LEFT == OFF)) {	//押されてなければ待機

		myprintf("left %f, right %f\n\r", encoder::left_velocity,
				encoder::right_velocity);

		/*
		 myprintf("r %d , l %d , fr %d , fl %d\n\r", photo::get_ad(right),
		 photo::get_ad(left), photo::get_ad(front_right),
		 photo::get_ad(front_left));
		 */
		wait_ms(100);

	}

	motor::sleep_motor();

	char select_mode = mode::select_mode(6, MUKI_UP);
	carcuit::set_run_mode(select_mode);

	while ((SWITCH_RIGHT == OFF) && (SWITCH_LEFT == OFF)) {	//押されてなければ待機
		my7seg::light(select_mode);

	}

	my7seg::turn_off();

//	gyro::set_gyro_ref();

	switch (select_mode) {
	case 0:
		mouse::set_fail_flag(false);

		motor::stanby_motor();

		mouse::reset_angle();
		mouse::set_ideal_velocity(0);
		mouse::set_ideal_angular_velocity(0);
		control::reset_delta(sen_all);

		control::start_control();
		my7seg::count_down(3, 500);

		control::start_wall_control();

		mouse::set_distance_m(0);

		run::accel_run(0.09, SEARCH_VELOCITY, 0);
		float_log.reset_log();
		run::slalom(small, MUKI_RIGHT, 0);
		run::accel_run(0.09, 0, 0);
		wait_ms(1000);

		break;

	case 1:
		my7seg::turn_off();
		wait_ms(1000);
		mouse::set_position(0, 0);
		mouse::set_direction(MUKI_UP);
		mode::search_mode();
		break;

	case 2:
		mouse::set_fail_flag(false);

		motor::stanby_motor();

		mouse::reset_angle();
		mouse::set_ideal_velocity(0);
		mouse::set_ideal_angular_velocity(0);
		control::reset_delta(sen_all);

		control::start_control();
		my7seg::count_down(3, 500);

		control::start_wall_control();

		mouse::set_distance_m(0);

		my7seg::turn_off();
		//run::accel_run(0.09, SEARCH_VELOCITY, 0);
		float_log.reset_log();
		//run::spin_turn(90);
		//run::slalom_by_sin(small, MUKI_LEFT, 0);
		run::accel_run(0.18*5, 0, 0);
		wait_ms(100);
		break;

	case 3:
		for (int i = 0; i < 360; i++) {
			myprintf("%d -> %f \n\r", i,
					9.8 * 9.8 * 3 * my_math::sin(3 * 9.8 * i * CONTROL_PERIOD)
							/ 2);
			wait_ms(10);
		}
		break;
	case 4:
		my7seg::count_down(3, 500);
		motor::stanby_motor();
		float_log.reset_log();
		left_hand::run2(3,3);
		wait_ms(1000);
		break;

	case 5:
		wait_ms(1000);

		mouse::set_fail_flag(false);

		motor::stanby_motor();

		mouse::reset_angle();
		mouse::set_ideal_velocity(0);
		mouse::set_ideal_angular_velocity(0);
		control::reset_delta(sen_all);

		control::start_control();
		my7seg::count_down(3, 500);
/*
		while (1) {
		}
*/
		control::start_wall_control();

		float_log.reset_log();

		mouse::set_distance_m(0);
		//run::accel_run(0.18 * 5, 0, 0);
		run::spin_turn(90);
	}

	wait_ms(1000);
	/*

	 motor::stanby_motor();

	 mouse::reset_angle();
	 mouse::set_ideal_velocity(0);
	 mouse::set_ideal_angular_velocity(0);
	 control::reset_delta(sen_all);

	 control::start_control();
	 my7seg::count_down(3, 500);

	 control::start_wall_control();

	 float_log.reset_log();

	 mouse::set_distance_m(0);

	 //run::accel_run(0.18, SEARCH_VELOCITY, 0);
	 run::accel_run(0.18, 0, 0);

	 //	run::accel_run(0.18 * 15, 0, select_mode);
	 */

	//adachi::run_next_action(stop);
	motor::sleep_motor();
	wait_ms(1000);

	my7seg::light(8);

	while ((SWITCH_RIGHT == OFF) && (SWITCH_LEFT == OFF)) {	//押されてなければ待機
	}

	for (int i = 0; i < LOG_COUNT; i++) {
		myprintf("%f  %f \n\r", float_log.get_log(0, i),
				float_log.get_log(1, i));
	}

}

void interrupt_cmt0() {
	MSTP( CMT0 ) = 0;		// 　CMT0スタンバイ解除
	set_count_ms(get_count_ms() + 1);		//ms(ミリ秒)のカウントを1増加
	mouse::add_one_count_ms();

	gyro::set_gyro();				//gyroの値を取得
	gyro::cal_angle();				//gyroから角度を計算
	gyro::cal_angular_velocity();	//gyroから角速度を計算[°/s]

	encoder::cal_encoder();

	mouse::cal_accel();
	mouse::cal_distance();

	control::cal_delta();			//姿勢制御に用いる偏差を計算
	control::posture_control();

	motor::mtu_set();	//mtuをdutyに応じて変更

	control::fail_safe();

	float_log::put_log(mouse::get_distance_m(),photo::get_ad(right));
			//gyro::get_angular_velocity(),mouse::get_ideal_angular_velocity());

	}

void interrupt_cmt1() {
	MSTP( CMT1 ) = 0;		// 　CMT1スタンバイ解除
	const static int wait_number = 1000;
	photo::turn_off_all();

//TODO LEDモード選択で光らないモードを作る！

	photo::set_ad(right, false);
	photo::light(right);
	for (int i = 0; i < wait_number; i++) {
	}
	photo::set_ad(right, true);
	photo::turn_off(right);

	photo::set_ad(left, false);
	photo::light(left);
	for (int i = 0; i < wait_number; i++) {
	}
	photo::set_ad(left, true);
	photo::turn_off(left);

	photo::set_ad(front_right, false);
	photo::light(front_right);
	for (int i = 0; i < wait_number; i++) {
	}
	photo::set_ad(front_right, true);
	photo::turn_off(front_right);

	photo::set_ad(front_left, false);
	photo::light(front_left);
	for (int i = 0; i < wait_number; i++) {
	}
	photo::set_ad(front_left, true);
	photo::turn_off(front_left);

}

void interrupt_mtu0_tgid0() {
	MSTP( MTU0 ) = 0;		//スタンバイ解除

}

#ifdef __cplusplus
void abort(void) {

}
#endif
