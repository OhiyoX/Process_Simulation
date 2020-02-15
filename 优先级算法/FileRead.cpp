#include <stdio.h>
#include <stdlib.h>
#include "FileRead.h"
#include "string.h"
#include "math.h"


//-------------复杂函数--------------//
int IntOrCharG::FileRead(char* filename) {
	SGroup S;
	FileOpen(S, filename);
	StrgTrim(S);
	return 1;
}



int IntOrCharG::FileOpen(SGroup &S, char* filename) {
	//打开文件，送入字符数组SGroup S
	int i;
	char *file_status;
	char str[FileMax][Rowleng];//二维数组str，
	/*str数组第一行表示为vexnum和arcnum，从第二行起为弧的信息*/
	FILE *fp;
	//printf("please input filename:\n");
	//filename = (char *)"ALGraph.txt";
	if (!(fp = fopen(filename, "r"))) {
		printf("can't open file!\n"); exit(-1);
	}//if
	file_status = str[0];
	S.len = 0;
	for (i = 0;;) {//把文件内容送入str数组，读到末尾时结束循环
		file_status = fgets(S.str[i], Rowleng, fp);
		if (!file_status) break;
		S.len = ++i;
	}//for
	fclose(fp);
	return 1;
}

int IntOrCharG::StrgTrim(SGroup &S) {
	//将S数组中的注释去掉，并提取数据类型至iorc
	char ss[2];
	SGroup Ssub;
	int i, j, k, signum = 0;
	Ssub.len = 0;//Ssub长度初始化
	for (i = 0, j = 0; i < S.len; i++) {
		strncpy(ss, S.str[i], 2);
		if (!strncmp(ss, "//", 2)) continue;//"//"实际为'/','/','\0'三个字符
		else {
			strcpy(Ssub.str[j++], S.str[i]);
			Ssub.len++;
		}
	}
	for (k = 0; k < Ssub.len; k++) {//k表示行
		LineTrim(Ssub, k);
		IorC_Div(k, signum);
	}
	len = Ssub.len;
	return 1;
}

int IntOrCharG::LineTrim(SGroup &S, int o) {
	//将某行的字符转化为相应的类型
	int i, j, k;
	char temp[50];//数字长度

	for (i = 0, k = 0; i < Rowleng;) {//这里不能同时用i++，否则会造成字符为数字时跳数
		if (S.str[o][i] == 32 || S.str[o][i] == '\t') {//空格
			i++; continue;
		}//跳过空格

		if (S.str[o][i] == 10 || S.str[o][i] == 0) break;//换行
		else if (S.str[o][i] > 44 && S.str[o][i] < 65) {//当字符表示数字或符号时，且不在首列
			j = 0;
			do {
				temp[j++] = S.str[o][i];//把数字送入数组
				i++;
			} while (S.str[o][i] != 32 && S.str[o][i] != 10 && S.str[o][i] != 0 && S.str[o][i] != '\t');//不为空格，不为换行，不为\0，不为\t
			temp[j] = '\0';
			R[o].iorcElem[k++].Int = atol(temp);//将temp字符组转为整型，赋值给iorc[o][k]
		}
		else {//当字符表示字符时,/或首字符为0时
			//if (i == 0 && S.str[o][i] == 48) iorc.Y[o][k++].Int = 0;
			R[o].iorcElem[k++].Char = S.str[o][i];
			i++;
		}//else
	}//for
	Rowlen[o] = k;
	return 1;
}

int IntOrCharG::IorC_Div(int o, int &signum) {
	//将IntOrChar数组分类，,o为数组第o行，使用signum标记
	//if (o == 0) iorc.R[o].iorcDiv = signum++;						//iorc首组标识为0，其后根据首组输入顶点标识
	//else { 
	R[o].iorcDiv = signum;
	++Divlen[signum];//加组成员数
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
	if (c[0] == '-') { //负数
		--len;
		if (c[len] == '\r') --len;
		for (i = 1; i < len + 1; i++) {
			dat = dat - __int64((c[i] - '0')*pow(10, (len - i)));
		}
	}
	else {//正数
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