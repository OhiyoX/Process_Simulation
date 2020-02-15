#pragma once

class CPUinfo {
public:
	int pc;//对应下一条Instruc_ID
	//程序计数器,程序计数器是用于存放下一条指令所在单元的地址的地方。下一个pid
	int ir;//对应Instruc_Type
	//指令寄存器，是临时放置从内存里面取得的程序指令的寄存器，
	//用于存放当前从主存储器读出的正在执行的一条指令。这里使用Inst_type
	int psw;//当前Instruct_ID
	//程序状态字，当前指令id
	//--------方法
	CPUinfo();//构造函数
};
CPUinfo::CPUinfo() {
	pc = -1;
	ir = -1;
	psw = -1;
}
