#include <fstream>
#include <string>
#include <iostream>
#include <streambuf>
using namespace std;

// 按照此思路 很容易就能输入或输出一个需要的excel 文件
int mainForCsv() {
	// 定义文件输出流
	ofstream oFile;

	// 打开要输出的文件
	oFile.open("scoresheet.csv", ios::out | ios::trunc);
	oFile << "姓名" << "," << "年龄" << "," << "班级" << "," << "班主任" << endl;
	oFile << "张三" << "," << "22" << "," << "1" << "," << "JIM" << endl;
	oFile << "李四" << "," << "23" << "," << "3" << "," << "TOM" << endl;
	oFile.close();


	// 打开要输入的文件 
	ifstream iFile("scoresheet.csv");
	string   readStr((std::istreambuf_iterator<char>(iFile)), std::istreambuf_iterator<char>());
	cout << readStr.c_str();

	return 0;
}