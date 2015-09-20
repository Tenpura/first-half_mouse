/*
 * extern_c.h
 *
 *  Created on: 2015/05/30
 *      Author: Koizumi
 */

#ifndef EXTERN_C_H_
#define EXTERN_C_H_

#ifdef __cplusplus		//コンパイラがC++のとき

//serial.h
extern "C" void put1byte(char c);
extern "C" void putnbyte(char *buf, int len);
extern "C" int myprintf(const char *fmt, ...);

//init.h
extern "C" void Init_ALL(void);
extern "C" void Init_MTU ( void );
extern "C" void Init_CMT0 ( void );
extern "C" void Init_SCI(void);
extern "C" void Init_SYSTEM(void);
extern "C" void Init_AD(void);

//ad_convert.h
extern "C" unsigned int ad_convert_an000();
extern "C" unsigned int ad_convert_an001();
extern "C" unsigned int ad_convert_an002();
extern "C" unsigned int ad_convert_an003();
extern "C" unsigned int ad_convert_an100();
extern "C" unsigned int ad_convert_an101();
extern "C" unsigned int ad_convert_an102();
extern "C" unsigned int ad_convert_an103();
extern "C" float get_battery();
extern "C" unsigned int ad_convert_gyro();
extern "C" unsigned int ad_convert_gyro_ref();

//割り込み
extern "C" void interrupt_cmt0();
extern "C" void interrupt_cmt1();
extern "C" void interrupt_mtu0_tgid0();
extern "C" void interrupt_speaker();

//ユーザー関数
extern "C" void wait_ms(unsigned int wait_ms_count);//ms(ミリ秒)単位で待つ関数。
extern "C" void set_count_ms(unsigned long set_ms_value);//msをカウントする変数に代入。
extern "C" unsigned long get_count_ms();//msのカウントを返す。

extern "C" void sound_bz(unsigned int Hz, unsigned int bz_miri_second);


#endif

#endif /* EXTERN_C_H_ */
