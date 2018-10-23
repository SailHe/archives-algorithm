#include "SailHeAlgorithm.h"
//#include<bits/stdc++.h> //包涵了几乎所有文件
/*__int64 I64d
Rows = r;Cols  = c;//报表行列数
1s 一般可以执行 10 ^ 7 次操作
DP:动态规划
D:\LaberDate\
字体测试
XxKkIiLl 10 oO{}{}
Pp
lllll
11111
*/

/*********************************************排 序********************************************************/

/*qsort(a, 排序长度, 一个元素的大小, intCmp);*/
template<class CmpType>
int lessQsortCmp(const void *min, const void *max){
	return ((CmpType*)min)->year - ((CmpType*)max)->year;
}
template<class CmpType>
int structCmp(const void *min, const void *max){
	int cp = ((CmpType*)min)->value - ((CmpType*)max)->value;//按主价值 从小到大排序
	if (cp == 0)
		cp = ((CmpType*)max)->value2 - ((CmpType*)min)->value2;//按次价值 从大到小排序
	return cp;
}
/*sort(a, a+n, boolCmp)*/
template<class CmpType>
bool lessSortCmp(CmpType min, CmpType max){
	return min.value < max.value;
}
//冒泡排序
void BubbleSort(int a[], int n, int K, int cmp(const void *a, const void *b)){
	//对数组a中的n个元素进行的第K次
	int temp = 0;
	//K代表遍数时不能减一
	for (int i = 0; i < K; i++){
		//此处必须减一
		for (int j = 0; j < n - i - 1; j++){
			if (cmp(&a[j + 1], &a[j])){
				temp = a[j + 1];
				a[j + 1] = a[j];
				a[j] = temp;
			}
		}
	}
}
//选择排序
template<class CmpType>
void SlectSort(CmpType a[], CmpType *an, int(*cmp)(CmpType *a, CmpType *b)){
	for (; a < an - 1; ++a){
		for (CmpType *j = a + 1; j < an; ++j){
			if (cmp(j, a)){
				swap(*a, *j);
			}
		}
	}
}

/**************************************************************************/
//生成n行 的杨辉三角
void buildPtriangle(int n, int table[][30]){
	table[0][0] = 1;
	for (int r = 1; r < n; r++){
		//每行的数字个数=行数+1(行数从0开始)
		//若需要每行前面的空格的话 每行第一个数字前面的空格数 = 2*[(maxR-currentR(1开始))的那一行的数字数]  ==> 2*(n-r-1)
		//int blakCnt = 0;
		for (int c = 0; c < r + 1; c++){
			if (c != 0)
				table[r][c] = table[r - 1][c] + table[r - 1][c - 1];
			else
				table[r][c] = table[r - 1][c];
		}
	}
}
//打印n行的杨辉三角
void PrintPtriangle(int n, int table[][30]){
	for (int pr = 0; pr < n; pr++){
		for (int pc = 0; pc < n; pc++){
			if (table[pr][pc] != 0)
				printf("%d", table[pr][pc]);
			if (pc < pr){
				printf(" ");
			}
			else if (pc == pr && pr != n - 1)
				puts("");
		}
	}
	puts("\n");
}



/************************************************贪心**********************************************************/
// 密里根油滴实验程序
void MiLIGen(double u, double v1){
	printf("Q = %f e-19\n", (1e19*3.16e-8*5e-3 / u*pow(v1, 1.5)));
}

// 计数: 返回子串s1与s2匹配的字符数 比较长度len)
int countMatchingChar(char *s1, char *s2, int len){
	int i, count;
	for (i = count = 0; s1[i] && s2[i] && len--; ++i){
		s1[i] == s2[i] ? count++ : 0;
	}
	return count;
}

// 判断一个字串是否回文(堆栈实现)
bool isPlalindrome(char const*str, int len) {
	stack<char> s;
	int i = 0;
	while (i < len) {
		if (i > len / 2) {
		nomalJudge:
			if (s.top() != str[i])
				return false;
			else {
				s.pop();
			}
		}
		else if (i == len / 2) {
			if (len % 2 == 0) {
				goto nomalJudge;
			}
			else {//当且仅当长度奇数的中间位置需要特别判断
				//do nothing;
			}
		}
		else {
			s.push(str[i]);
		}
		++i;
	}
	return true;
}
// 判断给定字串是否拥有匹配的括号
bool isMatchingParenthesis(char const*str, int len) {
	stack<char> s;
	for (int i = 0; i < len; ++i) {
		//右括号比对
		if (str[i] == ')' || str[i] == ']' || str[i] == '}') {
			char c = 0;
			switch (str[i]) {
			case ')':c = '('; break;
			case ']':c = '['; break;
			case '}':c = '{'; break;
			default:break;
			}
			if (s.empty() || s.top() != c)
				return false;
			s.pop();
		}
		//左括号压栈
		else if (str[i] == '(' || str[i] == '[' || str[i] == '{') {
			s.push(str[i]);
		}
		else {
			//do nothing
		}
	}
	//可能还有多出来的左括号
	return s.empty();
}
// 判断给定堆栈操作是否合法
bool isValidityOfStack(char const*str, int len, int cap) {
	int size_ = 0;
	for (int i = 0; i < len; ++i) {
		if (str[i] == 'S') {
			if (size_ < cap)
				++size_;
			else
				return false;
		}
		else if (str[i] == 'X') {
			if (size_ > 0)
				--size_;
			else
				return false;
		}
	}
	return size_ == 0;
}



int st[5/*00002*/];
/*
二分枚举+贪心
http://acm.hdu.edu.cn/showproblem.php?pid=4004
6 1 2
2

25 3 3
11
2
18
*/
//贪心
bool canJump(int s[], int n, int m, int jumpPower) {
	int step = 0;
	int from = 0;
	int to = 1;
	int out = true;
	//到达终点时结束
	while (to < n) {
		out = true;
		while (to < n && s[to] - s[from] <= jumpPower) {
			//贪心，通过尽可能多的石头
			++to;
			out = false;
		}
		from = to - 1;
		++step;
		//说明jumpPower小了，有的地方跳不过去  || 步数过多
		if (out || step > m)
			return false;
	}
	return true;
}
int solveFrogJump() {
	//河宽L, 石头数量为N, 最多跳M次, 求最小所需跳跃力
	int L, N, M;
	while (cin >> L >> N >> M) {
		N += 2;
		//st[n]表示第n块石头距离起始点的距离
		st[0] = 0;
		for (int i = 1; i < N - 1; scanf("%d", &st[i++]));
		sort(st + 1, st + N - 1);
		int left = st[1];
		int right = st[N - 1] = L;
		st[0] = 0;
		//二分枚举(取一个使得canJump成立的最小值 jumpPower 初始时是第一块石头的距离)
		while (left < right) {
			int mid = (left + right) >> 1;
			//若可以在小于m步的前提下跳过去 那么尝试小一些的jumpPower,但要保留上一次的jumpPower值：mid不减1
			if (canJump(st, N, M, mid)) {
				right = mid;
			}
			else {
				//在无法跳过的前提下，mid值无需保留
				left = mid + 1;
			}
		}
		cout << left << endl;
	}
	return 0;
}



