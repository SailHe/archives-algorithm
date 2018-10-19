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
*
/************����*****0*********45*******90*********135*******180********225*******270********315****/
//int dir[8][2] = { { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 } };
const int Dir8[8][2] = { { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 } };
const int Dir4[4][2] = { { 0, -1 /*��*/ }, { 0, 1 /*��*/ }, { -1, 0 /*��*/ }, { 1, 0 /*��*/ } };

//�Ǿۺ�  ����
class MyPoint{
public:
	double x, y;
	MyPoint() = default;
	MyPoint(double x, double y){
		x = y, y = y;
	}
	MyPoint(istream &in){
		//scanf("%lf%lf", &(this->x), &(this->y));
		in >> this->x >> this->y;
	}
	MyPoint O(){
		MyPoint O = { 0.0, 0.0 };
		return O;
	}
};


//������������
double dDistance(MyPoint const &p1, MyPoint const &p2){
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;
	return sqrt(dx*dx + dy*dy);
}
//������p1, p2, p3��ɵ������ε����
double dTriangleArea(MyPoint const &p1, MyPoint const &p2, MyPoint const &p3){
	double d12 = dDistance(p1, p2)
		, d13 = dDistance(p1, p3)
		, d23 = dDistance(p2, p3);
	//���׹�ʽ
	double dL = (d12 + d13 + d23) / 2.0;
	return sqrt(dL * (dL - d12) * (dL - d13) * (dL - d23));
}
//����pλ��p1, p2, p3��ɵ�����������ô����true ���򷵻�false
bool isInternalTriangle(MyPoint const &p1, MyPoint const &p2, MyPoint const &p3, MyPoint const &p){
	double a12p = dTriangleArea(p1, p2, p)
		, a13p = dTriangleArea(p1, p3, p)
		, a23p = dTriangleArea(p2, p3, p)
		, a123 = dTriangleArea(p1, p2, p3);
	//���Ǹ���ָ�������������֮�������������������Χ�����ʱ �����������ڲ� ���ȹ��߿��ܻᱻ�� һ��1E-5����
	return (fabs(a123 - (a12p + a13p + a23p)) < EPS);
}

void testForTriangle(){
	//freopen("input", "r", stdin);
	MyPoint p1, p2, p3, p;
	while (8 == scanf("%lf%lf%lf%lf%lf%lf%lf%lf"
		, &p1.x, &p1.y
		, &p2.x, &p2.y
		, &p3.x, &p3.y
		, &p.x, &p.y)){
		puts(isInternalTriangle(p1, p2, p3, p) ? "Yes" : "No");
	}
}


/*��һ���д�ӡ�������Ƶ�n���ݹ�ʵ�֡�*/
void dectobin(int n)
{
	if (n < 2)
		printf("%d", n);
	else
	{
		dectobin(n / 2);
		printf("%d", n % 2);
	}
}
/* ��epsΪ������ ���ַ� ����F��[L, R]����Сֵ */
double divThree(double L, double R, double eps, double(*F)(double))
{
	double Ll, Rr;
	while (R - L > eps)
	{
		Ll = (2 * L + R) / 3;/* ���� */
		Rr = (2 * R + L) / 3;
		if (F(Ll) > F(Rr))
			L = Ll;
		else
			R = Rr;
	}
	return F(L);/* ������һ������ */
}/* ��ϰ��HDU2899 */
/* �����Ƿ���Ϲ��ɶ��� */
int pythTriangle(int a, int b, int c)
{
	return a*a == b*b + c*c || b*b == a*a + c*c || c*c == a*a + b*b;
}
//ansΪs1+s2(s1��s2�Ĳ���)(ֱ��ans.push_back(temp)����û��Ҫ��ʵ������s1��s2)
//����s1��s2��������λ��, ansΪs1+s2(ֱ��push_back����) a0,a1,a2...an-1����λ��ָa[(n-1)/2] (��n����)����(n+1)/2����(a0Ϊ��һ����)
int median(vector<int> ans){
	sort(ans.begin(), ans.end());
	if (ans.size() % 2 == 0)
		return ans[ans.size() / 2 - 1];
	else
		return ans[ans.size() / 2];
}
/*��ŵ���ݹ�ⷨ*/
void move(int n, char a, char b, char c)
{/* Move(n,'A','B','C');*/
	if (n == 1)
		printf("%c To %c\n", a, c);    //��nֻ��1����ʱ��ֱ�Ӵ�a�ƶ���c
	else
	{
		move(n - 1, a, c, b);            //��n-1��Ҫ��aͨ��c�ƶ���b
		printf("%c To %c\n", a, c);
		move(n - 1, b, a, c);            //n-1���ƶ�����֮��b�俪ʼ�̣�bͨ��a�ƶ���c����ߺ������
	}
}
/*���������ж�*/
int divisible(char *Bigint, int MOD)
{
	int len = strlen(Bigint);
	int ans, i;
	for (ans = i = 0; i < len; i++){
		ans = (ans * 10 + (Bigint[i] - '0')) % MOD;
	}
	return ans == 0 ? true : false;
}
/*���������ֱ������end�ַ�*/
void refresh(char end)
{
	while (getchar() != end);
}
/*��������*/
double dis(MyPoint a, MyPoint b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}
/*���-�յ�-��Ծ����-��뾶*/
int jump(MyPoint s, MyPoint e, double Power, double R)
{
	return dis(s, e) <= R + Power;
}
//����ʵ�� �ױ�
void swapMy(int &a, int &b)
{
	a += b;
	b = a - b;
	a = a - b;
}
//�����������ʱ����λ��Сʱ��
double getDifftime(time_t t_start)
{
	time_t t;
	double all;
	time(&t);
	all = difftime(t, t_start);
	return all / 3600;
	/*
	clock_t Start = clock();
	clock_t Stop = clock();
	printf("����˷�ִ��ʱ�䣺%lf\n", (double)(Stop - Start)/CLK_TCK);
	*/
}
/*���ش��ű� ���26��*/
__int64* illArrange()
{
	__int64 *M = (__int64*)malloc(26 * sizeof(__int64));
	M[0] = 0, M[1] = 1, M[2] = 1;
	for (int n = 3; n < 26; n++)
		M[n] = (n - 1) * (M[n - 1] + M[n - 2]);
	return M;
}
/*����˷� Product = Product*a����������ͬ*/
void matrixMultiply(const int n, int(*a)[25], int Product[][25]/*�����*/)
{
	int r, c, i, tmp;
	int b[25][25];
	memcpy(b, Product, n*25 * 4);
	for (r = 0; r < n; r++)
	{
		for (c = 0; c < n; c++)
		{
			for (tmp = i = 0; i < n; i++)//���б�����Ӧ��� �ۼӸ�tmp �ٸ��� Product
				tmp += a[r][i] * b[i][c];
			Product[r][c] = tmp;
		}
	}
}

//���digit��lhs��rhs��ȫ����  ���ֵ���
void penetration(char *digit, int lhs, int rhs){
	if (lhs == rhs)
		puts(digit);
	else{
		for (int i = lhs; i <= rhs; i++){
			swap(digit[lhs], digit[i]);
			penetration(digit, lhs + 1, rhs);
			swap(digit[lhs], digit[i]);
		}
	}
}
/*�������1-n��ȫ����*/
void pentration(int n){
	char digit[11] = "123456789";
	digit[n] = '\0';
	//penetration(digit, 0, n - 1);
	do{
		puts(digit);//����һ�������� #include<algorithm>
	} while (next_permutation(digit, digit + n));
	digit[n] = n + '0';
}
//��תmaxR�еĶ�ά����a ÿ��λ������[��cSt,��cEn)��Ԫ��
void reversal(int a[][105], int cSt, int cEn, int maxR = 1) {
	for (int r = 0; r < maxR; r++) {
		for (int c = cSt, i = 0; i < (cEn - cSt) / 2; i++, c++){
			int temp = a[r][c];
			a[r][c] = a[r][cEn - i - 1];
			a[r][cEn - i - 1] = temp;
		}
	}
}
/*��R�� C�еĶ�ά����������kλ*/
void leftMoveK(int a[][105], int R, int C, int k){
	/*
	8 3
	1 2 3 4 5 6 7 8
	8 7 6 5 4/ 3 2 1
	4 5 6 7 8/ 1 2 3
	*/
	k %= C;
	reversal(a, 0, C, R);/*��ת����Ԫ��*/
	reversal(a, 0, C - k, R);/*��תǰC-k��Ԫ��*/
	reversal(a, C - k, C, R);/*��ת��k��Ԫ��*/
}
/*����kλ<==>����C-kλ leftMoveK(a, R, C, C-k); <==> rightMoveK(a, R, C, k);*/
void rightMoveK(int a[][105], int R, int C, int k){
	/*
	8 3
	1 2 3 4 5 6 7 8
	8 7 6/ 5 4 3 2 1
	6 7 8/ 1 2 3 4 5
	*/
	k %= C;
	reversal(a, 0, C, R);
	reversal(a, 0, k, R);/*��תǰk��Ԫ��*/
	reversal(a, k, C, R);/*��ת��C-k��Ԫ��*/
}
//������
int Reverse(int number)
{
	int temp = number;
	int sum = 0, product = 1;
	int top = 0;
	int temptop;
	for (top = 0; temp != 0; temp /= 10, top++);
	//for (top = 1; temp /= 10; top++);
	temp = number;
	while (temp != 0)
	{
		temptop = top-- - 1;
		product = 1;
		while (temptop-- != 0)
		{
			product *= 10;
		}
		sum += (temp % 10) * product;
		temp /= 10;
	}
	return sum;
}
//��������x��D���ֵĴ���
int digitCountD(const int x, const int D)
{
	int temp = x;
	int count = 0;
	do
	{
		count += ((temp % 10) > 0 ? (temp % 10) : -(temp % 10)) == D ? 1 : 0;
		temp /= 10;
	} while (temp != 0);
	return count;
}
//��ȡ������λ��
int digitTop(int number)
{
	int top = 1;
	while (number /= 10)top++;
	return top;
}
//ɾ��res�е��ַ��ִ�del
void delssInS(char *res, char *del)
{
	int i, res_len, del_len;
	char *p;
	del_len = strlen(del);
	res_len = strlen(res);

	for (p = strstr(res, del); p != NULL; p = strstr(res, del))
	{
		for (i = p - res; i < res_len - del_len + 1; i++)
		{
			res[i] = res[i + del_len];
		}
		res_len = strlen(res);
	}
}
/*�����һ�����������������(longest continuous increment sequence)*/
//���س��� ����:���ڴ�������������е�����[lhs, rhs)
size_t lonConIncSeq(const vector<int> a, int &lhs, int &rhs){
	int maxLen = 0, n = a.size();
	lhs = 0, rhs = -1;//rhs==-1ʹ��������ǳ���ʱΪ0
	for (int l = 0, r = 0, len = 0;; ++r){
		len = r - l + 1;
		maxLen = rhs - lhs + 1;
		/*������������� (��һ��������:��ȡ��)*/
		if (len > maxLen){
			lhs = l, rhs = r;
		}
		else{}
		if (r >= n - 1)
			break;//r < n - 1 ��֤n==1ʱ��ȷ
		if (a[r] < a[r + 1]){}
		//��ǰ���в��ǵ�������
		else{
			l = r + 1;//���þֲ���������
		}
	}
	++rhs;
	return maxLen;
}
/*********************************************�� ��********************************************************/

/*qsort(a, ���򳤶�, һ��Ԫ�صĴ�С, intCmp);*/
template<class CmpType>
int lessQsortCmp(const void *min, const void *max)
{
	return ((CmpType*)min)->year - ((CmpType*)max)->year;
}
template<class CmpType>
int structCmp(const void *min, const void *max)
{
	int cp = ((CmpType*)min)->value - ((CmpType*)max)->value;//������ֵ ��С��������
	if (cp == 0)
		cp = ((CmpType*)max)->value2 - ((CmpType*)min)->value2;//���μ�ֵ �Ӵ�С����
	return cp;
}
/*sort(a, a+n, boolCmp)*/
template<class CmpType>
bool lessSortCmp(CmpType min, CmpType max)
{
	return min.value < max.value;
}
//ð������

void BubbleSort(int a[], int n, int K, int cmp(const void *a, const void *b))
{
	int temp = 0;//������a�е�n��Ԫ�ؽ��еĵ�K��
	for (int i = 0; i < K; i++)//K�������ʱ���ܼ�һ
	{
		for (int j = 0; j < n - i - 1; j++)//�˴������һ
		{
			if (cmp(&a[j + 1], &a[j]))
			{
				temp = a[j + 1];
				a[j + 1] = a[j];
				a[j] = temp;
			}
		}
	}
}
//ѡ������
template<class CmpType>
void SlectSort(CmpType a[], CmpType *an, int(*cmp)(CmpType *a, CmpType *b))
{
	for (; a < an - 1; a++)
	{
		for (CmpType *j = a + 1; j < an; j++)
		{
			if (cmp(j, a))
			{
				swap(*a, *j);
			}
		}
	}
}
/*********************************************�� ��********************************************************/
//���ֲ���re[n]��key
int dichotomy(int re[], int n, int key) {
	sort(re, re + n);
	int left = 0, right = n, mid = 0;
	while (left <= right) {
		mid = (right + left) / 2;
		if (re[mid] < key) {
			left = mid + 1;
		}//��1֮ǰ��ȷ��mid���Ϸ�, ���ǿ��԰��������õ���һ�εļ�����, ������1��һ�ο��ܻ����¼���mid
		else if (re[mid] > key){
			right = mid - 1;/**ͬ��**/
		}
		else{
			return mid;
		}
	}
	return -1;
}
/*����ö��+̰��*/
int canJump(int s[], int n, int m, int jump)
{
	int step = 0;
	int from = 0;
	int to = 1;
	int out = true;
	while (to < n)//�����յ�ʱ����
	{
		out = true;
		while (to < n && s[to] - s[from] <= jump)
		{
			to++;//̰�ģ�ͨ�������ܶ��ʯͷ
			out = false;
		}
		from = to - 1;
		step++;
		if (out || step > m)//˵��jumpС�ˣ��еĵط�������ȥ  || ��������
			return false;
	}
	return true;
}
int st[5/*00002*/];
int main_1_()
{
	int L, M, N;
	while (cin >> L >> N >> M)
	{
		N += 2;
		st[0] = 0;
		for (int i = 1; i < N - 1; scanf("%d", &st[i++]));
		sort(st + 1, st + N - 1);
		int left = st[1];
		int right = st[N - 1] = L;
		st[0] = 0;
		while (left < right)
		{
			int mid = (left + right) >> 1;
			if (canJump(st, N, M, mid))//��������С��m����ǰ��������ȥ ��ô����СһЩ��jump��,��Ҫ������һ�ε�jumpֵ��mid����1
				right = mid;
			else//���޷�������ǰ���£�midֵ���豣��
				left = mid + 1;
		}
		cout << left << endl;
	}
	return 0;
}
/*********************************************DATE********************************************************/
//�����ж�
int isIntercalary(int year)
{
	return (year % 4 == 0 && year % 100 != 0 || year % 400 == 0);
}
//���������ǵ�ǰ��ݵĵڼ���
int today(int year, int month, int day)
{
	int t, j, month_day[2][13] = {
		{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
		{ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 } //����
	};
	for (t = j = 0; j < month; t += month_day[(year % 4 == 0 && year % 100 != 0 || year % 400 == 0)][j++]);
	return t + day;
}
//����t1��t2��ķ���ʱ���min�� ����Сʱ��min������ڲ���ָ����  t��ʽ08:05 ==>805
int timeDiffrence(int t1, int t2, int *t_h, int *t_min)
{
	if (t1 % 100 <= t2 % 100)
	{
		*t_h = t2 / 100 - t1 / 100;
		*t_min = t2 % 100 - t1 % 100;
	}
	else
	{
		*t_h = t2 / 100 - t1 / 100 - 1;
		*t_min = t2 % 100 - t1 % 100 + 60;
	}
	return *t_h * 60 + *t_min;
}
/*********************************************�� ѧ********************************************************/
/*��������*/
I64* catalanTable(){
	/*����ʽʽ:fact(2 * n) / (fact(n + 1) * fact(n))���г���,�ᵼ�¾��Ȳ���.���𲻿���Ĵ���*/
	/*�ݹ�ʽ1: C(n)=((4*n-2)/(n+1))*C(n-1);����õݹ�ʽ�ᱬ*/
	int maxN = 36;
	I64 *cata = (I64*)malloc(maxN*sizeof(I64));
	memset(cata, 0, maxN * sizeof(I64));//C(i)=0�����ʼ��
	cata[0] = cata[1] = 1;
	/*�ݹ�ʽ2:C(0)=1, C(1)��1, n>=2ʱ, C(n)= C(0)*C(n-1) + ... +C(i)*C(n-i-1) + ... + C(n-1)C(0)*/
	for (int n = 2; n < maxN; n++){
		for (int i = 0; i < n; i++)
			cata[n] += cata[i] * cata[n - i - 1];
	}
	return cata;
}
//tmp = pow(iNum, 2)�ĺ��漸λ ���о�func
int f(int iNum, int m){
	int tmp = 0;
	for (long long w = m; w > 0; w /= 10)
	{
		long long d = (iNum / w - (iNum / (10 * w)) * 10)*w;
		long long e = iNum % (10 * m / w);
		tmp += (d*e);
	}
	return tmp;
}
/**************************************************************************/
__int64 Quickfact(__int64 a, __int64 b, __int64 mod)
{
	__int64 ans = 0;
	while (b)
	{
		if (b & 1)
		{
			ans = (ans + a) % mod;
		}
		a = (a << 1) % mod;
		b >>= 1;
	}
	return ans;
}
__int64 Quickpow(__int64 C, __int64 R, __int64 k)
{
	__int64 ans = 1;
	while (R > 0)
	{
		R & 1 ? ans = Quickfact(ans, C, k) : ans;
		C = Quickfact(C, C, k);
		R >>= 1;
	}
	return ans;
}
//�����ݼ��װ� m^n % k��k��
I64 quickPow(I64 m, I64 n, I64 k)
{
	I64 ans = 1;
	while (n > 0)
	{
		n & 1 ? ans = ((ans%k)*(m%k)) % k : ans;
		m = ((m%k)*(m%k)) % k;
		n >>= 1;
	}
	return ans;
}
I64 quickPow(I64 m, I64 n)
{
	I64 ans = 1;
	while (n > 0){
		//n & 1 ? ans = ans*m : ans;
		if (n & 1)
			ans *= m;
		m *= m;
		n >>= 1;
	}
	return ans;
}
/*�����������*/
double logR(double value, double base = 5) {
	return log(value) / log(base);
}
/*�� ��*/
double fact(int n){
	double product = n;
	while (--n > 0)product *= n;
	return n == 0 ? 1 : product;//0�Ľ׳�Ϊ1
}
//int �޶��ڽ׳˱�
int* factTable(int maxN = 13)/*int 13!���� double��long long int 18!��*/{
	int *Fact = (int *)malloc(sizeof(int)* maxN);
	Fact[0] = Fact[1] = 1;
	for (int n = 2; n < maxN; ++n) 
		Fact[n] = Fact[n - 1] * n;
	return Fact;
}
/*
n < m && m != 0
	 fact(n)
 ________________
fact(n - m) * fact(m)
*/
//�����
int C(int n, int m){
	int f = 1, i = 1;
	f = n - m < 0 ? 0 : f;//n < mʱc(n,m) = 0
	m = n - m < m ? n - m : m;//int ����n30 m29ʱ   c(30, 29) = c(30, 1)
	while (i <= m)
	{
		f *= n--;//n * (n - 1) * ...m
		f /= i++;//1 *    2    * ...m
	}
	return f;
}
//������
int A(int n, int m)
{
	int d, f = 1;
	for (d = n - m; d < n && d >= 0; --n)
	{
		f *= n;
	}
	return f;
}
//����n�� ���������
void buildPtriangle(int n, int table[][30])
{
	table[0][0] = 1;
	for (int r = 1; r < n; r++)
	{
		//ÿ�е����ָ���=����+1(������0��ʼ)
		//����Ҫÿ��ǰ��Ŀո�Ļ� ÿ�е�һ������ǰ��Ŀո��� = 2*[(maxR-currentR(1��ʼ))����һ�е�������]  ==> 2*(n-r-1)
		//int blakCnt = 0;
		for (int c = 0; c < r + 1; c++)
		{
			if (c != 0)
				table[r][c] = table[r - 1][c] + table[r - 1][c - 1];
			else
				table[r][c] = table[r - 1][c];
		}
	}
}
//��ӡn�е��������
void PrintPtriangle(int n, int table[][30])
{
	for (int pr = 0; pr < n; pr++)
	{
		for (int pc = 0; pc < n; pc++)
		{
			if (table[pr][pc] != 0)
				printf("%d", table[pr][pc]);
			if (pc < pr)
			{
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
int gcdEx_(int a, int b, int &x, int &y){
	if (b == 0){
		x = 1;
		y = 0;
		return a;
	}
	else{
		int g = gcdEx_(b, a%b, x, y);
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
//pku1061-���ܵ�Լ��
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
	int g = gcdEx(a, b, x0, y0);
	x0 = x0*c / g;
	y0 = y0*c / g;
	int tB = y0*g / a;
	x = infinity / 2, y = x;
	FOR(t, tB - 5, tB + 5){
		int xT = x0 + b / g*t;
		int yT = y0 - a / g*t;
		if (abs(xT) + abs(yT) < abs(x) + abs(y)){//�˴�ȡ�����
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
	}return t;
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
	if (mP == 0)//�����ظ�����, �����ڱ����μ���ĵط��ֿ���ѡ��Ĭ�ϲ���, ���������
	for (i = 0, mP = 1; i < n; i++) mP *= m[i];//��m[i]�Ļ�mProduct
	for (i = 0, ans = 0; i < n; i++){
		gcdEx(mP / m[i], m[i], t, y);//��M[i] = mP / m[i]��ģ��Ԫt[i](������Ϊ��)
		ans += (a[i] * t * (mP / m[i]));
	}
	ans = (ans + mP) % mP;//ans���п���Ϊ����
	return ans;
}

//����(����)�ж�
int isPrime_(int num)
{
	double k = sqrt((double)num);
	int i = 0;
	for (i = 2; i <= k && num % i != 0; i++);
	return i > k && num > 1 ? true : false;
}
//����(����)�ж� sqrt��ƽ���Ż���(δʵ��)
bool isPrime(int num){
	int i = 0;
	for (i = 2; i*i <= num && num % i != 0; i++);
	return i*i > num && num > 1;
}
//�����Ӻ�(���������Լ����
int factorSum(int x)
{
	int sum = 1, i;//1��Ӧx  ֻ��1����x
	assert(x != 0);//0û������
	for (i = 2; i * i <= x; i++)//i���ܱ�
	{
		if (x % i == 0)
		{
			sum += i;
			sum += i == x / i ? 0 : x / i;//����xֻ��һ��
		}
	}//�����Ӱ���1�������������ر��0û�����ӣ�1������Ϊ1�����������Ӻ�Ϊ����Ľ�����
	return sum;
}
//�����Ӻͱ�(Լ�� ����) (10^7һǧ��2.51s)(400w 10^6 900ms)
void factorSumTableSieve(const int maxn, int a[/*maxn*/])
{
	for (int i = 1; i < maxn; i++)
	{
		for (int j = i + i; j < maxn; j += i)
		{
			a[j] += i;
		}
	}
}
//����(����) ɸѡ�� ������ɫ��(Sieve Eratosthenes)(0 1==-1, a[i]==0��ʾ����) PS:maxN = 1e7ʱ����1s�� 1e9�ƺ��޷������ڴ�
int* primeSieve(const int maxN = 2) {
	int *prime = (int*)malloc(maxN*sizeof(int));
	memset(prime, 0, maxN*sizeof(int));
	prime[0] = prime[1] = -1;
	for (int i = 2; i < maxN; i++){
		for (int j = i + i; j < maxN; j += i){
			prime[j] += i;
		}
	}
	return prime;
}
//������Ŀ
int factorCount(int x)
{
	int i, count = 0;
	double k = sqrt((double)x);//1 ��Ӧ x ��2   k��Ӧk ��1
	for (i = 1; i <= k; i++)
	{
		if (x % i == 0)
			count += i * i == x ? 1 : 2;//������k��   �������ӳɶԳ���
	}
	return count;
}

//��������
double sqrtMy(double x, double eps = 1e-9)/*����*/
{
	double last, next;
	for (last = 1, next = 2;;)
	{
		last = next;
		next = (last + (x / last)) / 2;
		if (fabs(next - last) < eps)
		{
			return next;/*????*/
		}
	}
	return -1;
}
//�󷽲�
double variance(float x[], int n)
{

	double aver, sum = 0, D = 0;
	int temp, i;
	temp = n;
	while (temp-- != 0)
	{
		sum += x[temp];
	}
	aver = sum / n;
	for (i = 0; i < n; i++)
	{
		D += (x[i] - aver) * (x[i] - aver) / n;
	}
	return D;
}
// ŷ������:���������������[1,n]����n ������ �ĸ��� �ĺ���
int  Eular(int n)
{
	//ͨʽ����(x) = x(1-1/p1)*(1-1/p2)*(1-1/p3)��(1-1/pn)
	//����p1,p2����pnΪx�����л���������(������)��x�ǲ�Ϊ0��������
	int i, ret = n;
	double k = sqrt((double)n);
	for (i = 2; i <= k; i++)
	{
		if (n%i == 0)
		{
			ret = ret / i*(i - 1);
			while (n%i == 0)
				n /= i;
		}
	}
	n > 1 ? ret = ret / n*(n - 1) : 0;
	return ret;
}
//��ʲ���ģ�ȡ���һ������ʤ
int BaShen(int n, int min, int max)
{
	int situation = n % (max + min);
	if (0 < situation && situation <= min)
		return 0;//���ָ�
	else
		return 1;//����ʤ
}
//����n����ʱ��󽻵���Ŀ
int pointLine(int n)
{
	return n*(n - 1) / 2;
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
/*������͵�ʵ�����*/
void MiLIGen(double u, double v1)
{
	printf("Q = %f e-19\n", (1e19*3.16e-8*5e-3 / u*pow(v1, 1.5)));
}

/*�����Ӵ�s1��s2ƥ����ַ��� �Ƚϳ���len)*/
int matching(char *s1, char *s2, int len)
{
	int i, count;
	for (i = count = 0; s1[i] && s2[i] && len--; i++)
	{
		s1[i] == s2[i] ? count++ : 0;
	}
	return count;
}

class Fraction {
	double up = 0;//����
	double dw = 0;//��ĸ
public:
	Fraction(int up, int dw) {
		this->up = up;
		this->dw = dw;
	}

	Fraction(double up, double dw) {
		this->up = up;
		this->dw = dw;
	}

	~Fraction(){
		//printf("������");
	}

	static Fraction ZERO() {
		return Fraction(0, 1);
	}

	static int gcd(int a, int b) {
		return a % b == 0 ? b : gcd(b, a % b);
	}

	double getUp() {
		return up;
	}

	void setUp(double up) {
		this->up = up;
	}

	double getDw() {
		return dw;
	}

	void setDw(double dw) {
		this->dw = dw;
	}

	void plash(Fraction *a) {
		this->up *= a->dw;
		this->up += a->up * this->dw;
		this->dw *= a->dw;
		//Fraction::~Fraction();
		delete a;
	}

	void toSimple() {
		/**������*/
		int Ga = gcd((int) this->up, (int) this->dw);
		this->up /= Ga;
		this->dw /= Ga;
	}

	double toDouble() {
		if (dw == 0)
			throw dw;
		return this->up / this->dw;
	}
};