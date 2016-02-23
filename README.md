/******************************************************/
    Instant Control of Peripherals Using STC89C54RD
/******************************************************/
1. Abstracts
This article defines models how STC89C54RD communicates
with realtime peripheral devices like blueteeth, 
LCD(liquid crystal display) , DHT11, servo， DC motors, 
ultrasound modules.All of these devices contribute to a 
system(a car) which can measure the temperature and humidity
of the surrounding, the distance measured in the car's head
and tail.These data will be  uploaded to our PC or Android
phone. Meanwhile, we can use the car as a toy as it can 
perform like a toy car under the control of our PC or Android
phone, or controled by itself.
摘要：
本文章介绍了一个通过微处理器STC89C54RD与一些外设进行实时通信的既成方案，
这些外设包括:蓝牙转串口模块、LCD1602显示屏、DHT11温湿度计、舵机、直流
电机以及超声波模块。正是由这些设备组成了一个可以测量周围环境温湿度和车体
前后距离并将这些数据上传到PC和安卓手机的智能小车。另外，本车还可以作为
可以使用PC或安卓手机遥控和自动巡航的玩具车使用。
2.项目介绍
本项目旨在通过微处理器：51单片机控制小车以实现一些数据（温湿度和距离）的
测量及上传，为了充分利用外设，还实现了一些具有玩具车特征的功能，下面将
所有功能分为两大类总结如下：
	1.数据测量及上传：测量及上传温湿度和距离，通过控制端软件进行手动或自动
	更新，通过蓝牙转串口模块进行数据传输；两个用于测距的超声波模块分别搭载
	在两个舵机上面，舵机被固定在车头和车尾，可通过控制端软件对舵机（亦即超
	声波模块）的角度进行前、左、右三个角度的大幅度调整或微调以测量不同方向
	的距离，达到360度全覆盖；
	2.玩具车功能：手动遥控：控制车子速度与方向（前后左右停），设置5个档位并
	模拟真实汽车的加速过程（通过定时器产生PWM来实现）; 自动巡航：由用户使用
	控制软件使其进入自动巡航模式，具有自动避障功能， 如在行进过程中遇到死胡
	同，无需U形转弯，直接车尾变车头，因为两头均装有超声波模块。自动巡航模式
	的算法使用状态机来实现。
LCD1602显示屏用于系统状态显示。
3.说明
	本项目实现的智能小车只是项目的可见成果的一部分，其实小车上运行的程序相当于
	一台服务器，更重要的是用户使用的客户端软件（控制端），为使使用方便，我特意
	开发了运行于Android和Windows两种系统的软件版本，由于我的PC蓝牙不可用，开发
	的PC端软件需要配合一块蓝牙转串口模块和电平转换接口来使用。至于硬件的制作，
	比如各个硬件模块的连接，请参阅各模块厂家说明文档。