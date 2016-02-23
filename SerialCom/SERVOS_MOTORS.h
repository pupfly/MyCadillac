#ifndef _SERVO_MOTOR_
#define _SERVO_MOTOR_
#define DANGEROUS_DIATANCE 32
#define DELAY_TIME 120
#define STEER_F 0
#define STEER_L 1
#define STEER_R 2
/***********************
******控制电机**********
**********************/

//控制电机转向
extern void motor_ctrl(const unsigned char);
extern unsigned char get_direction();
//设置电机转速
extern void set_speed(unsigned char s);//设置速度上限
extern unsigned char get_speed();//获取速度上限
extern void set_speed_direct();//直接设置速度为所设档位
//电机速度步进
extern void accelerator();

/***********************
******控制舵机**********
	前舵机：F
	后舵机：B
**********************/

//初始化舵机
extern void init_servos_motors();
//设置角度的函数
extern void set_angleF(unsigned char);
extern void set_angleB(unsigned char);
//舵机角度加减
extern void angleFDec();
extern void angleFInc();
extern void angleBDec();
extern void angleBInc();
//自动运行
extern void manage_auto();
extern void turn_Left();
extern void turn_Right();
extern void steer_turn_Left();
extern void steer_turn_Right();
extern void steer_turn_F();
extern void backward();
extern void forward();
extern void measure_f(unsigned int*);
extern void set_auto(bit);
extern bit is_auto();
extern void set_orientation(bit);
extern bit get_orientation();
extern void inform_danger();
extern void inform_safe();
extern bit is_dangerous();
extern void set_steer_state(unsigned char);
extern unsigned char get_steer_state();
#endif