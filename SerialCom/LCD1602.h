#ifndef _LCD1602_
#define	_LCD1602_


extern void delay(unsigned int time);
//向LCD1602发送一条命令
extern void LCD1602_COMMAND(unsigned char command);
//向LCD1602发送一个字符
extern void LCD1602_DATA(unsigned char d);
//初始化LCD1602
extern void LCD_1602_INIT();
//屏幕和地址指针清零
extern void LCD1602_CLEAR();
//换行
extern void LCD1602_NEWLINE();
//从当前屏幕最后一个字处开始显示所给字符串
extern void LCD1602_cat_str(unsigned char *str, unsigned char len);
//清屏并从头显示一个字符串，自动换行
extern void LCD1602_new_str(unsigned char *str, unsigned char len);
#endif
