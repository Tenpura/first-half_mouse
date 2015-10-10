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

unsigned short gyro::get_gyro() {
	return gyro_value;
}

float gyro::get_gyro_ref() {
	long gyro_sum = 0;
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
	least_square_slope += (float) ((LEAST_SQUARE_TIME * xy_sum - x_sum * y_sum)
			/ (LEAST_SQUARE_TIME * x_square_sum - x_sum * x_sum));

//	least_square_slope *= 1.5;
}

void gyro::set_gyro_ref() {
	gyro_ref = get_gyro_ref();

	set_least_square_slope();	//最小二乗法による補正項導出
	mouse::reset_count();
}

void gyro::reset_angle() {
	default_angle = 0;
	before_angle = 0;
	angle = 0;
}

void gyro::cal_angle() {
	before_angle = angle;
	default_angle += (gyro_value - gyro_ref) * GYRO_CONST * LOGIK_V / 4096;
	//最小二乗法で補正する
	angle = default_angle
			- (least_square_slope * (float) mouse::get_count_ms());
}

float gyro::get_angle() {
	return angle;	//°
}

void gyro::cal_angular_velocity() {
	angular_velocity = radian((get_angle() - before_angle) * 1000);
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

void motor::set_duty_left(const signed char set_duty) {
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

void motor::set_duty_right(const signed char set_duty) {
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
}

void motor::stanby_motor() {
	PORTB.DR.BIT.B0 = 1;	//モータードライバースタンバイ
}

motor::motor() {
}

motor::~motor() {

}

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
	//LEDが光っているなら消えてるときとの差分を取る
	if (on_light) {
		switch (sensor_type) {
		case right:
			right_ad = (ad_convert_an102() - right_ref);
			break;

		case left:
			left_ad = (ad_convert_an002() - left_ref);
			break;

		case front_right:
			front_right_ad = (ad_convert_an103() - front_right_ref);
			break;

		case front_left:
			front_left_ad = (ad_convert_an001() - front_left);
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

unsigned int photo::get_ad(PHOTO_TYPE sensor_type) {
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
const PID gyro_gain = { 5, 70, 0 };
const PID photo_gain = { 0.00000001, 0, 0.0 };
const PID encoder_gain = { 150, 5000, 0 };

PID control::gyro_delta, control::photo_delta, control::encoder_delta;
bool control::control_phase = false;
bool control::wall_control_flag = false;

float control::cross_delta_gain(SEN_TYPE sensor) {
	switch (sensor) {
	case sen_gyro:
		return (gyro_delta.P * gyro_gain.P + gyro_delta.I * gyro_gain.I
				+ gyro_delta.D * gyro_gain.D);

	case sen_encoder:
		return (encoder_delta.P * encoder_gain.P
				+ encoder_delta.I * encoder_gain.I
				+ encoder_delta.D * encoder_gain.D);

	case sen_photo:
		return ((float) photo_delta.P * photo_gain.P
				+ photo_delta.I * photo_gain.I + photo_delta.D * photo_gain.D);
	}
	return 0;
}

void control::cal_delta() {
	float before_p_delta;
	volatile float photo_right_delta = 0, photo_left_delta = 0;
	static float right_before, left_before;
	float right_now, left_now;

	//エンコーダーのΔ計算
	before_p_delta = encoder_delta.P;	//積分用
	encoder_delta.P = (mouse::get_ideal_velocity() - encoder::get_velocity());
	encoder_delta.I += (encoder_delta.P * CONTROL_PERIOD);
	//encoder_delta.D = (encoder_delta.P - before_p_delta) * 1000;

	//センサーのΔ計算
	before_p_delta = photo_delta.P;
	left_now = photo::get_ad(left);
	right_now = (float) photo::get_ad(right);

	//速度が低いと制御が効きすぎるので（相対的に制御が大きくなる）、切る
	if (encoder::get_velocity() <= (SEARCH_VELOCITY * 0.8)) {
		photo_left_delta = 0;
		photo_right_delta = 0;

	} else {

		if (photo::check_wall(MUKI_RIGHT)) {		//右壁がある
			//壁の切れ目に吸い込まれないように
			/*
			 if (ABS(right_now - right_before) > 20000) {
			 photo_right_delta = 0;
			 } else {
			 */
			photo_right_delta = (parameter::get_ideal_photo(right)
					- photo::get_ad(right));
			//}

			if (photo::check_wall(MUKI_LEFT)) {		//両壁がある
				//壁の切れ目に吸い込まれないように
				if (ABS(left_now - left_before) > 20000) {
					photo_left_delta = 0;
				} else {
					photo_left_delta = (parameter::get_ideal_photo(left)
							- photo::get_ad(left));
				}
			} else {
				//片方のときは、壁のある方を2倍かけることで疑似的に両壁アリと同じ制御量にする
				photo_left_delta = 0;
				photo_right_delta = 2
						* (parameter::get_ideal_photo(right)
								- photo::get_ad(right));

			}
		} else {			//右がない
			photo_right_delta = 0;

			if (photo::check_wall(MUKI_LEFT)) {		//左だけある
				//壁の切れ目に吸い込まれないように
				if (ABS(left_now - left_before) > 20000) {
					photo_left_delta = 0;
				} else {
					photo_left_delta = 2
							* (parameter::get_ideal_photo(left)
									- photo::get_ad(left));
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

	//XXX ジャイロが少し左に向くので、右向きは強めに制御をかける
	if (photo_right_delta < 0) {
		photo_right_delta *= 2;
	}

	photo_delta.P = (-photo_right_delta + photo_left_delta);
	photo_delta.I += (photo_delta.P * CONTROL_PERIOD);
	//photo_delta.D = (photo_delta.P - before_p_delta) * 1000;

//ジャイロのΔ計算
	before_p_delta = gyro_delta.P;	//積分用
	gyro_delta.P = (mouse::get_ideal_angular_velocity()
			- gyro::get_angular_velocity());
	gyro_delta.I += (gyro_delta.P * CONTROL_PERIOD);
//gyro_delta.D = (gyro_delta.P - before_p_delta) * 1000;

}

float control::control_velocity() {
	return cross_delta_gain(sen_encoder);
}

float control::control_angular_velocity() {
	if (wall_control_flag) {
		return (cross_delta_gain(sen_gyro) + cross_delta_gain(sen_photo));
	} else {
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
				- (mouse::get_ideal_angular_velocity() * TREAD_W / 2 / 1000);

	} else {									//左のモーターなら
		velocity = mouse::get_ideal_velocity()
				+ (mouse::get_ideal_angular_velocity() * TREAD_W / 2 / 1000);

	}
	return (velocity / (2 * PI() * TIRE_R / 1000) * SPAR / PINION * M_SUM_ORM
			/ MOTOR_ORM / MOTOR_CONST / get_battery()) * 100;
}

void control::posture_control() {
	if (get_control_phase()) {			//姿勢制御を掛けるなら
		if ((FF_SWITCH == OFF) && (FB_SWITCH == OFF)) {		//両方制御を掛けないとき
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
	if (ABS(encoder_delta.P) > 0.7) {
		motor::sleep_motor();
		mouse::set_fail_flag(true);
	}
}

control::control() {
}

control::~control() {

}
