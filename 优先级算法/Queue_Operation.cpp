#include "Progress_Status.h"
#include <stdlib.h>
#include <stdio.h>

Status_Queue::Status_Queue() {//初始化等待队列和就绪队列
	running = -1;//无进程
	InitQueue(ready);//初始化就绪表
	InitQueue(waiting);//初始化等待表
}

int Status_Queue::InitQueue(LinkQueue &Q) {
	//初始化队列Q
	Q.front = Q.rear = (ProgQueuePtr)malloc(sizeof(ProgNode));
	if (!Q.front) exit(-2);
	Q.front->data = -1;
	Q.front->next = NULL;
	Q.qlen = 0;
	return 1;
}

int Status_Queue::EmptyQueue(LinkQueue &Q) {
	//若队列Q为空，返回1，否则返回0
	if (Q.front == Q.rear&&Q.front != NULL)
		return 1;
	else
		return 0;
}

int Status_Queue::DeQueue(LinkQueue &Q, QElemType &e) {
	//若队列Q不为空，首结点出队，用e返回，并返回1；否则返回0
	if (Q.front == Q.rear) return -1;
	ProgQueuePtr p = Q.front->next;
	e = p->data;
	Q.front->next = p->next;
	if (Q.rear == p) {
		Q.rear = Q.front; Q.front->next = NULL;
	}
	free(p);
	Q.qlen--;
	return 1;
}

int Status_Queue::EnQueue(LinkQueue &Q, QElemType e) {
	//结点e入队Q
	ProgQueuePtr p = (ProgQueuePtr)malloc(sizeof(ProgNode));
	if (!p) {
		printf("Apply for memory error,new element can't enqueue.\n");
		exit(-2);
	}
	p->data = e;
	p->next = NULL;
	Q.rear->next = p;//插入队尾
	Q.rear = p;//修改队尾指针
	Q.qlen++;
	return 1;
}

int Status_Queue::QueueLength(LinkQueue &Q) {
	//返回队列Q的长度，即元素个数
	int count = 0;
	ProgQueuePtr p = Q.front->next;
	while (p) {
		p = p->next;
		count++;
	}
	return count;
}

int Status_Queue::GetHead(LinkQueue &Q, QElemType &e) {
	//若队列Q不为空，用e返回其首结点，并返回1,否则返回0
	if (EmptyQueue(Q))
		return 0;
	e = Q.front->next->data;
	return 1;
}

int Status_Queue::MakeNode(ProgQueuePtr &p, QElemType e) {
	p = (ProgQueuePtr)malloc(sizeof(ProgNode));
	if (!p) exit(-2);
	p->data = e;
	p->next = NULL;
	return 1;
}

int Status_Queue::InsAfter(LinkQueue &Q, ProgQueuePtr &h, QElemType e) {//在h后面插入以e为元素的结点
	ProgQueuePtr q;
	MakeNode(q, e);
	if (h && q)
	{
		q->next = h->next;
		h->next = q;
		Q.qlen++;
		if (h == Q.rear) Q.rear = h->next;//如果h是尾巴，在插入q之后，将q视为尾巴
		return 1;
	}
	else return 0;
}
