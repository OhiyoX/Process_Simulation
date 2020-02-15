#include "PCB_Struction.h"
#include <Windows.h>
#include <fstream>
#include "FileRead.h"
#include <iostream>
using namespace std;

//��Դ�ļ�����input�ļ����������
int PCB::PCB_Make(int num) {
	Pid = num;

	SYSTEMTIME sys;
	GetLocalTime(&sys);
	srand(sys.wMilliseconds);
	Priority = rand() % 3 + 1;//���ȼ�1-3
	InTimes = num * (10000 / PNUM)+ rand() % (10000 / PNUM);//Ҫ��15second�ڰ����н��̴�����
	Basetime = InTimes;
	ProState = 0;//����״̬��Ĭ��Ϊ�ȴ�
	RunTimes = 0;//��ʼΪ��
	InstrucNum = rand() % 16 + 5;//���5-20��ָ����
	int i;
	for (i = 0; i < InstrucNum; i++) {
		Inst[i].Instruc_ID = i;//����ÿ��ָ���ID
		Inst[i].Instruc_State = rand() % 2;
		switch (Inst[i].Instruc_State) {
		case 0:Inst[i].Instruc_Times = 100 * (rand() % 4 + 1);
			Inst[i].Instruc_Type = rand() % 3;
			break;//�û�̬
		case 1:Inst[i].Instruc_Times = 500;
			Inst[i].Instruc_Type = rand() % 2 + 4;
			break;//PV����
		case 2:Inst[i].Instruc_Times = 100 * (rand() % 4 + 1);
			Inst[i].Instruc_Type = rand() % 3;
			break;//ϵͳ����
		}
	}
	PSW = -1;//Ĭ��Ϊ-1
	return 1;
}

int PCB::PCB_outFile() {
	ofstream pcbFile, pcbFile1;
	if (flag == 0) {
		pcbFile.open("13615206-pcbs-input.txt", ios::out);
		if (!pcbFile.good()) { cout << "open file failed" << endl; exit(0); }
		pcbFile << "//ProID\tPrior\tInTms\tProSt\tRunTms\tInstNum\tInst_ID\tInst_St\tInst_Tm\tInst_Ty\tPSW"
			<< "\n";
		flag = 1;
		pcbFile.close();
	}
	int i;
	pcbFile.open("13615206-pcbs-input.txt", ios::app);
	for (i = 0; i < InstrucNum; i++) {
		pcbFile << Pid << "\t"
			<< Priority << "\t"
			<< InTimes << "\t"
			<< ProState << "\t"
			<< RunTimes << "\t"
			<< InstrucNum << "\t"
			<< Inst[i].Instruc_ID << "\t"
			<< Inst[i].Instruc_State << "\t"
			<< Inst[i].Instruc_Times << "\t"
			<< Inst[i].Instruc_Type << "\t"
			<< PSW << "\n";
	}
	pcbFile << "###\n";
	if (i == InstrucNum) pcbFile.close();
	return 1;
}

PCB::PCB() {//��ʼ���캯��
	Pid = -1;
	Priority = -1;
	InTimes = -1;
	ProState = -1;
	RunTimes = -1;
	InstrucNum = -1;
	int i;
	for (i = 0; i < INSTMAX; i++) {
		Inst[i].Instruc_ID = -1;
		Inst[i].Instruc_State = -1;
		Inst[i].Instruc_Times = -1;
		Inst[i].Instruc_Type = -1;
	}
	PSW = -1;
}

int PCB::PCB_inPut(int id) {//id��ʾ��Ӧ��pid��//Ҳ��G�����
	IntOrCharG G;
	char* filename = (char*)"13615206-pcbs-input.txt";
	G.FileRead(filename);

	Pid = int(G.R[id_succession + 1].iorcElem[0].Int);
	Priority = int(G.R[id_succession + 1].iorcElem[1].Int);
	InTimes = int(G.R[id_succession + 1].iorcElem[2].Int);
	ProState = int(G.R[id_succession + 1].iorcElem[3].Int);
	RunTimes = int(G.R[id_succession + 1].iorcElem[4].Int);
	InstrucNum = int(G.R[id_succession + 1].iorcElem[5].Int);
	int i;
	for (i = 0; i < G.Divlen[id]; i++) {
		int gid;
		if (id == 0) gid = i;
		else gid = id_succession + i;
		Inst[i].Instruc_ID = int(G.R[gid].iorcElem[6].Int);
		Inst[i].Instruc_State = int(G.R[gid].iorcElem[7].Int);
		Inst[i].Instruc_Times = int(G.R[gid].iorcElem[8].Int);
		Inst[i].Instruc_Type = int(G.R[gid].iorcElem[9].Int);
	}
	PSW = int(G.R[id_succession + 1].iorcElem[10].Int);
	id_succession += G.Divlen[id];
	return 1;
}