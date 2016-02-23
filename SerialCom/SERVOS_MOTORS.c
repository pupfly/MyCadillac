#include <reg52.h>
#include "SERVOS_MOTORS.h"
#include "COMMON_FUNC.h"
#include "LCD1602.h"
#include "ULTRASOUND.h"
sbit servo_F = P2^3;
sbit servo_B = P2^2;
static uchar angle_F, angle_B;
static uchar speed, speed_limit;
static uchar count = 0;
static uint timer_30s = 0;
static uchar direction = 0x00;
static bit auto_mode = 0;
static bit orientation = 0;//0代表当前在前行，1代表后退
static uchar danger = 0;//距离过近计数变量
static uchar steer_state = 0;//舵机朝向，0-前，1-左，2-右
//初始化定时器0
static void init_T0_for_servo()
{
	TMOD |= 0x01;//定时器0为工作方式一
	IE |= 0x82;
	TH0 = 0xFF;
	TL0 = 0xA3;
	TR0 = 1;
}
//初始化舵机
extern void init_servos_motors()
{
	init_T0_for_servo();
	set_angleF(11);
	set_angleB(11);
	motor_ctrl('s');
	set_speed(120);//默认三档
}
//设置角度的函数
//角度参考值（angle-角度）：5-0、10-45、12-90、20-135、25-180
extern void set_angleF(uchar angle)
{
	angle_F = angle;
}
extern void set_angleB(uchar angle)
{
	angle_B = angle;
}

//控制电机转向
extern void motor_ctrl(const uchar d)
{
	switch(d)
	{
		case 'D':
			direction = 0xAA;
			set_orientation(1);
			break;
		case 'U':
			direction = 0x55;
			set_orientation(0);
			break;
		case 'L':
			direction = 0x66;
			speed = speed_limit;
			break;
		case 'R':
			direction = 0x99;
			speed = speed_limit;
			break;
		case 's':
			direction = 0x00;
			speed = 0;
			break;
	}
}

extern unsigned char get_direction()
{
	return direction;
}

//设置电机转速
extern void set_speed(unsigned char s)//设置速度上限
{
	speed_limit = s;
	if(is_auto())
	{
		set_speed_direct();
	}
}
extern unsigned char get_speed()//获取速度上限
{
	return speed_limit;
}
extern void set_speed_direct()//直接设置速度为所设档位
{
	speed = speed_limit;
}
//模拟油门
extern void accelerator()
{
	if(direction == 0x00)
		return;
	if(speed <= speed_limit)
		speed++;
	else
		speed--;
	delay_ms(2);
}

//舵机角度加减
extern void angleFDec()
{
	if(angle_F == 0)
		return;
	angle_F--;
}
extern void angleFInc()
{
	if(angle_F == 25)
		return;
	angle_F++;
}
extern void angleBDec()
{
	if(angle_B == 0)
		return;
	angle_B--;
}
extern void angleBInc()
{
	if(angle_B == 25)
		return;
	angle_B++;
}

//定时器0中断函数，0.1ms
void timer0_intrrupt() interrupt 1
{
	TH0 = 0xFF;
	TL0 = 0xA3;
	count++;
	if(count < angle_F)
	{
		servo_F = 1;
	}
	else
	{
		servo_F = 0;
	}
	if(count < angle_B)
	{
		servo_B = 1;
	}
	else
	{
		servo_B = 0;
	}
	if(count < speed)
	{
		P1 = direction;
	}
	else
	{
		P1 = 0x00;
	}
	if(count >= 200)
	{
		count = 0;
		timer_30s++;
		if(timer_30s >= 1500)
		{
			set_timeup_30s(1);
			timer_30s = 0;
		}
	}
}

//自动运行
extern void measure_f(unsigned int* d)//测前进方向的距离
{
	if(get_orientation())
	{
		measure_distanceB(d);
	}
	else
	{
		measure_distanceF(d);
	}
}
extern void manage_auto()
{
	uint distance = 0;
	measure_f(&distance);
	
	if(distance > DANGEROUS_DIATANCE)
	{
		inform_safe();
		if(get_steer_state() == STEER_L)
		{
			steer_turn_F();
			turn_Left();
		}
		else if(get_steer_state() == STEER_R)
		{
			steer_turn_F();
			turn_Right();
		}
		forward();
		return;
	}
	else
	{
		
		inform_danger();
		if(is_dangerous())//inform_danger()被调用三次后运行此处
		{
			motor_ctrl('s');
			if(get_steer_state() == STEER_F)
			{
				steer_turn_Left();
			}
			else if(get_steer_state() == STEER_L)
			{
				steer_turn_F();
				steer_turn_Right();
			}
			else if(get_steer_state() == STEER_R)
			{
				steer_turn_F();
				backward();
			}
			return;
		}
		else
		{
			return;
		}
	}
}
extern void turn_Left()
{
	unsigned char t_speed = get_speed();
	set_speed(120);//因为自动运行转弯角度固定，此处以三档为准
	if(get_orientation())
	{
		LCD1602_new_str("Turning Right!", sizeof("Turning Right!") - 1);
		motor_ctrl('R');
	}
	else
	{
		LCD1602_new_str("Turning Left!", sizeof("Turning Left!") - 1);
		motor_ctrl('L');
	}
	delay_ms(DELAY_TIME);
	set_speed(t_speed);//恢复原速度
	if(get_orientation())
	{
		LCD1602_new_str("Backward!", sizeof("Backward!") - 1);
		motor_ctrl('D');
	}
	else
	{
		LCD1602_new_str("Forward!", sizeof("Forward!") - 1);
		motor_ctrl('U');
	}
}
extern void turn_Right()
{
	unsigned char t_speed = get_speed();
	set_speed(120);//因为自动运行转弯角度固定，此处以三档为准
	if(get_orientation())
	{
		LCD1602_new_str("Turning Left!", sizeof("Turning Left!") - 1);
		motor_ctrl('L');
	}
	else
	{
		LCD1602_new_str("Turning Right!", sizeof("Turning Right!") - 1);
		motor_ctrl('R');
	}
	delay_ms(DELAY_TIME);
	set_speed(t_speed);//恢复原速度
	if(get_orientation())
	{
		LCD1602_new_str("Backward!", sizeof("Backward!") - 1);
		motor_ctrl('D');
	}
	else
	{
		LCD1602_new_str("Forward!", sizeof("Forward!") - 1);
		motor_ctrl('U');
	}
}

extern void steer_turn_Left()
{
	LCD1602_new_str("Detecting...", sizeof("Detecting...") - 1);
	if(get_orientation())
	{
		set_angleB(5);
	}
	else
	{
		set_angleF(17);
	}
	set_steer_state(STEER_L);
	delay_ms(123);
}

extern void steer_turn_Right()
{
	LCD1602_new_str("Detecting...", sizeof("Detecting...") - 1);
	if(get_orientation())
	{
		set_angleB(22);
	}
	else
	{
		set_angleF(5);
	}
	set_steer_state(STEER_R);
	delay_ms(123);
}

extern void steer_turn_F()
{
	if(get_orientation())
	{
		set_angleB(11);
	}
	else
	{
		set_angleF(11);
	}
	set_steer_state(STEER_F);
	delay_ms(123);
}

extern void backward()
{
	if(get_orientation())
	{
		LCD1602_new_str("Forward!", sizeof("Forward!") - 1);
		motor_ctrl('U');
	}
	else
	{
		LCD1602_new_str("Backward!", sizeof("Backward!") - 1);
		motor_ctrl('D');
	}
	set_speed_direct();
}

extern void forward()
{
	if(get_orientation() == 0)
	{
		LCD1602_new_str("Forward!", sizeof("Forward!") - 1);
		motor_ctrl('U');
	}
	else
	{
		LCD1602_new_str("Backward!", sizeof("Backward!") - 1);
		motor_ctrl('D');
	}
	set_speed_direct();
}

extern void set_auto(bit value)
{
	auto_mode = value;
}
extern bit is_auto()
{
	return auto_mode;
}

extern void set_orientation(bit value)
{
	orientation = value;
}

extern bit get_orientation()
{
	return orientation;
}

extern void inform_danger()
{
	danger++;
}

extern void inform_safe()
{
	danger = 0;
}

extern bit is_dangerous()
{
	if(danger >= 3)
		return 1;
	return 0;
}

extern void set_steer_state(unsigned char value)
{
	steer_state = value;
}

extern unsigned char get_steer_state()
{
	return steer_state;
}