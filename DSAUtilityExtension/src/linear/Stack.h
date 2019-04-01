#pragma once
#include "LinkedList.h"

// 链接堆栈
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
	//不允许
	void operator = (Stack const &rhs) const {
		return *this;
	}
	Stack(Stack const &rhs) {}
	/**********************************************链栈******Head->Next 是栈顶**********/
	typedef StackElementType LinkedStackElementType;
	typedef struct LinkedStackImpl *LinkedStackPosition;
	struct LinkedStackImpl
	{
		LinkedStackElementType Data;
		LinkedStackImpl *Next;
	};
	typedef struct LinkedStackImpl *LinkedStack;
	//创建一个连在LinkedListNode之前的链栈结点 返回栈头
	LinkedStack ConnectLinkedListNode(LinkedStack LinkedListNode)
	{
		//LinkedStack ps = (LinkedStack)malloc(sizeof(struct LinkedStackImpl));
		LinkedStack ps = new LinkedStackImpl();
		ps->Next = LinkedListNode;
		return ps;
	}
	//创建并返回头式链栈的地址
	LinkedStack CreateLinkedStack()
	{
		return ConnectLinkedListNode(NULL);
	}
	//压入
	void PushLinkedStack(LinkedStackElementType Item, LinkedStack Head)
	{
		Head->Next = ConnectLinkedListNode(Head->Next);
		Head->Next->Data = Item;
	}
	//判断链栈是否为空
	int IsEmptyLinkedStack(LinkedStack Head)
	{
		return Head->Next == NULL;
	}
	//返回栈顶
	LinkedStackElementType TopLinkedStack(LinkedStack Head)
	{
		LinkedStack TopItemNode = Head->Next;
		return TopItemNode->Data;
	}
	//删除并返回栈顶
	LinkedStackElementType PopLinkedStack(LinkedStack Head)
	{
		LinkedStack TopItemNode = Head->Next;
		LinkedStackElementType TopItem = TopItemNode->Data;
		//如果为空IsEmptyLinkedStack(Head) 此处会异常
		Head->Next = Head->Next->Next;
		//free(TopItemNode);
		delete TopItemNode;
		TopItemNode = NULL;
		return TopItem;
	}
	//销毁堆栈
	void DestroyLinkedStack(LinkedStack *ps) {
		while (!IsEmptyLinkedStack(*ps)) {
			PopLinkedStack(*ps);
		}
		//free(*ps);
		delete *ps;
		*ps = NULL;
	}

	/*******************************链栈实现 后缀表达式 及其 求值****/
	//返回运算符ch的优先级 in=true表示是栈内优先级  非运算符返回-1
	int Priority(int ch, int in)
	{
		int pri = -1;
		if (ch == '(')
			pri = in ? 1 : 4;//'('栈内优先级1栈外与')'一样
		else if (ch == '+' || ch == '-')
			pri = 2;
		else if (ch == '*' || ch == '/')
			pri = 3;
		else if (ch == ')')
			pri = 4;
		return pri;
	}
	//将字串ansBuf转化为 后缀表达式 参数ps运行前后都一定是空栈 ***乘号不可省略***
	void Postfix(char ansBuf[], LinkedStack ps)
	{
		LinkedStackElementType c;
		int sub = 0, i = 0;
		while (ansBuf[i] != 0)//中缀转后缀
		{
			c = ansBuf[i++];
			if (isdigit(c))//运算数-输出到结果
				ansBuf[sub++] = c;
			else if (Priority(c, false) != -1)//运算符-讨论
			{
				if (c == ')')
				{
					while (ps->Next->Data != '(')
						ansBuf[sub++] = PopLinkedStack(ps);
					PopLinkedStack(ps);
				}
				else
				{
					/*工作栈非空 && c的优先级<=栈顶--将栈顶弹到结果 --【直到】 工作栈为空 || c的优先级>栈顶---将c压栈*/
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
	//在ansBuf不被破坏的前提下 返回后缀表达式ansBuf的值 ps是工作栈不需为空
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
				if (!IsEmptyLinkedStack(ps))//只有一个数字的表达式
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
	/*若两个空格之间是操作数则返回该操作 否则将数字存于num中并返回'\0'*/
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
	//返回后缀表达式ansBuf的值 空格分开运算元素 可处理负数
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
				if (IsEmptyLinkedStack(ps)) {//运算符号缺少 或者多余
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
					if (po1 - 0.0 == 0) {//除以0错误
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
		else {//运算数多余
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
	//实现的LinkedStack的API可以自动管理该'类型' 因此即使其是指针 也不需要加_后缀
	LinkedStack baseImpl;
	//LinkedList<StackElementType> baseImpl;
};
