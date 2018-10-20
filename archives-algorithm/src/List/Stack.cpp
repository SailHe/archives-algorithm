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

