#include "stdafx.h"
#include "LinkedList.h"
#include "Stack.h"


/*******************************��ջʵ�� ��׺���ʽ ���� ��ֵ****/
//���������ch�����ȼ� in=true��ʾ��ջ�����ȼ�  �����������-1
int priority(int ch, int in)
{
	int pri = -1;
	if (ch == '(')
		pri = in ? 1 : 4;//'('ջ�����ȼ�1ջ����')'һ��
	else if (ch == '+' || ch == '-')
		pri = 2;
	else if (ch == '*' || ch == '/')
		pri = 3;
	else if (ch == ')')
		pri = 4;
	return pri;
}
/*�������ո�֮���ǲ������򷵻ظò��� �������ִ���num�в�����'\0'*/
char getNum(int *i, char const *ori, char *num) {
	int sub = 0;
	while (1) {
		if (ori[*i] == ' ' || ori[*i] == '\0') {
			num[sub] = '\0';
			if (sub == 1 && !isdigit(num[0]))
				return num[0];
			else
				return '\0';
		}
		else {
			num[sub++] = ori[(*i)++];
			continue;
		}
	}
}

/*������������֮���ǲ������򷵻ظò��� �������ִ���num�в�����'\0'*/
char getNumber(int *i, char const *ori, char *num) {
	int sub = 0;
	while (1) {
		if (!isdigit(ori[*i]) || ori[*i] == '\0') {
			num[sub] = '\0';
			if (sub == 1 && !isdigit(num[0]))
				return num[0];
			else
				return '\0';
		}
		else {
			num[sub++] = ori[(*i)++];
			continue;
		}
	}
}
//���ִ� inBuf ת��Ϊ ��׺���ʽ ����ps����ǰ��һ���ǿ�ջ ***�˺Ų���ʡ��***
void postfix(char const *inBuf, char *ansBuf, Stack<int> &ps)
{
	char c;
	int sub = 0, i = 0, ansSub = 0;
	static char num[50] = { 0 };
	while (inBuf[i] != 0)//��׺ת��׺
	{
		c = getNumber(&i, inBuf, num);
		//������-��������
		//if (isdigit(c)) inBuf[sub++] = c;
		if (c == '\0') {
			for (int iNum = 0; num[iNum] != '\0' && iNum < 50; ++iNum) {
				ansBuf[ansSub++] = num[iNum];
			}
		}
		else if (priority(c, false) != -1)//�����-����
		{
			if (c == ')')
			{
				while (ps.top() != '(')
					ansBuf[ansSub++] = ps.pop();
				ps.pop();
			}
			else
			{
				/*����ջ�ǿ� && c�����ȼ�<=ջ��--��ջ��������� --��ֱ���� ����ջΪ�� || c�����ȼ�>ջ��---��cѹջ*/
				while (!ps.isEmpty() && priority(c, false) <= priority(ps.top(), true))
					ansBuf[ansSub++] = ps.pop();
				ps.push(c);
			}
		}
		else
		{
			printf("ERROR LinkedStackElementType:%c", c);
			exit(0);
		}
	}
	while (!ps.isEmpty())
		ansBuf[ansSub++] = ps.pop();
	ansBuf[ansSub++] = 0;
}
//��ansBuf�����ƻ���ǰ���� ���غ�׺���ʽansBuf��ֵ ps�ǹ���ջ����Ϊ��
int comput(char const *ansBuf, Stack<int> &ps)
{
	int i = 0;
	char c, po1, po2;
	while (ansBuf[i] != 0)
	{
		c = ansBuf[i++];
		if (isdigit(c)) {
			ps.push(c - '0');
		}
		else
		{
			if (!ps.isEmpty())
				po1 = ps.pop();
			else
				puts("ERROR-comput");
			if (!ps.isEmpty())//ֻ��һ�����ֵı��ʽ
				po2 = ps.pop();
			else
			{
				ps.push(po1);
				continue;
			}
			switch (c)
			{
			case '+':ps.push(po2 + po1); break;
			case '-':ps.push(po2 - po1); break;
			case '*':ps.push(po2 * po1); break;
			case '/':ps.push(po2 / po1); break;
			}
		}
	}
	return ps.pop();
}

//���غ�׺���ʽansBuf��ֵ �ո�ֿ�����Ԫ�� �ɴ�����
int evalPostfix(char const *ansBuf) {
	static char num[50] = { 0 };
	Stack<char> ps = Stack<char>();
	int i = 0;
	char c = 0;
	char po1, po2;
	do {
		if ((c = getNum(&i, ansBuf, num)) == '\0') {
			//ps.push(atof(num));
			ps.push(atoi(num));
		}
		else {
			if (ps.isEmpty()) {//�������ȱ�� ���߶���
				//ps.destroy();
				return MAX_INT32;
			}
			po1 = ps.pop();
			if (ps.isEmpty()) {
				//ps.destroy();
				return MAX_INT32;
			}
			po2 = ps.pop();
			switch (c) {
			case '+':ps.push(po2 + po1); break;
			case '-':ps.push(po2 - po1); break;
			case '*':ps.push(po2 * po1); break;
			case '/': {
				if (po1 - 0.0 == 0) {//����0����
					//ps.destroy();
					return MAX_INT32;
				}
				else
					ps.push(po2 / po1); break;
			}
			default:break;
			}
		}
	} while (ansBuf[i++] != 0);
	char re = ps.pop();
	if (ps.isEmpty()) {
		//ps.destroy();
		return re;
	}
	else {//����������
		//ps.destroy();
		return MAX_INT32;
	}
}

void outputValue(char const *inBuf, char *ansBuf, Stack<int> &stackHead) {
	postfix(inBuf, ansBuf, stackHead);
	puts(ansBuf);
	printf("%d\n", comput(ansBuf, stackHead));
}
int main___123()
{
	Stack<int> stackHead = Stack<int>();
	char ansBuf[100] = { 0 };
	//454+*2/    18
	outputValue("4*(5+4)/2", ansBuf, stackHead);
	/*
	
	//2963/+5-*9*4+    112
	char ansBuf3[20] = "2*(9+6/3-5)*9+4";
	outputValue(ansBuf3, stackHead);

	//���������Ǵ����
	char ansBuf4[20] = "23+3*3";
	outputValue(ansBuf4, stackHead);
	
	char ansBuf5[20] = "23+(3*3)";
	outputValue(ansBuf5, stackHead);
	
	char ansBuf[100] = { 0 };
	gets_s(ansBuf);
	outputValue(ansBuf, stackHead);
	*/

	return 0;
}

int MainForStack() {
	Stack<int> s = Stack<int>();
	for (int i = 0; i < 10; ++i) {
		s.push(i);
	}
	while (!s.isEmpty()) {
		printf("%d\n", s.pop());
	}
	
	Stack<string> ss;
	for (int i = 0; i < 10; ++i) {
		ss.push(string("123456"));
	}
	while (!ss.isEmpty()) {
		cout << ss.pop() << endl;
	}

	return 0;
}

//Demo
int MainForLinkedList() {

	LinkedList<string> lists;
	for (int i = 0; i < 10; ++i) {
		lists.insertData("123456");
	}
	auto it = lists.findKth(1);
	while (it->Next_ != NULL) {
		cout << it->Data << endl;
	}

	LinkedList<int> list1 = LinkedList<int>();
	LinkedList<int> list2 = LinkedList<int>();
	LinkedList<int> list3 = LinkedList<int>();
	int n;
	puts("���������Ԫ�� ��-1��β");
	list1.input().output();
	list2.input().output();
	puts("begain\n");

	puts("��ת��list2\n");
	list2.reverse().output();

	puts("�ϲ�list1 list2\n");
	list1.merge(list2).output();
	LinkedList<int>::Iterator iter = list1.findKth(1);
	n = list1.length();
	int i = 0;
	while (iter->Next_) {
		++i;

		LinkedList<int>::Iterator findIter = list1.findKth(i);
		if (findIter != NULL) {
			list3.insertInP(findIter->Data, findIter);
			printf("find ������%d��Ԫ��%d\n", i, findIter->Data);
		}
		else {
			puts("find error");
		}

		LinkedList<int>::Iterator findIterRe = list1.findKth(-1);
		if (findIterRe != NULL) {
			printf("find ������%d��Ԫ��%d\n", 1, findIterRe->Data);
		}
		else {
			puts("find error");
		}

		int deleteTemp = findIterRe->Data;
		LinkedList<int>::Iterator findDataPo = list1.findData(deleteTemp);
		if (list1.deleteInP(findDataPo) != NULL) {
			printf("delete %d\n", deleteTemp);
			list1.output();
		}
		else {
			puts("delete error");
		}
	}
	return 0;
}

int main() {
	MainForStack();
	MainForLinkedList();
	return 0;
}