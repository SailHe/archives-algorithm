#include "../stdafx.h"
/*****************************************************堆栈*************************************************************/
/*
操作对象集：含>=0个元素的有穷线性表
操作集：	堆栈类型StackSimpleImpl 元素类型 SimpleStackElemType
*/
/*****************************************************堆栈************************************************************/
#define MAX_SIZE_SIMPLESTACK 250
typedef int SimpleStackElemType;
typedef struct StackSimpleImpl *Stacker;
typedef SimpleStackElemType *SimpleStackPosition;
/**************************************************简易数组实现************************************/
struct StackSimpleImpl {
	//Structure
	SimpleStackElemType Data[MAX_SIZE_SIMPLESTACK];
	int top;
};
//建栈 使已生成一个长度为MAX_SIZE_SIMPLESTACK栈初始化为空栈
void SimpleStackInit(Stacker s) {
	memset(s->Data, 0, sizeof(struct StackSimpleImpl));
	s->top = -1;
}
//判断堆栈是否已满
int IsFullSimpleStack(Stacker s) {
	return s->top == MAX_SIZE_SIMPLESTACK - 1;
}
//压入
void PushSimpleStack(Stacker s, SimpleStackElemType item) {
	if (IsFullSimpleStack(s)) {
		puts("堆栈已满");
		return;
	}
	else {
		s->Data[++(s->top)] = item;
		return;
	}
}
//判断堆栈是否为空
int IsEmptySimpleStack(Stacker s) {
	return s->top == -1;
}
//返回栈顶指针
SimpleStackPosition PopSimpleStack(Stacker s) {
	if (IsEmptySimpleStack(s)) {
		return 0;
	}
	else {
		return s->Data + (s->top--);
	}
}

//Demo
int MainForSimpleStack() {
	struct StackSimpleImpl s;
	Stacker ps = &s;
	SimpleStackInit(ps);
	PushSimpleStack(ps, 1);
	PushSimpleStack(ps, 2);
	PushSimpleStack(ps, 3);
	while (!IsEmptySimpleStack(ps)) {
		printf("%d\n", *PopSimpleStack(ps));
	}
	//printf("%d\n", *PopSimpleStack(ps));
	return 0;
}



/*****************************************动态数组双栈************/
enum EnumDeStackTag { DOUBLE_DE_STACK, FIRST_DE_STACK, SECONDE_DE_STACK };//栈标
typedef int DeElementType;
typedef struct DeStackImple *DeStack;
//动态数组实现 && 一组 两栈
typedef struct DeStackImple
{
	DeElementType *Data;
	int Top1;//栈标一
	int Top2;//栈标二
	int MaxSize;//仅在堆栈创建时赋值
};
//初始化堆栈
void InitDeStack(DeStack ps)
{
	//memset(ps->Data, 0, ps->MaxSize*sizeof(DeElementType));
	ps->Top1 = -1;
	ps->Top2 = ps->MaxSize;
}
//创建并初始化一个高度为MaxSize的堆栈
DeStack CreateDeStack(int MaxSize)
{
	DeStack ps = (DeStack)malloc(sizeof(DeStackImple));
	ps->Data = (DeElementType *)malloc(MaxSize * sizeof(DeElementType));
	ps->MaxSize = MaxSize;
	InitDeStack(ps);
	return ps;
}
//判断堆栈是否已满
int IsFullDeStack(DeStack s)
{
	return s->Top1 == (s->Top2 - 1);
}
//压入
void PushDeStack(DeElementType item, int tag, DeStack ps)
{
	if (IsFullDeStack(ps))
		printf("堆栈%d满\n", tag);
	else
	{
		switch (tag)
		{
		case 1:ps->Data[++(ps->Top1)] = item; return;
		case 2:ps->Data[--(ps->Top2)] = item; return;
		default:puts("ERROR tag");
		}
	}
	exit(0);
}
//判断堆栈是否为空 DOUBLE_DE_STACK:整体; FIRST_DE_STACK:栈1; SECONDE_DE_STACK:栈2
int IsEmptyDeStack(DeStack ps, EnumDeStackTag tag)
{
	switch (tag)
	{
	case 0:return ps->Top1 == -1 && ps->Top2 == ps->MaxSize;
	case 1:return ps->Top1 == -1;
	case 2:return ps->Top2 == ps->MaxSize;
	}
}
//返回栈顶
DeElementType PopDeStack(EnumDeStackTag tag, DeStack s)
{
	if (IsEmptyDeStack(s, tag))
		printf("堆栈%d空\n", tag);
	else
	{
		switch (tag)
		{
		case 1:return s->Data[(s->Top1)--];
		case 2:return s->Data[(s->Top2)++];
		default:puts("ERROR tag");
		}
	}
	exit(0);
}

/*************************动态数组双栈实现 后缀表达式 及其 求值****/
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
//将字串a转化为 后缀表达式 但 ps的栈1中也含有结果***乘号不可省略***
void Postfix(char a[], DeStack ps)
{
	DeElementType c;
	int i = 0;
	EnumDeStackTag result = FIRST_DE_STACK, work = SECONDE_DE_STACK;
	while (a[i] != 0)//中缀转后缀
	{
		c = a[i++];
		if (isdigit(c))//运算数-输出到结果
			PushDeStack(c, result, ps);
		else if (Priority(c, false) != -1)//运算符-讨论
		{
			if (c == ')')
			{
				while (ps->Data[ps->Top2] != '(')
					PushDeStack(PopDeStack(work, ps), result, ps);
				PopDeStack(work, ps);
			}
			else
			{
				/*工作栈非空 && c的优先级<=栈顶--将栈顶弹到结果 --【直到】 工作栈为空 || c的优先级>栈顶---将c压栈*/
				while (!IsEmptyDeStack(ps, work) && Priority(c, false) <= Priority(ps->Data[ps->Top2], true))
					PushDeStack(PopDeStack(work, ps), result, ps);
				PushDeStack(c, work, ps);
			}
		}
		else
		{
			printf("ERROR DeElementType:%c", c);
			exit(0);
		}
	}
	while (!IsEmptyDeStack(ps, work))
		PushDeStack(PopDeStack(work, ps), result, ps);
	for (i = 0; i <= ps->Top1; i++)
		a[i] = ps->Data[i];
	a[i] = 0;
}
//返回ps的结果栈中后缀表达式的值
int Comput(char *a, DeStack ps, EnumDeStackTag work, EnumDeStackTag result)
{
	int i = 0;
	DeElementType c, po1, po2;
	InitDeStack(ps);
	while (a[i] != 0)
	{
		c = a[i++];
		if (isdigit(c))
			PushDeStack(c - '0', work, ps);
		else
		{
			if (!IsEmptyDeStack(ps, work))
				po1 = PopDeStack(work, ps);
			else
				puts("ERROR-Comput");
			if (!IsEmptyDeStack(ps, work))//只有一个数字的表达式
				po2 = PopDeStack(work, ps);
			else
			{
				PushDeStack(po1, work, ps);
				continue;
			}
			switch (c)
			{
			case '+':PushDeStack(po2 + po1, work, ps); break;
			case '-':PushDeStack(po2 - po1, work, ps); break;
			case '*':PushDeStack(po2 * po1, work, ps); break;
			case '/':PushDeStack(po2 / po1, work, ps); break;
			}
		}
	}
	return PopDeStack(work, ps);
}

//Demo
int MainForDeStack()
{
	const int MAX_SIZE = 400;
	char ansBuf[MAX_SIZE];
	DeStack ps = CreateDeStack(MAX_SIZE - 1);
	//fgets(ansBuf, 400, stdin);
	gets_s(ansBuf);
	//gets(ansBuf);
	Postfix(ansBuf, ps);
	puts(ansBuf);
	printf("%d\n", Comput(ansBuf, ps, SECONDE_DE_STACK, FIRST_DE_STACK));
	return 0;
}


/**********************************************链栈******Head->Next 是栈顶**********/
typedef int LinkedStackElementType;
typedef struct LinkedStackImpl *LinkedStack;
typedef struct LinkedStackImpl *LinkedStackPosition;
typedef struct LinkedStackImpl
{
	LinkedStackElementType Data;
	LinkedStack Next;
};
//创建一个连在LinkedListNode之前的链栈结点 返回栈头
LinkedStack ConnectLinkedListNode(LinkedStack LinkedListNode)
{
	LinkedStack ps = (LinkedStack)malloc(sizeof(struct LinkedStackImpl));
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
LinkedStackElementType PopLinkedStack(LinkedStack Head)
{
	LinkedStack TopItemNode = Head->Next;
	LinkedStackElementType TopItem = TopItemNode->Data;
	//如果为空IsEmptyLinkedStack(Head) 此处会异常
	Head->Next = Head->Next->Next;
	free(TopItemNode);
	TopItemNode = NULL;
	return TopItem;
}
//销毁堆栈
void DestroyLinkedStack(LinkedStack *ps) {
	while (!IsEmptyLinkedStack(*ps)) {
		PopLinkedStack(*ps);
	}
	free(*ps);
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
