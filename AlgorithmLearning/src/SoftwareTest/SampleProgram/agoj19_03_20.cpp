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

// �ŵ��궨�����bug
#include "ExtendSpace.h"
#include "../SoftwareTest.h"

#define EPS 1E-5
#define MAX_N 505

#define MAX_R 1001
#define MAX_C 1001

//#define MAX_N 1000+5
#define MAX_INT32 2147483647
#define MIN_INT32 (-MAX_INT32-1)
#define ARRAY_TEMPLATE template<class T, class Iterator>


class SampleProgram_03_20 : public SoftwareTest::SampleProgram {
public:
	// ��Ҫ����1000����������
	int run() override {
		int leftBound, rightLmit;
		while (std::cin >> leftBound >> rightLmit) {
			std::cin.get();
			int currentValue;
			std::vector<int> integetList;
			bool isEndInput = false;
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
				if (StandardExtend::inRange(leftBound, currentValue, rightLmit + 1) && integetList.size() < 1001) {
					integetList.push_back(currentValue);
				}
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
*/
