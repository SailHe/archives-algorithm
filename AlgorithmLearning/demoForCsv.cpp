#include <fstream>
#include <string>
#include <iostream>
#include <streambuf>
using namespace std;

// ���մ�˼· �����׾�����������һ����Ҫ��excel �ļ�
int mainForCsv() {
	// �����ļ������
	ofstream oFile;

	// ��Ҫ������ļ�
	oFile.open("scoresheet.csv", ios::out | ios::trunc);
	oFile << "����" << "," << "����" << "," << "�༶" << "," << "������" << endl;
	oFile << "����" << "," << "22" << "," << "1" << "," << "JIM" << endl;
	oFile << "����" << "," << "23" << "," << "3" << "," << "TOM" << endl;
	oFile.close();


	// ��Ҫ������ļ� 
	ifstream iFile("scoresheet.csv");
	string   readStr((std::istreambuf_iterator<char>(iFile)), std::istreambuf_iterator<char>());
	cout << readStr.c_str();

	return 0;
}