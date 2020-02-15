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

//======================ȫ��=============//
CPUinfo CPU_info;//��ʼ��������
Status_Queue ProgQ;//��ʼ����̬����
Progress PCB_table[PNUM];//��ʼ��PCB_table������ģ������ʱ��PCB��
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
	//------------PCB.txtд------------//
	PCB *pcb_temp = new PCB[PNUM];//����һ��PCB.txt
	int i;
	for (i = 0; i < PNUM; i++) {
		pcb_temp[i].PCB_Make(i);
		pcb_temp[i].PCB_outFile();
	}
	delete[]pcb_temp;
	//--------------PCB.txt�����ڴ�-----------//
	PCB pcb_check[PNUM];//��txt���ж�ȡ�����ڴ�,pcb_check�ǽ�������鿴��
	for (i = 0; i < PNUM; i++) {
		pcb_check[i].PCB_inPut(i);
	}
	//--------------ʱ�ӿ�ʼ��ʱ-------------------//
	oClock.Clock_Initiate();
	while (!oClock.Interrupt_Decide(pcb_check));//�ȴ���һ�δ�������

	do {
		Inst_Executing();
		oClock.Interrupt_Decide(pcb_check);//ʱ���жϼ��
	} while (Progress::progCount<PNUM);//�������ִ����
	cout << "\n=========���н���������ϣ�===========" << endl;
	return 1;
}


//======================Clock��====================//
//==============================================//
int Clock_Interrupt::Interrupt_Decide(PCB pcb[]) {//�ж�ʱ���жϺ���
	//����ʱ���ж�
	int happen = 0;
	current_time = currentTime();

	if (current_time - pre_time >= 20) {//�ж�ʱ���ж�
			//--------ʱ���жϵ���--------------
		//�ж�IO����ʱ���Ƿ񵽴
		
		if (!ProgQ.EmptyQueue(ProgQ.waiting)) {//�ж����������Ƿ�Ϊ��
			int wait_id; ProgQ.GetHead(ProgQ.waiting, wait_id);
			//����Ϊ��
			int wait_id_time = PCB_table[wait_id].Inst[PCB_table[wait_id].PSW].Instruc_Times;
			if ((current_time-initiate_time- PCB_table[wait_id].RunTimes) >= wait_id_time) {//IO
				//����ʱ�ѵ������������ײ���IOʱ�䣬������������
				ProgQ.DeQueue(ProgQ.waiting, wait_id);
				PCB_table[wait_id].PSW++;

				if (PCB_table[wait_id].PSW+1 == PCB_table[wait_id].InstrucNum) {//����ָ��ִ����
					Progress::progCount++;
					if (ProgQ.running > -1) {}
					else {
						PCB_table[wait_id].Set_RunTimes();
						RunResult_Out(wait_id);
						cout << "����ID��" << wait_id << "��ϵͳ������ɣ��������" << "count" << Progress::progCount << endl;
						if (!ProgQ.EmptyQueue(ProgQ.ready)) {//����������в���
							int ok; ProgQ.DeQueue(ProgQ.ready, ok);
							ProgQ.Set_Running(ok);//���Ѿ���̬��ȡ��һ������������Ϊ����̬
							cout << "����ID��" << ProgQ.running << "���뿪�������У�����ʼ����" << endl;
						}
					}
				}
				else { //δִ����
					ProgQ.Set_Ready(wait_id);//�����������
					cout << "����ID��" << wait_id << "��ϵͳ������ɣ������������" << endl;
					if (ProgQ.running > -1) {
						if (PCB_table[wait_id].Priority > PCB_table[ProgQ.running].Priority) {
							ProgQ.Set_Ready(ProgQ.running);
							cout << "����ID��" << ProgQ.running << "��������������У�" ;

							int ok; ProgQ.DeQueue(ProgQ.ready, ok);
							ProgQ.Set_Running(ok);
							cout << "����ID��" << ProgQ.running << "���뿪�������У�����ʼ����" << endl;
						}
					}
					else {
						int ok; ProgQ.DeQueue(ProgQ.ready, ok);
						ProgQ.Set_Running(ok);
						cout << "����ID��" << ProgQ.running << "���뿪�������У�����ʼ����" << endl;
					}
				}
			}
		}

		while (matchTime() >= pcb[pcb_check_id].InTimes&&pcb_check_id < PNUM) {//��pcbԤ����ж��Ƿ���ʱ��ƥ��Ľ�������
			//----------��ƥ��-------
			PCB_table[pcb_check_id].Progress_Initiation(pcb[pcb_check_id]);//ʹ�ý��̴�������
			pcb_check_id++;
			happen = 1;
		}
		pre_time = current_time;
	}
		return happen;//����0��ʾδ���ҵ��������󣬷���1��ʾ���ҵ���������
}


void Clock_Interrupt::Clock_Initiate() {//��ʼ���캯��
	pcb_check_id = 0;
	initiate_time = currentTime();
	pre_time = initiate_time;
}


__int64 Clock_Interrupt::Get_Interval() {//��ȡ��ǰ���ϴ��жϵļ��
	return currentTime() - pre_time;
}
__int64 Clock_Interrupt::matchTime() {//��cctime����ɳ�������ʱ��
	return current_time - initiate_time;
}
__int64 Clock_Interrupt::currentTime() {//��ȡ��ǰ��ϵͳʱ�䣻�Ժ���Ϊ��λ
	return timeGetTime();
}


//=================������====================//
//=========================================//
int Progress::Progress_Initiation(PCB &pcb_now) {//��������
	this->Info_from_PCB(pcb_now);//����PCB��Ϣ
	this->Set_inTimes();//��ȡ��������ʱ��
		cout << "\n=============�������󣬽���ID:"
			<< this->Pid << "�����ȼ�Ϊ��"
			<< this->Priority << "����" << this->InstrucNum
			<< "��ָ������ɹ�\n" << endl;
	this->Progress_Switch();//���̵���
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

//����ת�����������̬�Ľ��̵��ȣ������������̣��Լ����н��̵��ٰ���
int Progress::Progress_Switch() {
	if (ProgQ.running > -1) {//������̬�н���,Prog.running�洢����̬����id
		if (this->Priority > PCB_table[ProgQ.running].Priority) {//�Ƚ����ȼ�
			ProgQ.Set_Ready(ProgQ.running);//�ɽ��̱�����Ϣ�������������
			cout<<"����ID��"<<ProgQ.running<<"������������У�"<<"����ID��"<<this->Pid<<"��ʼ����"<<endl;
			ProgQ.Set_Running(this->Pid);
			RunResult_Out(this->Pid);
		}
		else {
			ProgQ.Set_Ready(this->Pid);
			RunResult_Out(this->Pid);
			cout << "����ID��" << this->Pid << "�������������" << endl;
		}
	}
	else {//------����̬�޽���
		ProgQ.Set_Running(this->Pid);
		//RunResult_Out(this->Pid);
		cout << "����ID��" << this->Pid << "��׼������" << endl;
	}
	return 1;
}
//-------------�������ȼ�--------------------//
void Progress::Set_Priority() {
	//�������ȼ����ο������㷨
}
//------------------���ý���״̬-------------------//
void Progress::Set_ProState() {
	//---------��ǰ�Ľ���״̬�����ܴ�������̬���߾���̬��������̬
	if (ProgQ.running == this->Pid) {//��������������������
		switch (this->Inst[this->PSW + 1].Instruc_State) {//��Ҫִ��ָ���״̬
		case 0: //Ϊϵͳ����
			this->ProState = 1;
		case 1://PV
		case 2://�û�̬
			this->ProState = 2;
		}
	}
	else {//�����ھ�����������������
		this->ProState = 0;
	}
}

void Progress::Set_Instruct_State() {
	//����ָ��
}

void Progress::Set_RunTimes() {//��������ʱ��
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



//================����״̬����================//
//========================================//
void Status_Queue::Set_Ready(int &pid) {
	if (ProgQ.running == pid) {//������������ȼ�ռ��,������̬�˵�����̬
		PCB_table[pid].Set_PSW(CPU_info.psw);//���洦���������
		PCB_table[pid].Set_RunTimes();//��������ʱ��
	}
		PCB_table[pid].ProState = 0;//����Ϊ����̬
	Priority_Decide(ProgQ.ready, pid);// �������ȼ��Զ��н����������ö���
}

void Status_Queue::Set_Waiting(int &pid) {
	PCB_table[pid].ProState = 1;//���ý���״̬
	PCB_table[pid].Set_RunTimes();//��������ʱ��
	ProgQ.EnQueue(ProgQ.waiting, pid);// �������ȼ��Զ��н����������ö���
}
void Status_Queue::Set_Running(int &pid) {
	ProgQ.running = pid;//��������̬Pid
	PCB_table[pid].ProState = 2;//���ý���״̬
	PCB_table[pid].Set_Priority();//�������ȼ�
	//--------------�ָ������������
	CPU_info.psw = PCB_table[pid].PSW;//״̬��
	CPU_info.pc = PCB_table[pid].PSW + 1;//���������
	CPU_info.ir = PCB_table[pid].Inst[PCB_table[pid].PSW].Instruc_Type;//ָ��Ĵ���
}

void Status_Queue::Priority_Decide(LinkQueue &Q, int pid) {//�Ƚ����ȼ�
	ProgQueuePtr p, q;
	if (EmptyQueue(Q)) {//������Ϊ��
		EnQueue(Q, pid);//��pid���������
	}
	else {                      //����Ϊ��
		p = q = Q.front;
		for (p = Q.front->next;;) {
			//------------�жϼ���Ľ������ȼ��Ƿ���ڴ˽������ȼ�
			if (PCB_table[pid].Priority > PCB_table[p->data].Priority) {//������
				InsAfter(Q, q, pid);//��pǰ����q�󣩲������
				q = q->next; break;
			}
			else {//��С��
				q = q->next;//����
				if (p == Q.rear) //���
				{
					EnQueue(ready, pid); break;//
				}
				p = p->next;
			}
		}
	}
}

//======================αָ��ִ��=================//
//==============================================//
void Inst_Executing() {//ָ��ִ��ģ�⺯��
	if (ProgQ.running > -1) {//�ж�һ�������Ƿ�ִ����
		Execute_Running( );
	}
}

void Execute_Running() {
	int ppid = ProgQ.running;//id��Ϊ����̬����id
	int done_inst_id, next_inst_id, into_state;//done_inst_id��ʾ��ִ�е���λ��,into_state��ʾ��Ҫִ��ָ���״̬
	done_inst_id = CPU_info.psw;//�鿴��ִ�е���ָ��λ��
	into_state = CPU_info.ir;//�鿴��Ҫִ��ָ��Ľ���״̬
	next_inst_id = CPU_info.pc;
	//���ý���״̬
	switch (into_state) {
	case 0:
	case 1:
	case 2:
	case 3:PCB_table[ppid].ProState = 2; break;//��Ҫ����ָ��Ϊ�û�̬������ִ��
	case 4:
	case 5:PCB_table[ppid].ProState = 1; break;//��Ҫ����ָ�����ϵͳ���ã�
	}

	Inst_Practice(PCB_table[ppid].Inst[next_inst_id],next_inst_id);//ִ������
}

void Inst_Practice(Instructs &inst,int next_inst_id) {//ָ��ľ���ʵʩ����
	int flag_do = 0;
	switch (inst.Instruc_Type) {
	case 0://������
	case 1://������
	case 2://������
	case 3: {
		cout << "����ID��" << ProgQ.running
			<< "������ִ�е�" << CPU_info.psw + 2 << "��ָ����û�̬"
			<< "" << " "
			<< endl;
		//����CPU��Ϣ
		CPU_info.psw = next_inst_id;
		CPU_info.pc = next_inst_id + 1;
		CPU_info.ir = PCB_table[ProgQ.running].Inst[next_inst_id + 1].Instruc_Type;
		Sleep(PCB_table[ProgQ.running].Inst[CPU_info.psw].Instruc_Times);
		PCB_table[ProgQ.running].PSW = next_inst_id;//���浱ǰָ��id
		PCB_table[ProgQ.running].Set_RunTimes();//����������ʱ��

		if (PCB_table[ProgQ.running].PSW + 1 == PCB_table[ProgQ.running].InstrucNum) {//����ָ��ִ����
			Progress::progCount++;
			cout << "count" << Progress::progCount << endl;
			flag_do = 1;
			RunResult_Out(ProgQ.running);
		}
		else { flag_do = 0; }
		break;
	}//0-3�����û�̬
	case 4:
	case 5: {
		cout << "����ID��" << ProgQ.running
			<< "������ִ�е�" << CPU_info.psw + 2 << "��ָ����ں�̬������ϵͳ���ã�������������" << endl;
		ProgQ.Set_Waiting(ProgQ.running);//������������
		PCB_table[ProgQ.running].Set_RunTimes();//����������ʱ��
		RunResult_Out(ProgQ.running);
		flag_do = 1;
		break; }
	}
	if (flag_do == 1) {
		ProgQ.running = -1;
		if (!ProgQ.EmptyQueue(ProgQ.ready)) {
			int ok; ProgQ.DeQueue(ProgQ.ready, ok);
			ProgQ.Set_Running(ok);
			cout << "����ID��" << ProgQ.running << "���뿪�������У�����ʼ����" << endl;
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