#pragma once
#include "PCB_Struction.h"
class Clock_Interrupt {
public:
	__int64 initiate_time;//时钟开始的时间
   //-----------方法---------//
	void Clock_Initiate();//初始构造函数

	__int64 Get_Interval();//获得当前时间间隔：当前时间减去上次时间 
	int Interrupt_Decide(PCB pcb[]);//时钟中断判断
	static __int64 currentTime();//当前时间，可供外部使用

private:
	__int64 current_time;
	__int64 pre_time;//上次时间
	__int64 matchTime();//将cctime换算成程序运行时间

	int pcb_check_id;//查询pcb创建表序号记录
};