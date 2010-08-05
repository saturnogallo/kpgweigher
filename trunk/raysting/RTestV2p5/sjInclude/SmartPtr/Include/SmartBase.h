// SmartBase.h

#ifndef _SmartBase_h_
#define _SmartBase_h_

#include "DbgHelp.h"

// 模板基类
template<typename TYPE>
class CSmartBase
{
protected:
	int		_len;	// 元素个数
	TYPE*	_p;		// 元素Buffer

#ifdef _DEBUG
	int*	_valid_cnt;	// 指向记录可用元素个数的变量
#endif

protected:
	CSmartBase();

public:
	bool IsValid() const;

	int MaxLength() const;	// 元素个数
	int MaxSize() const;	// 字节尺寸

	// 返回 _p
	TYPE* Get();
	const TYPE* Get() const;

	// 返回 _p[index]
	TYPE& operator [] (int index);
	const TYPE& operator [] (int index) const;

	// 返回 _p+index
	TYPE* operator + (int index);
	const TYPE* operator + (int index) const;

	// 不提供 operator TYPE* ()『容易引起混乱，需要明确调用函数 Get()』
};

#define _CSmartBase CSmartBase<TYPE>

template<typename TYPE>
inline _CSmartBase::CSmartBase()
	{	DEBUG_EXP(_valid_cnt = &_len);	}

template<typename TYPE>
inline bool _CSmartBase::IsValid() const
	{	return NULL != _p;	}

template<typename TYPE>
inline int _CSmartBase::MaxLength() const
	{	return _len;	}

template<typename TYPE>
inline int _CSmartBase::MaxSize() const
	{	return _len * sizeof(TYPE);	}

template<typename TYPE>
inline TYPE* _CSmartBase::Get()
	{	return _p;	}

template<typename TYPE>
inline const TYPE* _CSmartBase::Get() const
	{	return _p;	}

template<typename TYPE>
inline TYPE& _CSmartBase::operator [] (int index)
	{	ASSERT(0 <= index && index < *_valid_cnt); return _p[index];	}

template<typename TYPE>
inline const TYPE& _CSmartBase::operator [] (int index) const
	{	ASSERT(0 <= index && index < *_valid_cnt); return _p[index];	}

template<typename TYPE>
inline TYPE* _CSmartBase::operator + (int index)
	{	ASSERT(0 <= index && index < *_valid_cnt); return _p+index;		}

template<typename TYPE>
inline const TYPE* _CSmartBase::operator + (int index) const
	{	ASSERT(0 <= index && index < *_valid_cnt); return _p+index;		}

#endif //_SmartBase_h_
