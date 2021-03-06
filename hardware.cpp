/*
 * hardware.cpp
 *
 *  Created on: 2015/06/13
 *      Author: Koizumi
 */

#include"hardware.h"

//自作7セグ関連

void my7seg::light(const unsigned char number) {

	//7セグで表示できるわけねえだろ！
	if (number > 9) {
		error();
		myprintf("my7segで1桁以上の数字を光らせようとしています!!\n\r");
		return;
	}
	switch (number) {

	case 0:
		PORT7.DR.BIT.B1 = 0;	//LED5
		PORT7.DR.BIT.B2 = 0;	//LED4
		PORT7.DR.BIT.B3 = 0;	//LED2
		PORT7.DR.BIT.B4 = 0;	//LED6
		PORT2.DR.BIT.B2 = 0;	//LED1
		PORTA.DR.BIT.B3 = 1;	//LED3
		PORTA.DR.BIT.B4 = 0;	//LED8
		PORTB.DR.BIT.B2 = 0;	//LED7
		break;
	case 1:
		PORT7.DR.BIT.B1 = 0;	//LED5
		PORT7.DR.BIT.B2 = 0;	//LED4
		PORT7.DR.BIT.B3 = 1;	//LED2
		PORT7.DR.BIT.B4 = 1;	//LED6
		PORT2.DR.BIT.B2 = 1;	//LED1
		PORTA.DR.BIT.B3 = 1;	//LED3
		PORTA.DR.BIT.B4 = 1;	//LED8
		PORTB.DR.BIT.B2 = 1;	//LED7
		break;
	case 2:
		PORT7.DR.BIT.B1 = 0;	//LED5
		PORT7.DR.BIT.B2 = 1;	//LED4
		PORT7.DR.BIT.B3 = 0;	//LED2
		PORT7.DR.BIT.B4 = 0;	//LED6
		PORT2.DR.BIT.B2 = 0;	//LED1
		PORTA.DR.BIT.B3 = 0;	//LED3
		PORTA.DR.BIT.B4 = 0;	//LED8
		PORTB.DR.BIT.B2 = 1;	//LED7
		break;
	case 3:
		PORT7.DR.BIT.B1 = 0;	//LED5
		PORT7.DR.BIT.B2 = 0;	//LED4
		PORT7.DR.BIT.B3 = 0;	//LED2
		PORT7.DR.BIT.B4 = 1;	//LED6
		PORT2.DR.BIT.B2 = 0;	//LED1
		PORTA.DR.BIT.B3 = 0;	//LED3
		PORTA.DR.BIT.B4 = 0;	//LED8
		PORTB.DR.BIT.B2 = 1;	//LED7
		break;
	case 4:
		PORT7.DR.BIT.B1 = 0;	//LED5
		PORT7.DR.BIT.B2 = 0;	//LED4
		PORT7.DR.BIT.B3 = 1;	//LED2
		PORT7.DR.BIT.B4 = 1;	//LED6
		PORT2.DR.BIT.B2 = 0;	//LED1
		PORTA.DR.BIT.B3 = 0;	//LED3
		PORTA.DR.BIT.B4 = 1;	//LED8
		PORTB.DR.BIT.B2 = 0;	//LED7
		break;
	case 5:
		PORT7.DR.BIT.B1 = 1;	//LED5
		PORT7.DR.BIT.B2 = 0;	//LED4
		PORT7.DR.BIT.B3 = 0;	//LED2
		PORT7.DR.BIT.B4 = 1;	//LED6
		PORT2.DR.BIT.B2 = 0;	//LED1
		PORTA.DR.BIT.B3 = 0;	//LED3
		PORTA.DR.BIT.B4 = 0;	//LED8
		PORTB.DR.BIT.B2 = 0;	//LED7
		break;
	case 6:
		PORT7.DR.BIT.B1 = 1;	//LED5
		PORT7.DR.BIT.B2 = 0;	//LED4
		PORT7.DR.BIT.B3 = 0;	//LED2
		PORT7.DR.BIT.B4 = 0;	//LED6
		PORT2.DR.BIT.B2 = 0;	//LED1
		PORTA.DR.BIT.B3 = 0;	//LED3
		PORTA.DR.BIT.B4 = 0;	//LED8
		PORTB.DR.BIT.B2 = 0;	//LED7
		break;
	case 7:
		PORT7.DR.BIT.B1 = 0;	//LED5
		PORT7.DR.BIT.B2 = 0;	//LED4
		PORT7.DR.BIT.B3 = 1;	//LED2
		PORT7.DR.BIT.B4 = 1;	//LED6
		PORT2.DR.BIT.B2 = 1;	//LED1
		PORTA.DR.BIT.B3 = 1;	//LED3
		PORTA.DR.BIT.B4 = 0;	//LED8
		PORTB.DR.BIT.B2 = 0;	//LED7
		break;
	case 8:
		PORT7.DR.BIT.B1 = 0;	//LED5
		PORT7.DR.BIT.B2 = 0;	//LED4
		PORT7.DR.BIT.B3 = 0;	//LED2
		PORT7.DR.BIT.B4 = 0;	//LED6
		PORT2.DR.BIT.B2 = 0;	//LED1
		PORTA.DR.BIT.B3 = 0;	//LED3
		PORTA.DR.BIT.B4 = 0;	//LED8
		PORTB.DR.BIT.B2 = 0;	//LED7
		break;
	case 9:
		PORT7.DR.BIT.B1 = 0;	//LED5
		PORT7.DR.BIT.B2 = 0;	//LED4
		PORT7.DR.BIT.B3 = 0;	//LED2
		PORT7.DR.BIT.B4 = 1;	//LED6
		PORT2.DR.BIT.B2 = 0;	//LED1
		PORTA.DR.BIT.B3 = 0;	//LED3
		PORTA.DR.BIT.B4 = 0;	//LED8
		PORTB.DR.BIT.B2 = 0;	//LED7
		break;
	}
}

void my7seg::turn_off() {
	PORT7.DR.BIT.B1 = 1;	//LED5
	PORT7.DR.BIT.B2 = 1;	//LED4
	PORT7.DR.BIT.B3 = 1;	//LED2
	PORT7.DR.BIT.B4 = 1;	//LED6
	PORT2.DR.BIT.B2 = 1;	//LED1
	PORTA.DR.BIT.B3 = 1;	//LED3
	PORTA.DR.BIT.B4 = 1;	//LED8
	PORTB.DR.BIT.B2 = 1;	//LED7
}

void my7seg::light_error() {
	PORT7.DR.BIT.B1 = 1;	//LED5
	PORT7.DR.BIT.B2 = 1;	//LED4
	PORT7.DR.BIT.B3 = 0;	//LED2
	PORT7.DR.BIT.B4 = 0;	//LED6
	PORT2.DR.BIT.B2 = 0;	//LED1
	PORTA.DR.BIT.B3 = 0;	//LED3
	PORTA.DR.BIT.B4 = 0;	//LED8
	PORTB.DR.BIT.B2 = 0;	//LED7
}

void my7seg::blink(const unsigned char number,
		const unsigned short blink_cycle_ms,
		const unsigned short repeat_count) {
	for (int i = 0; i < repeat_count; i++) {
		light(number);
		wait_ms(blink_cycle_ms);
		turn_off();
		wait_ms(blink_cycle_ms);
	}
}

void my7seg::count_down(const unsigned char start_number,
		const unsigned short cycle_ms) {
	for (int i = start_number; i > 0; i--) {
		light(i);
		wait_ms(cycle_ms);
	}
	light(0);
}

//gyro関連

unsigned short gyro::gyro_value;
float gyro::default_angle;
float gyro::angle, gyro::before_angle, gyro::gyro_ref;
float gyro::angular_velocity;
float gyro::least_square_slope = 0;

void gyro::set_gyro() {
	gyro_value = ad_convert_gyro();
}

float gyro::get_gyro() {
	return (float) gyro_value;
}

float gyro::get_gyro_ref() {
	float gyro_sum = 0;
	for (int i = 0; i < 1000; i++) {
		gyro_sum += get_gyro();
		wait_ms(1);
	}

	return gyro_sum / 1000;
}

void gyro::set_least_square_slope() {
	//x,yとかはWikiの最小二乗法と同じ。xは時間[ms],yは角度[°]
	float x_square_sum = 0, x_sum = 0, y_sum = 0, xy_sum = 0;
	float temp_count = 0;
	float temp_gyro;

	mouse::reset_count();

	while (mouse::get_count_ms() < LEAST_SQUARE_TIME) {
		temp_count = (float) mouse::get_count_ms();
		temp_gyro = gyro::get_angle();

		y_sum += temp_gyro;
		x_sum += temp_count;
		x_square_sum += (temp_count * temp_count);
		xy_sum += (temp_count * temp_gyro);

		//1ms経つまで待機
		while (temp_count >= mouse::get_count_ms()) {
		}
	}

	if ((x_square_sum - x_sum * x_sum) == 0) {
		least_square_slope = 0;
		return;
	}
	least_square_slope = (float) ((LEAST_SQUARE_TIME * xy_sum - x_sum * y_sum)
			/ (LEAST_SQUARE_TIME * x_square_sum - x_sum * x_sum));

	//least_square_slope /= 2;

}

void gyro::set_gyro_ref() {
	gyro_ref = get_gyro_ref();

	set_least_square_slope();	//最小二乗法による補正項導出
	mouse::reset_count();
	mouse::reset_angle();
}

void gyro::reset_angle() {
	default_angle = 0;
	before_angle = 0;
	angle = 0;
}

void gyro::cal_angle() {
	before_angle = angle;
	//右回りのとき
	if ((get_gyro() - gyro_ref) > 0) {
		default_angle += (get_gyro() - gyro_ref) * GYRO_CONST_RIGHT * LOGIK_V
				/ 4096;
	} else {
		default_angle += (get_gyro() - gyro_ref) * GYRO_CONST_LEFT * LOGIK_V
				/ 4096;
	}
	//最小二乗法で補正する
	angle = default_angle
			- (least_square_slope * (float) mouse::get_count_ms());
}

float gyro::get_angle() {
	return angle;	//°
}

void gyro::set_angle(float set_degree) {
	angle = set_degree;
}

void gyro::cal_angular_velocity() {
	float before_angular_velocity = angular_velocity;
	angular_velocity = radian((get_angle() - before_angle) * 1000);

	//FIXME 対処療法。スラローム中あまりに激しく変化する場合は切る
	if (mouse::slalom_flag) {
		if (ABS(angular_velocity - before_angular_velocity) > 5) {
			angular_velocity = before_angular_velocity;
		}
	}
}

float gyro::get_angular_velocity() {
	return angular_velocity;	//[rad/s]
}

gyro::gyro() {

}

gyro::~gyro() {

}

//encoder関連
float encoder::left_velocity, encoder::right_velocity, encoder::velocity;

void encoder::cal_encoder() {
	static float sample_data[MOVING_AVERAGE] = { 0 };	//データを保存しておく配列
	float sum = 0;

	left_velocity = ((MTU2.TCNT - 32762) * ENCODER_CONST * TIRE_R);	//m/s
	right_velocity = ((MTU1.TCNT - 32762) * ENCODER_CONST * TIRE_R);//count*[rad/count]/[sec]*[m]
	MTU1.TCNT = 32762;
	MTU2.TCNT = 32762;

	for (signed char i = 0; (i + 1) < MOVING_AVERAGE; i++) {
		sample_data[i + 1] = sample_data[i];	//配列を1つずらす
		sum += sample_data[i + 1];			//ついでに加算する
	}
	sample_data[0] = (right_velocity + left_velocity) / 2;	//配列の最初に入れる
	sum += sample_data[0];

	velocity = (sum / MOVING_AVERAGE);
}

float encoder::get_velocity() {

	return velocity;
}

encoder::encoder() {
}

encoder::~encoder() {

}

//motor関連
signed char motor::right_duty = 0, motor::left_duty = 0;

void motor::set_duty_left(const signed short set_duty) {
	//上限を切る
	if (set_duty > MAX_DUTY) {
		left_duty = MAX_DUTY;
	} else if (set_duty < -MAX_DUTY) {
		left_duty = - MAX_DUTY;
	}

	if (set_duty == 0) {					//0のときはstop
		PORTB.DR.BIT.B4 = 0;	//モーターA
		PORTB.DR.BIT.B5 = 0;	//モーターA
		PORT9.DR.BIT.B4 = 0;

	} else if (set_duty > 0) {				//正のときは正転
		PORTB.DR.BIT.B4 = 0;	//モーターA
		PORTB.DR.BIT.B5 = 1;	//モーターA
		PORT9.DR.BIT.B4 = 1;

	} else {									//負のときは逆転
		PORTB.DR.BIT.B4 = 1;	//モーターA
		PORTB.DR.BIT.B5 = 0;	//モーターA
		PORT9.DR.BIT.B4 = 0;
	}

	left_duty = set_duty;

}

signed short motor::get_duty_left() {
	return left_duty;
}

void motor::set_duty_right(const signed short set_duty) {
	//上限を切る
	if (set_duty > MAX_DUTY) {
		right_duty = MAX_DUTY;
	} else if (set_duty < -MAX_DUTY) {
		right_duty = -MAX_DUTY;
	}

	if (set_duty == 0) {					//0のときはstop
		PORTB.DR.BIT.B6 = 0;	//モーターB
		PORTB.DR.BIT.B7 = 0;	//モーターB

	} else if (set_duty > 0) {				//正のときは正転
		PORTB.DR.BIT.B6 = 0;	//モーターB
		PORTB.DR.BIT.B7 = 1;	//モーターB

	} else {									//負のときは逆転
		PORTB.DR.BIT.B6 = 1;	//モーターB
		PORTB.DR.BIT.B7 = 0;	//モーターB
	}

	right_duty = set_duty;

}

signed short motor::get_duty_right() {
	return right_duty;
}

void motor::mtu_set() {
	MTU0.TGRA = MTU0.TGRB * ABS(get_duty_left()) / 100;	//Duty変更
	MTU0.TGRC = MTU0.TGRD * ABS(get_duty_right()) / 100;		//Duty変更
}

void motor::sleep_motor() {
	PORTB.DR.BIT.B0 = 0;	//モータードライバースリープ
	motor::set_duty_right(0);
	motor::set_duty_left(0);

}

void motor::stanby_motor() {
	PORTB.DR.BIT.B0 = 1;	//モータードライバースタンバイ
}

motor::motor() {
}

motor::~motor() {

}

signed int photo::right_delta, photo::left_delta, photo::front_right_delta,
		photo::front_left_delta;
signed int photo::right_ad, photo::left_ad, photo::front_right_ad,
		photo::front_left_ad;
signed int photo::right_ref, photo::left_ref, photo::front_right_ref,
		photo::front_left_ref;

bool photo::light_flag = false;

void photo::switch_led(PHOTO_TYPE sensor_type, unsigned char one_or_zero) {
	switch (sensor_type) {
	case right:
		PORTA.DR.BIT.B5 = one_or_zero;
		break;

	case left:
		PORT9.DR.BIT.B1 = one_or_zero;
		break;

	case front_right:
		PORT1.DR.BIT.B0 = one_or_zero;
		break;

	case front_left:
		PORT9.DR.BIT.B2 = one_or_zero;
		break;
	}
}

void photo::light(PHOTO_TYPE sensor_type) {
	switch_led(sensor_type, 1);
}

void photo::turn_off(PHOTO_TYPE sensor_type) {
	switch_led(sensor_type, 0);
}

void photo::turn_off_all() {
	turn_off(right);
	turn_off(left);
	turn_off(front_right);
	turn_off(front_left);

}

void photo::set_ad(PHOTO_TYPE sensor_type, bool on_light) {
	signed int temp_ad;

	//LEDが光っているなら消えてるときとの差分を取る
	if (on_light) {
		switch (sensor_type) {
		case right:
			temp_ad = right_ad;
			right_ad = (ad_convert_an102() - right_ref);
			right_delta = right_ad - temp_ad;
			break;

		case left:
			temp_ad = left_ad;
			left_ad = (ad_convert_an002() - left_ref);
			left_delta = left_ad - temp_ad;
			break;

		case front_right:
			temp_ad = front_right_ad;
			front_right_ad = (ad_convert_an103() - front_right_ref);
			front_right_delta = front_right_ad - temp_ad;
			break;

		case front_left:
			temp_ad = front_left_ad;
			front_left_ad = (ad_convert_an001() - front_left);
			front_left_delta = front_left_ad - temp_ad;
			break;
		}

		//消えてるなら基準値として記録
	} else {
		switch (sensor_type) {
		case right:
			right_ref = ad_convert_an102();
			break;

		case left:
			left_ref = ad_convert_an002();
			break;

		case front_right:
			front_right_ref = ad_convert_an103();
			break;

		case front_left:
			front_left_ref = ad_convert_an001();
			break;
		}
	}
}

signed int photo::get_ad(PHOTO_TYPE sensor_type) {
	switch (sensor_type) {
	case right:
		return right_ad;
		break;

	case left:
		return left_ad;
		break;

	case front_right:
		return front_right_ad;
		break;

	case front_left:
		return front_left_ad;
		break;
	}

	return 0;
}

signed int photo::get_ad_delta(PHOTO_TYPE sensor_type) {
	switch (sensor_type) {
	case right:
		return right_delta;
		break;

	case left:
		return left_delta;
		break;

	case front_right:
		return front_right_delta;
		break;

	case front_left:
		return front_left_delta;
		break;
	}

	return 0;
}

bool photo::check_wall(unsigned char muki) {

	switch (muki) {
	case MUKI_RIGHT:
		if (right_ad >= parameter::get_min_wall_photo(right)) {
			return true;
		}
		return false;

	case MUKI_LEFT:
		if (left_ad >= parameter::get_min_wall_photo(left)) {
			return true;
		}
		return false;

	case MUKI_UP:
		if ((front_right_ad >= parameter::get_min_wall_photo(front_right))
				|| (front_left_ad >= parameter::get_min_wall_photo(front_left))) {
			return true;
		}
		return false;
	}

	return false;

}

photo::photo() {
}

photo::~photo() {
}

//XXX 各種ゲイン
//control関連
const PID gyro_gain = { 40, 150, 0 };
const PID photo_gain = { 0.05, 0.01, 0.0 };
const PID encoder_gain = { 280, 18000, 0 };

const PID angle_gain = { 0, 0, 0 };		//角度に対するゲイン　Pゲインは角速度のIゲインと同じなので0にしとく

const PID gyro_gain_straight = { 24, 3.0, 0 };

PID control::gyro_delta, control::photo_delta, control::encoder_delta;
PID control::angle_delta;
bool control::control_phase = false;
bool control::wall_control_flag = false;

float control::cross_delta_gain(SEN_TYPE sensor) {
	switch (sensor) {
	case sen_gyro:
		if (mouse::slalom_flag) {
			return (gyro_delta.P * gyro_gain.P + gyro_delta.I * gyro_gain.I
					+ gyro_delta.D * gyro_gain.D + angle_delta.I * angle_gain.I);
		} else {
			return (gyro_delta.P * gyro_gain_straight.P
					+ gyro_delta.I * gyro_gain_straight.I
					+ gyro_delta.D * gyro_gain_straight.D);
		}
		break;

	case sen_encoder:
		return (encoder_delta.P * encoder_gain.P
				+ encoder_delta.I * encoder_gain.I
				+ encoder_delta.D * encoder_gain.D);
		break;

	case sen_photo:
		return ((float) photo_delta.P * photo_gain.P
				+ photo_delta.I * photo_gain.I + photo_delta.D * photo_gain.D);
		break;
	}
	return 0;
}

void control::cal_delta() {
	const static char left_wall_brake = 4;	//壁の切れ目を判別するための閾値
	const static char right_wall_brake = 3;

	float before_p_delta;
	volatile float photo_right_delta = 0, photo_left_delta = 0;
	static float right_before, left_before;
	float right_now, left_now;
	static float right_ideal = (float) parameter::get_ideal_photo(right),
			left_ideal = (float) parameter::get_ideal_photo(left);

	if (get_control_phase() == false) {			//姿勢制御を掛けないなら計算しない
		return;
	}

	//エンコーダーのΔ計算
	before_p_delta = encoder_delta.P;	//積分用
	encoder_delta.P = (mouse::get_ideal_velocity() - encoder::get_velocity());
	encoder_delta.I += (encoder_delta.P * CONTROL_PERIOD);
	//encoder_delta.D = (encoder_delta.P - before_p_delta) * 1000;

	//センサーのΔ計算
	before_p_delta = photo_delta.P;
	left_now = (float) photo::get_ad(left);
	right_now = (float) photo::get_ad(right);

	//速度が低いと制御が効きすぎるので（相対的に制御が大きくなる）、切る
	if (encoder::get_velocity() <= (SEARCH_VELOCITY / MOUSE_MODE * 0.8)) {
		photo_left_delta = 0;
		photo_right_delta = 0;

	} else if ((photo::get_ad(front_right)
			>= parameter::get_ideal_photo(front_right))) {
		//前壁が近いときは横のセンサが前壁を横壁と誤認する可能性あり
		photo_left_delta = 0;
		photo_right_delta = 0;

	} else {

		if (photo::check_wall(MUKI_RIGHT)) {		//右壁がある
			//壁の切れ目に吸い込まれないように
			if ((right_now - right_before) > right_wall_brake) {
				photo_right_delta = 0;
			} else {
				photo_right_delta = (right_ideal - right_now);
			}

			if (photo::check_wall(MUKI_LEFT)) {		//両壁がある
				//壁の切れ目に吸い込まれないように
				if ((left_now - left_before) > left_wall_brake) {
					photo_left_delta = 0;
				} else {
					photo_left_delta = (left_ideal - left_now);
				}
			} else {
				//片方のときは、壁のある方を2倍かけることで疑似的に両壁アリと同じ制御量にする
				photo_left_delta = 0;
				photo_right_delta = 2 * (right_ideal - right_now);

			}
		} else {			//右がない
			photo_right_delta = 0;

			if (photo::check_wall(MUKI_LEFT)) {		//左だけある
				//壁の切れ目に吸い込まれないように
				if ((left_now - left_before) > left_wall_brake) {
					photo_left_delta = 0;
				} else {
					photo_left_delta = 2 * (left_ideal - left_now);
				}
			} else {
				//両方ない
				photo_left_delta = 0;
				photo_right_delta = 0;
			}
		}
	}

	//一個前の値を保存
	right_before = right_now;
	left_before = left_now;

	photo_delta.P = (photo_right_delta - photo_left_delta);
	photo_delta.I += (photo_delta.P * CONTROL_PERIOD);
	//photo_delta.D = (photo_delta.P - before_p_delta) * 1000;

//ジャイロのΔ計算
	before_p_delta = gyro_delta.P;	//積分用
	gyro_delta.P = (mouse::get_ideal_angular_velocity()
			- gyro::get_angular_velocity());
	gyro_delta.I += (gyro_delta.P * CONTROL_PERIOD);
	//gyro_delta.D = (gyro_delta.P - before_p_delta) * 1000;
	angle_delta.P = gyro_delta.I;
	angle_delta.I += (angle_delta.P * CONTROL_PERIOD);

}

float control::control_velocity() {
	return cross_delta_gain(sen_encoder);
}

float control::control_angular_velocity() {
	if (mouse::back_run_flag) {
		photo_delta.I = 0;
		return cross_delta_gain(sen_gyro);
	}

	if (wall_control_flag) {
		if (photo_delta.P == 0) {
			//何もしない
			//photo_delta.I = 0;
		} else {
			gyro_delta.I = 0;
			angle_delta.I = 0;
		}
		return (cross_delta_gain(sen_gyro) + cross_delta_gain(sen_photo));

	} else {
		photo_delta.I = 0;
		return cross_delta_gain(sen_gyro);
	}
}

void control::start_wall_control() {
	wall_control_flag = true;
}

void control::stop_wall_control() {
	wall_control_flag = false;
}

void control::start_control() {
	control_phase = true;
}

void control::stop_control() {
	control_phase = false;
}

float control::get_feedback(signed char right_or_left) {
	if (right_or_left == MUKI_RIGHT) {			//右のモーターなら
		return (control_velocity() - control_angular_velocity());

	} else {										//左のモーターなら
		return (control_velocity() + control_angular_velocity());

	}
}

float control::get_feedforward(const signed char right_or_left) {
	float velocity;		//目標速度

	if (right_or_left == MUKI_RIGHT) {			//右のモーターなら
		velocity = mouse::get_ideal_velocity()
				- (mouse::get_ideal_angular_velocity() * TREAD_W
						* CONTROL_PERIOD / 2);

	} else {									//左のモーターなら
		velocity = mouse::get_ideal_velocity()
				+ (mouse::get_ideal_angular_velocity() * TREAD_W
						* CONTROL_PERIOD / 2);

	}
	return (velocity / (2 * PI() * TIRE_R / 1000) * SPAR / PINION * M_SUM_ORM
			/ MOTOR_ORM / MOTOR_CONST / get_battery()) * 100;
}

void control::posture_control() {
	if (get_control_phase()) {			//姿勢制御を掛けるなら
		if ((FF_SWITCH == OFF) && (FB_SWITCH == OFF)) {			//両方制御を掛けないとき
			return;
		}

		float duty_r = 0, duty_l = 0;		//dutyを保存する

		if (FF_SWITCH == ON) {			//FF制御を掛けるなら
			duty_r = get_feedforward(MUKI_RIGHT);
			duty_l = get_feedforward(MUKI_LEFT);
		}
		if (FB_SWITCH == ON) {				//FB制御を掛けるなら
			duty_r += get_feedback(MUKI_RIGHT);
			duty_l += get_feedback(MUKI_LEFT);
		}

		motor::set_duty_right((int) duty_r);
		motor::set_duty_left((int) duty_l);
	}

	return;
}

bool control::get_control_phase() {
	return control_phase;
}

void control::reset_delta(SEN_TYPE sensor_type) {
	switch (sensor_type) {
	case sen_gyro:
		gyro_delta.P = 0;
		gyro_delta.I = 0;
		gyro_delta.D = 0;
		angle_delta.P = 0;
		angle_delta.I = 0;
		break;

	case sen_photo:
		photo_delta.P = 0;
		photo_delta.I = 0;
		photo_delta.D = 0;
		break;

	case sen_encoder:
		encoder_delta.P = 0;
		encoder_delta.I = 0;
		encoder_delta.D = 0;
		break;

	case sen_all:
		gyro_delta.P = 0;
		gyro_delta.I = 0;
		gyro_delta.D = 0;

		angle_delta.P = 0;
		angle_delta.I = 0;

		photo_delta.P = 0;
		photo_delta.I = 0;
		photo_delta.D = 0;
		encoder_delta.P = 0;
		encoder_delta.I = 0;
		encoder_delta.D = 0;
		break;
	}
}

void control::fail_safe() {
//TODO 閾値どのくらいかわからない。Gyroも参照すべき？
	if (ABS(encoder_delta.P) > 0.8) {
		motor::sleep_motor();
		control::stop_control();
		mouse::set_fail_flag(true);
	}
}

control::control() {
}

control::~control() {

}
