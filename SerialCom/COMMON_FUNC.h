#ifndef _COMMON_FUNC_
#define _COMMON_FUNC_

typedef unsigned char uchar;
typedef unsigned int uint;
//LCD数字
extern unsigned char code num2char[10];

extern void delay_ms(unsigned int);
extern void delay_10us();
//为串口通信设置的处理交互式通信交互标志函数
extern bit get_response_flag();
extern void set_response_flag(bit);
//定时相关函数，30s
extern void set_timeup_30s(bit);
extern bit is_timeup_30s();
//连接状态
extern void set_connected(bit);
extern bit is_connected();
//自动更新状态
extern void set_auto_renew(bit);
extern bit get_auto_renew();
#endif