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

//շת�����Լ��, ����(����)�ж�
int gcd(int a, int b)
{
	return b == 0 ? a : gcd(b, a % b);
}
/*
��չ��ŷ������㷨��շת�������Extended Euclidean algorithm:
����:
	��֪����a��b, �����a��b�����Լ����ͬʱ, ���ҵ����㱴���ʽax + by = gcd(a, b)������x��y;��Ҳ��������, ����һ���ܿ����Ǹ�����
��a�Ǹ��������԰�����ת����|a|(-x) + by = gcd(|a|, b)��Ȼ���� x' = (-x)
	���ö��������Է��̵Ľ�*//*
����ֵ      :��� Լ��,����,���� gcd(greatest common divisor) �� hcf(highest common factor)
��֪����a, b:����Ǹ�����(���Ǹ�������Ҫ�����㷨)
���ز���x, y:������ �� ģ��Ԫ...
*//*
�����ʽ(������)����:���������Լ���ı����������������������������ʾ
����δ֪��x��y�ķ��� ax + by = m��������<==>m��d�ı�����(d����������a��b�����Լ��)
�����ʽ�н�ʱ��Ȼ�������������⣬ÿ���x y����Ϊ��������������չŷ������㷨��á�
Ӧ��:
���ڲ�����������ax+by=c����c �� gcd(a,b)����������÷��̴���������
���򲻴��� x , y �����⡣
�����Ƿ����һ����k��ʹ��m��kȦ���n��һ��������⣺ k*m - n = 1;�Ƿ��н�
1 % gcd(m,n) == 0
*/
int gcdEx_OLD(int a, int b, int &x, int &y){
	if (b == 0){
		x = 1;
		y = 0;
		return a;
	}
	else{
		int g = gcdEx_OLD(b, a%b, x, y);
		int t = x;
		x = y;
		y = t - (a / b)*y;
		return g;
	}

}
//��չŷ������㷨������ʽ��
int gcdEx(int a, int b, int &x, int &y){
	if (b == 0){
		x = 1; y = 0;
		return a;
	}
	int g = gcdEx(b, a%b, y, x);
	y -= a / b*x;
	return g;
}
I64 gcdEx(I64 a, I64 b, I64 &x, I64 &y){
	if (b == 0){
		x = 1; y = 0;
		return a;
	}
	I64 g = gcdEx(b, a%b, y, x);
	y -= a / b*x;
	return g;
}
/*
����һ(���涨��)��ax + by = gcd(a, b)����������
���������gcd(a, b) = 1���򷽳�ax �� c (mod b)��[0, b-1]����Ψһ��.
֤��:���ڷ��� ax + by = c
<==> ax �� c (mod b)			==> �����̴����ؽ�x,��ôx + k*b���Ƿ��̵Ľ�
����������gcd(a, b) = g���򷽳�ax �� c (mod b)��[0, b/g - 1]����Ψһ��.
֤��:���ڷ��� a/g*x+b/g*y = c/g	(��x,y��������; g = gcd(a, b) ==> ��������Ǹ����� ==> �ұ�Ҳ�������� ==> ����x,y����������)
<==> a/g*x �� c/g (mod b/g)	==> �����̴����ؽ�x,��ôx + k*(b/g)���Ƿ��̵Ľ�
*/
//PKU1061-���ܵ�Լ��
//�����Է���ax+by = c����С�Ǹ�������x(ֻ�ܱ�֤x��������) �������ⲻ���ڷ���false
int linearEquation(I64 a, I64 &x, I64 b, I64 &y, I64 c){
	I64 x0, y0,
		g = gcdEx(a, b, x0, y0);
	if (c%g != 0)
		return false;
	I64 rx = b / g;
	x = x0*c / g;
	x = (x%rx + rx) % rx;
	//while (x < 0 && rx > 0) x += rx; x %= rx;
	y = (c - a*x) / b;
	return true;
}
//PKU2142-HDU1356-The Balance
/*
�������ڷ���ax+by = c
ͨ��:	x = x0 + b*t
y = y0 - a*t
(��:x0, y0Ϊ���̵�һ���; ��:gcd(a, b) = 1; ��:tΪ��һ����.)

��:	g = gcdEx(a, b, x0, y0);
x0 = x0*c / g;
y0 = y0*c / g;
��:	x = x0 + b / g*t
y = y0 - a / g*t
����		|x|+|y| = |x0+b/g*t| + |y0-a/g*t|
��֪		|x0+b/g*t|����������|y0-a/g*t|�����ݼ�.
���涨	a>b
��ô		ԭ����Ϊ�ȼ������İ�ͼ�� (��t����y0 - a/g*t == 0 ʱȡ��Сֵ)
��		|x|+|y| ��t = y0*g/a ����(��)ȡ��Сֵ
*/
//�����Է���ax+by = c ʹ��|x|+|y|��С��һ���x,y
void linearEquation(int a, int &x, int b, int &y, int c){
	int hasSwap = false;
	if (a < b) swap(a, b), hasSwap = true;
	I64 x0, y0;
	I64 g = gcdEx(a, b, x0, y0);
	x0 = x0*c / g;
	y0 = y0*c / g;
	int tB = y0*g / a;
	x = infinity / 2, y = x;
	FOR(t, tB - 5, tB + 5){
		int xT = x0 + b / g*t;
		int yT = y0 - a / g*t;
		//�˴�ȡ�����
		if (abs(xT) + abs(yT) < abs(x) + abs(y)){
			x = xT;
			y = yT;
		}
	}
	if (hasSwap) swap(x, y);
}
/*
modulo inverse
����:����a��ģm��Ԫt, �����ڴ�ӡ���󲢽�������;
a*modInv(a, m) % m = 1
��Ϊat%m=1��Ҫ������2�Ļ���2*at��Ҫ������Ϊ3�Ļ���3*at
����

ͬ��� ��: ��һ������������������ ����ͬ��������ƶ�����[ͬ��]��
ͬ���ϵ(��ɢ��ѧ):aRb <==> a��b��m��ʱ������ͬ������r, ��ʱ, ��m��ģ��, ��a��b(mod m); (modulo); ����:a��bģmͬ��
ͬ���ϵ��һ��[�ȼ۹�ϵ]:�Է�,�Գ�,����

ͬ����(congruence class��residue class):�ɶ���ģnͬ�������������ɵ�һ������; ����������֪��ģn����Ҳ�ɱ��Ϊ[a]��
ͬ����Ĵ�����(representative):ͬ�����е����������ͬ�������һԪ��

ģ��Ԫ: t �� a^(-1)(mod m) �� at �� 1(mod m); ��at��1ģmͬ��, ��t��a�Ķ�ͬ��m��ģ��Ԫ, Ҳ����ģm�����۵���;
��ax + my = 1 <==> (1 �� 1) ax+my �� 1 �� ax(mod m) ==> ax �� 1; ���ݶ���, x����a����ģm��һ��ģ��Ԫ��
*/
int modInv(int a, int m){
	int t, y;
	if (gcdEx(a, m, t, y) != 1) {
		cerr << a << " " << m << "������ ģ��Ԫ������!" << endl;
		exit(-1);
	}
	return t;
}
/*
�й�ʣ�ඨ��: ans = sum(a[i] * t[i] * M[i])
+ k*ans(��ģmP�������·�����ֻ��ansһ����)
����ֵ:����ͬʱ%ģ����ֱ���������������num
��֪����m:ģ����(Ҫ��m������������, �޲������)
��֪����a:������(ǿ����)				num%m[i] = r[i]
numģm[i]ͬ���������(������)	num �� a[i](mod m[i])
��������:
�ﲻ֪��(xx��֮x����ģ��), ��������(������ģ�� ��һ�����Ե�������ͬ�������)
*/
int chineseReminder(int *m, int *a, int n, int mP = 0){
	int ans, t, y, i;
	//�����ظ�����, �����ڱ����μ���ĵط��ֿ���ѡ��Ĭ�ϲ���, ���������
	if (mP == 0) {
		//��m[i]�Ļ�mProduct
		for (i = 0, mP = 1; i < n; i++) {
			mP *= m[i];
		}
	}
	for (i = 0, ans = 0; i < n; i++){
		//��M[i] = mP / m[i]��ģ��Ԫt[i](������Ϊ��)
		gcdEx(mP / m[i], m[i], t, y);
		ans += (a[i] * t * (mP / m[i]));
	}
	//ans���п���Ϊ����
	ans = (ans + mP) % mP;
	return ans;
}

// ŷ������:���������������[1,n]����n ������ �ĸ��� �ĺ���
int  Eular(int n){
	//ͨʽ����(x) = x(1-1/p1)*(1-1/p2)*(1-1/p3)��(1-1/pn)
	//����p1,p2����pnΪx�����л���������(������)��x�ǲ�Ϊ0��������
	int i, ret = n;
	double k = sqrt((double)n);
	for (i = 2; i <= k; i++){
		if (n%i == 0){
			ret = ret / i*(i - 1);
			while (n%i == 0)
				n /= i;
		}
	}
	n > 1 ? ret = ret / n*(n - 1) : 0;
	return ret;
}
//��ʲ���ģ�ȡ���һ������ʤ
int BaShen(int n, int min, int max){
	int situation = n % (max + min);
	if (0 < situation && situation <= min) {
		//���ָ�
		return 0;
	}
	else {
		//����ʤ
		return 1;
	}
}
//����lineNum����ʱ��󽻵���Ŀ
int countMaxIntersection(int lineNum){
	return lineNum*(lineNum - 1) / 2;
}






/************************************************����**********************************************************/
//01���� V�Ǳ���������
void zoback(int V, int back[], int cost, int value)
{
	for (int v = V; v >= cost; v--)
	{
		back[v] = max(back[v], back[v - cost] + value);
	}
}
//01���������Ƿ� V�Ǳ���������  ����value = 0; back[0] = 1;
void ZOBack(int V, int back[], int cost, int value)
{
	for (int v = cost; v <= V; v++)
	{
		back[v] += back[v - cost] + value;
	}
}
void CompletePack(int dp[], int m, int cost, int weight)   //���ر���
{
	for (int i = cost; i <= m; i++)
	dp[i] = max(dp[i], dp[i - cost] + weight);
}
void ZeroOnePack(int dp[], int m, int cost, int weight)   // 01����
{
	for (int i = m; i >= cost; i--)
		dp[i] = max(dp[i], dp[i - cost] + weight);
}
void MultiplyPack(int dp[], int cost, int m, int weight, int amount)   //��ȫ����
{
	if (cost*amount >= m)
	CompletePack(dp, m, cost, weight);
	else
	{
		int k = 1;
		while (k<amount)
		{
			ZeroOnePack(dp, m, k*cost, k*weight);
			amount -= k;
			k <<= 1;
		}
		ZeroOnePack(dp, m, amount*cost, amount*weight);
	}
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



