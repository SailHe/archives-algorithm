#include<iostream>
#include<algorithm>
#include<math.h>
using namespace std;
#define MAX 1001

struct TPoint
{
	int x;
	int y;
};

struct TRange
{
	double dFrom;
	double dTo;
};

TPoint tIslands[MAX];
TRange tSeg[MAX];

void vInput(int nN);
bool bCheck(int nN, int nD);
void vSort(int nN);
bool bCmp(const TRange &tA, const TRange &tB);
int nGetAns(int nN);
void vOut(int nC, int nOut);

int main()
{
	int nNum, nDist;
	int nAns;
	int nCase;

	nCase = 1;
	while (cin >> nNum >> nDist)
	{
		if ((nNum == 0) && (nDist == 0))
		{
			return 0;
		}
		vInput(nNum);
		if (bCheck(nNum, nDist))
		{
			vSort(nNum);
			nAns = nGetAns(nNum);
		}
		else
		{
			nAns = -1;
		}
		vOut(nCase, nAns);
		nCase++;
	}
	return 0;
}

void vInput(int nN)
{
	int i;
	for (i = 1; i <= nN; i++)
	{
		cin >> tIslands[i].x >> tIslands[i].y;
	}
}

bool bCheck(int nN, int nD)
{
	int i;
	for (i = 1; i <= nN; i++)
	{
		if (tIslands[i].y > nD)
		{
			return false;
		}
		tSeg[i].dFrom = tIslands[i].x - sqrt(1.0*nD*nD - 1.0*tIslands[i].y*tIslands[i].y);
		tSeg[i].dTo = tIslands[i].x + sqrt(1.0*nD*nD - 1.0*tIslands[i].y*tIslands[i].y);
	}
	return true;
}

void vSort(int nN)
{
	sort(&tSeg[1], &tSeg[nN + 1], bCmp);
}

bool bCmp(const TRange &tA, const TRange &tB)
{
	return tA.dFrom < tB.dFrom;
}

int nGetAns(int nN)
{
	int nRet;
	int i;
	double dTemp;
	nRet = 1;
	dTemp = tSeg[1].dTo;
	for (i = 2; i <= nN; i++)
	{
		if (dTemp < tSeg[i].dFrom)
		{
			nRet++;
			dTemp = tSeg[i].dTo;
		}
		else
		{
			if (dTemp > tSeg[i].dTo)
			{
				dTemp = tSeg[i].dTo;
			}
		}
	}
	return nRet;
}

void vOut(int nC, int nOut)
{
	cout << "Case " << nC << ": " << nOut << endl;
}
