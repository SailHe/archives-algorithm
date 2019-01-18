#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <random>
#include <time.h>

//#include "MathLibrary.h"
#include "ExtendSpace.h"
#include "BigInteger.h"
using namespace std;


int main_2(){
	/*BigInteger b = BigInteger(35);
	b.print();*/
	printf("²¹ÂëcomplementCode¼ÆËã\n");
	int onum;
	while (~scanf("%d", &onum)){
		BinaryTransition oBinCode = BinaryTransition(100);
		vector<int> lowTop;
		lowTop.resize(30);
		//Ô­Âë
		int totalBit = BinaryTransition::decimalToRadixLowTop(onum, lowTop.begin(), 2);
		oBinCode.outputWithSymbol(lowTop, 7);

		//·´Âë
		oBinCode.reverseCode(lowTop.begin(), totalBit);
		oBinCode.outputWithSymbol(lowTop, 7);

		++onum;

	}
	return 0;
}

#include"./memory/ArrayLinearList.h"

int main() {
	LinearList::mainForArrayLinearList();
}
