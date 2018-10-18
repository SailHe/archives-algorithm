/*****************************************************堆栈*************************************************************/
/*
操作对象集：含>=0个元素的有穷线性表
操作集：	堆栈类型stack 元素类型 QElemType
*/
/*****************************************************堆栈************************************************************/
#include <stdio.h>
#include <string.h>
#define size 250
#define ERROR_ELE_VALUE -1

typedef int QElemType;
typedef struct stack *Stack;
/**************************************************数组实现************************************/
struct stack
{
	QElemType Data[size];
	int top;
};
const int LEN = sizeof(struct stack);
//使已生成一个长度为size栈初始化为空栈
void create_stack(Stack s)
{
	memset(s->Data, 0, LEN);
	s->top = -1;
}
//判断堆栈是否已满
int IsFull(Stack s)
{
	return s->top == size - 1;
}
//压入
void push(Stack s, QElemType item)
{
	if (IsFull(s))
	{
		puts("堆栈已满");
		return;
	}
	else
	{
		s->Data[++(s->top)] = item;
		return;
	}
}
//判断堆栈是否为空
int IsEmpty(Stack s)
{
	return s->top == -1;
}
//返回栈顶
QElemType pop(Stack s)
{
	if (IsEmpty(s))
	{
		puts("堆栈空");
		return ERROR_ELE_VALUE;
	}
	else
	{
		return s->Data[s->top--];
	}
}
/*
建栈 ： struct stack s;
Stack ps = &s;
create_stack(ps);
*/
/**************************************************************************动态数组双栈************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#define size 400
enum TAG { cp, one, two };//栈标
typedef int QElemType;
typedef struct stack *Stack;
//动态数组实现 && 一组 两栈
typedef struct stack
{
	QElemType *Data;
	int Top1;//栈标一
	int Top2;//栈标二
	int MaxSize;//仅在堆栈创建时赋值
};
const int LEN = sizeof(stack);
//初始化堆栈
void IndiStack(Stack ps)
{
	//memset(ps->Data, 0, ps->MaxSize*sizeof(QElemType));
	ps->Top1 = -1;
	ps->Top2 = ps->MaxSize;
}
//创建并初始化一个高度为MaxSize的堆栈
Stack Create_Stack(int MaxSize)
{
	Stack ps = (Stack)malloc(LEN);
	ps->Data = (QElemType *)malloc(MaxSize*sizeof(QElemType));
	ps->MaxSize = MaxSize;
	IndiStack(ps);
	return ps;
}
//判断堆栈是否已满
int IsFull(Stack s)
{
	return s->Top1 == (s->Top2 - 1);
}
//压入
void push(QElemType item, int tag, Stack ps)
{
	if (IsFull(ps))
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
//判断堆栈是否为空 cp:整体; one:栈1; two:栈2
int IsEmpty(Stack ps, TAG tag)
{
	switch (tag)
	{
	case 0:return ps->Top1 == -1 && ps->Top2 == ps->MaxSize;
	case 1:return ps->Top1 == -1;
	case 2:return ps->Top2 == ps->MaxSize;
	}
}
//返回栈顶
QElemType pop(TAG tag, Stack s)
{
	if (IsEmpty(s, tag))
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
void Postfix(char a[], Stack ps)
{
	QElemType c;
	int i = 0;
	TAG result = one, work = two;
	while (a[i] != 0)//中缀转后缀
	{
		c = a[i++];
		if (isdigit(c))//运算数-输出到结果
			push(c, result, ps);
		else if (Priority(c, false) != -1)//运算符-讨论
		{
			if (c == ')')
			{
				while (ps->Data[ps->Top2] != '(')
					push(pop(work, ps), result, ps);
				pop(work, ps);
			}
			else
			{
				/*工作栈非空 && c的优先级<=栈顶--将栈顶弹到结果 --【直到】 工作栈为空 || c的优先级>栈顶---将c压栈*/
				while (!IsEmpty(ps, work) && Priority(c, false) <= Priority(ps->Data[ps->Top2], true))
					push(pop(work, ps), result, ps);
				push(c, work, ps);
			}
		}
		else
		{
			printf("ERROR QElemType:%c", c);
			exit(0);
		}
	}
	while (!IsEmpty(ps, work))
		push(pop(work, ps), result, ps);
	for (i = 0; i <= ps->Top1; i++)
		a[i] = ps->Data[i];
	a[i] = 0;
}
//返回ps的结果栈中后缀表达式的值
int Comput(char *a, Stack ps, TAG work, TAG result)
{
	int i = 0;
	QElemType c, po1, po2;
	IndiStack(ps);
	while (a[i] != 0)
	{
		c = a[i++];
		if (isdigit(c))
			push(c - '0', work, ps);
		else
		{
			if (!IsEmpty(ps, work))
				po1 = pop(work, ps);
			else
				puts("ERROR-Comput");
			if (!IsEmpty(ps, work))//只有一个数字的表达式
				po2 = pop(work, ps);
			else
			{
				push(po1, work, ps);
				continue;
			}
			switch (c)
			{
			case '+':push(po2 + po1, work, ps); break;
			case '-':push(po2 - po1, work, ps); break;
			case '*':push(po2 * po1, work, ps); break;
			case '/':push(po2 / po1, work, ps); break;
			}
		}
	}
	return pop(work, ps);
}
int main()
{
	char ans[size];
	Stack ps = Create_Stack(size - 1);
	gets(ans);
	Postfix(ans, ps);
	puts(ans);
	printf("%d\n", Comput(ans, ps, two, one));
	return 0;
}
/*****************************************************************链栈******Head->Next 是栈顶**********/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#define SIZE 400
#define ERROR_ELE_VALUE -1
#define Infinity 1e8
typedef int QElemType;
typedef struct Stack *PStack;
typedef struct Stack
{
	QElemType Data;
	PStack Next;
};
const int LEN = sizeof(struct Stack);
//创建一个连在PN之前的链栈结点 返回栈头
PStack ConnectP(PStack PN)
{
	PStack ps = (PStack)malloc(LEN);
	ps->Next = PN;
	return ps;
}
//创建并返回头式链栈的地址
PStack CreateStack()
{
	return ConnectP(NULL);
}
//压入
void push(QElemType Item, PStack Head)
{
	Head->Next = ConnectP(Head->Next);
	Head->Next->Data = Item;
}
//判断链栈是否为空
int IsEmpty(PStack Head)
{
	return Head->Next == NULL;
}
//返回栈顶
QElemType pop(PStack Head)
{
	QElemType Item;
	PStack SNext = Head->Next;
	if (IsEmpty(Head))
	{
		puts("堆栈空");
		return ERROR_ELE_VALUE;
	}
	else
	{
		Item = Head->Next->Data;
		Head->Next = Head->Next->Next;
		free(SNext);
		SNext = NULL;
		return Item;
	}
}
//销毁堆栈
void deleteStack(PStack *ps){
	while (!IsEmpty(*ps)){
		pop(*ps);
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
//将字串ans转化为 后缀表达式 参数ps运行前后都一定是空栈 ***乘号不可省略***
void Postfix(char ans[], PStack ps)
{
	QElemType c;
	int sub = 0, i = 0;
	while (ans[i] != 0)//中缀转后缀
	{
		c = ans[i++];
		if (isdigit(c))//运算数-输出到结果
			ans[sub++] = c;
		else if (Priority(c, false) != -1)//运算符-讨论
		{
			if (c == ')')
			{
				while (ps->Next->Data != '(')
					ans[sub++] = pop(ps);
				pop(ps);
			}
			else
			{
				/*工作栈非空 && c的优先级<=栈顶--将栈顶弹到结果 --【直到】 工作栈为空 || c的优先级>栈顶---将c压栈*/
				while (!IsEmpty(ps) && Priority(c, false) <= Priority(ps->Next->Data, true))
					ans[sub++] = pop(ps);
				push(c, ps);
			}
		}
		else
		{
			printf("ERROR QElemType:%c", c);
			exit(0);
		}
	}
	while (!IsEmpty(ps))
		ans[sub++] = pop(ps);
	ans[sub] = 0;
}
//在ans不被破坏的前提下 返回后缀表达式ans的值 ps是工作栈不需为空
int Comput(char *ans, PStack ps)
{
	int i = 0;
	QElemType c, po1, po2;
	while (ans[i] != 0)
	{
		c = ans[i++];
		if (isdigit(c))
			push(c - '0', ps);
		else
		{
			if (!IsEmpty(ps))
				po1 = pop(ps);
			else
				puts("ERROR-Comput");
			if (!IsEmpty(ps))//只有一个数字的表达式
				po2 = pop(ps);
			else
			{
				push(po1, ps);
				continue;
			}
			switch (c)
			{
			case '+':push(po2 + po1, ps); break;
			case '-':push(po2 - po1, ps); break;
			case '*':push(po2 * po1, ps); break;
			case '/':push(po2 / po1, ps); break;
			}
		}
	}
	return pop(ps);
}
/*若两个空格之间是操作数则返回该操作 否则将数字存于num中并返回'\0'*/
char getNum(int *i, char const*ori, char *num){
	int sub = 0;
	while (1){
		if (ori[*i] == ' ' || ori[*i] == '\0'){
			num[sub] = '\0';
			if (sub == 1 && !isdigit(num[0]))
				return num[0];
			else
				return '\0';
		}
		else{
			num[sub++] = ori[(*i)++];
			continue;
		}
	}
}
//返回后缀表达式ans的值 空格分开运算元素 可处理负数
QElemType EvalPostfix(char const*ans){
	static char num[50] = { 0 };
	PStack ps = CreateStack();
	int i = 0;
	char c = 0;
	QElemType po1, po2;
	do{
		if ((c = getNum(&i, ans, num)) == '\0'){
			push(atof(num), ps);
		}
		else{
			if (IsEmpty(ps)){//运算符号缺少 或者多余
				deleteStack(&ps);
				return Infinity;
			}
			po1 = pop(ps);
			if (IsEmpty(ps)){
				deleteStack(&ps);
				return Infinity;
			}
			po2 = pop(ps);
			switch (c){
			case '+':push(po2 + po1, ps); break;
			case '-':push(po2 - po1, ps); break;
			case '*':push(po2 * po1, ps); break;
			case '/':{
						 if (po1 - 0.0 == 0){//除以0错误
							 deleteStack(&ps);
							 return Infinity;
						 }
						 else
							 push(po2 / po1, ps); break;
			}
			default:break;
			}
		}
	} while (ans[i++] != 0);
	QElemType re = pop(ps);
	if (IsEmpty(ps)){
		deleteStack(&ps);
		return re;
	}
	else{//运算数多余
		deleteStack(&ps);
		return Infinity;
	}
}
int main()
{
	PStack Head = CreateStack();
	char ans[SIZE] = { 0 };
	gets(ans);
	Postfix(ans, Head);
	puts(ans);
	printf("%d\n", Comput(ans, Head));
	return 0;
}
/*判断一个字串是否回文(堆栈实现)*/
bool plalindrome(char const*str, int len){
	stack<char> s;
	int i = 0;
	while (i < len){
		if (i > len / 2){
		nomalJudge:
			if (s.top() != str[i])
				return false;
			else{
				s.pop();
			}
		}
		else if (i == len / 2){
			if (len % 2 == 0){
				goto nomalJudge;
			}
			else{//当且仅当长度奇数的中间位置需要特别判断
				//do nothing;
			}
		}
		else{
			s.push(str[i]);
		}
		++i;
	}
	return true;
}
/*parenthesisMatching(括号匹配)*/
bool parMat(char const*str, int len){
	stack<char> s;
	for (int i = 0; i < len; ++i){
		/*右括号比对*/
		if (str[i] == ')' || str[i] == ']' || str[i] == '}'){
			char c = 0;
			switch (str[i]){
			case ')':c = '('; break;
			case ']':c = '['; break;
			case '}':c = '{'; break;
			default:break;
			}
			if (s.empty() || s.top() != c)
				return false;
			s.pop();
		}
		/*左括号压栈*/
		else if (str[i] == '(' || str[i] == '[' || str[i] == '{'){
			s.push(str[i]);
		}
		else;//do nothing
	}
	return s.empty();//可能还有多出来的左括号
}
/*返回堆栈操作的合法性*/
bool validityOfStack(char const*str, int len, int cap){
	int size_ = 0;
	for (size_t i = 0; i < len; i++){
		if (str[i] == 'S'){
			if (size_ < cap)
				++size_;
			else
				return false;
		}
		else if (str[i] == 'X'){
			if (size_ > 0)
				--size_;
			else
				return false;
		}
	}
	return size_ == 0;
}
/*
4*(5+4)/2
454+*2/
18

2*(9+6/3-5)*9+4
2963/+5-*9*4+
112
*/
/********************************************************************************************************************/