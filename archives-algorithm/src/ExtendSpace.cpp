#include "ExtendSpace.h" 
namespace StandardExtend{
	/****************Ascll*******************/

	char toLowerAlph(char c){
		if ('0' <= c && c <= '9'){
			return 'a' + c - '0';
		}
		else{
			if ('A' <= c && c <= 'Z'){
				return 'a' + c - 'A';
			}
			else{
				return c;
			}
		}
	}

	int toIntNum(char alphOrCharNum){
		if ('0' <= alphOrCharNum && alphOrCharNum <= '9'){
			return alphOrCharNum - '0';
		}
		else{
			if ('A' <= alphOrCharNum && alphOrCharNum <= 'Z'){
				return 10 + alphOrCharNum - 'A';
			}
			else{
				return 10 + alphOrCharNum - 'a';
			}
		}
	}

	//10ToA(Alph)
	char toUppercaseAscllChar(int num){
		return num > 9 ? num - 10 + 'A' : num + '0';
	}

	//'A' == 'a' == '0' ... 'J' == 'j' == '9' 以此类推
	bool isAa0Equal(char a, char b){
		return toLowerAlph(a) == toLowerAlph(b);
	}
};
