#pragma once
#include "PCB_Struction.h"

//-------------进程DS------------//
struct progData {//进程数据块
	int stack[10];
};

class PDB {//进程数据块
public:
	progData prog[PNUM];
	PDB();//初始化
};


class Progress ://一个进程的组成结构
	public PCB//PC Block
{
public:
	//----------属性-----------//
	static int progCount;//完成进程计数
	//----------Set-函数-------------//
	void Set_inTimes();//设置进程开始时间
	void Set_Priority();//设置优先级
	void Set_ProState();//设置程序状态
	void Set_Instruct_State();//设置指令状态
	void Set_RunTimes();//设置进程运行时间
	void Set_PSW(int inst_id);//设置状态字
	//int Info_Output();//输出当前程序运行信息
	void Info_from_PCB(PCB &pcb);
	int Progress_Switch();//进程切换
	int Progress_Initiation(PCB &pcb_now);//创建进程
};

//------------------Psedo_Instruction
void Inst_Executing();//指令执行模拟函数
void Inst_Practice(Instructs &inst,int next_inst_id);//具体执行过程
void Execute_Running();//执行算法
void RunResult_Out(int abc);//输出进程信息