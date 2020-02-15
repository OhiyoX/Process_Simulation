#pragma once
typedef int QElemType;//������ṹ������������

typedef struct ProgNode {
	QElemType data;
	ProgNode *next;
}ProgNode, *ProgQueuePtr;

typedef struct {
	ProgQueuePtr front;//��ͷָ��
	ProgQueuePtr rear;//��βָ��
	int qlen;
}LinkQueue;

class Status_Queue {
public:
	//-----------����------------
	int running;//����̬,��ʾ����̬�Ľ���id����Ϊ-1��ʾ������̬
	LinkQueue ready;//����̬
	LinkQueue waiting;//����̬

	//----------����--------------
	Status_Queue();//��ʼ���캯��
	void Set_Running(int &pid);
	void Set_Ready(int &pid);//���þ�������
	void Set_Waiting(int &pid);
	void Priority_Decide(LinkQueue &Q, int pid);

	//------------���к���-----------//
	int InitQueue(LinkQueue &Q);
	//��ʼ������Q
	int EmptyQueue(LinkQueue &Q);
	//������QΪ�գ�����TRUE�����򷵻�FALSE
	int DeQueue(LinkQueue &Q, QElemType &e);
	//������Q��Ϊ�գ��׽����ӣ���e���أ�������OK�����򷵻�ERROR
	int EnQueue(LinkQueue &Q, QElemType e);
	//���e���Q
	int QueueLength(LinkQueue &Q);
	//���ض���Q�ĳ��ȣ���Ԫ�ظ���
	int GetHead(LinkQueue &Q, QElemType &e);
	//������Q��Ϊ�գ���e�������׽�㣬������OK,���򷵻�ERROR
	int InsAfter(LinkQueue &Q, ProgQueuePtr &h, QElemType e);//��h���������q���

	int MakeNode(ProgQueuePtr &p, QElemType e);//����һ����eΪԪ�صĽ��
};
