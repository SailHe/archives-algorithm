#pragma warning(disable:4996)
#include "Teater.h"


//using namespace StandardExtend;
using namespace std;

int mainForDllTest() {
	// StandardExtend::toLowerAlph('c');
	cout << DllHelloWorld() << endl;
	cout << DllTestNameSpace::dllHelloWorldInNameSpace() << endl;
	auto temp = DllTestNameSpace::DllTestClass(200);
	cout << DllTestNameSpace::DllTestClass(1221).toString() << endl;


	// Initialize a Fibonacci relation sequence.
	fibonacci_init(1, 1);
	// Write out the sequence values until overflow.
	do {
		if (92 == fibonacci_index()) {
			StandardExtend::testAndOut("���һ��u long long ��Χ�ڵķƲ�������", fibonacci_current(), 12200160415121876738);
			/*std::cout << fibonacci_index() << ": "
				<< fibonacci_current() << std::endl;*/
		}
	} while (fibonacci_next());
	// Report count of values written before overflow.
	/*std::cout << fibonacci_index() + 1 <<
		" Fibonacci sequence values fit in an " <<
		"unsigned 64-bit integer." << std::endl;*/
	return 0;
}

// ����/ƽ̨ �Ĺ����Բ���
int runfunctionalTest() {
	//FILE *inFile = stdin, *outFile;
	//freopen("input", "r", stdin);
	//freopen_s(&inFile, "input", "r", stdin);
	StandardExtend::outputDebugFormat("�������ʽ���ַ����������%d\n", 10);
	mainForDllTest();
	
	return 0;
}

int main(int argc, char const *argv[]) {
	runfunctionalTest();
	runRadixTest();
	runDataStructureTest();

	return 0;
}
