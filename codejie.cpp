/*
  Ҫ�󣺴�txt�ļ�����Դ���򣬸��ݴʷ�����ʶ�𵥴ʣ���д��Ӧ�������ÿ��ʶ����Ķ�Ԫʽ�����ڴʷ���������ʾ������Ϣ������λ��

  ��Ʒ�����

  1.���룺Դ��������ַ�������һ���ַ���������Ϊ�������ݴ棬���⣺���볬����δ����˴�Ϊ�Ż�����
  2.��������ɶ�Ԫʽ(���ͣ�����)

  �汾1������������
  �汾2��txt�ı���ȡ

  3.�������壺
   1.����������input_program
   2.��ָ��pointer
   3.���ڶ�ȡ���ʵ��ַ���temp
   4.�����Զ���״̬������state
   5.��ʶ����identifytable

  4.ͼ�λ������Ż�
*/

#include<iostream>
#include<Windows.h>
#include<iomanip>
#include<stdio.h>
#include<cstring>
#include<string>
#include<fstream>//�ļ�����
using namespace std;
//
const char* key[] = { "do","end","for","if","printf","scanf","then","while","return","int","void","begin"};//�ؼ���
const char* operation[] = { "+","-","*","/","++","--" };//���������
const char* s[] = { "(",")","[","]","{","}" };//����
const char* relation[] = { "<=",">",">=","==","<",">=","!=" };//��ϵ�����
const char* seperator[] = { ",",";" };//�ָ���
const char* logical[] = { "&","|","&&","||","^","!"};//�߼������

//����
string type[] = {"�ؼ���","��ʶ��","���������","����","����","��ϵ�����","�ָ���","Error","�߼������"};

//ȫ�ֱ���˵��
#define Max 5000 //Դ������󻺳���
#define Length 100//��ʶ����ĳ���
#define NumberMax 10//������󻺳���
#define AlphabetMax 10//��ĸ��󻺳���
char input_program[Max];//�ݴ�Դ�������
int pointer = 0;//��ָ��,��ʼ��Ϊ0
char temp;//���ڶ�ȡ�ĵ��ʵ��ַ���
int state;//�Զ���״̬
int row=1;//��¼���ʵ�����
int col=1;//��¼���ʵ�����
char number[NumberMax];//���ֻ�����
char alphabet[AlphabetMax];//��ĸ������
char signal[2];//���Ż�����

//��ʶ����
struct table
{
	string name;//����
	string type;//����
	string value;//�ֽ���ĵ���
	int row;//��������λ��
	int col;//
};

typedef struct {
	table t[Length];//
	int n;//��ʶ����Ԫ�ظ���
}Identifytable;

Identifytable  identifytable;//��ʶ����

//����˵��
void Init();//��ʼ��
bool IsNumber(char s);//�ж��Ƿ�������
bool IsWord(char s);//�ж��Ƿ�Ϊ��ĸ
void print();//���ʶ��Ķ�Ԫʽ
void InsertTable(string , char* , string,int,int);//���뵽��ʶ������
void analysis();//
void Symbol();//������ʶ��
void Number();//��������
void Keyword();//�����ؼ���

int main() {
	Init();//��ʼ��
	int choice;
	fstream f;//�ļ���������
	string str,s1;//s1��ʾÿ�ж������ַ�����strΪƴ�Ӻ�Ľ�������ڴʷ�����
	cout << "��ѡ��ģʽ��1.���������Դ����  2.��ȡ�ı��ļ���Դ����" << endl;
	cin >> choice;
	switch (choice)
	{
	case 1:
		cout << "������Դ���������#��Ϊ����)��" << endl;
		do {          //д��Դ����
			input_program[pointer++] = getchar();
		} while (input_program[pointer - 1] != '#');
		getchar();
		input_program[pointer - 1] = '\0';//ȥ������#
		pointer = 0;//ָ�����
		break;
	case 2:
		f.open("source.txt", ios::in);
		if (!f.is_open())
		{
			cout << "�ļ���ʧ��" << endl;
			return 0;
		}
		cout << "�ı��ļ���Դ��������:" << endl;
		while (getline(f,s1))//��ȡ�ļ���
		{
			cout << s1 << endl;//����ı��Ĵ���
			str += s1;
		}
		str[str.size() - 1] = '\0';
		strcpy_s(input_program, str.c_str());//���ܴ����ַ������bug
		cout << "--------------------------" << endl;
		f.close();//�ر��ļ�
		break;
	default:
		return 0;
		break;
	}
	//
	pointer = 0;
	while(pointer<strlen(input_program))
		 analysis();
	print();//���
	system("pause");
	return 0;
}




void Init() {
	identifytable.n = 0;//��ʼ����ʶ����ĳ���
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
	cout << "-------------------�ʷ������������-------------------" << endl;
	cout << setw(6) << "(  ����" << ","  << "  ����)" << endl;
	cout << "------------------------------------------------------" << endl;
	for (int i = 0; i < identifytable.n; i++)
	{
		if(identifytable.t[i].type=="Error")
			cout << "(  " << identifytable.t[i].type << "," << identifytable.t[i].value << "  )" <<"����λ��:��"<<identifytable.t[i].row<<"�� �� ��"<<identifytable.t[i].col<<"��"<<endl;
		else
		cout << "(  "<<identifytable.t[i].type << "," <<identifytable.t[i].value<< "  )" << endl;
	}
}

//���뵽��ʶ������
void InsertTable(string name,char* ch,string type,int row,int col)
{
	int n = identifytable.n++;//����һ������+1
	identifytable.t[n].name = name;
	identifytable.t[n].type = type;
	int i;
	for ( i = 0; i <=strlen(ch); i++)
		identifytable.t[n].value+= ch[i];//string��̬�����ڴ棬ע�����ʹ���±���ʣ�������������Ϊ�ַ���δ��ʼ��
	identifytable.t[n].row = row;
	identifytable.t[n].col = col;
}

void analysis() {
	//�����ո�ͻ��з�
	while (input_program[pointer] == ' ' || input_program[pointer] == '\n')
	{
		if (input_program[pointer] == '\n')
		{
			row++;
			col = 1;
		}
		pointer++;
	}
	//pointerָ���һ����Ϊ�ո�ͻ��е�λ��
	if (IsNumber(input_program[pointer]))//��Ϊ���������Number
	{
		Number();
		col++;
	}
	else if (IsWord(input_program[pointer])) {//��Ϊ��ĸ�����KeyWord
		Keyword();
		col++;
	}
	else//����
	{
		Symbol();
		col++;
	}
}
//��������
void Number() {
	state = 4;//��ʶ������
	strcpy_s(number, 2, "\0");//���
	int i = 0;
	while (IsNumber(input_program[pointer]))
		number[i++] = input_program[pointer++];//���ִ���
	number[i] = '\0';
	InsertTable("name" + identifytable.n, number, type[state],row-1,col);
	return;
}
//��������
void Symbol() {
	state = 7;//Ĭ��Ϊ����
	bool flag=false;//��Ƿ���λ�� ��Ĭ��Ϊ1λ
	temp = input_program[pointer++];//��ǰ����
	char ch = input_program[pointer];//��ǰ���ŵ���һ�����ţ�����++,>=�����
	strcpy_s(signal, 2, "\0");//��շ��Ż�����
	signal[0] = temp;//
	switch (temp)
	{
	case '+':
        state = 2;//�����������
		if (ch == '+') {
			signal[1] = ch;
			flag = true;
		}
		break;
	case '-':
        state = 2;//���������
		if (ch == '-')
		{
			signal[1] = ch;
			flag = true;
		}
		break;
	case '<':
        state = 5;//�ǹ�ϵ�����
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
		state = 8;//Ĭ�����߼������
		if (ch == '=')
		{
			signal[1] = ch;
			state = 5;//�ǹ�ϵ�����
			flag = true;
		}
		break;
	case '&':
        state = 8;//���߼������
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
		state = 2;//�����������
		break;
	case '/':
		state = 2;
		break;
	default:
		break;
	}
	//�������
	for(int i=0;i<6;i++)
		if (temp == s[i][0])
		{
			state = 3;//������
		}
	//���ָ���
	for (int i = 0; i < 2; i++)
		if (temp == seperator[i][0])
			state = 6;//�ָ���
	//����־
	if (!flag)
		signal[1] = '\0';
	InsertTable("name" + identifytable.n, signal, type[state],row-1,col);
}
//�����ؼ��ֻ��߱�ʶ��
void Keyword() {
	strcpy_s(alphabet, 2, "\0");//�����ĸ������
	int i = 0;
	while (IsWord(input_program[pointer])||IsNumber(input_program[pointer]))//��ȡ��������ĸ��������
	{
		alphabet[i++] = input_program[pointer++];//
	}
	//
	state = 1;//Ĭ���Ǳ�ʶ��
	alphabet[i] = '\0';
	//�ȽϹؼ���,���ҹؼ��ֱ�
	for (int j = 0; j < 12; j++)
	{
		if (strcmp(alphabet, key[j]) == 0)//˵���ǹؼ���
		{
			state = 0;//�ǹؼ���
			break;
		}
		else//�鿴�Ƿ��Ǳ�ʶ��
		{
			for (int j = 0; j < strlen(alphabet); j++)
			{
				if(IsNumber(alphabet[j])||!(IsWord(alphabet[j])))//�����ֿ�ͷ�ģ���������ĸ��ͷ��
					state=7;//error
			}
		}
	}
	//���뵽��ʶ������
	InsertTable("name" + identifytable.n, alphabet, type[state],row-1,col);
}