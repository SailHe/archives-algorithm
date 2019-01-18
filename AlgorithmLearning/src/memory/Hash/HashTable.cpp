#include "HashTable.h"

namespace HashTableC {
	int NextPrime(int N) {
		int i, p = (N % 2 == 1) ? N + 2 : N + 1; /*�Ӵ���N����һ��������ʼ */

		while (p <= MAX_INT32) {
			for (i = (int)sqrt(p); i > 2; i--)
				if (!(p%i)) break; /* p�������� */
			if (i == 2) break; /* for����������˵��p������ */
			else  p += 2; /* ������̽��һ������ */
		}
		return p;
	}
}
