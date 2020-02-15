#pragma once
#include "PCB_Struction.h"
class Clock_Interrupt {
public:
	__int64 initiate_time;//ʱ�ӿ�ʼ��ʱ��
   //-----------����---------//
	void Clock_Initiate();//��ʼ���캯��

	__int64 Get_Interval();//��õ�ǰʱ��������ǰʱ���ȥ�ϴ�ʱ�� 
	int Interrupt_Decide(PCB pcb[]);//ʱ���ж��ж�
	static __int64 currentTime();//��ǰʱ�䣬�ɹ��ⲿʹ��

private:
	__int64 current_time;
	__int64 pre_time;//�ϴ�ʱ��
	__int64 matchTime();//��cctime����ɳ�������ʱ��

	int pcb_check_id;//��ѯpcb��������ż�¼
};