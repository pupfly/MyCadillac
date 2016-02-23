#ifndef _SERIALCOM_
#define _SERIALCOM_

//串口配置
extern void init_serial_com();
//向串口发送字符串,应答式
extern void send_string(unsigned char*, unsigned char);

#endif