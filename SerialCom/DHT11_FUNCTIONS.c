#include "DHT11_FUNCTIONS.h"
#include <reg52.h>
#include "COMMON_FUNC.h"
//计数变量
#define NUMBER 20
//端口声明
sbit DHT11_DATA = P3^2;

#define SIZE 5
static uchar status;
static uchar data_array[SIZE];

//读取一个字节
static uchar DHT11_READ_VALUE()
{
	uchar count, value = 0, i;
	status = OK;
	for(i = 8;i > 0;i--)
	{
		value <<= 1;
		count = 0;
		//读取一位数据前等待50us低电平
		while((DHT11_DATA) != 1 && (count++ < NUMBER));
		if(count > NUMBER)
		{
			status = ERROR;
			return ERROR;
		}
		
		count = 0;
		
		delay_10us();
		delay_10us();
		delay_10us();
		
		//26-28us高电平表示该位为0，70us为1
		if(DHT11_DATA != 0)//数据1
		{
			value++;
			//等待剩余高电平时间结束
			while((DHT11_DATA) != 0 && (count++ < NUMBER))
				DHT11_DATA = 1;
			
			if(count > NUMBER)
			{
				status = ERROR;
				return 0;
			}
		}
	}
	return value;
}

//读取温度和湿度
extern uchar READ_DHT11(int* t_h, int* h_h, int* t_l, int* h_l)
{
	uchar check = 0, i = 0, count = 0;
	//全局中断关闭
	EA = 0;
	//拉低数据线18ms以上
	DHT11_DATA = 0;
	delay_ms(20);
	DHT11_DATA = 1;
	//延时20-40us等待80us应答信号
	delay_10us();
	delay_10us();
	delay_10us();
	delay_10us();
	delay_10us();
	
	if(DHT11_DATA != 0)
	{
		EA = 1;
		return ERROR;
	}
	else
	{
		//等待剩余低电平时间结束
		while((DHT11_DATA) == 0 && (count++ < NUMBER));
		if(count > NUMBER)
		{
			EA = 1;
			DHT11_DATA = 1;
			return ERROR;
		}
		count = 0;
		DHT11_DATA = 1;//释放数据线
		//应答信号后的80us高电平
		while((DHT11_DATA) != 0 && (count++ < NUMBER));
		if(count >= NUMBER)
		{
			EA = 1;
			DHT11_DATA = 1;
			return ERROR;
		}
		//读出湿温度
		for(i = 0;i < SIZE;i++)
		{
			data_array[i] = DHT11_READ_VALUE();
			if(status == ERROR)
			{
				EA = 1;
				DHT11_DATA = 1;
				return ERROR;
			}
			if(i != SIZE - 1)
			{
				check += data_array[i];
			}
		}
		//没发生读取错误，校验数据
		if(check == data_array[SIZE - 1])
		{
			*h_h = data_array[0];
			*h_l = data_array[1];
			*t_h = data_array[2];
			*t_l = data_array[3];
			
			
			EA = 1;
			DHT11_DATA = 1;
			return OK;
		}
		else
		{
			EA = 1;
			return ERROR;
		}
	}
}

extern unsigned char READ_DHT11_toStr(unsigned char* t_str, unsigned char* h_str)
{
	int th, tl, hh, hl;
	if(READ_DHT11(&th, &hh, &tl, &hl) == OK)
	{
		t_str[0] = 'T';
		t_str[1] = num2char[th / 10];
		t_str[2] = num2char[th % 10];
		h_str[0] = 'H';
		h_str[1] = num2char[hh / 10];
		h_str[2] = num2char[hh % 10];
		return OK;
	}
	else
	{
		return ERROR;
	}
}