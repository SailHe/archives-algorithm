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
		//初始化,最下面一层的max数组就是三角形最下面一层的值
		max[n][i] = s[n][i];
	}
	for (int i = n - 1; i >= 1; i--)
	{//从倒数第二层向上计算
		for (int j = 1; j <= i; j++)
		{//计算第 i 层的每一个点的 max[][]
			int one = max[i + 1][j];//要么向下一层上和它最近的左边走
			int two = max[i + 1][j + 1];//要么向下一层上和它最近的右边走
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