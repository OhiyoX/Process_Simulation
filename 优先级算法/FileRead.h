#pragma once
#define FileMax 400//是临时数组的最大行数
#define Lenn 250//数据结构的最大行数
#define Rowleng 100//每行的数据数

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
}IntOrChar;//对每一个字符进行分类存储

typedef struct {
	IntOrChar iorcElem[Lenn];//数据最小单元
	int iorcDiv;//小组号
}IntOrCharRow;//iorc组每行

class IntOrCharG {
public:
	IntOrCharRow R[Lenn];
	int len;
	int Rowlen[Lenn];//提取数据行的len为行数，rowlen为单行的数据数
	int Divlen[Lenn];//组成员数
	IntOrCharG();//构造函数
	//==========函数声明===============//
	//-------------接口函数------------//

	int FileRead(char*);

private:
	//--------------功能函数---------------//
	int FileOpen(SGroup &S, char*);
	//打开文件，送入字符数组SGroup S

	int StrgTrim(SGroup &S);
	//将S数组中的注释去掉，并提取数据类型至iorc

	int LineTrim(SGroup &S, int o);
	//将某行的字符转化为相应的类型,数字转为整型

	int IorC_Div(int o, int&signum);
	//将IntOrChar数组分类，使用signum标记
	__int64 atol(char * c);
	//char 转long

};





/*-------------INSTRUCTION-------------------
		使用方法：
		1.
			在源文件目录下创建"XXX.txt"

		2.在XXX.txt中以如下格式输入：
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

		3.将接口函数的cpp文件及header文件加入至目标文件中。

		4.自己看着办吧。

-----------------------------------------------------*/
