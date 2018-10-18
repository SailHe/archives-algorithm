/*
#include <iostream>
using namespace std;

string s1, s2;
void vChange();
string sCal();
void vPrint(string s);

int main()
{
	cin >> s1 >> s2;
	vChange();
	vPrint(sCal());
	return 0;
}
string sCal()
{
	string ans = "";
	int i, s, sa, sb, carry = 0;
	for (i = s1.length() - 1; i >= 0; i--)
	{
		sa = s1[i] - '0';
		sb = s2[i] - '0';
		s = (sa + sb + carry) % 10;
		carry = (sa + sb + carry) / 10;
		ans += s + '0';
	}
	if (carry == 1)
	{
		ans += '1';
	}
	return ans;
}
void vPrint(string s)
{
	int j, i;
	for (i = s.length() - 1; i >= 0; i--)
	{
		if (s[i] != '0')
		{
			j = i;
			break;
		}
	}
	if (i == -1)
		cout << '0';
	else
	{
		for (; j >= 0; j--)
			cout << s[j];
	}
}
void vChange()
{
	string tmp;
	int s1_len, s2_len;
	s1_len = s1.size();
	s2_len = s2.size();
	if (s1_len > s2_len) {
		tmp.resize(s1_len - s2_len, '0');
		s2 = tmp + s2;
	}
	else if (s1_len < s2_len) {
		tmp.resize(s2_len - s1_len, '0');
		s1 = tmp + s1;
	}
}

*/