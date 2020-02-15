#pragma once
#define FileMax 400//����ʱ������������
#define Lenn 250//���ݽṹ���������
#define Rowleng 100//ÿ�е�������

typedef struct {
	char str[FileMax][Rowleng];
	int len;
}SGroup;

typedef struct {
	int vexnum, arcnum;
}Graph_Info;

typedef union IntOrCharNode
{
	__int64 Int;
	char Char;
}IntOrChar;//��ÿһ���ַ����з���洢

typedef struct {
	IntOrChar iorcElem[Lenn];//������С��Ԫ
	int iorcDiv;//С���
}IntOrCharRow;//iorc��ÿ��

class IntOrCharG {
public:
	IntOrCharRow R[Lenn];
	int len;
	int Rowlen[Lenn];//��ȡ�����е�lenΪ������rowlenΪ���е�������
	int Divlen[Lenn];//���Ա��
	IntOrCharG();//���캯��
	//==========��������===============//
	//-------------�ӿں���------------//

	int FileRead(char*);

private:
	//--------------���ܺ���---------------//
	int FileOpen(SGroup &S, char*);
	//���ļ��������ַ�����SGroup S

	int StrgTrim(SGroup &S);
	//��S�����е�ע��ȥ��������ȡ����������iorc

	int LineTrim(SGroup &S, int o);
	//��ĳ�е��ַ�ת��Ϊ��Ӧ������,����תΪ����

	int IorC_Div(int o, int&signum);
	//��IntOrChar������࣬ʹ��signum���
	__int64 atol(char * c);
	//char תlong

};





/*-------------INSTRUCTION-------------------
		ʹ�÷�����
		1.
			��Դ�ļ�Ŀ¼�´���"XXX.txt"

		2.��XXX.txt�������¸�ʽ���룺
					//this is the first data array
					xx xx xx xx xx
					//this is the second data group
					xx xx xx
					xx xx xx
					xx xx xx
					###
					xx xx xx
					###
					//end with "###",start at second Group,u must DivideGroup with "###"

		3.���ӿں�����cpp�ļ���header�ļ�������Ŀ���ļ��С�

		4.�Լ����Ű�ɡ�

-----------------------------------------------------*/
