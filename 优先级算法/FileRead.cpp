#include <stdio.h>
#include <stdlib.h>
#include "FileRead.h"
#include "string.h"
#include "math.h"


//-------------���Ӻ���--------------//
int IntOrCharG::FileRead(char* filename) {
	SGroup S;
	FileOpen(S, filename);
	StrgTrim(S);
	return 1;
}



int IntOrCharG::FileOpen(SGroup &S, char* filename) {
	//���ļ��������ַ�����SGroup S
	int i;
	char *file_status;
	char str[FileMax][Rowleng];//��ά����str��
	/*str�����һ�б�ʾΪvexnum��arcnum���ӵڶ�����Ϊ������Ϣ*/
	FILE *fp;
	//printf("please input filename:\n");
	//filename = (char *)"ALGraph.txt";
	if (!(fp = fopen(filename, "r"))) {
		printf("can't open file!\n"); exit(-1);
	}//if
	file_status = str[0];
	S.len = 0;
	for (i = 0;;) {//���ļ���������str���飬����ĩβʱ����ѭ��
		file_status = fgets(S.str[i], Rowleng, fp);
		if (!file_status) break;
		S.len = ++i;
	}//for
	fclose(fp);
	return 1;
}

int IntOrCharG::StrgTrim(SGroup &S) {
	//��S�����е�ע��ȥ��������ȡ����������iorc
	char ss[2];
	SGroup Ssub;
	int i, j, k, signum = 0;
	Ssub.len = 0;//Ssub���ȳ�ʼ��
	for (i = 0, j = 0; i < S.len; i++) {
		strncpy(ss, S.str[i], 2);
		if (!strncmp(ss, "//", 2)) continue;//"//"ʵ��Ϊ'/','/','\0'�����ַ�
		else {
			strcpy(Ssub.str[j++], S.str[i]);
			Ssub.len++;
		}
	}
	for (k = 0; k < Ssub.len; k++) {//k��ʾ��
		LineTrim(Ssub, k);
		IorC_Div(k, signum);
	}
	len = Ssub.len;
	return 1;
}

int IntOrCharG::LineTrim(SGroup &S, int o) {
	//��ĳ�е��ַ�ת��Ϊ��Ӧ������
	int i, j, k;
	char temp[50];//���ֳ���

	for (i = 0, k = 0; i < Rowleng;) {//���ﲻ��ͬʱ��i++�����������ַ�Ϊ����ʱ����
		if (S.str[o][i] == 32 || S.str[o][i] == '\t') {//�ո�
			i++; continue;
		}//�����ո�

		if (S.str[o][i] == 10 || S.str[o][i] == 0) break;//����
		else if (S.str[o][i] > 44 && S.str[o][i] < 65) {//���ַ���ʾ���ֻ����ʱ���Ҳ�������
			j = 0;
			do {
				temp[j++] = S.str[o][i];//��������������
				i++;
			} while (S.str[o][i] != 32 && S.str[o][i] != 10 && S.str[o][i] != 0 && S.str[o][i] != '\t');//��Ϊ�ո񣬲�Ϊ���У���Ϊ\0����Ϊ\t
			temp[j] = '\0';
			R[o].iorcElem[k++].Int = atol(temp);//��temp�ַ���תΪ���ͣ���ֵ��iorc[o][k]
		}
		else {//���ַ���ʾ�ַ�ʱ,/�����ַ�Ϊ0ʱ
			//if (i == 0 && S.str[o][i] == 48) iorc.Y[o][k++].Int = 0;
			R[o].iorcElem[k++].Char = S.str[o][i];
			i++;
		}//else
	}//for
	Rowlen[o] = k;
	return 1;
}

int IntOrCharG::IorC_Div(int o, int &signum) {
	//��IntOrChar������࣬,oΪ�����o�У�ʹ��signum���
	//if (o == 0) iorc.R[o].iorcDiv = signum++;						//iorc�����ʶΪ0���������������붥���ʶ
	//else { 
	R[o].iorcDiv = signum;
	++Divlen[signum];//�����Ա��
	if (R[o].iorcElem[0].Char == '#') {
		signum++;
	}
	//}
	return 1;
}

__int64 IntOrCharG::atol(char * c) {
	int len;		int i;
	__int64 dat = 0;
	len = strlen(c);
	if (c[0] == '-') { //����
		--len;
		if (c[len] == '\r') --len;
		for (i = 1; i < len + 1; i++) {
			dat = dat - __int64((c[i] - '0')*pow(10, (len - i)));
		}
	}
	else {//����
		for (i = 0; i < len; i++) {
			dat = dat + __int64((c[i] - '0')*pow(10, (len - 1 - i)));
		}
	}
	return dat;
}


IntOrCharG::IntOrCharG() {
	int i, j;
	len = 0;
	for (i = 0; i < Lenn; i++) {
		Divlen[i] = 0;
		R[i].iorcDiv = 0;
		for (j = 0; j < Lenn; j++) {
			R[i].iorcElem[j].Int = 0;
			Rowlen[i] = 0;
		}
	}
}