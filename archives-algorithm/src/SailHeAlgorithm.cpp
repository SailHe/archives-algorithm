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
/*********************************************查 找********************************************************/
/*二分枚举+贪心*/
int canJump(int s[], int n, int m, int jump){
	int step = 0;
	int from = 0;
	int to = 1;
	int out = true;
	//到达终点时结束
	while (to < n){
		out = true;
		while (to < n && s[to] - s[from] <= jump){
			to++;//贪心，通过尽可能多的石头
			out = false;
		}
		from = to - 1;
		++step;
		//说明jump小了，有的地方跳不过去  || 步数过多
		if (out || step > m)
			return false;
	}
	return true;
}
int st[5/*00002*/];
int main_1_(){
	int L, M, N;
	while (cin >> L >> N >> M){
		N += 2;
		st[0] = 0;
		for (int i = 1; i < N - 1; scanf("%d", &st[i++]));
		sort(st + 1, st + N - 1);
		int left = st[1];
		int right = st[N - 1] = L;
		st[0] = 0;
		while (left < right){
			int mid = (left + right) >> 1;
			//若可以在小于m步的前提下跳过去 那么尝试小一些的jump力,但要保留上一次的jump值：mid不减1
			if (canJump(st, N, M, mid))
				right = mid;
			else {
				//在无法跳过的前提下，mid值无需保留
				left = mid + 1;
			}
		}
		cout << left << endl;
	}
	return 0;
}
/*********************************************DATE********************************************************/
//闰年判断
int isIntercalary(int year){
	return (year % 4 == 0 && year % 100 != 0 || year % 400 == 0);
}
//计算这天是当前年份的第几天
int today(int year, int month, int day){
	int t, j, month_day[2][13] = {
		{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
		{ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 } //闰年
	};
	for (t = j = 0; j < month; t += month_day[(year % 4 == 0 && year % 100 != 0 || year % 400 == 0)][j++]);
	return t + day;
}
//返回t1与t2间的分数时间差min， 并将小时，min结果存于参数指针中  t格式08:05 ==>805
int timeDiffrence(int t1, int t2, int *t_h, int *t_min){
	if (t1 % 100 <= t2 % 100){
		*t_h = t2 / 100 - t1 / 100;
		*t_min = t2 % 100 - t1 % 100;
	}
	else{
		*t_h = t2 / 100 - t1 / 100 - 1;
		*t_min = t2 % 100 - t1 % 100 + 60;
	}
	return *t_h * 60 + *t_min;
}
/*********************************************数 学********************************************************/
/*卡塔兰数*/
I64* catalanTable(){
	/*定义式式:fact(2 * n) / (fact(n + 1) * fact(n))若有除法,会导致精度不足.引起不可逆的错误*/
	/*递归式1: C(n)=((4*n-2)/(n+1))*C(n-1);此题该递归式会爆*/
	int maxN = 36;
	I64 *cata = (I64*)malloc(maxN*sizeof(I64));
	memset(cata, 0, maxN * sizeof(I64));//C(i)=0必须初始化
	cata[0] = cata[1] = 1;
	/*递归式2:C(0)=1, C(1)＝1, n>=2时, C(n)= C(0)*C(n-1) + ... +C(i)*C(n-i-1) + ... + C(n-1)C(0)*/
	for (int n = 2; n < maxN; n++){
		for (int i = 0; i < n; i++)
			cata[n] += cata[i] * cata[n - i - 1];
	}
	return cata;
}
//tmp = pow(iNum, 2)的后面几位 待研究func
long long f(int iNum, int m){
	long long tmp = 0;
	for (long long w = m; w > 0; w /= 10)
	{
		long long d = (iNum / w - (iNum / (10 * w)) * 10)*w;
		long long e = iNum % (10 * m / w);
		tmp += (d*e);
	}
	return tmp;
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

//辗转相除求公约数, 互素(互质)判定
int gcd(int a, int b)
{
	return b == 0 ? a : gcd(b, a % b);
}
/*
扩展的欧几里得算法（辗转相除法）Extended Euclidean algorithm:
功能:
	已知整数a、b, 在求得a、b的最大公约数的同时, 可找到满足贝祖等式ax + by = gcd(a, b)的整数x、y;（也称裴蜀数, 其中一个很可能是负数）
若a是负数，可以把问题转化成|a|(-x) + by = gcd(|a|, b)，然后令 x' = (-x)
	利用定理求线性方程的解*//*
返回值      :最大公 约数,因数,因子 gcd(greatest common divisor) 或 hcf(highest common factor)
已知参数a, b:任意非负整数(若是负数可能要更新算法)
返回参数x, y:贝祖数 或 模逆元...
*//*
贝祖等式(裴蜀定理)大意:两数的最大公约数的倍数可以用两数的整数倍相加来表示
关于未知数x和y的方程 ax + by = m有整数解<==>m是d的倍数。(d是任意整数a、b的最大公约数)
裴蜀等式有解时必然有无穷多个整数解，每组解x y都称为裴蜀数，可用扩展欧几里得算法求得。
应用:
对于不定整数方程ax+by=c，若c 是 gcd(a,b)的整数倍则该方程存在整数解
否则不存在 x , y 整数解。
讨论是否存在一整数k，使得m在k圈后比n大一；就是求解： k*m - n = 1;是否有解
1 % gcd(m,n) == 0
*/
int gcdEx_Old(int a, int b, int &x, int &y){
	if (b == 0){
		x = 1;
		y = 0;
		return a;
	}
	else{
		int g = gcdEx_Old(b, a%b, x, y);
		int t = x;
		x = y;
		y = t - (a / b)*y;
		return g;
	}

}
//扩展欧几里得算法精简正式版
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
定理一(贝祖定理)：ax + by = gcd(a, b)必有整数解
定理二：若gcd(a, b) = 1，则方程ax ≡ c (mod b)在[0, b-1]上有唯一解.
证明:对于方程 ax + by = c
<==> ax ≡ c (mod b)			==> 若方程存在特解x,那么x + k*b还是方程的解
定理三：若gcd(a, b) = g，则方程ax ≡ c (mod b)在[0, b/g - 1]上有唯一解.
证明:对于方程 a/g*x+b/g*y = c/g	(若x,y是整数解; g = gcd(a, b) ==> 方程左边是个整数 ==> 右边也该是整数 ==> 否则x,y不是整数解)
<==> a/g*x ≡ c/g (mod b/g)	==> 若方程存在特解x,那么x + k*(b/g)还是方程的解
*/
//PKU1061-青蛙的约会
//求线性方程ax+by = c的最小非负整数解x(只能保证x满足条件) 若整数解不存在返回false
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
定理：对于方程ax+by = c
通解:	x = x0 + b*t
y = y0 - a*t
(①:x0, y0为方程的一组解; ②:gcd(a, b) = 1; ③:t为任一整数.)

①:	g = gcdEx(a, b, x0, y0);
x0 = x0*c / g;
y0 = y0*c / g;
②:	x = x0 + b / g*t
y = y0 - a / g*t
于是		|x|+|y| = |x0+b/g*t| + |y0-a/g*t|
易知		|x0+b/g*t|单调递增，|y0-a/g*t|单调递减.
若规定	a>b
那么		原函数为先减后增的凹图像 (当t满足y0 - a/g*t == 0 时取最小值)
即		|x|+|y| 在t = y0*g/a 附近(③)取最小值
*/
//求线性方程ax+by = c 使得|x|+|y|最小的一组解x,y
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
		//此处取等则错
		if (abs(xT) + abs(yT) < abs(x) + abs(y)){
			x = xT;
			y = yT;
		}
	}
	if (hasSwap) swap(x, y);
}
/*
modulo inverse
功能:返回a的模m逆元t, 不存在打印错误并结束程序;
a*modInv(a, m) % m = 1
因为at%m=1：要求余数2的话，2*at，要求余数为3的话，3*at
定义

同余号 ≡: 若一个正整数除两个整数 得相同余数，则称二整数[同余]。
同余关系(离散数学):aRb <==> a与b用m除时具有相同的余数r, 此时, 称m是模数, 记a≡b(mod m); (modulo); 读作:a与b模m同余
同余关系是一个[等价关系]:自反,对称,传递

同余类(congruence class或residue class):由对于模n同余的所有整数组成的一个集合; 若从上下文知道模n，则也可标记为[a]。
同余类的代表数(representative):同余类中的拿来代表该同余类的任一元素

模逆元: t ≡ a^(-1)(mod m) 或 at ≡ 1(mod m); 若at与1模m同余, 则t是a的对同余m的模逆元, 也叫做模m的数论倒数;
若ax + my = 1 <==> (1 ≡ 1) ax+my ≡ 1 ≡ ax(mod m) ==> ax ≡ 1; 根据定义, x即是a关于模m的一个模逆元。
*/
int modInv(int a, int m){
	int t, y;
	if (gcdEx(a, m, t, y) != 1) {
		cerr << a << " " << m << "不互素 模逆元不存在!" << endl;
		exit(-1);
	}
	return t;
}
/*
中国剩余定理: ans = sum(a[i] * t[i] * M[i])
+ k*ans(在模mP的意义下方程组只有ans一个解)
返回值:满足同时%模数组分别等于余数组的整数num
已知参数m:模数组(要求m中任两数互质, 无参数检查)
已知参数a:余数组(强参数)				num%m[i] = r[i]
num模m[i]同余代表数组(弱参数)	num ≡ a[i](mod m[i])
经典问题:
物不知数(xx数之x就是模数), 生理周期(周期是模数 上一次外显的日期是同余代表数)
*/
int chineseReminder(int *m, int *a, int n, int mP = 0){
	int ans, t, y, i;
	//避免重复计算, 但是在必须多次计算的地方又可以选择默认参数, 更具灵活性
	if (mP == 0) {
		//求m[i]的积mProduct
		for (i = 0, mP = 1; i < n; i++) {
			mP *= m[i];
		}
	}
	for (i = 0, ans = 0; i < n; i++){
		//求M[i] = mP / m[i]的模逆元t[i](极可能为负)
		gcdEx(mP / m[i], m[i], t, y);
		ans += (a[i] * t * (mP / m[i]));
	}
	//ans极有可能为负数
	ans = (ans + mP) % mP;
	return ans;
}

// 欧拉函数:在数论中用于求解[1,n]中与n 互质数 的个数 的函数
int  Eular(int n){
	//通式：φ(x) = x(1-1/p1)*(1-1/p2)*(1-1/p3)…(1-1/pn)
	//其中p1,p2……pn为x的所有互异质因数(质因子)，x是不为0的整数。
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
//巴什博弈：取最后一个的人胜
int BaShen(int n, int min, int max){
	int situation = n % (max + min);
	if (0 < situation && situation <= min)
		return 0;//先手负
	else
		return 1;//先手胜
}
//返回n条边时最大交点数目
int pointLine(int n){
	return n*(n - 1) / 2;
}






/************************************************背包**********************************************************/
//01背包 V是背包总容量
void zoback(int V, int back[], int cost, int value)
{
	for (int v = V; v >= cost; v--)
	{
		back[v] = max(back[v], back[v - cost] + value);
	}
}
//01背包数量记法 V是背包总容量  参数value = 0; back[0] = 1;
void ZOBack(int V, int back[], int cost, int value)
{
	for (int v = cost; v <= V; v++)
	{
		back[v] += back[v - cost] + value;
	}
}
void CompletePack(int dp[], int m, int cost, int weight)   //多重背包
{
	for (int i = cost; i <= m; i++)
	dp[i] = max(dp[i], dp[i - cost] + weight);
}
void ZeroOnePack(int dp[], int m, int cost, int weight)   // 01背包
{
	for (int i = m; i >= cost; i--)
		dp[i] = max(dp[i], dp[i - cost] + weight);
}
void MultiplyPack(int dp[], int cost, int m, int weight, int amount)   //完全背包
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
/************************************************贪心**********************************************************/
/*密里根油滴实验程序*/
void MiLIGen(double u, double v1)
{
	printf("Q = %f e-19\n", (1e19*3.16e-8*5e-3 / u*pow(v1, 1.5)));
}

/*返回子串s1与s2匹配的字符数 比较长度len)*/
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
	double up = 0;//分子
	double dw = 0;//分母
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
		//printf("已析构");
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
		/**公倍数*/
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


/*判断一个字串是否回文(堆栈实现)*/
bool plalindrome(char const*str, int len) {
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
/*parenthesisMatching(括号匹配)*/
bool parMat(char const*str, int len) {
	stack<char> s;
	for (int i = 0; i < len; ++i) {
		/*右括号比对*/
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
		/*左括号压栈*/
		else if (str[i] == '(' || str[i] == '[' || str[i] == '{') {
			s.push(str[i]);
		}
		else;//do nothing
	}
	return s.empty();//可能还有多出来的左括号
}
/*返回堆栈操作的合法性*/
bool validityOfStack(char const*str, int len, int cap) {
	int size_ = 0;
	for (size_t i = 0; i < len; i++) {
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





