#include "FUNCTION_FOR_MAIN.h"


//收到数据的标志位
static bit DATA_COME;

//来自串口的数据
static uchar DATA_BUF_R;

//初始化设备
extern void init()
{
	DATA_COME = 0;
	init_serial_com();
	init_servos_motors();
	LCD_1602_INIT();
}

//消息处理程序
extern void TranslateMsg(uchar msg)
{
	uchar buf[5], b_h[3], b_t[3];
	uchar count;
	switch(msg)
	{
		//对电机的操作
		case 'U':
		case 'L':
		case 'R':
		case 'D':
			motor_ctrl(msg);
			break;
		case 's':
			set_auto(0);
			motor_ctrl('s');
			break;
		case 'l':
			set_angleF(17);
			break;
		case 'f':
			set_angleF(11);
			break;
		case 'r':
			set_angleF(5);
			break;
		case '1':
			set_angleB(5);
			break;
		case '2':
			set_angleB(11);
			break;
		case '3':
			set_angleB(22);
			break;
		case '-':
			angleFDec();
			break;
		case '+':
			angleFInc();
			break;
		case 'J':
			angleBDec();
			break;
		case 'j':
			angleBInc();
			break;
		case 'A'://一档
			set_speed(40);
			LCD1602_new_str("Level 1", sizeof("Level 1")  - 1);
			break;
		case 'B':
			set_speed(80);
			LCD1602_new_str("Level 2", sizeof("Level 2")  - 1);
			break;
		case 'C':
			set_speed(120);
			LCD1602_new_str("Level 3", sizeof("Level 3")  - 1);
			break;
		case 'E':
			set_speed(160);
			LCD1602_new_str("Level 4", sizeof("Level 4")  - 1);
			break;
		case 'F':
			set_speed(200);
			LCD1602_new_str("Level 5", sizeof("Level 5")  - 1);
			break;
		case 'T':
			send_string("MOK",sizeof("MOK") - 1);
			break;
		case 'N':
			measure_distanceF2str(buf);
			send_string(buf, 5);
			measure_distanceB2str(buf);
			send_string(buf, 5);
			break;
		case 'n':
			count = 0;
			while(READ_DHT11_toStr(b_t, b_h) == ERROR && (++count) <= 3);//DHT11不太稳定，测量三次
			if(count <= 3)
			{
				send_string(b_t, 3);
				send_string(b_h, 3);
			}
			else
			{
				send_string("TRetry", sizeof("TRetry") - 1);
				send_string("HRetry", sizeof("HRetry") - 1);
			}
			break;
		case 'S':
			set_connected(1);
			break;
		case 'O':
			set_connected(0);
			break;
		case '#':
			set_auto_renew(1);
			break;
		case '$':
			set_auto_renew(0);
			break;
		case 'P':
			set_auto(1);
			set_speed(120);
			break;
	}
}

//串口收到信息标志置位
extern void set_data_come(bit value)
{
	DATA_COME = value;
}

extern bit get_data_come()
{
	return DATA_COME;
}

//获取接收到的数据
extern uchar get_data()
{
	return DATA_BUF_R;
}

//串口中断函数
void serial_com() interrupt 4
{
	RI = 0;	
	DATA_BUF_R = SBUF;
	if(DATA_BUF_R == '&')//收到信息交换标志（继续发送）
	{
		set_response_flag(1);
	}
	else
	{
		DATA_COME = 1;//收到其他控制信息
	}
}