// AutoPtr.h

#ifndef _AutoPtr_h_
#define _AutoPtr_h_

#include "SmartBase.h"

#ifdef _MSC_VER
	#pragma warning(disable: 4284)	// CAutoPtr::operator -> not a UDT or reference to a UDT.
#endif

// 功	能：自动指针
// 说	明：
//		在外部释放指针的方法
//		if(ptr.MaxLength() > 1)
//			delete[] ptr.Drop();
//		else if(ptr.MaxLength() == 1)
//			delete ptr.Drop();
template<typename TYPE>
class CAutoPtr : public _CSmartBase
{
public:
	explicit CAutoPtr(int len = 0);
	CAutoPtr(TYPE* p, int len);

	void Release();

	// 左对象的指针将会被释放，r的指针会转移给左对象
	CAutoPtr& operator = (CAutoPtr& r);

	// 克隆，simple_obj，是否简单对象，true使用memcpy，false使用TYPE的赋值运算符
	CAutoPtr& Clone(const CAutoPtr& r, bool simple_obj);

	// 同 operator = ()
	CAutoPtr(CAutoPtr& r);

	~CAutoPtr();

	// 返回 _p
	TYPE* operator -> () const;

	// 返回 *_p
	TYPE& operator * ();

	// 放弃指针控制权，返回_p，len返回元素个数
	TYPE* Drop(int* len = NULL);
};

#define _CAutoPtr CAutoPtr<TYPE>

//=============================================================================//

template<typename TYPE>
_CAutoPtr::CAutoPtr(int len /* = 0 */)
{
	ASSERT(len >= 0);

	if(0 == len)
		_p = NULL;
	else if(1 == len)
		_p = new TYPE();
	else
		_p = new TYPE[len];

	_len = NULL!=_p ? len : 0;
}

template<typename TYPE>
inline _CAutoPtr::CAutoPtr(TYPE* p, int len)
{
	ASSERT(len > 0);
	_p = p;
	_len = NULL!=_p ? len : 0;
}

template<typename TYPE>
void _CAutoPtr::Release()
{
	if(IsValid()) {
		ASSERT(_len > 0);

		if(1 == _len)
			delete _p;
		else
			delete[] _p;

		_len = 0;
		_p = NULL;
	}
}

template<typename TYPE>
_CAutoPtr& _CAutoPtr::operator = (_CAutoPtr& r)
{
	if(this == &r)
		return *this;

	Release();

	_len = r._len;
	_p = r._p;

	r._len = 0;
	r._p = NULL;

	return *this;
}

template<typename TYPE>
_CAutoPtr& _CAutoPtr::Clone(const CAutoPtr& r, bool simple_obj)
{
	if(this == &r)
		return *this;

	*this = CAutoPtr(r._len);

	if(simple_obj) {
		memcpy(_p, r._p, r.MaxSize());
	}
	else {
		for(int i=0; i<r._len; i++)
			_p[i] = r._p[i];
	}

	return *this;
}

template<typename TYPE>
inline _CAutoPtr::CAutoPtr(_CAutoPtr& r)
{
	_len = 0;
	_p = NULL;
	*this = r;
}

template<typename TYPE>
inline _CAutoPtr::~CAutoPtr()
	{	Release();	}

template<typename TYPE>
inline TYPE* _CAutoPtr::operator -> () const
	{	ASSERT(IsValid()); return _p;	}

template<typename TYPE>
inline TYPE& _CAutoPtr::operator * ()
	{	ASSERT(IsValid()); return *_p;	}

template<typename TYPE>
inline TYPE* _CAutoPtr::Drop(int* len /* = NULL */)
{
	if(len)
		*len = _len;

	TYPE* p = _p;
	_len = 0;
	_p = NULL;

	return p;
}

#endif //_AutoPtr_h_
