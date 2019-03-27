#include<stdio.h>
#include<string.h>
#include<string>
#include<cmath>
#include<vector>
#include<iostream>
#include<queue>
#include<functional>//greater
#include<list>
#include<stack>
#include<map>
#include<iomanip>//cout���ֲ�����

#include "else/SoftwareTest.h"
#include "ExtendSpace.h"

class SampleProgram_03_20 : public SoftwareTestSpace::SampleProgram {
	/*
	PS: 
	https://www.jianshu.com/p/d686ad9de817
	[�﷨��]https://memeda.github.io/%E6%8A%80%E6%9C%AF/2016/09/29/CPP%E8%AF%AD%E6%B3%95%E7%B3%96.html
	https://www.cnblogs.com/Aion/p/3449756.html
	��������˵lambda��ȫ��ʹ�ö��Ϸ����ڴ������ָ���Ƚϰ�ȫ
	��Ϊ����ĳ���������ٺ� �亯���հ�����ʹ��, ��ʱ�ͻᵼ������
	*/
	// ��������������
	std::vector<int> integetList;
public:
	// ��Ҫ����1000����������
	int run() override {
		int leftBound, rightLmit;
		while (std::cin >> leftBound >> rightLmit) {
			std::cin.get();
			int currentValue;
			std::vector<int> integetList;
			bool isEndInput = false;
			int maxCount = 1000;
			while (true) {
				char nextC = std::cin.peek();
				if (nextC == '\n' || std::cin.eof()) {
					break;
				}
				std::cin >> currentValue;
				// nextC = std::cin.get();
				if (currentValue == -99999 || isEndInput) {
					isEndInput = true;
					continue;
				}
				if (StandardExtend::inRange(leftBound, currentValue, rightLmit + 1) && maxCount > 0) {
					integetList.push_back(currentValue);
				}
				// ע��'�߼���'�ض�
				--maxCount;
			}
			auto sumValue = StandardExtend::sumValueStatistics(integetList.begin(), integetList.end(), 0);
			double avlValue = 0.0;
			if (integetList.size() != 0) {
				avlValue = StandardExtend::avlValueStatistics(integetList.begin(), integetList.end(), 0.0);
			}
			std::cout << std::fixed << std::setprecision(2) << integetList.size() << " " << sumValue << " " << avlValue << std::endl;
		}
		return 0;
	}

	std::function<void(void)> inputDataGenClosure() override {
		// ������Ϊ1005, int��Χ�� ÿ���������ֵ 21370
		const unsigned MAX_SIZE_R_LIMIT = 1001;

		// -����ʱ�Ϳ��Կ�����Щ�����ڵ���ʱ��������[�����]���ͷŵ�, ��ʹ�����ò���Ҳһ��, �����java��ͬ(��GC���ƾ�����)
		// ����������������: ���������Ч���� ֵȫ������ ֵȫ������
		unsigned maxSizeEffectiveCount = 100, maxOpEffectiveCount = 1, maxNeEffectiveCount = 1;
		// ȫ�������� ȫ����������
		unsigned maxAllEffectiveCount = 1, maxAllNotEffectiveCount = 1;

		// -��˲��ö��Ϸ����ڴ汣���䲻���ͷ�, ʹ��map�ṹ��������Ķ������, lambdaҲ�����Ƶ�������, ��ò�Ʋ���new������
		// -ʹ��std::shared_ptrֱ�Ӹ�ֵ���񼴿�, ��auto_ptr, unique_ptr���ƶ���ֵ����ò�Ʋ���
		// -std::shared_ptr<int> a(new int(1));auto b = std::move(a); // �����ݴ˴�ʵ���Ƕ����Ե�, ����Ǳ�����û��ʵ��
		std::shared_ptr<std::map<std::string, unsigned>> specialValMap(new std::map<std::string, unsigned>());
		(*specialValMap)["maxSizeEffectiveCount"] = 100;
		(*specialValMap)["maxOpEffectiveCount"] = 1;
		(*specialValMap)["maxNeEffectiveCount"] = 1;
		(*specialValMap)["maxAllEffectiveCount"] = 1;
		(*specialValMap)["maxAllNotEffectiveCount"] = 1;
		return [this, MAX_SIZE_R_LIMIT, specialValMap=std::move(specialValMap)]() {
			int base = RandomSpace::RandomP1(200);
			int lowerBound = base, upperBound = base + RandomSpace::RandomP1(21370 - base);
			printf("%d %d\n", lowerBound, upperBound);
			// 1%�Ļ��ʳ���1��MAX_SIZE_R_LIMIT, ��������[10, MAX_SIZE_R_LIMIT)
			unsigned size = RandomSpace::IsProbability(1, 1, (*specialValMap)["maxSizeEffectiveCount"]) ?
				MAX_SIZE_R_LIMIT : RandomSpace::RandomUnsignedRange(10, MAX_SIZE_R_LIMIT);
			bool printfEnd = false;
			integetList.clear();
			bool isAllOp = RandomSpace::IsProbability(1, 2, (*specialValMap)["maxOpEffectiveCount"]);
			bool isAllNe = false;
			if (!isAllOp) {
				isAllNe = RandomSpace::IsProbability(1, 2, (*specialValMap)["maxNeEffectiveCount"]);
			}
			for (unsigned i = 0; i < size; ++i) {
				base = RandomSpace::RandomP1(lowerBound);
				int currentValue = -99999;
				// 2/size �Ļ���������;����ֹ��־(size > 10) ��Ȼ ������Ͳ����ٴγ���
				if (RandomSpace::IsProbability(size - 2, size) || printfEnd) {
					// 0.5�ĸ����Ͻ��С��������(Ϊ��ʹ���ݸ����: ��������, С���½���������ĸ��ʸ���)
					int realUpperBound = 0;
					if (RandomSpace::IsProbability(1, 2, (*specialValMap)["maxAllEffectiveCount"])) {
						currentValue = RandomSpace::RandomRange(lowerBound, upperBound);
					}
					else {
						if (RandomSpace::IsProbability(1, 2, (*specialValMap)["maxAllNotEffectiveCount"])) {
							currentValue = -RandomSpace::RandomNotRange(lowerBound, upperBound);
						}
						else {
							RandomSpace::loopUntilNotEq([&]() {
								return realUpperBound = upperBound - (base * (RandomSpace::IsProbability(1, 2) ?
									RandomSpace::RandomP1(upperBound / lowerBound) * 3 : 1));
							}, 0);
							currentValue = RandomSpace::RandomP1(realUpperBound);
						}
					}
					// ȫ��ȫ�� �߼�
					if ((currentValue < 0 && isAllOp) || (currentValue > 0 && isAllNe)) {
						currentValue = -currentValue;
					}
					if (!printfEnd && StandardExtend::inRange(lowerBound, currentValue, upperBound + 1) && i < MAX_SIZE_R_LIMIT - 1) {
						integetList.push_back(currentValue);
					}
				}
				else {
					printfEnd = true;
				}

				printf(i == 0 ? "%d" : " %d", currentValue);
			}
			puts("");
		};
	}

	std::function<void(std::ofstream &)> outputDataGenClosure() override {
		return [this](std::ofstream &output) {
			auto sumValue = StandardExtend::sumValueStatistics(integetList.begin(), integetList.end(), 0);
			double avlValue = 0.0;
			if (integetList.size() != 0) {
				avlValue = StandardExtend::avlValueStatistics(integetList.begin(), integetList.end(), 0.0);
			}
			output << std::fixed << std::setprecision(2) << integetList.size() << " " << sumValue << " " << avlValue << std::endl;
		};
	}

};

/*

����������ҵ��
������������
�������ɸ������������ڸ�����Χ�ڣ����߽�ֵ��ֵ�ĸ������ܺ͡�ƽ��ֵ��
������˵����
�������ж���������ݣ�����ÿ��������ݣ����������У���һ��Ϊ�����������ֱ��ʾ��
����Χ������ֵ���ڶ���Ϊ���ɸ������������ĸ��������� 1000���������ֵ����ݱ����ԣ�
������־Ϊ-99999�����ܴ���û�н�����־����ֹ������������
�����˵����
����ÿ�����룬���Ϊ��������ǰ�������������ֱ��ʾ�������ܺͣ����һ��ƽ��ֵΪʵ
����Ҫ��ƽ��ֵ������λС���������֮����һ���ո����û�з���Ҫ������룬��ô��
�� 0 0 0.00��
��Sample Input��

2 7
1 2 3 4 5 6 7 8 -99999
3 6
-1 1 -99999

��Sample Output��
6 27 4.50
0 0 0.00
��˵����
�ڿ�ʼ���Խ������ʱ�����Կ��ǰѸ�������Ϊ������ 10 ����������ڰ����Ʒſ�������Ŀ��Ҫ��ġ�
��������Ϊ 10 ʱ�����¼������������������Ҫͨ���ģ�
3 9
0 1 2 3 4 5 6 7 8 9 10 11 -99999
3 5
0 1 2 3 4 5 6 -99999
3 5
0 1 2 3 4 5 6
3 5
0 1 2 3 4 5 6 7 8 9 10 11 12
3 5
0 1 2 3 4 5 6 7 8 9 -99999
3 5
0 1 2 3 4 5 6 7 -99999 1 2 3 4 5
3 5
0 1 2 3 4 5 6 7 8 9 10 11 12 110 -99999
������˵����
�����Ŀ��Ҫ��ֹһ�οΣ���ˣ�
һ������Ʋ�������
�����޸Ĳ�������
�������ݲ���������ƴ��룬������Ĵ���Ҫͨ�����Լ��Ĳ���������

1 102
100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 -100 100
���������1001����, �����ڶ���, ������1000����������Ҫ�� (��ĿҪ����Գ���1000���Ĳ���)
����
999 99900 100.00

����һ������: ����һ����unsigned���ֵΪsize������ ��Ҫ������ļ������᲻�ᱬը(���������)
PS: MAX_INT32���Լ������100GB ���������������Լ������ʱ��
*/
