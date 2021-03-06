// stdafx.h: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>

// 在此处引用程序需要的其他标头

struct SystemTime
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int millsecond;

	SystemTime & operator= (SystemTime st)
	{
		this->year = st.year;
		this->month = st.month;
		this->day = st.day;
		this->hour = st.hour;
		this->minute = st.minute;
		this->second = st.second;
		this->millsecond = st.millsecond;
		return *this;
	}
};

