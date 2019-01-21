#include "Teater.h"

using namespace std;

int mainForMath() {
	StandardExtend::testAndOut("2018���Ƿ�����: ", (bool)StandardExtend::isIntercalary(2018), false);
	StandardExtend::testAndOut("2018-10-22��2018��ĵڼ���: ", StandardExtend::calcNumberOfDays(2018, 10, 22), 295);

	puts("ȫ����: ");
	MathExtend::pentration(3);
	puts("�׳�: ");
	printf("%.2f\n", MathExtend::fact(10));
	StandardExtend::testAndOut("����� 3ȡ2", MathExtend::C(3, 2), 3);
	StandardExtend::testAndOut("����� 3ȡ1", MathExtend::C(3, 1), 3);
	StandardExtend::testAndOut("����� 49ȡ6", MathExtend::C(49, 6), 13983816);
	StandardExtend::testAndOut("������ 3ȡ2", MathExtend::A(3, 2), 6);
	StandardExtend::testAndOut("������ 8ȡ3", MathExtend::A(8, 3), 336);


	for (I64 i = 0; i < 10; ++i) {
		StandardExtend::testAndOut("������ " + std::to_string(i) + "^2", MathExtend::quickPow(i, 2), (I64)std::pow(i, 2));
	}
	int testCount = 500;
	StandardExtend::testAndDiffClock([&]() {
		for (int i = 0; i < testCount; ++i) {
			std::pow(2, 62);
		}
	}, "std::pow");
	StandardExtend::testAndDiffClock([&]() {
		for (int i = 0; i < testCount; ++i) {
			MathExtend::quickPow(2, 62);
		}
	}, "��ͨ������");
	StandardExtend::testAndDiffClock([&]() {
		for (int i = 0; i < testCount; ++i) {
			MathExtend::quickPow(2, 62, MAX_INT64);
		}
	}, "��mod�Ŀ�����");
	StandardExtend::testAndDiffClock([&]() {
		for (int i = 0; i < testCount; ++i) {
			MathExtend::quickPow_OLD(2, 62, MAX_INT64);
		}
	}, "����quickFact�Ŀ�����");
	StandardExtend::testAndOut("������ 10^18", MathExtend::quickPow_OLD(10, 18, MAX_INT64), (I64)std::pow(10, 18));
	for (int i = MAX_INT32 - 1; i < MAX_INT32; ++i) {
		auto a = MathExtend::powLastBit(i, 1) % 10;
		auto b = MathExtend::quickPow(i, 2, 10);
		StandardExtend::testAndOut(std::to_string(i) + "^2" + "�����1bit", a, b);
	}

	auto primeTable = MathExtend::generateSievePrimeS(100);
	int count = 0;
	for (int i = 0; i < 10; ++i) {
		bool isP = MathExtend::isPrime(i);
		StandardExtend::testAndOut("����" + to_string(i) + " �Ƿ�����:", isP, primeTable[i] == 0);
	}
	free(primeTable);



	Utility::Double::setEPS(EPS_DOUBLE);
	auto f1 = [](double x) {
		return pow(x, 2) + 2 * x + 1;
	};
	auto x1 = MathExtend::trichotomy<double, double>(-10.0, 10.0, [&](double x) {
		//��x=-1.0ʱ y��С=0.0
		return f1(x);
	}, EPS);
	StandardExtend::testAndOut("һԪ���η�����С���ƽ�: ", Utility::Double(x1), Utility::Double(-1.0));
	StandardExtend::testAndOut("һԪ���η�����С����ֵ: ", Utility::Double(f1(x1)), Utility::Double(0.0));

	auto f2 = [](double x) {
		return -pow(x, 2) + 2 * x + 1;
	};
	auto x2 = MathExtend::trichotomy<double, double>(-10.0, 10.0, [&](double x) {
		//��x=1.0ʱ y���=2.0 (�˷���ֻ�ṩ��Сֵ ����ǰ����'-'��)
		return -(f2(x));
	}, EPS);
	StandardExtend::testAndOut("һԪ���η��������ƽ�: ", Utility::Double(x2), Utility::Double(1.0));
	StandardExtend::testAndOut("һԪ���η���������ֵ: ", Utility::Double(f2(x2)), Utility::Double(2.0));

	const int size = 9;
	//˳���������
	int a[size] = { 6, 5, 3, 0, 1, 4, 2, 6, 7 };
	Utility::BubbleSort(a, size, size, Utility::lessQsortCmp<int>);
	Utility::SlectSort(a, a + size, Utility::moreQsortCmp<int>);
	int aa[] = { 44, 12, 59, 36, 62, 43, 94, 7, 35, 52, 85 };
	Utility::shellSort<int>(aa, 11);

	std::sort(a, a + size);
	auto it1 = MathExtend::dichotomy(a, size, 6);
	bool exist = std::binary_search(a, a + size, 6);
	auto pair = std::equal_range(a, a + size, 6);

	StandardExtend::testAndDiffClock([&]() {
		I64 x, y;
		StandardExtend::testAndOut("2*x + 1*y + 5 = 0����С�Ǹ�������x�Ƿ����: ", (bool)MathExtend::linearEquationCondition1(2LL, x, 1LL, y, 5LL), true);
		StandardExtend::testAndOut("��x: ", x, 0LL);
		StandardExtend::testAndOut("��y: ", y, 5LL);
	}, "��Ԫһ�����Է�����С�Ǹ�������");

	int aaInput[][3] = {
		{700 ,300, 200 },
		{500 ,200, 300 },
		{500 ,200, 500 },
		{275 ,110, 330 },
		{275 ,110, 385 },
		{648 ,375, 4002},
		{3, 1 ,10000 }
	};
	int aaOutput[][2] = {
		{1, 3},
		{1, 1},
		{1, 0},
		{0, 3},
		{1, 1},
		{49, 74},
		{3333, 1}
	};
	StandardExtend::testAndDiffClock([&]() {
		int x, y;
		for (int i = 0; i < 7; ++i) {
			MathExtend::linearEquationCondition2(aaInput[i][0], x, aaInput[i][1], y, aaInput[i][2]);
			x = abs(x);
			y = abs(y);
			StandardExtend::testAndOut("��x: ", x, aaOutput[i][0]);
			StandardExtend::testAndOut("��y: ", y, aaOutput[i][1]);
		}
	}, "��Ԫһ�����Է��� ����|x| + |y|��С�Ľ�");
	StandardExtend::testAndDiffClock([&]() {
		int x, y, x1, y1, x2, y2;
		for (int i = 0; i < 7; ++i) {
			MathExtend::linearEquationCondition1(aaInput[i][0], x1, aaInput[i][1], y1, aaInput[i][2]);
			//����
			MathExtend::linearEquationCondition1(aaInput[i][1], x2, aaInput[i][0], y2, aaInput[i][2]);
			if ((abs(x1) + abs(y1)) < (abs(x2) + abs(y2))) {
				x = abs(x1);
				y = abs(y1);
			}
			else {
				//�����Ƿ���
				x = abs(y2);
				y = abs(x2);
			}
			StandardExtend::testAndOut("��x: ", x, aaOutput[i][0]);
			StandardExtend::testAndOut("��y: ", y, aaOutput[i][1]);
		}
	}, "��Ԫһ�����Է��� ����|x| + |y|��С�Ľ�");

	StandardExtend::testAndDiffClock([&]() {
		int m[] = { 3, 5, 7 };
		int a[] = { 2, 3, 2 };
		int minResult = MathExtend::chineseReminder(m, a, 3, 0);
		StandardExtend::testAndOut("��С��������: ", minResult, 23);
	}, "�й�ʣ�ඨ�� ������ͬ�෽����");

	StandardExtend::testAndDiffClock([&]() {
		I64 illTable[] = { 0, 0, 1, 2, 9, 44, 265, 1854 };
		JCE::ArrayList<I64> illArrangeList;
		MathExtend::buildIllArrangeList(illArrangeList);
		for (JCE::SizeType i = 0; i < 8; ++i) {
			StandardExtend::testAndOut("����Value: ", illArrangeList[i], illTable[i]);
		}
		StandardExtend::outPutIterable(illArrangeList.begin(), illArrangeList.end(), 0, '\0', ',');
		StandardExtend::outPutIterable(illArrangeList.begin(), illArrangeList.end(), 20, '\0', (JCE::SizeType)5);
		StandardExtend::outPutIterable(illArrangeList.begin(), illArrangeList.end(), 20, '\0', [](auto left) {
			static int count = 0;
			return ++count % 5 == 0;
		});
	}, "���ű�");

	cout << "��ӡn�е��������" << endl;
	int table[10][StandardExtend::MAX_C] = { 0 };
	MathExtend::buildPtriangleTable(table, 10);
	StandardExtend::outPut2DArrayTrangle(table, 10, 3);

	cout << " ====== Math test end" << endl;
	return 0;
}

int mainForAlgorithm() {
	//@see http://acm.hdu.edu.cn/showproblem.php?pid=2602
	//��Ʒ����, ��������, �����ö�̬�滮�����С
	const int itemNum = 5, cap = 10, dpSize = cap + 1;
	int dp[dpSize] = { 0 };
	int cost[itemNum] = { 5, 4, 3, 2, 1 }, value[itemNum] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < itemNum; ++i) {
		MathExtend::ZeroOneBackpackProcess(dp, cap, cost[i], value[i]);
		//MathExtend::ZOBackpackProcess(dp, cap, 0, value[i]);
	}
	StandardExtend::outPutIterable(dp, dp + dpSize);

	memset(dp, 0, dpSize);
	for (int i = 0; i < itemNum; ++i) {
		MathExtend::CompletePackProcess(dp, cap, cost[i], value[i]);
	}
	StandardExtend::outPutIterable(dp, dp + dpSize);

	memset(dp, 0, dpSize);
	//{ 1,2,3,4,5,6,7,8,9,10 };
	int number[itemNum] = { 1,2,3,4,5 };
	for (int i = 0; i < itemNum; ++i) {
		MathExtend::MultiplyPackProcess(dp, cap, cost[i], value[i], number[i]);
	}
	StandardExtend::outPutIterable(dp, dp + dpSize);

	StandardExtend::testAndDiffClock([&]() {
		double a = 3.0, b = 4.0, c = 5.0;
		vector<Utility::Double> temper;
		temper.push_back(a);
		temper.push_back(b);
		temper.push_back(c);
		if (Utility::Triangle::isTrangle(a, b, c)) {
			unsigned count = Utility::Triangle::equalityElementCount(temper);
			puts(0 == count ? "yb"
				: (count == 3 ? "db" : "dy"));
		}
		else {
			puts("no");
		}
	}, "�����������ж�");

	cout << " ====== Algorithm test end" << endl;

	return 0;
}

int mainForBigInteger() {
	int d = 3;
	BigInteger a(1), b(2), c(d);
	//int d = c;
	//d *= a;
	StandardExtend::testAndOut("����������� �ӷ�: ", a + b == c, true);
	StandardExtend::testAndOut("����������� �˷�: ", a * b == b, true);
	StandardExtend::testAndOut("�����ӷ�������1: ", a + b == b + a, true);
	StandardExtend::testAndOut("�����ӷ�������2: ", 1 + c == c + 1, true);
	StandardExtend::testAndOut("����������� +=: ", a.plus(b) == (c += 0), true);
	StandardExtend::testAndOut("����������� ==�Լ���һ�ε�Ч��: ", a == c, true);
	StandardExtend::testAndOut("�����˷�������1: ", (b * a) == c * 2, true);
	StandardExtend::testAndOut("�����˷�������2: ", (a * b) == 2 * c, true);
	StandardExtend::testAndOut("�����������: ", c + 1 == 4, true);
	//cin >> c;
	cout << "cout ����: " << c << endl;

	a = 1;
	//2=a*2
	a.plus(a);
	//4=a*=4
	a.plus(a);
	StandardExtend::testAndOut("������", a, BigInteger(4));

	StandardExtend::testAndOut("������", a.multiply(a), BigInteger(16));
	StandardExtend::testAndOut("������", a, BigInteger(4));
	//16=4*4
	a.muity(a);
	StandardExtend::testAndOut("������", a, BigInteger(16));
	//256 = 16*16
	a *= a;
	StandardExtend::testAndOut("������", a, BigInteger(256));
	a = a * a;
	StandardExtend::testAndOut("������", a, BigInteger(65536));

	StandardExtend::testAndOut("ż���ж�", a.isEvenNumber(), true);
	StandardExtend::testAndOut("ż���ж�", (++a).isEvenNumber(), false);
	StandardExtend::testAndOut("�������� С - ��: ", c - a, BigInteger(3 - 65537));
	StandardExtend::testAndOut("�������� �� - С: ", a - c, BigInteger(65537 - 3));
	StandardExtend::testAndOut("�������� ��ν�λ: ", BigInteger(1) - BigInteger(1000), BigInteger(1 - 1000));
	StandardExtend::testAndOut("�������� λ: ", BigInteger(1) - BigInteger(1000), BigInteger(1 - 1000));

	string illResultStr = "34332795984163804765195977526776142032365783805375784983543400282685180793327632432791396429850988990237345920155783984828001486412574060553756854137069878601";
	BigInteger bigNumForIllResilt = BigInteger(illResultStr);
	char ill100ReslS[500] = "";
	JCE::ArrayList<BigInteger> illList;
	MathExtend::buildIllArrangeList(illList, 1001);
	// 158 λ10�������ַ�
	int length = illList[100].length();
	illList[100].print(ill100ReslS, length + 1);
	string ill100ReslStr = ill100ReslS;
	string tempStr(10, '\0');
	tempStr[0] = '1';
	StandardExtend::testAndOut("std::string length() �� sizeһ��:", tempStr.length(), tempStr.size());
	StandardExtend::testAndOut("�������ڵĴ������� 100ʱ��Ĵ�����", ill100ReslStr, illResultStr);
	StandardExtend::testAndOut("����==�Ƚ�: ", bigNumForIllResilt == illList[100], true);
	StandardExtend::testAndOut("����>�Ƚ�: ", bigNumForIllResilt > illList[99], true);
	StandardExtend::testAndOut("����<�Ƚ�: ", bigNumForIllResilt < illList[101], true);
	cout << "cout ����: " << bigNumForIllResilt << endl;
	cout << "cout ����: " << c << endl;

	StandardExtend::testAndDiffClock([&]() {
		StandardExtend::testAndOut("10*3", BigInteger(10).muityDouble(3), BigInteger(30));
		StandardExtend::testAndOut("10*4", BigInteger(10).muityDouble(4), BigInteger(40));

		StandardExtend::testAndOut("bigNumForIllResilt*bigNumForIllResilt"
			, BigInteger(bigNumForIllResilt).muityDouble(1024), bigNumForIllResilt * 1024);
		StandardExtend::testAndOut("1024*0", BigInteger(1024).muityDouble(0), BigInteger(0));
		StandardExtend::testAndOut("1024*1", BigInteger(1024).muityDouble(1), BigInteger(1024));
		StandardExtend::testAndOut("512*2", BigInteger(512).muityDouble(2), BigInteger(1024));
		StandardExtend::testAndOut("256*4", BigInteger(256).muityDouble(4), BigInteger(1024));
		StandardExtend::testAndOut("128*8", BigInteger(128).muityDouble(8), BigInteger(1024));
	}, "�����Ҳ����������ĳ˷�(��ʱ���ֳ˷���)");

	StandardExtend::testAndDiffClock([&]() {
		StandardExtend::testAndOut("10*3", BigInteger(10).muity(3), BigInteger(30));
		StandardExtend::testAndOut("10*4", BigInteger(10).muity(4), BigInteger(40));

		StandardExtend::testAndOut("bigNumForIllResilt*bigNumForIllResilt"
			, BigInteger(bigNumForIllResilt).muity(1024), bigNumForIllResilt * 1024);
		StandardExtend::testAndOut("1024*0", BigInteger(1024).muity(0), BigInteger(0));
		StandardExtend::testAndOut("1024*1", BigInteger(1024).muity(1), BigInteger(1024));
		StandardExtend::testAndOut("512*2", BigInteger(512).muity(2), BigInteger(1024));
		StandardExtend::testAndOut("256*4", BigInteger(256).muity(4), BigInteger(1024));
		StandardExtend::testAndOut("128*8", BigInteger(128).muity(8), BigInteger(1024));
	}, "�ֳ˷�");

	StandardExtend::testAndDiffClock([&]() {
		string a = to_string(MAX_INT64), b = to_string(MAX_INT64);
		char result[128] = "";
		BigInteger ba(a), bb(b);
		(ba + bb).print(result, 128);
		TransitionUtility::bigPlush(a, b, a);
		StandardExtend::testAndOut("64λ�з����������ֵ���: ", TransitionUtility::formatString(a), string(result));
	}, "�ַ��������ӷ�");
	/*
	python:
		a = 1
		n = 10000
		for i in range(1,n+1):
			a = a * i

		print(a)
	*/
	StandardExtend::testAndDiffClock([&]() {
		string resultOfFact100 = "93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000";
		string resultOfFact500 = "1220136825991110068701238785423046926253574342803192842192413588385845373153881997605496447502203281863013616477148203584163378722078177200480785205159329285477907571939330603772960859086270429174547882424912726344305670173270769461062802310452644218878789465754777149863494367781037644274033827365397471386477878495438489595537537990423241061271326984327745715546309977202781014561081188373709531016356324432987029563896628911658974769572087926928871281780070265174507768410719624390394322536422605234945850129918571501248706961568141625359056693423813008856249246891564126775654481886506593847951775360894005745238940335798476363944905313062323749066445048824665075946735862074637925184200459369692981022263971952597190945217823331756934581508552332820762820023402626907898342451712006207714640979456116127629145951237229913340169552363850942885592018727433795173014586357570828355780158735432768888680120399882384702151467605445407663535984174430480128938313896881639487469658817504506926365338175055478128640000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
		//1000�׳˹�3w��λ ����ǰ59λ 28462596809170545189[0]
		string resultOfFact10000 = "28462596809170545189064132121198688901480514017027992307941";
		char *buffer = new char[42560];
		BigInteger(1).fact(100).print(buffer, 256);
		StandardExtend::testAndOut("100�Ľ׳�: ", string(buffer), resultOfFact100);

		BigInteger(1).fact(500).print(buffer, 2560);
		StandardExtend::testAndOut("500�Ľ׳�: ", string(buffer), resultOfFact500);

		/*//DEBUGģʽ��Ҫ��1min����
		BigInteger(1).fact(10000).print(buffer, 40000);
		buffer[59] = '\0';
		StandardExtend::testAndOut("10000�Ľ׳˵�ǰ59λ: ", string(buffer), resultOfFact10000);
		*/
		delete buffer;
		buffer = nullptr;
	}, "�������ٽ׳�");

	//freopen("input", "r", stdin);
	/*char num[25];
	char out[25] = { 0 };
	while (cin >> num) {
		bool yes = true;
		BigInteger bnum = BigInteger(num);
		bnum.print(out);
		string num1 = string(out);
		bnum.muity(2).print(out);
		string num2 = string(out);
		for (JCE::SizeType i = 0; i < num1.size(); ++i) {
			char c = num2[i];
			int sub = num1.find(c);
			if (sub == num1.npos) {
				yes = false;
				break;
			}
			else {
				num1[sub] = ' ';
			}
		}
		puts(yes ? "Yes" : "No");
		bnum.print();
	}*/
	return 0;
}

int testForBiginteger() {
	// freopen("input", "r", stdin);
	vector<int> ans(5);
	int t = 1;
	char origin[1005] = "28462596809170545189064132121198688901480514 500";
	// scanf("%d", &t); getchar();
	BigInteger s = BigInteger(9) + BigInteger(1);
	s = 1;
	int n = 10001;
	s.reserve(1005);
	/*
	28462596809170545189064132121198688901480514
	500
	12201368259911100687012387854230469
	*/

	s.fact(10).print();
	while (t-- > 0) {
		BinaryTransition bt = BinaryTransition(100, 4, 3);
		// gets(origin);
		char *a = origin, *b = NULL;
		for (b = origin; *b != ' '; ++b);
		*b++ = '\0';
		//bt.transition(a, ans);
		BigInteger A = BigInteger(a);
		printf("A ");
		A.print();
		BigInteger B = BigInteger(b);
		printf("B ");
		B.print();
		BigInteger sum = A + B;
		// sum.muity(10000);
		printf("S ");
		sum.print();
		//sum.fact();
		//sum.print();
	}
	return 0;
}

int mainForFibonacci() {
	double temp_ = 0.375;
	printf("%.4f\n", temp_);
	printf("%.2f\n", temp_);
	printf("%.f\n", temp_);
	temp_ = 0.30;
	printf("%.4f\n", temp_);
	printf("%.2f\n", temp_);
	printf("%.f\n", temp_);
	clock_t start = clock();
	clock_t stop = clock();
	I64 f[101];
	int testN[14] = { 10, 20, 30, 40, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59 };
	MathExtend::buildFibonacci(f, 101);
	puts("n               �ݹ���   ��ʱ(ms)      ��̬�滮���    ��ʱ(ms)");
	// ֻ����ǰ3��(4���Է�ʱ��..)
	for (int i = 0; i < 3; ++i) {
		printf("%d ", testN[i]);
		start = clock();
		printf("%20I64d", MathExtend::fibonacciRec(testN[i]));
		stop = clock();
		printf("%10.f", (double)(stop - start));// (double)(stop - start) / CLK_TCK //��

		start = clock();
		//printf("%20I64d", f[testN[i]]);
		printf("%20I64d", MathExtend::fibonacci(testN[i]));
		stop = clock();
		printf("%10.f\n", (double)(stop - start));// (double)(stop - start) / CLK_TCK //��
	}
	return 0;
}


int runMathTest() {
	mainForFibonacci();
	testForBiginteger();
	mainForMath();
	StandardExtend::testAndDiffClock(mainForAlgorithm);
	StandardExtend::testAndDiffClock(mainForBigInteger);
	return 0;
}
