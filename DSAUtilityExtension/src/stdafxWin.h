// 包含了Win系API的stdafx

#pragma once
#include "stdafx.h"

// 冲突max宏: minwindef.h
// The parentheses around max prevent macro expansion. This works with all function macros.
// @see https://stackoverflow.com/questions/7449620/how-to-call-stdmin-when-min-has-been-defined-as-a-macro
// (std::max)(a, b)
// MAXINT32等宏
// QueryPerformanceFrequency; CreateDirectory; 
#include <windows.h>
