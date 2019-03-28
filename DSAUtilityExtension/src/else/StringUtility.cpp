#include "StringUtility.h"
// wcstombs_s; mbstowcs_s
#include <cstdlib>

namespace StringUtility {

	const std::size_t BUFFER_SIZE = 1024;

	char buffer[BUFFER_SIZE];
	wchar_t wbuffer[BUFFER_SIZE];

	void demo() {
		size_t i;
		char *pCharBuffer = (char *)malloc(BUFFER_SIZE);
		// wchar_t WcharBufferArr[] = L"Hello, world."; // A1
		wchar_t WcharBufferArr[BUFFER_SIZE];
		wcscpy_s(WcharBufferArr, L"Hello, world.");
		wchar_t *pWcharBufferAll = WcharBufferArr;

		printf("Convert wide-character string:\n");

		char CStr[] = "string to convert";
		size_t len = strlen(CStr) + 1;
		size_t converted = 0;
		wchar_t *WStr = (wchar_t*)malloc(len * sizeof(wchar_t));
		mbstowcs_s(&converted, WStr, len, CStr, _TRUNCATE);

		// wchar Conversion to char
		wcstombs_s(&i, pCharBuffer, (size_t)BUFFER_SIZE, WcharBufferArr, (size_t)BUFFER_SIZE);
		memset(pWcharBufferAll, 0, 13 * sizeof(wchar_t));
		// char Conversion to wchar
		// ���ϴ˾��ʹWStr��free�쳣 ��
		// mbstowcs_s(&i, WStr, (size_t)BUFFER_SIZE, pCharBuffer, (size_t)BUFFER_SIZE);
		mbstowcs_s(&i, wbuffer, (size_t)BUFFER_SIZE, pCharBuffer, (size_t)BUFFER_SIZE);
		// dest ΪA1����ʾ�Ŀ��ַ�����Ͳ���: ����ת�� ����ʹsrc�� ����
		mbstowcs_s(&i, pWcharBufferAll, (size_t)BUFFER_SIZE, pCharBuffer, (size_t)BUFFER_SIZE);

		// Output
		// printf(" Characters converted: %u\n", i); // i ��size_t ����unsigned, ��Ȼsize_t������unsigned
		// std::cout << i << std::endl;
		printf(" Multibyte character: %s\n\n", pCharBuffer);

		// Free multibyte character buffer
		free(WStr);
		WStr = NULL;
		free(pCharBuffer);
		pCharBuffer = NULL;
	}

	// @see https://gist.github.com/1901/5684151
	// @see https://www.cnblogs.com/hnrainll/archive/2011/05/07/2039700.html
	// Key words: std::wstring std::string
	/*void toStringFromeWstring(std::wstring const &origin, std::string &dest) {
		std::setlocale(LC_CTYPE, "");

		size_t const mbs_len = wcstombs(NULL, origin.c_str(), 0);
		std::vector<char> tmp(mbs_len + 1);
		wcstombs(&tmp[0], origin.c_str(), tmp.size());

		dest.assign(tmp.begin(), tmp.end() - 1);
	}

	void toWstringFromString(std::string const &origin, std::wstring &dest) {
		std::setlocale(LC_CTYPE, "zh_CN");

		size_t const wcs_len = mbstowcs(NULL, origin.c_str(), 0);
		std::vector<wchar_t> tmp(wcs_len + 1);
		mbstowcs(&tmp[0], origin.c_str(), origin.size());

		dest.assign(tmp.begin(), tmp.end() - 1);
	}*/

	std::string toMstring(std::wstring const &origin) {
		std::setlocale(LC_CTYPE, "");
		std::size_t mbs_len = 0;
		std::string dest;
		// std::wcstombs();
		wcstombs_s(
			&mbs_len,
			// &dest[0], dest.length(), // Expression:sizelnBytes>retsize
			buffer, BUFFER_SIZE,
			&origin[0], origin.length()
		);
		dest.assign(buffer, buffer + origin.length());
		return dest;
	}

	std::wstring toWstring(std::string const &origin) {
		std::setlocale(LC_CTYPE, "zh_CN");
		std::wstring dest;
		std::size_t mbs_len = 0;
		// std::wcstombs();
		mbstowcs_s(
			&mbs_len,
			wbuffer, BUFFER_SIZE,
			&origin[0], origin.length()
		);
		dest.assign(wbuffer, wbuffer + origin.length());
		// stl��string��wstring��ͳһ�����Ĳ�ͬ����, ������ʵ����string���ƶ�(��ֵ��ֵ) @see xstring.h 2300��
		/*
		// ��������û��ʵ����ֵ����, ���Ե���ô�ò���
		std::string s;
		s = toMstring(L"ss");
		*/
		return dest;
	}

}
