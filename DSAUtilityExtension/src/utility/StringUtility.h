#pragma once
#include "../pch.h"
#include "../DSAUEdll.h"
// 指定编码(c语言库locale.h)
#include <clocale>

namespace StringUtility {

	// 把一个wstring转化为string
	// DSAUTILITYEXTENSION_API void toStringFromeWstring(std::wstring const &origin, std::string &dest);
	DSAUTILITYEXTENSION_API std::string toMstring(std::wstring const &origin);
	// 把一个string转化为wstring
	// DSAUTILITYEXTENSION_API void toWstringFromString(std::string const &origin, std::wstring &dest);
	DSAUTILITYEXTENSION_API std::wstring toWstring(std::string const &origin);

}
