#pragma once

class CPUinfo {
public:
	int pc;//��Ӧ��һ��Instruc_ID
	//���������,��������������ڴ����һ��ָ�����ڵ�Ԫ�ĵ�ַ�ĵط�����һ��pid
	int ir;//��ӦInstruc_Type
	//ָ��Ĵ���������ʱ���ô��ڴ�����ȡ�õĳ���ָ��ļĴ�����
	//���ڴ�ŵ�ǰ�����洢������������ִ�е�һ��ָ�����ʹ��Inst_type
	int psw;//��ǰInstruct_ID
	//����״̬�֣���ǰָ��id
	//--------����
	CPUinfo();//���캯��
};
CPUinfo::CPUinfo() {
	pc = -1;
	ir = -1;
	psw = -1;
}
