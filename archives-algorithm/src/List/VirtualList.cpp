#include "../Stack.h"

typedef int ElementTypeList;
typedef struct ArrayLinkedListNode *ArrayLinkedList;
struct ArrayLinkedListNode
{
	ElementTypeList Data;
	//用虚拟空间的下标代表地址
	int Next;
};
ArrayLinkedList CreatArrayLinkedList(int MAX_SIZE);
ArrayLinkedList AddressSpace = CreatArrayLinkedList(100005);
//创建并初始化
ArrayLinkedList CreatArrayLinkedList(int MAX_SIZE)
{
	int LEN_ARRAY_LINKED_LIST = sizeof(struct ArrayLinkedListNode);
	ArrayLinkedList arList = (ArrayLinkedList)malloc(LEN_ARRAY_LINKED_LIST*MAX_SIZE);
	memset(arList, -1, LEN_ARRAY_LINKED_LIST*MAX_SIZE);
	return arList;
}
/*将以root为根的数组链表的前k个元素逆转 subK: 被改变第k个元素的下标 返回新根节点下标*/
int ReversalListKInterval(int k, int *subK, int root, Stack<int> &subStack)
{
	int sub = 0;
	if (root == -1)
		return root;
	for (sub = root; k-- >= 0; sub = AddressSpace[sub].Next) {
		*subK = sub;
		if (sub == -1 && k >= 0)/*不足k个且不是最后一个*/
			return root;
	}
	for (sub = root; sub != *subK; sub = AddressSpace[sub].Next) {
		subStack.push(sub);
		root = sub;
	}
	for (sub = root; !subStack.isEmpty(); sub = AddressSpace[sub].Next)
	{
		AddressSpace[sub].Next = subStack.pop();
	}
	AddressSpace[sub].Next = *subK;/*保证当前的连通*/
	*subK = sub;
	return root;
}
/*
最大N,最后剩K-1不反转
00002 6 2
00000 2 00004
00001 4 00005
00002 1 00000
00003 6 -1
00004 3 00001
00005 5 00003
*/
int mainForRek() {
	int n, k;
	int root, sub, subK;
	Stack<int> subStack;
	while (~scanf("%d%d%d\n", &root, &n, &k)) {
		while (n-- > 0) {
			scanf("%d", &sub);
			scanf("%d%d", &AddressSpace[sub].Data, &AddressSpace[sub].Next);
		}
		puts("");
		root = ReversalListKInterval(k, &subK, root, subStack);
		while (subK != -1 && AddressSpace[subK].Next != -1) {
			sub = subK;
			AddressSpace[sub].Next = ReversalListKInterval(k, &subK, AddressSpace[subK].Next, subStack);
		}
		for (sub = root; sub != -1; sub = AddressSpace[sub].Next) {
			if (AddressSpace[sub].Next == -1)
				break;
			printf("%05d %d %05d\n", sub, AddressSpace[sub].Data, AddressSpace[sub].Next);
		}
		printf("%05d %d %d\n", sub, AddressSpace[sub].Data, AddressSpace[sub].Next);
	}
	return 0;
}




/*******************************链栈实现 后缀表达式 及其 求值****/
//返回运算符ch的优先级 in=true表示是栈内优先级  非运算符返回-1
int priority(int ch, int in)
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
/*若两个空格之间是操作数则返回该操作 否则将数字存于num中并返回'\0'*/
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

/*若两个非数字之间是操作数则返回该操作 否则将数字存于num中并返回'\0'*/
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
//将字串 inBuf 转化为 后缀表达式 参数ps运行前后都一定是空栈 ***乘号不可省略***
void postfix(char const *inBuf, char *ansBuf, Stack<int> &ps)
{
	char c;
	int sub = 0, i = 0, ansSub = 0;
	static char num[50] = { 0 };
	while (inBuf[i] != 0)//中缀转后缀
	{
		c = getNumber(&i, inBuf, num);
		//运算数-输出到结果
		//if (isdigit(c)) inBuf[sub++] = c;
		if (c == '\0') {
			for (int iNum = 0; num[iNum] != '\0' && iNum < 50; ++iNum) {
				ansBuf[ansSub++] = num[iNum];
			}
		}
		else if (priority(c, false) != -1)//运算符-讨论
		{
			if (c == ')')
			{
				while (ps.top() != '(')
					ansBuf[ansSub++] = ps.pop();
				ps.pop();
			}
			else
			{
				/*工作栈非空 && c的优先级<=栈顶--将栈顶弹到结果 --【直到】 工作栈为空 || c的优先级>栈顶---将c压栈*/
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
//在ansBuf不被破坏的前提下 返回后缀表达式ansBuf的值 ps是工作栈不需为空
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
			if (!ps.isEmpty())//只有一个数字的表达式
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

//返回后缀表达式ansBuf的值 空格分开运算元素 可处理负数
int evalPostfix(char const *ansBuf) {
	static char num[50] = { 0 };
	Stack<char> ps;
	int i = 0;
	char c = 0;
	char po1, po2;
	do {
		if ((c = getNum(&i, ansBuf, num)) == '\0') {
			//ps.push(atof(num));
			ps.push(atoi(num));
		}
		else {
			if (ps.isEmpty()) {//运算符号缺少 或者多余
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
				if (po1 - 0.0 == 0) {//除以0错误
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
	else {//运算数多余
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
	Stack<int> stackHead;
	char ansBuf[100] = { 0 };
	//454+*2/    18
	outputValue("4*(5+4)/2", ansBuf, stackHead);
	/*

	//2963/+5-*9*4+    112
	char ansBuf3[20] = "2*(9+6/3-5)*9+4";
	outputValue(ansBuf3, stackHead);

	//这个的输出是错误的
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
