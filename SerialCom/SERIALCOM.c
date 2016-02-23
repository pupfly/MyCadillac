#include "SERIALCOM.h"
#include <reg52.h>
#include "COMMON_FUNC.h"

//串口的配置,方式1，波特率9600
extern void init_serial_com()
{
	TMOD |= 0x20;
	TH1 = 0xFD;
	TL1 = 0xFD;
	TR1 = 1;
	SCON = 0x50;
	EA = 1;
	ES = 1;
	set_response_flag(0);
	set_connected(0);
}
//向串口发送长度为len的字符串，没有连接将直接返回
extern void send_string(unsigned char* str,unsigned char len)
{
	uchar i = 0;
	if(is_connected() == 0)
		return;
	//关闭串口中断,发送中断清零
	set_response_flag(0);
	ES = 0;
	TI = 0;
	//发送开始符
	SBUF = '$';
	while(!TI);
	TI = 0;
	//等待回应再继续发送
	ES = 1;
	while(get_response_flag() == 0);
	//开始发送
	while(i < len)
	{
		set_response_flag(0);
		ES = 0;
		SBUF = str[i++];
		while(!TI);
		TI = 0;
		//等待回应再继续发送
		ES = 1;
		while(get_response_flag() == 0);
	}
	//发送终止符
	set_response_flag(0);
	ES = 0;
	SBUF = '#';
	while(!TI);
	TI = 0;
	//等待回应
	ES = 1;
	while(get_response_flag() == 0);
}
