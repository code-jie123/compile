/*
  要求：从txt文件读入源程序，根据词法规则，识别单词，填写相应表，并输出每次识别出的二元式。存在词法错误则显示错误信息，出错位置

  设计分析：

  1.输入：源程序代码字符串，用一个字符串数组作为缓冲区暂存，问题：输入超出如何处理，此处为优化部分
  2.输出：若干二元式(类型，单词)

  版本1：命令行输入
  版本2：txt文本读取

  3.变量定义：
   1.缓冲区数组input_program
   2.读指针pointer
   3.正在读取单词的字符串temp
   4.保存自动机状态的整数state
   5.标识符表identifytable

  4.图形化界面优化
*/

#include<iostream>
#include<Windows.h>
#include<iomanip>
#include<stdio.h>
#include<cstring>
#include<string>
#include<fstream>//文件操作
using namespace std;
//
const char* key[] = { "do","end","for","if","printf","scanf","then","while","return","int","void","begin"};//关键字
const char* operation[] = { "+","-","*","/","++","--" };//算术运算符
const char* s[] = { "(",")","[","]","{","}" };//括号
const char* relation[] = { "<=",">",">=","==","<",">=","!=" };//关系运算符
const char* seperator[] = { ",",";" };//分隔符
const char* logical[] = { "&","|","&&","||","^","!"};//逻辑运算符

//类型
string type[] = {"关键字","标识符","算术运算符","括号","数字","关系运算符","分隔符","Error","逻辑运算符"};

//全局变量说明
#define Max 5000 //源程序最大缓冲区
#define Length 100//标识符表的长度
#define NumberMax 10//数字最大缓冲区
#define AlphabetMax 10//字母最大缓冲区
char input_program[Max];//暂存源程序代码
int pointer = 0;//读指针,初始化为0
char temp;//正在读取的单词的字符串
int state;//自动机状态
int row=1;//记录单词的行数
int col=1;//记录单词的列数
char number[NumberMax];//数字缓冲区
char alphabet[AlphabetMax];//字母缓冲区
char signal[2];//符号缓冲区

//标识符表
struct table
{
	string name;//名称
	string type;//类型
	string value;//分解出的单词
	int row;//单词所在位置
	int col;//
};

typedef struct {
	table t[Length];//
	int n;//标识符表元素个数
}Identifytable;

Identifytable  identifytable;//标识符表

//函数说明
void Init();//初始化
bool IsNumber(char s);//判断是否是数字
bool IsWord(char s);//判断是否为字母
void print();//输出识别的二元式
void InsertTable(string , char* , string,int,int);//插入到标识符表中
void analysis();//
void Symbol();//分析标识符
void Number();//分析数字
void Keyword();//分析关键字

int main() {
	Init();//初始化
	int choice;
	fstream f;//文件操作对象
	string str,s1;//s1表示每行读出的字符串，str为拼接后的结果，用于词法分析
	cout << "请选择模式：1.命令窗口输入源代码  2.读取文本文件的源代码" << endl;
	cin >> choice;
	switch (choice)
	{
	case 1:
		cout << "请输入源代码程序（以#号为结束)：" << endl;
		do {          //写入源程序
			input_program[pointer++] = getchar();
		} while (input_program[pointer - 1] != '#');
		getchar();
		input_program[pointer - 1] = '\0';//去除最后的#
		pointer = 0;//指针归零
		break;
	case 2:
		f.open("source.txt", ios::in);
		if (!f.is_open())
		{
			cout << "文件打开失败" << endl;
			return 0;
		}
		cout << "文本文件的源代码如下:" << endl;
		while (getline(f,s1))//读取文件行
		{
			cout << s1 << endl;//输出文本的代码
			str += s1;
		}
		str[str.size() - 1] = '\0';
		strcpy_s(input_program, str.c_str());//可能存在字符串溢出bug
		cout << "--------------------------" << endl;
		f.close();//关闭文件
		break;
	default:
		return 0;
		break;
	}
	//
	pointer = 0;
	while(pointer<strlen(input_program))
		 analysis();
	print();//输出
	system("pause");
	return 0;
}




void Init() {
	identifytable.n = 0;//初始化标识符表的长度
	temp = '\0';
	state = 7;
}



bool IsNumber(char s) {
	if (s >= '0' && s <= '9')
		return true;
	else
		return false;
}

bool IsWord(char s) {
	if (s >= 'a' && s <= 'z')
		return true;
	else if (s >= 'A' && s <= 'Z')
		return true;
	else if (s == '$')
		return true;
	else
		return false;
}

void print() {
	cout << "-------------------词法分析结果如下-------------------" << endl;
	cout << setw(6) << "(  类型" << ","  << "  单词)" << endl;
	cout << "------------------------------------------------------" << endl;
	for (int i = 0; i < identifytable.n; i++)
	{
		if(identifytable.t[i].type=="Error")
			cout << "(  " << identifytable.t[i].type << "," << identifytable.t[i].value << "  )" <<"出错位置:第"<<identifytable.t[i].row<<"行 ， 第"<<identifytable.t[i].col<<"列"<<endl;
		else
		cout << "(  "<<identifytable.t[i].type << "," <<identifytable.t[i].value<< "  )" << endl;
	}
}

//插入到标识符表中
void InsertTable(string name,char* ch,string type,int row,int col)
{
	int n = identifytable.n++;//插入一个长度+1
	identifytable.t[n].name = name;
	identifytable.t[n].type = type;
	int i;
	for ( i = 0; i <=strlen(ch); i++)
		identifytable.t[n].value+= ch[i];//string动态分配内存，注意如果使用下标访问，会产生溢出，因为字符串未初始化
	identifytable.t[n].row = row;
	identifytable.t[n].col = col;
}

void analysis() {
	//跳过空格和换行符
	while (input_program[pointer] == ' ' || input_program[pointer] == '\n')
	{
		if (input_program[pointer] == '\n')
		{
			row++;
			col = 1;
		}
		pointer++;
	}
	//pointer指向第一个不为空格和换行的位置
	if (IsNumber(input_program[pointer]))//若为数字则调用Number
	{
		Number();
		col++;
	}
	else if (IsWord(input_program[pointer])) {//若为字母则调用KeyWord
		Keyword();
		col++;
	}
	else//符号
	{
		Symbol();
		col++;
	}
}
//分析数字
void Number() {
	state = 4;//标识是数字
	strcpy_s(number, 2, "\0");//清空
	int i = 0;
	while (IsNumber(input_program[pointer]))
		number[i++] = input_program[pointer++];//数字存入
	number[i] = '\0';
	InsertTable("name" + identifytable.n, number, type[state],row-1,col);
	return;
}
//分析符号
void Symbol() {
	state = 7;//默认为出错
	bool flag=false;//标记符号位数 ，默认为1位
	temp = input_program[pointer++];//当前符号
	char ch = input_program[pointer];//当前符号的下一个符号，例如++,>=的情况
	strcpy_s(signal, 2, "\0");//清空符号缓冲区
	signal[0] = temp;//
	switch (temp)
	{
	case '+':
        state = 2;//是算术运算符
		if (ch == '+') {
			signal[1] = ch;
			flag = true;
		}
		break;
	case '-':
        state = 2;//算术运算符
		if (ch == '-')
		{
			signal[1] = ch;
			flag = true;
		}
		break;
	case '<':
        state = 5;//是关系运算符
		if (ch == '=')
		{
			signal[1] = ch;
			flag = true;
		}
		break;
	case '>':
        state = 5;//
		if (ch == '=')
		{
			signal[1] = ch;
			flag = true;
		}
		break;
	case '!':
		state = 8;//默认是逻辑运算符
		if (ch == '=')
		{
			signal[1] = ch;
			state = 5;//是关系运算符
			flag = true;
		}
		break;
	case '&':
        state = 8;//是逻辑运算符
		if (ch == '&')
		{
			signal[1] = ch;
			flag = true;
		}
		break;
	case '|':
		state = 8;
		if (ch == '|')
		{
			signal[1] = ch;
			flag = true;
		}
		break;
	case '*':
		state = 2;//是算术运算符
		break;
	case '/':
		state = 2;
		break;
	default:
		break;
	}
	//检查括号
	for(int i=0;i<6;i++)
		if (temp == s[i][0])
		{
			state = 3;//是括号
		}
	//检查分隔符
	for (int i = 0; i < 2; i++)
		if (temp == seperator[i][0])
			state = 6;//分隔符
	//检查标志
	if (!flag)
		signal[1] = '\0';
	InsertTable("name" + identifytable.n, signal, type[state],row-1,col);
}
//分析关键字或者标识符
void Keyword() {
	strcpy_s(alphabet, 2, "\0");//清空字母缓冲区
	int i = 0;
	while (IsWord(input_program[pointer])||IsNumber(input_program[pointer]))//读取连续的字母数字序列
	{
		alphabet[i++] = input_program[pointer++];//
	}
	//
	state = 1;//默认是标识符
	alphabet[i] = '\0';
	//比较关键字,查找关键字表
	for (int j = 0; j < 12; j++)
	{
		if (strcmp(alphabet, key[j]) == 0)//说明是关键字
		{
			state = 0;//是关键字
			break;
		}
		else//查看是否是标识符
		{
			for (int j = 0; j < strlen(alphabet); j++)
			{
				if(IsNumber(alphabet[j])||!(IsWord(alphabet[j])))//以数字开头的，不是以字母开头的
					state=7;//error
			}
		}
	}
	//插入到标识符表中
	InsertTable("name" + identifytable.n, alphabet, type[state],row-1,col);
}