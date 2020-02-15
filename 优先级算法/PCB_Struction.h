#pragma once
#define PNUM 10//进程数目
#define INSTMAX 20

typedef struct Instructs {
	int Instruc_ID;
	int Instruc_State;//指令的状态，0表示用户态，1表示PV（系统调用并有阻塞，不允许其他进程插队），2表示系统调用（允许插队）
	int Instruc_Times;//单条指令运行时间
	int Instruc_Type;//指令类型，0-3表示用户态的指令，4-5表示系统调用
}ProgInstructs[INSTMAX];//设置每一个进程的指令，最大为20条
//----------------PCB-DS-------------//
class PCB {
public:
	//----------方法
	int PCB_Make(int);//生成PCB信息
	int PCB_outFile();//输出到文件
	int PCB_inPut(int id);//从文件输入
	PCB();//初始构造函数

	//-----------属性
	int Pid;//进程标识号
	int Priority;
	__int64 InTimes;//进程创建时间
	int ProState;//进程状态，ready=0,waiting=1,running=2
	__int64 RunTimes;//
	int InstrucNum;
	ProgInstructs Inst;//指令信息数据结构
	int PSW;//当前运行指令序号

private:
	static __int64 Basetime;//前一个进程的创建时间
	static int flag;//flag表示是否是第一个实例化，若是第一个（flag==0），则输出标题，用于outFile函数
	static int id_succession;//当使用输入文件的时候，id_ptr有助于定位输入表中pcb的位置
};