#include <iostream>
#pragma comment( lib,"winmm.lib" )
#include <Windows.h>
#include "FileRead.h"
#include "Progress_Status.h"
#include "CPU_Operation.h"
#include "Progress_Operation.h"
#include "Clock_Interrupt.h"
#include <fstream>
using namespace std;

//======================全局=============//
CPUinfo CPU_info;//初始化处理器
Status_Queue ProgQ;//初始化三态队列
Progress PCB_table[PNUM];//初始化PCB_table，用于模拟运行时的PCB表
PDB pData;
Clock_Interrupt oClock;
int PCB::flag = 0;
__int64 PCB::Basetime = 0;
int PCB::id_succession = 0;
int Progress::progCount = 0;
int ojbk = 0;


//=================MAIN=================//
//======================================//
int main() {
	//------------PCB.txt写------------//
	PCB *pcb_temp = new PCB[PNUM];//生成一张PCB.txt
	int i;
	for (i = 0; i < PNUM; i++) {
		pcb_temp[i].PCB_Make(i);
		pcb_temp[i].PCB_outFile();
	}
	delete[]pcb_temp;
	//--------------PCB.txt读入内存-----------//
	PCB pcb_check[PNUM];//从txt表中读取送入内存,pcb_check是进程请求查看表
	for (i = 0; i < PNUM; i++) {
		pcb_check[i].PCB_inPut(i);
	}
	//--------------时钟开始计时-------------------//
	oClock.Clock_Initiate();
	while (!oClock.Interrupt_Decide(pcb_check));//等待第一次创建进程

	do {
		Inst_Executing();
		oClock.Interrupt_Decide(pcb_check);//时钟中断检查
	} while (Progress::progCount<PNUM);//所有命令都执行完
	cout << "\n=========所有进程运行完毕！===========" << endl;
	return 1;
}


//======================Clock类====================//
//==============================================//
int Clock_Interrupt::Interrupt_Decide(PCB pcb[]) {//判断时钟中断函数
	//进入时钟中断
	int happen = 0;
	current_time = currentTime();

	if (current_time - pre_time >= 20) {//判断时钟中断
			//--------时钟中断到达--------------
		//判断IO操作时间是否到达？
		
		if (!ProgQ.EmptyQueue(ProgQ.waiting)) {//判断阻塞队列是否为空
			int wait_id; ProgQ.GetHead(ProgQ.waiting, wait_id);
			//若不为空
			int wait_id_time = PCB_table[wait_id].Inst[PCB_table[wait_id].PSW].Instruc_Times;
			if ((current_time-initiate_time- PCB_table[wait_id].RunTimes) >= wait_id_time) {//IO
				//若此时已到达阻塞队列首部的IO时间，激活阻塞队列
				ProgQ.DeQueue(ProgQ.waiting, wait_id);
				PCB_table[wait_id].PSW++;

				if (PCB_table[wait_id].PSW+1 == PCB_table[wait_id].InstrucNum) {//所有指令执行完
					Progress::progCount++;
					if (ProgQ.running > -1) {}
					else {
						PCB_table[wait_id].Set_RunTimes();
						RunResult_Out(wait_id);
						cout << "进程ID：" << wait_id << "，系统调用完成，运行完成" << "count" << Progress::progCount << endl;
						if (!ProgQ.EmptyQueue(ProgQ.ready)) {//如果就绪队列不空
							int ok; ProgQ.DeQueue(ProgQ.ready, ok);
							ProgQ.Set_Running(ok);//唤醒就绪态，取出一个就绪进程设为运行态
							cout << "进程ID：" << ProgQ.running << "，离开就绪队列，将开始运行" << endl;
						}
					}
				}
				else { //未执行完
					ProgQ.Set_Ready(wait_id);//加入就绪队列
					cout << "进程ID：" << wait_id << "，系统调用完成，进入就绪队列" << endl;
					if (ProgQ.running > -1) {
						if (PCB_table[wait_id].Priority > PCB_table[ProgQ.running].Priority) {
							ProgQ.Set_Ready(ProgQ.running);
							cout << "进程ID：" << ProgQ.running << "，将进入就绪队列，" ;

							int ok; ProgQ.DeQueue(ProgQ.ready, ok);
							ProgQ.Set_Running(ok);
							cout << "进程ID：" << ProgQ.running << "，离开就绪队列，将开始运行" << endl;
						}
					}
					else {
						int ok; ProgQ.DeQueue(ProgQ.ready, ok);
						ProgQ.Set_Running(ok);
						cout << "进程ID：" << ProgQ.running << "，离开就绪队列，将开始运行" << endl;
					}
				}
			}
		}

		while (matchTime() >= pcb[pcb_check_id].InTimes&&pcb_check_id < PNUM) {//查pcb预设表判断是否有时刻匹配的进程请求
			//----------若匹配-------
			PCB_table[pcb_check_id].Progress_Initiation(pcb[pcb_check_id]);//使用进程创建函数
			pcb_check_id++;
			happen = 1;
		}
		pre_time = current_time;
	}
		return happen;//返回0表示未查找到进程请求，返回1表示查找到进程请求
}


void Clock_Interrupt::Clock_Initiate() {//初始构造函数
	pcb_check_id = 0;
	initiate_time = currentTime();
	pre_time = initiate_time;
}


__int64 Clock_Interrupt::Get_Interval() {//获取当前与上次中断的间隔
	return currentTime() - pre_time;
}
__int64 Clock_Interrupt::matchTime() {//将cctime换算成程序运行时间
	return current_time - initiate_time;
}
__int64 Clock_Interrupt::currentTime() {//获取当前的系统时间；以毫秒为单位
	return timeGetTime();
}


//=================进程类====================//
//=========================================//
int Progress::Progress_Initiation(PCB &pcb_now) {//创建进程
	this->Info_from_PCB(pcb_now);//复制PCB信息
	this->Set_inTimes();//获取创建进程时间
		cout << "\n=============发现请求，进程ID:"
			<< this->Pid << "，优先级为："
			<< this->Priority << "，有" << this->InstrucNum
			<< "条指令，创建成功\n" << endl;
	this->Progress_Switch();//进程调度
	return 1;
}

void Progress::Info_from_PCB(PCB &pcb) {
	this->Pid = pcb.Pid;
	this->Priority = pcb.Priority;
	this->InTimes = pcb.InTimes;
	this->ProState = pcb.ProState;
	this->RunTimes = pcb.RunTimes;//5
	this->InstrucNum = pcb.InstrucNum;
	int i;
	for (i = 0; i < this->InstrucNum; i++) {
		this->Inst[i] = pcb.Inst[i];
	}
	this->PSW = pcb.PSW;//10
}

void Progress::Set_inTimes() {
	this->InTimes = Clock_Interrupt::currentTime() -oClock.initiate_time;
}

//进程转换负责对运行态的进程调度，包括到来进程，以及已有进程的再安排
int Progress::Progress_Switch() {
	if (ProgQ.running > -1) {//若运行态有进程,Prog.running存储运行态进程id
		if (this->Priority > PCB_table[ProgQ.running].Priority) {//比较优先级
			ProgQ.Set_Ready(ProgQ.running);//旧进程保存信息，放入就绪队列
			cout<<"进程ID："<<ProgQ.running<<"，进入就绪队列，"<<"进程ID："<<this->Pid<<"开始运行"<<endl;
			ProgQ.Set_Running(this->Pid);
			RunResult_Out(this->Pid);
		}
		else {
			ProgQ.Set_Ready(this->Pid);
			RunResult_Out(this->Pid);
			cout << "进程ID：" << this->Pid << "，进入就绪队列" << endl;
		}
	}
	else {//------运行态无进程
		ProgQ.Set_Running(this->Pid);
		//RunResult_Out(this->Pid);
		cout << "进程ID：" << this->Pid << "，准备运行" << endl;
	}
	return 1;
}
//-------------设置优先级--------------------//
void Progress::Set_Priority() {
	//设置优先级，参考优先算法
}
//------------------设置进程状态-------------------//
void Progress::Set_ProState() {
	//---------当前的进程状态，可能处于运行态或者就绪态或者阻塞态
	if (ProgQ.running == this->Pid) {//若处于阻塞或正在运行
		switch (this->Inst[this->PSW + 1].Instruc_State) {//将要执行指令的状态
		case 0: //为系统调用
			this->ProState = 1;
		case 1://PV
		case 2://用户态
			this->ProState = 2;
		}
	}
	else {//若处于就绪，即将发生运行
		this->ProState = 0;
	}
}

void Progress::Set_Instruct_State() {
	//设置指令
}

void Progress::Set_RunTimes() {//设置运行时间
	this->RunTimes = Clock_Interrupt::currentTime() - (this->InTimes+oClock.initiate_time);
}

void Progress::Set_PSW(int inst_id) {
	this->PSW = inst_id;
}

PDB::PDB() {
	int i, j;
	for (i = 0; i < PNUM; i++) {
		for (j = 0; j < 10; j++) {
			prog[i].stack[j] = 0;
		}
	}
}



//================设置状态队列================//
//========================================//
void Status_Queue::Set_Ready(int &pid) {
	if (ProgQ.running == pid) {//如果被更高优先级占用,从运行态退到就绪态
		PCB_table[pid].Set_PSW(CPU_info.psw);//保存处理机上下文
		PCB_table[pid].Set_RunTimes();//保存运行时间
	}
		PCB_table[pid].ProState = 0;//设置为就绪态
	Priority_Decide(ProgQ.ready, pid);// 根据优先级对队列进行排序设置队列
}

void Status_Queue::Set_Waiting(int &pid) {
	PCB_table[pid].ProState = 1;//设置进程状态
	PCB_table[pid].Set_RunTimes();//保存运行时间
	ProgQ.EnQueue(ProgQ.waiting, pid);// 根据优先级对队列进行排序设置队列
}
void Status_Queue::Set_Running(int &pid) {
	ProgQ.running = pid;//设置运行态Pid
	PCB_table[pid].ProState = 2;//设置进程状态
	PCB_table[pid].Set_Priority();//设置优先级
	//--------------恢复处理机上下文
	CPU_info.psw = PCB_table[pid].PSW;//状态字
	CPU_info.pc = PCB_table[pid].PSW + 1;//程序计数器
	CPU_info.ir = PCB_table[pid].Inst[PCB_table[pid].PSW].Instruc_Type;//指令寄存器
}

void Status_Queue::Priority_Decide(LinkQueue &Q, int pid) {//比较优先级
	ProgQueuePtr p, q;
	if (EmptyQueue(Q)) {//若队列为空
		EnQueue(Q, pid);//将pid加入队列中
	}
	else {                      //若不为空
		p = q = Q.front;
		for (p = Q.front->next;;) {
			//------------判断加入的进程优先级是否大于此结点的优先级
			if (PCB_table[pid].Priority > PCB_table[p->data].Priority) {//若大于
				InsAfter(Q, q, pid);//在p前（即q后）插入进程
				q = q->next; break;
			}
			else {//若小于
				q = q->next;//下移
				if (p == Q.rear) //如果
				{
					EnQueue(ready, pid); break;//
				}
				p = p->next;
			}
		}
	}
}

//======================伪指令执行=================//
//==============================================//
void Inst_Executing() {//指令执行模拟函数
	if (ProgQ.running > -1) {//判断一个进程是否执行完
		Execute_Running( );
	}
}

void Execute_Running() {
	int ppid = ProgQ.running;//id号为运行态进程id
	int done_inst_id, next_inst_id, into_state;//done_inst_id表示已执行到的位置,into_state表示将要执行指令的状态
	done_inst_id = CPU_info.psw;//查看已执行到的指令位置
	into_state = CPU_info.ir;//查看将要执行指令的进入状态
	next_inst_id = CPU_info.pc;
	//设置进程状态
	switch (into_state) {
	case 0:
	case 1:
	case 2:
	case 3:PCB_table[ppid].ProState = 2; break;//将要运行指令为用户态，继续执行
	case 4:
	case 5:PCB_table[ppid].ProState = 1; break;//将要运行指令（发生系统调用）
	}

	Inst_Practice(PCB_table[ppid].Inst[next_inst_id],next_inst_id);//执行命令
}

void Inst_Practice(Instructs &inst,int next_inst_id) {//指令的具体实施过程
	int flag_do = 0;
	switch (inst.Instruc_Type) {
	case 0://待补充
	case 1://待补充
	case 2://待补充
	case 3: {
		cout << "进程ID：" << ProgQ.running
			<< "，正在执行第" << CPU_info.psw + 2 << "条指令，在用户态"
			<< "" << " "
			<< endl;
		//更新CPU信息
		CPU_info.psw = next_inst_id;
		CPU_info.pc = next_inst_id + 1;
		CPU_info.ir = PCB_table[ProgQ.running].Inst[next_inst_id + 1].Instruc_Type;
		Sleep(PCB_table[ProgQ.running].Inst[CPU_info.psw].Instruc_Times);
		PCB_table[ProgQ.running].PSW = next_inst_id;//保存当前指令id
		PCB_table[ProgQ.running].Set_RunTimes();//设置已运行时间

		if (PCB_table[ProgQ.running].PSW + 1 == PCB_table[ProgQ.running].InstrucNum) {//所有指令执行完
			Progress::progCount++;
			cout << "count" << Progress::progCount << endl;
			flag_do = 1;
			RunResult_Out(ProgQ.running);
		}
		else { flag_do = 0; }
		break;
	}//0-3都是用户态
	case 4:
	case 5: {
		cout << "进程ID：" << ProgQ.running
			<< "，正在执行第" << CPU_info.psw + 2 << "条指令，在内核态，发生系统调用，进入阻塞队列" << endl;
		ProgQ.Set_Waiting(ProgQ.running);//放入阻塞队列
		PCB_table[ProgQ.running].Set_RunTimes();//设置已运行时间
		RunResult_Out(ProgQ.running);
		flag_do = 1;
		break; }
	}
	if (flag_do == 1) {
		ProgQ.running = -1;
		if (!ProgQ.EmptyQueue(ProgQ.ready)) {
			int ok; ProgQ.DeQueue(ProgQ.ready, ok);
			ProgQ.Set_Running(ok);
			cout << "进程ID：" << ProgQ.running << "，离开就绪队列，将开始运行" << endl;
		}
	}
}


void RunResult_Out(int abc) {
	ofstream resultFile,resultFile2;
	if (ojbk == 0) {
		resultFile.open("13615206-RunResults.txt", ios::out);
		if (!resultFile.good()) { cout << "open file failed" << endl; exit(0); }
		resultFile << "ProSt=ProState, ready=0,waiting=1, runnning=2\n"
			<< "Inst_ID: Progress Initiated=-1\n"
			<<"PID\tPrior\tInstID\tInstNum\tProSt\tInTm\tRunTm\tTotalTm\n";
		ojbk = 1;
		resultFile.close();
	}
	resultFile2.open("13615206-RunResults.txt", ios::app);
	resultFile2 << abc << "\t"
		<<PCB_table[abc].Priority<<"\t"
		<< PCB_table[abc].PSW << "\t"
		<< PCB_table[abc].InstrucNum << "\t"
		<<PCB_table[abc].ProState<<"\t"
		<<PCB_table[abc].InTimes<<"\t"
		<< PCB_table[abc].RunTimes<<"\t";
	if (PCB_table[abc].PSW + 1 == PCB_table[abc].InstrucNum)
		resultFile2 << PCB_table[abc].RunTimes<<"\t";
	else resultFile2 << "--" << "\t";
	resultFile2<<"\n";
}