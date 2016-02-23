#include <reg52.h>
#include "LCD1602.h"
#include "COMMON_FUNC.h"

sbit LCD1602_RS = P2^0;
sbit LCD1602_E = P2^1;

extern void LCD1602_COMMAND(unsigned char command)
{
	LCD1602_RS =0;
	P0 = command;
	delay_ms(5);
	LCD1602_E = 1;
	delay_ms(5);
	LCD1602_E = 0;
}


extern void LCD1602_DATA(unsigned char d)
{
	LCD1602_RS =1;
	P0 = d;
	delay_ms(5);
	LCD1602_E = 1;
	delay_ms(5);
	LCD1602_E = 0;
}

 
extern void LCD_1602_INIT()
{
	LCD1602_E = 0;

	LCD1602_COMMAND(0x38);//16*2显示，5*7点阵，8位数据接口
	LCD1602_COMMAND(0x0C);//开始显示，无光标
	LCD1602_COMMAND(0x06);//地址自动加一
	LCD1602_COMMAND(0x01);//显示清零，地址指针清零
}

//屏幕和地址指针清零
extern void LCD1602_CLEAR()
{
	LCD1602_COMMAND(0x01);//显示清零，地址指针清零
}

//换行
extern void LCD1602_NEWLINE()
{
	LCD1602_COMMAND(0x80 + 0x40);
}

//从当前屏幕最后一个字处开始显示所给字符串
extern void LCD1602_cat_str(unsigned char *str, unsigned char len)
{
	uchar i;
	
	for(i = 0;i < len && i < 32;i++)
	{
		LCD1602_DATA(str[i]);
		if(i == 15)//换行
		{
			LCD1602_NEWLINE();
		}
	}
}

//清屏并从头显示一个字符串，自动换行
extern void LCD1602_new_str(unsigned char *str, unsigned char len)
{
	LCD1602_CLEAR();
	LCD1602_COMMAND(0x80);
	LCD1602_cat_str(str, len);
}
