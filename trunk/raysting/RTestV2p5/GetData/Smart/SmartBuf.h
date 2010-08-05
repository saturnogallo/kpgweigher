// SmartBuf.h

#ifndef _SmartBuf_h_
#define _SmartBuf_h_

#include "SmartBase.h"

// 计算对齐为sizeof(int)的类型TYPE的尺寸
#define _CALC_INT_LEN(TYPE, len)	\
	( (sizeof(TYPE) * (len) + sizeof(int)-1) / sizeof(int) )

// 功	能：灵巧Buffer，用它定义局部数组时，小Buffer使用堆栈，大Buffer在全局堆中申请
// 参	数：TYPE，任意类型；stack_len，常数『>=0』，在堆栈里定义的类型TYPE的数组尺寸
// 注	意：CSmartBuf不会调用类型TYPE的构造『TYPE::TYPE()』和析构『TYPE::~TYPE()』函数，
//			可以在外面调用ConstructObjs(...) 和 DestructObjs(...)来构造和析构对象
template<typename TYPE, int stack_len>
class CSmartBuf : public _CSmartBase
{
	// 禁止拷贝和拷贝构造
	CSmartBuf(const CSmartBuf&);
	CSmartBuf& operator = (const CSmartBuf&);

protected:
	TYPE*	_heap;	// 全局堆buffer
	int		_stack[_CALC_INT_LEN(TYPE, stack_len)];	// 堆栈buffer

public:
	// 得到堆栈buffer字节长度
	const int StackSize() const;

	// 得到堆栈buffer长度
	const int StackLen() const;

	// 是否是使用的堆栈buffer
	bool IsStack() const;

	explicit CSmartBuf(int len = stack_len);
	~CSmartBuf();
};

#define _CSmartBuf CSmartBuf<TYPE, stack_len>

//=============================================================================//

template<typename TYPE, int stack_len>
inline const int _CSmartBuf::StackSize() const
{
#ifdef _DEBUG
	int size = sizeof(int) * _CALC_INT_LEN(TYPE, stack_len); size;
#endif
	return sizeof(int) * _CALC_INT_LEN(TYPE, stack_len);
}

template<typename TYPE, int stack_len>
inline const int _CSmartBuf::StackLen() const
{
#ifdef _DEBUG
	int len = StackSize() / sizeof(TYPE); len;
#endif
	return StackSize() / sizeof(TYPE);
}

template<typename TYPE, int stack_len>
inline bool _CSmartBuf::IsStack() const
{
#ifdef _DEBUG
	ASSERT(NULL != _p);
	if(_p == (TYPE*)_stack) {
		ASSERT(_len <= StackLen());
		ASSERT(NULL == _heap);
	}
	else {
		ASSERT(_p == _heap);
		ASSERT(_len > StackLen());
	}
#endif
	return _p == (TYPE*)_stack;
}

template<typename TYPE, int stack_len>
inline _CSmartBuf::CSmartBuf(int len /* = stack_len */)
{
#ifdef _DEBUG
	// 预定义堆栈尺寸不需要太大，否则没有什么实际意义
	if(StackSize() > 512*1024)
		TRACE("stack_size too large: %.3fkb", StackSize()/1024.0);
#endif	
	
	ASSERT(len >= 0);

	if(len <= StackLen()) {
		_heap = NULL;
		_p = (TYPE*)_stack;
	}
	else {
		_p = _heap = (TYPE*)new int[_CALC_INT_LEN(TYPE, len)];
	}

	_len = NULL!=_p ? len : 0;
}

template<typename TYPE, int stack_len>
inline _CSmartBuf::~CSmartBuf()
{
	if(_heap)
		delete[] (int*)_heap;
}

#endif //_SmartBuf_h_
