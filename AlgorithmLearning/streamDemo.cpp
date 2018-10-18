#include<iostream>
#include<fstream>
using namespace std;

void main()
{
	ifstream input("input.txt");
	ofstream output("output.txt");

	int n;
	int s[101][101] = { 0 };
	input >> n;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= i; j++)
		{
			input >> s[i][j];
		}
	}

	int max[101][101] = { 0 };
	for (int i = 1; i <= n; i++)
	{
		//��ʼ��,������һ���max�������������������һ���ֵ
		max[n][i] = s[n][i];
	}
	for (int i = n - 1; i >= 1; i--)
	{//�ӵ����ڶ������ϼ���
		for (int j = 1; j <= i; j++)
		{//����� i ���ÿһ����� max[][]
			int one = max[i + 1][j];//Ҫô����һ���Ϻ�������������
			int two = max[i + 1][j + 1];//Ҫô����һ���Ϻ���������ұ���
			if (one > two)
			{
				max[i][j] = s[i][j] + one;
			}
			else
			{
				max[i][j] = s[i][j] + two;
			}
		}
	}
	output << max[1][1];

	input.close();
	output.close();
}