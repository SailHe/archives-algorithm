#pragma once
#include <cstdlib>
#include <cassert>
#include <cstring>
#include "../DSAUEdll.h"

// ��̬�������� ģ�� (VirtualVariableArray)
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
	// (�������ڴ�ָ���ָ��, �������С)
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
	// ��ʹ�õĴ�С(һ����__capacity��� �������ûɶ�û�ɾ��)
	SizeType __size = 0u;
	// �����С(�����������)
	SizeType __capacity = 0u;
};

/*
�ۺ���(Struct)��
	1. ����
	2. [û��]��Щ���ݵ���, �ṹ������:
		���캯��(ʹ��new������ʱ���ܹ���);
		˽�л��ܱ����ĳ�Ա;
		����;
		�麯��(���ʹ��memset��ʼ���Ļ��麯����������);
����ƷǾۺ���
*/
// �ɱ�������������(��̬����) �ɳ�: DigitBitArray(����λԪ����) ʹ��realloc�����ڴ� ������Ч�ʸ���Varrays<Digit>
class DSAUTILITYEXTENSION_API DigitVarrays {

	// ����(�����Ǿۺ���)
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
	// (�������ڴ�ָ���ָ��, �������С)
	static void reallocProcess(Digit **result, SizeType newSize) {
		*result = (Digit*)realloc(*result, sizeof(Digit)*newSize);
		assert(*result != NULL);
	}
	// (��ʼ���ڴ�ָ��, ������С, ��ʼ����С)
	static void init(Digit *result, SizeType keepSize, SizeType initSize) {
		if (initSize >= keepSize) {
			// ��ʼ��(�ų�ԭ�е�__size)
			memset(result + keepSize, 0, sizeof(Digit)*(initSize - keepSize));
		}
	}

	int *digitArr_ = NULL;
	// ��ʹ�õĴ�С(һ����__capacity��� �������ûɶ�û�ɾ��)
	SizeType __size = 0u;
	// �����С(�����������)
	SizeType __capacity = 0u;
};
