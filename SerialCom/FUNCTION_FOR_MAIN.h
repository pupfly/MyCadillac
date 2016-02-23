#include <reg52.h>
#include "DHT11_FUNCTIONS.h"
#include "LCD1602.h"
#include "COMMON_FUNC.h"
#include "ULTRASOUND.h"
#include "SERIALCOM.h"
#include "SERVOS_MOTORS.h"

//初始化设备
extern void init();

//消息处理程序
extern void TranslateMsg(uchar msg);

//串口收到信息标志置位
extern void set_data_come(bit);

extern bit get_data_come();

//获取接收到的数据
extern uchar get_data();