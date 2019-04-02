#pragma once
#include <cstdlib>
#include <cassert>
#include <cstring>
#include "../DSAUEdll.h"

// 动态数组容器 模板 (VirtualVariableArray)
template<class ET>
class Varrays {
protected:
	// typedef typename int ET;
	typedef typename ET *Position;
	typedef typename ET *Iterator;
	typedef unsigned SizeType;

public:
	Varrays() {
		resize(0);
	}
	Varrays(SizeType size) {
		resize(size);
	}
	~Varrays() {
		delete[] baseImplArr;
		baseImplArr = nullptr;
	}

	void resize(SizeType newSize) {
		if (newSize <= __capacity) {
			// DNT
		}
		else {
			reallocProcess(&baseImplArr, __size, newSize);
		}
		__size = newSize;
	}
	void reserver(SizeType newCapacity) {
		reallocProcess(&baseImplArr, __size, newCapacity);
		__capacity = newCapacity;
	}
	template<typename Iterator1>
	void assign(Iterator1 begin, Iterator1 end) {
		Iterator tBegin = this->begin();
		Iterator tEnd = this->end();
		while (begin != end) {
			if (tBegin != tEnd) {
				*tBegin = *begin;
				++tBegin;
			}
			else {
				push_back(*begin);
			}
			++begin;
		}
	}
	void push_back(ET ele) {
		if (__size < __capacity) {
			// DNT
		}
		else {
			reserver(2 * __size + 1);
		}
		resize(__size + 1);
		*(end() - 1) = ele;
	}
	SizeType size() {
		return __size;
	}
	void clear() {
		__size = 0;
	}
	ET &operator[](int i) {
		assert(0 <= i);
		assert((SizeType)i < __size);
		return baseImplArr[i];
	}
	Iterator begin() {
		return baseImplArr;
	}
	Iterator end() {
		return baseImplArr + __size;
	}

private:
	// (重申请内存指针的指针, 新申请大小)
	static void reallocProcess(ET **result, SizeType oldSize, SizeType newSize) {
		Position newArr = new ET[newSize];
		assert(newArr != nullptr);
		SizeType tegSize = newSize > oldSize ? oldSize : newSize;
		for (SizeType i = 0; i < tegSize; ++i) {
			newArr[i] = (*result)[i];
		}
		delete[](*result);
		*result = newArr;
	}

	typedef typename Position BaseImpl;
	BaseImpl baseImplArr = nullptr;
	// 已使用的大小(一般与__capacity相等 这个可能没啥用会删除)
	SizeType __size = 0u;
	// 数组大小(已申请的容量)
	SizeType __capacity = 0u;
};

/*
聚合类(Struct)：
	1. 数组
	2. [没有]这些内容的类, 结构和联合:
		构造函数(使用new操作符时才能构造);
		私有或受保护的成员;
		基类;
		虚函数(如果使用memset初始化的话虚函数表会出问题);
否则称非聚合类
*/
// 可变整数数字数组(动态数组) 旧称: DigitBitArray(数字位元数组) 使用realloc申请内存 理论上效率高于Varrays<Digit>
class DSAUTILITYEXTENSION_API DigitVarrays {

	// 数字(必须是聚合类)
	typedef int Digit;
	typedef unsigned SizeType;

public:

	// BitIter -> Biter
	typedef Digit *Iterator;

	DigitVarrays() {
		resize(0);
	}
	DigitVarrays(SizeType size) {
		resize(size);
	}
	~DigitVarrays() {
		free(digitArr_);
		digitArr_ = NULL;
	}
	void resize(SizeType newSize) {
		if (newSize <= __capacity) {
			// DNT
		}
		else {
			reallocProcess(&digitArr_, newSize);
		}
		init(digitArr_, __size, newSize);
		__size = newSize;
	}
	void reserver(SizeType newCapacity) {
		reallocProcess(&digitArr_, newCapacity);
		// init(digitArr_, __capacity, newCapacity);
		__capacity = newCapacity;
	}
	template<typename Iterator1>
	void assign(Iterator1 begin, Iterator1 end) {
		Iterator tBegin = this->begin();
		Iterator tEnd = this->end();
		while (begin != end) {
			if (tBegin != tEnd) {
				*tBegin = *begin;
				++tBegin;
			}
			else {
				push_back(*begin);
			}
			++begin;
		}
	}
	void push_back(Digit ele) {
		if (__size < __capacity) {
			// DNT
		}
		else {
			reserver(2 * __size + 1);
		}
		resize(__size + 1);
		*(end() - 1) = ele;
	}
	SizeType size() {
		return __size;
	}
	void clear() {
		init(digitArr_, 0, __size);
		__size = 0;
	}
	Digit &operator[](int i) {
		assert(0 <= i);
		assert((SizeType)i < __size);
		return digitArr_[i];
	}
	Iterator begin() {
		return digitArr_;
	}
	Iterator end() {
		return digitArr_ + __size;
	}

private:
	// (重申请内存指针的指针, 新申请大小)
	static void reallocProcess(Digit **result, SizeType newSize) {
		*result = (Digit*)realloc(*result, sizeof(Digit)*newSize);
		assert(*result != NULL);
	}
	// (初始化内存指针, 保留大小, 初始化大小)
	static void init(Digit *result, SizeType keepSize, SizeType initSize) {
		if (initSize >= keepSize) {
			// 初始化(排除原有的__size)
			memset(result + keepSize, 0, sizeof(Digit)*(initSize - keepSize));
		}
	}

	int *digitArr_ = NULL;
	// 已使用的大小(一般与__capacity相等 这个可能没啥用会删除)
	SizeType __size = 0u;
	// 数组大小(已申请的容量)
	SizeType __capacity = 0u;
};
