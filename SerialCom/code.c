/**************************************
	作者：普鹏飞
	Email:792570367@qq.com
	创建时间：2016/02/04,11:45
**************************************/

#include "FUNCTION_FOR_MAIN.h"
int main()
{
	init();
	delay_ms(100);
	LCD1602_new_str("Initializing...", sizeof("Initializing...")  - 1);
	delay_ms(100);
	LCD1602_new_str("Cadillac Ready!", sizeof("Cadillac Ready!")  - 1);
	
	while(1)
	{
		if(get_data_come() == 1)
		{	
			set_data_come(0);
			TranslateMsg(get_data());
		}
		else if(is_auto())
		{
			manage_auto();
		}
		else if(is_timeup_30s() && get_auto_renew() && get_direction() == 0x00)//开启自动更新且没收到指令时，每30s回传一次数据
		{
			uchar buf[5], b_h[3], b_t[3];
			uchar count = 0;
			set_timeup_30s(0);
			//测量并传送距离
			measure_distanceF2str(buf);
			send_string(buf, 5);
			measure_distanceB2str(buf);
			send_string(buf, 5);
			//测量并传送温湿度
			while(READ_DHT11_toStr(b_t, b_h) == ERROR && (++count) <= 3);
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
		}
		else
		{
			accelerator();//执行加速器函数，模拟缓慢启动（减速）
		}
	}
}
