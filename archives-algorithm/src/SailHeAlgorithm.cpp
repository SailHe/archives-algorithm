#include "SailHeAlgorithm.h"
//#include<bits/stdc++.h> //�����˼��������ļ�
/*__int64 I64d
Rows = r;Cols  = c;//����������
1s һ�����ִ�� 10 ^ 7 �β���
DP:��̬�滮
D:\LaberDate\
�������
XxKkIiLl 10 oO{}{}
Pp
lllll
11111
*/

/*********************************************�� ��********************************************************/

/*qsort(a, ���򳤶�, һ��Ԫ�صĴ�С, intCmp);*/
template<class CmpType>
int lessQsortCmp(const void *min, const void *max){
	return ((CmpType*)min)->year - ((CmpType*)max)->year;
}
template<class CmpType>
int structCmp(const void *min, const void *max){
	int cp = ((CmpType*)min)->value - ((CmpType*)max)->value;//������ֵ ��С��������
	if (cp == 0)
		cp = ((CmpType*)max)->value2 - ((CmpType*)min)->value2;//���μ�ֵ �Ӵ�С����
	return cp;
}
/*sort(a, a+n, boolCmp)*/
template<class CmpType>
bool lessSortCmp(CmpType min, CmpType max){
	return min.value < max.value;
}
//ð������
void BubbleSort(int a[], int n, int K, int cmp(const void *a, const void *b)){
	//������a�е�n��Ԫ�ؽ��еĵ�K��
	int temp = 0;
	//K�������ʱ���ܼ�һ
	for (int i = 0; i < K; i++){
		//�˴������һ
		for (int j = 0; j < n - i - 1; j++){
			if (cmp(&a[j + 1], &a[j])){
				temp = a[j + 1];
				a[j + 1] = a[j];
				a[j] = temp;
			}
		}
	}
}
//ѡ������
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
//����n�� ���������
void buildPtriangle(int n, int table[][30]){
	table[0][0] = 1;
	for (int r = 1; r < n; r++){
		//ÿ�е����ָ���=����+1(������0��ʼ)
		//����Ҫÿ��ǰ��Ŀո�Ļ� ÿ�е�һ������ǰ��Ŀո��� = 2*[(maxR-currentR(1��ʼ))����һ�е�������]  ==> 2*(n-r-1)
		//int blakCnt = 0;
		for (int c = 0; c < r + 1; c++){
			if (c != 0)
				table[r][c] = table[r - 1][c] + table[r - 1][c - 1];
			else
				table[r][c] = table[r - 1][c];
		}
	}
}
//��ӡn�е��������
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



/************************************************̰��**********************************************************/
// ������͵�ʵ�����
void MiLIGen(double u, double v1){
	printf("Q = %f e-19\n", (1e19*3.16e-8*5e-3 / u*pow(v1, 1.5)));
}

// ����: �����Ӵ�s1��s2ƥ����ַ��� �Ƚϳ���len)
int countMatchingChar(char *s1, char *s2, int len){
	int i, count;
	for (i = count = 0; s1[i] && s2[i] && len--; ++i){
		s1[i] == s2[i] ? count++ : 0;
	}
	return count;
}

// �ж�һ���ִ��Ƿ����(��ջʵ��)
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
			else {//���ҽ��������������м�λ����Ҫ�ر��ж�
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
// �жϸ����ִ��Ƿ�ӵ��ƥ�������
bool isMatchingParenthesis(char const*str, int len) {
	stack<char> s;
	for (int i = 0; i < len; ++i) {
		//�����űȶ�
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
		//������ѹջ
		else if (str[i] == '(' || str[i] == '[' || str[i] == '{') {
			s.push(str[i]);
		}
		else {
			//do nothing
		}
	}
	//���ܻ��ж������������
	return s.empty();
}
// �жϸ�����ջ�����Ƿ�Ϸ�
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
����ö��+̰��
http://acm.hdu.edu.cn/showproblem.php?pid=4004
6 1 2
2

25 3 3
11
2
18
*/
//̰��
bool canJump(int s[], int n, int m, int jumpPower) {
	int step = 0;
	int from = 0;
	int to = 1;
	int out = true;
	//�����յ�ʱ����
	while (to < n) {
		out = true;
		while (to < n && s[to] - s[from] <= jumpPower) {
			//̰�ģ�ͨ�������ܶ��ʯͷ
			++to;
			out = false;
		}
		from = to - 1;
		++step;
		//˵��jumpPowerС�ˣ��еĵط�������ȥ  || ��������
		if (out || step > m)
			return false;
	}
	return true;
}
int solveFrogJump() {
	//�ӿ�L, ʯͷ����ΪN, �����M��, ����С������Ծ��
	int L, N, M;
	while (cin >> L >> N >> M) {
		N += 2;
		//st[n]��ʾ��n��ʯͷ������ʼ��ľ���
		st[0] = 0;
		for (int i = 1; i < N - 1; scanf("%d", &st[i++]));
		sort(st + 1, st + N - 1);
		int left = st[1];
		int right = st[N - 1] = L;
		st[0] = 0;
		//����ö��(ȡһ��ʹ��canJump��������Сֵ jumpPower ��ʼʱ�ǵ�һ��ʯͷ�ľ���)
		while (left < right) {
			int mid = (left + right) >> 1;
			//��������С��m����ǰ��������ȥ ��ô����СһЩ��jumpPower,��Ҫ������һ�ε�jumpPowerֵ��mid����1
			if (canJump(st, N, M, mid)) {
				right = mid;
			}
			else {
				//���޷�������ǰ���£�midֵ���豣��
				left = mid + 1;
			}
		}
		cout << left << endl;
	}
	return 0;
}



