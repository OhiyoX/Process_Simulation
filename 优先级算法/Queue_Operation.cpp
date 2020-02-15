#include "Progress_Status.h"
#include <stdlib.h>
#include <stdio.h>

Status_Queue::Status_Queue() {//��ʼ���ȴ����к;�������
	running = -1;//�޽���
	InitQueue(ready);//��ʼ��������
	InitQueue(waiting);//��ʼ���ȴ���
}

int Status_Queue::InitQueue(LinkQueue &Q) {
	//��ʼ������Q
	Q.front = Q.rear = (ProgQueuePtr)malloc(sizeof(ProgNode));
	if (!Q.front) exit(-2);
	Q.front->data = -1;
	Q.front->next = NULL;
	Q.qlen = 0;
	return 1;
}

int Status_Queue::EmptyQueue(LinkQueue &Q) {
	//������QΪ�գ�����1�����򷵻�0
	if (Q.front == Q.rear&&Q.front != NULL)
		return 1;
	else
		return 0;
}

int Status_Queue::DeQueue(LinkQueue &Q, QElemType &e) {
	//������Q��Ϊ�գ��׽����ӣ���e���أ�������1�����򷵻�0
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
	//���e���Q
	ProgQueuePtr p = (ProgQueuePtr)malloc(sizeof(ProgNode));
	if (!p) {
		printf("Apply for memory error,new element can't enqueue.\n");
		exit(-2);
	}
	p->data = e;
	p->next = NULL;
	Q.rear->next = p;//�����β
	Q.rear = p;//�޸Ķ�βָ��
	Q.qlen++;
	return 1;
}

int Status_Queue::QueueLength(LinkQueue &Q) {
	//���ض���Q�ĳ��ȣ���Ԫ�ظ���
	int count = 0;
	ProgQueuePtr p = Q.front->next;
	while (p) {
		p = p->next;
		count++;
	}
	return count;
}

int Status_Queue::GetHead(LinkQueue &Q, QElemType &e) {
	//������Q��Ϊ�գ���e�������׽�㣬������1,���򷵻�0
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

int Status_Queue::InsAfter(LinkQueue &Q, ProgQueuePtr &h, QElemType e) {//��h���������eΪԪ�صĽ��
	ProgQueuePtr q;
	MakeNode(q, e);
	if (h && q)
	{
		q->next = h->next;
		h->next = q;
		Q.qlen++;
		if (h == Q.rear) Q.rear = h->next;//���h��β�ͣ��ڲ���q֮�󣬽�q��Ϊβ��
		return 1;
	}
	else return 0;
}
