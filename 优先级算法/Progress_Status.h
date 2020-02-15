#pragma once
typedef int QElemType;//索引表结构，采用链队列

typedef struct ProgNode {
	QElemType data;
	ProgNode *next;
}ProgNode, *ProgQueuePtr;

typedef struct {
	ProgQueuePtr front;//队头指针
	ProgQueuePtr rear;//队尾指针
	int qlen;
}LinkQueue;

class Status_Queue {
public:
	//-----------属性------------
	int running;//运行态,表示运行态的进程id，若为-1表示无运行态
	LinkQueue ready;//就绪态
	LinkQueue waiting;//阻塞态

	//----------方法--------------
	Status_Queue();//初始构造函数
	void Set_Running(int &pid);
	void Set_Ready(int &pid);//设置就绪队列
	void Set_Waiting(int &pid);
	void Priority_Decide(LinkQueue &Q, int pid);

	//------------队列函数-----------//
	int InitQueue(LinkQueue &Q);
	//初始化队列Q
	int EmptyQueue(LinkQueue &Q);
	//若队列Q为空，返回TRUE，否则返回FALSE
	int DeQueue(LinkQueue &Q, QElemType &e);
	//若队列Q不为空，首结点出队，用e返回，并返回OK；否则返回ERROR
	int EnQueue(LinkQueue &Q, QElemType e);
	//结点e入队Q
	int QueueLength(LinkQueue &Q);
	//返回队列Q的长度，即元素个数
	int GetHead(LinkQueue &Q, QElemType &e);
	//若队列Q不为空，用e返回其首结点，并返回OK,否则返回ERROR
	int InsAfter(LinkQueue &Q, ProgQueuePtr &h, QElemType e);//在h结点后面插入q结点

	int MakeNode(ProgQueuePtr &p, QElemType e);//创建一个以e为元素的结点
};
