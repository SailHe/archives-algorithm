#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
int buy[7], target[7], i, ans, j;
int judge();
int main()
{
	while (1 == scanf("%d", buy+0))
	{
		for (i = 1; i < 7; i++)
			scanf("%d", &buy[i]);
		for (i = 0; i < 7; i++)
			scanf("%d", &target[i]);
		ans = judge();
		if (0 == ans)
		{
			printf("Poor Xiaoming\n");
		}
		else
		{
			printf("%d\n", ans);
		}

	}
	return 0;
}

/*
1 8 11 12 26 27 9
1 8 11 12 26 27 9
1 2 3 4 5 6 7
1 2 9 10 11 12 7
7 10 15 16 31 33 15
7 11 15 18 32 33 14
*/
int judge()
{
	int red = 0, blue = 0, nRet, t = 0;
	if (buy[6] == target[6])
		blue = 1;
	/*for (i = 0; i < 6; ++i){
		int *it = find(target, target + 6, buy[i]);
		if (it != target+6){
			++red;
		}
	}*/
	for (i = 0; i < 6 && t < 6; i++)
	{
		if (buy[t] == target[i])
		{
			t++;
			red++;
		}
		else if (buy[t] < target[i])
		{
			t++;
			i--;
		}
	}
	if ((red == 0 && blue == 0) || (red == 1 && blue == 0))
		nRet = 0;
	else if ((red == 2 && blue == 1) || (red == 1 && blue == 1)
		|| (blue == 1 && red == 0))
		nRet = 6;
	else if ((red == 3 && blue == 1) || (red == 4 && blue == 0))
		nRet = 5;
	else if ((red == 5 && blue == 0) || (red == 4 && blue == 1))
		nRet = 4;
	else if (red == 5 && blue == 1)
		nRet = 3;
	else if (red == 6 && blue == 0)
		nRet = 2;
	else if (red == 6 && blue == 1)
		nRet = 1;
	else
		nRet = 0;
	return nRet;
}
