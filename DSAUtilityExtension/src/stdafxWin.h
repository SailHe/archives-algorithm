// ������WinϵAPI��stdafx

#pragma once
#include "stdafx.h"

// ��ͻmax��: minwindef.h
// The parentheses around max prevent macro expansion. This works with all function macros.
// @see https://stackoverflow.com/questions/7449620/how-to-call-stdmin-when-min-has-been-defined-as-a-macro
// (std::max)(a, b)
// MAXINT32�Ⱥ�
// QueryPerformanceFrequency; CreateDirectory; 
#include <windows.h>
