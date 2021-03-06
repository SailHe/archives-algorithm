// MathLibrary.cpp : Defines the exported functions for the DLL.
// @see https://docs.microsoft.com/zh-cn/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=vs-2017
// 项目属性的C/C++预处理器中已经有了 DSAUTILITYEXTENSION_EXPORTS 但如果使用的是不同的宏名 则需在此处手动定义(否则与普通CPP文件无异)
//#define MATHLIBRARY_EXPORTS
//#include "stdafx.h"

#include <utility>
#include <limits.h>
#include <string>
#include "../MathLibrary.h"

// DLL internal state variables:
static unsigned long long previous_;  // Previous value, if any
static unsigned long long current_;   // Current sequence value
static unsigned index_;               // Current seq. position

// Initialize a Fibonacci relation sequence
// such that F(0) = a, F(1) = b.
// This function must be called before any other function.
void fibonacci_init(
	const unsigned long long a,
	const unsigned long long b)
{
	index_ = 0;
	current_ = a;
	previous_ = b; // see special case when initialized
}

// Produce the next value in the sequence.
// Returns true on success, false on overflow.
bool fibonacci_next()
{
	// check to see if we'd overflow result or position
	if ((ULLONG_MAX - previous_ < current_) ||
		(UINT_MAX == index_))
	{
		return false;
	}

	// Special case when index == 0, just return b value
	if (index_ > 0)
	{
		// otherwise, calculate next sequence value
		previous_ += current_;
	}
	std::swap(current_, previous_);
	++index_;
	return true;
}

// Get the current value in the sequence.
unsigned long long fibonacci_current()
{
	return current_;
}

// Get the current index position in the sequence.
unsigned fibonacci_index()
{
	return index_;
}

std::string DllHelloWorld() {
	return "Hello World!";
}

namespace DllTestNameSpace {
	std::string dllHelloWorldInNameSpace() {
		return DllHelloWorld();
	}
}

