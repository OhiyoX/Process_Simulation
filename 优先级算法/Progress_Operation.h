#pragma once
#include "PCB_Struction.h"

//-------------����DS------------//
struct progData {//�������ݿ�
	int stack[10];
};

class PDB {//�������ݿ�
public:
	progData prog[PNUM];
	PDB();//��ʼ��
};


class Progress ://һ�����̵���ɽṹ
	public PCB//PC Block
{
public:
	//----------����-----------//
	static int progCount;//��ɽ��̼���
	//----------Set-����-------------//
	void Set_inTimes();//���ý��̿�ʼʱ��
	void Set_Priority();//�������ȼ�
	void Set_ProState();//���ó���״̬
	void Set_Instruct_State();//����ָ��״̬
	void Set_RunTimes();//���ý�������ʱ��
	void Set_PSW(int inst_id);//����״̬��
	//int Info_Output();//�����ǰ����������Ϣ
	void Info_from_PCB(PCB &pcb);
	int Progress_Switch();//�����л�
	int Progress_Initiation(PCB &pcb_now);//��������
};

//------------------Psedo_Instruction
void Inst_Executing();//ָ��ִ��ģ�⺯��
void Inst_Practice(Instructs &inst,int next_inst_id);//����ִ�й���
void Execute_Running();//ִ���㷨
void RunResult_Out(int abc);//���������Ϣ