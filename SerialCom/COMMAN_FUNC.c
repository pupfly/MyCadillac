#include "COMMON_FUNC.h"
#include <reg52.h>

unsigned char code num2char[] = "0123456789";
static bit received = 0;
static bit time_up = 0;
static count = 0;//计数变量
static bit connect_flag = 0;
static bit auto_renew = 0;

extern void delay_ms(unsigned int time)
{
	int i, j;
	for(i = 0;i < time;i++)
		for(j = 0;j < 124;j++);
}

extern void delay_10us()
{
	unsigned char i;
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;
}

//为串口通信设置的处理交互式通信标志函数
extern bit get_response_flag()
{
	return received;
}
extern void set_response_flag(bit value)
{
	received = value;
}

//定时相关函数，3s
extern void set_timeup_30s(bit value)
{
	time_up = value;
}
extern bit is_timeup_30s()
{
	return time_up;
}

//连接状态
extern void set_connected(bit value)
{
	connect_flag = value;
}
extern bit is_connected()
{
	return connect_flag;
}

//自动更新状态
extern void set_auto_renew(bit value)
{
	auto_renew = value;
}

extern bit get_auto_renew()
{
	return auto_renew;
}