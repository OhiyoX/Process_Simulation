#pragma once
#define PNUM 10//������Ŀ
#define INSTMAX 20

typedef struct Instructs {
	int Instruc_ID;
	int Instruc_State;//ָ���״̬��0��ʾ�û�̬��1��ʾPV��ϵͳ���ò����������������������̲�ӣ���2��ʾϵͳ���ã������ӣ�
	int Instruc_Times;//����ָ������ʱ��
	int Instruc_Type;//ָ�����ͣ�0-3��ʾ�û�̬��ָ�4-5��ʾϵͳ����
}ProgInstructs[INSTMAX];//����ÿһ�����̵�ָ����Ϊ20��
//----------------PCB-DS-------------//
class PCB {
public:
	//----------����
	int PCB_Make(int);//����PCB��Ϣ
	int PCB_outFile();//������ļ�
	int PCB_inPut(int id);//���ļ�����
	PCB();//��ʼ���캯��

	//-----------����
	int Pid;//���̱�ʶ��
	int Priority;
	__int64 InTimes;//���̴���ʱ��
	int ProState;//����״̬��ready=0,waiting=1,running=2
	__int64 RunTimes;//
	int InstrucNum;
	ProgInstructs Inst;//ָ����Ϣ���ݽṹ
	int PSW;//��ǰ����ָ�����

private:
	static __int64 Basetime;//ǰһ�����̵Ĵ���ʱ��
	static int flag;//flag��ʾ�Ƿ��ǵ�һ��ʵ���������ǵ�һ����flag==0������������⣬����outFile����
	static int id_succession;//��ʹ�������ļ���ʱ��id_ptr�����ڶ�λ�������pcb��λ��
};