#pragma once
#include "LinkedList.h"

// ���Ӷ�ջ
template<class StackElementType>
class Stack {
public:
	Stack() {
		baseImpl = CreateLinkedStack();
	}
	~Stack() {
		DestroyLinkedStack(&baseImpl);
	}
	void push(StackElementType topEle) {
		PushLinkedStack(topEle, baseImpl);
	}
	StackElementType top() {
		return TopLinkedStack(baseImpl);;
	}
	StackElementType pop() {
		return PopLinkedStack(baseImpl);;
	}
	bool isEmpty() {
		return IsEmptyLinkedStack(baseImpl);
	}
	
protected:
	//������
	void operator = (Stack const &rhs) const {
		return *this;
	}
	Stack(Stack const &rhs) {}
	/**********************************************��ջ******Head->Next ��ջ��**********/
	typedef StackElementType LinkedStackElementType;
	typedef struct LinkedStackImpl *LinkedStackPosition;
	struct LinkedStackImpl
	{
		LinkedStackElementType Data;
		LinkedStackImpl *Next;
	};
	typedef struct LinkedStackImpl *LinkedStack;
	//����һ������LinkedListNode֮ǰ����ջ��� ����ջͷ
	LinkedStack ConnectLinkedListNode(LinkedStack LinkedListNode)
	{
		//LinkedStack ps = (LinkedStack)malloc(sizeof(struct LinkedStackImpl));
		LinkedStack ps = new LinkedStackImpl();
		ps->Next = LinkedListNode;
		return ps;
	}
	//����������ͷʽ��ջ�ĵ�ַ
	LinkedStack CreateLinkedStack()
	{
		return ConnectLinkedListNode(NULL);
	}
	//ѹ��
	void PushLinkedStack(LinkedStackElementType Item, LinkedStack Head)
	{
		Head->Next = ConnectLinkedListNode(Head->Next);
		Head->Next->Data = Item;
	}
	//�ж���ջ�Ƿ�Ϊ��
	int IsEmptyLinkedStack(LinkedStack Head)
	{
		return Head->Next == NULL;
	}
	//����ջ��
	LinkedStackElementType TopLinkedStack(LinkedStack Head)
	{
		LinkedStack TopItemNode = Head->Next;
		return TopItemNode->Data;
	}
	//ɾ��������ջ��
	LinkedStackElementType PopLinkedStack(LinkedStack Head)
	{
		LinkedStack TopItemNode = Head->Next;
		LinkedStackElementType TopItem = TopItemNode->Data;
		//���Ϊ��IsEmptyLinkedStack(Head) �˴����쳣
		Head->Next = Head->Next->Next;
		//free(TopItemNode);
		delete TopItemNode;
		TopItemNode = NULL;
		return TopItem;
	}
	//���ٶ�ջ
	void DestroyLinkedStack(LinkedStack *ps) {
		while (!IsEmptyLinkedStack(*ps)) {
			PopLinkedStack(*ps);
		}
		//free(*ps);
		delete *ps;
		*ps = NULL;
	}

	/*******************************��ջʵ�� ��׺���ʽ ���� ��ֵ****/
	//���������ch�����ȼ� in=true��ʾ��ջ�����ȼ�  �����������-1
	int Priority(int ch, int in)
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
	//���ִ�ansBufת��Ϊ ��׺���ʽ ����ps����ǰ��һ���ǿ�ջ ***�˺Ų���ʡ��***
	void Postfix(char ansBuf[], LinkedStack ps)
	{
		LinkedStackElementType c;
		int sub = 0, i = 0;
		while (ansBuf[i] != 0)//��׺ת��׺
		{
			c = ansBuf[i++];
			if (isdigit(c))//������-��������
				ansBuf[sub++] = c;
			else if (Priority(c, false) != -1)//�����-����
			{
				if (c == ')')
				{
					while (ps->Next->Data != '(')
						ansBuf[sub++] = PopLinkedStack(ps);
					PopLinkedStack(ps);
				}
				else
				{
					/*����ջ�ǿ� && c�����ȼ�<=ջ��--��ջ��������� --��ֱ���� ����ջΪ�� || c�����ȼ�>ջ��---��cѹջ*/
					while (!IsEmptyLinkedStack(ps) && Priority(c, false) <= Priority(ps->Next->Data, true))
						ansBuf[sub++] = PopLinkedStack(ps);
					PushLinkedStack(c, ps);
				}
			}
			else
			{
				printf("ERROR LinkedStackElementType:%c", c);
				exit(0);
			}
		}
		while (!IsEmptyLinkedStack(ps))
			ansBuf[sub++] = PopLinkedStack(ps);
		ansBuf[sub] = 0;
	}
	//��ansBuf�����ƻ���ǰ���� ���غ�׺���ʽansBuf��ֵ ps�ǹ���ջ����Ϊ��
	int Comput(char *ansBuf, LinkedStack ps)
	{
		int i = 0;
		LinkedStackElementType c, po1, po2;
		while (ansBuf[i] != 0)
		{
			c = ansBuf[i++];
			if (isdigit(c))
				PushLinkedStack(c - '0', ps);
			else
			{
				if (!IsEmptyLinkedStack(ps))
					po1 = PopLinkedStack(ps);
				else
					puts("ERROR-Comput");
				if (!IsEmptyLinkedStack(ps))//ֻ��һ�����ֵı��ʽ
					po2 = PopLinkedStack(ps);
				else
				{
					PushLinkedStack(po1, ps);
					continue;
				}
				switch (c)
				{
				case '+':PushLinkedStack(po2 + po1, ps); break;
				case '-':PushLinkedStack(po2 - po1, ps); break;
				case '*':PushLinkedStack(po2 * po1, ps); break;
				case '/':PushLinkedStack(po2 / po1, ps); break;
				}
			}
		}
		return PopLinkedStack(ps);
	}
	/*�������ո�֮���ǲ������򷵻ظò��� �������ִ���num�в�����'\0'*/
	char GetNum(int *i, char const *ori, char *num) {
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
	//���غ�׺���ʽansBuf��ֵ �ո�ֿ�����Ԫ�� �ɴ�����
	LinkedStackElementType EvalPostfix(char const *ansBuf) {
		static char num[50] = { 0 };
		LinkedStack ps = CreateLinkedStack();
		int i = 0;
		char c = 0;
		LinkedStackElementType po1, po2;
		do {
			if ((c = GetNum(&i, ansBuf, num)) == '\0') {
				PushLinkedStack(atof(num), ps);
			}
			else {
				if (IsEmptyLinkedStack(ps)) {//�������ȱ�� ���߶���
					DestroyLinkedStack(&ps);
					return MAX_INT32;
				}
				po1 = PopLinkedStack(ps);
				if (IsEmptyLinkedStack(ps)) {
					DestroyLinkedStack(&ps);
					return MAX_INT32;
				}
				po2 = PopLinkedStack(ps);
				switch (c) {
				case '+':PushLinkedStack(po2 + po1, ps); break;
				case '-':PushLinkedStack(po2 - po1, ps); break;
				case '*':PushLinkedStack(po2 * po1, ps); break;
				case '/': {
					if (po1 - 0.0 == 0) {//����0����
						DestroyLinkedStack(&ps);
						return MAX_INT32;
					}
					else
						PushLinkedStack(po2 / po1, ps); break;
				}
				default:break;
				}
			}
		} while (ansBuf[i++] != 0);
		LinkedStackElementType re = PopLinkedStack(ps);
		if (IsEmptyLinkedStack(ps)) {
			DestroyLinkedStack(&ps);
			return re;
		}
		else {//����������
			DestroyLinkedStack(&ps);
			return MAX_INT32;
		}
	}

	/*
	4*(5+4)/2
	454+*2/
	18

	2*(9+6/3-5)*9+4
	2963/+5-*9*4+
	112
	*/
	int MainForLinkedStack()
	{
		LinkedStack Head = CreateLinkedStack();
		PushLinkedStack(1, Head);
		PushLinkedStack(2, Head);
		PushLinkedStack(3, Head);
		PushLinkedStack(4, Head);
		while (!IsEmptyLinkedStack(Head)) {
			printf("%d\n", PopLinkedStack(Head));
		}
		//printf("%d\n", PopLinkedStack(Head));

		char ansBuf[400] = { 0 };
		gets_s(ansBuf);
		Postfix(ansBuf, Head);
		puts(ansBuf);
		printf("%d\n", Comput(ansBuf, Head));
		return 0;
	}

private:
	//ʵ�ֵ�LinkedStack��API�����Զ������'����' ��˼�ʹ����ָ�� Ҳ����Ҫ��_��׺
	LinkedStack baseImpl;
	//LinkedList<StackElementType> baseImpl;
};
