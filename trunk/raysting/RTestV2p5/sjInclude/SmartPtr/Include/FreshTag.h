// FreshTag.h

#ifndef _FreshTag_h_
#define _FreshTag_h_

#include "SmartArray.h"

// 功	能：最新对象管理器
// 注	意：对象尺寸不可以动态改变
template<typename TYPE, int stack_len>
class CFreshTag
{
protected:
	CSmartArray<TYPE, stack_len, false> _ar;

protected:
	int Find(const TYPE& obj) const;

public:
	explicit CFreshTag(int max_tags = stack_len);

	int Tags() const;

	// 在队列末添加一个新元素，如果原来已经存在，则把obj移到最后
	// 如果原来队列已满，返回false『CFreshTag的尺寸不可以动态改变』
	// 否则返回true
	bool Add(const TYPE& obj);

	// 队列中没有obj返回false
	bool Del(const TYPE& obj);

	// 取最后一个元素索引号，没有返回-1
	int Last() const;

	// 元素访问
	TYPE& operator [] (int index);
	const TYPE& operator [] (int index) const;
};

#define _CFreshTag CFreshTag<TYPE, stack_len>

//=============================================================================//

template<typename TYPE, int stack_len>
inline int _CFreshTag::Find(const TYPE& obj) const
{
	for(int i=0; i<_ar.Items(); i++) {
		if(obj == _ar[i])
			return i;
	}
	return -1;
}

template<typename TYPE, int stack_len>
inline _CFreshTag::CFreshTag(int max_tags /* = stack_len */)
	: _ar(max_tags) {}

template<typename TYPE, int stack_len>
inline int _CFreshTag::Tags() const
	{	return _ar.Items();	}

template<typename TYPE, int stack_len>
bool _CFreshTag::Add(const TYPE& obj)
{
	int i = Find(obj);

	if(-1 == i) {	// 原来的队列里面没有obj
		if(_ar.Items() == _ar.MaxLength()) {	// 已满
			ASSERT(false);
			return false;
		}
		else {
			return _ar.Add(obj, false);
		}
	}
	else {			// 把obj移动到最后
		for(int j=i; j<_ar.Items()-1; j++) {
			_ar[j] = _ar[j+1];
		}

		_ar[_ar.Items()-1] = obj;
		return true;
	}
}

template<typename TYPE, int stack_len>
bool _CFreshTag::Del(const TYPE& obj)
{
	int i = Find(obj);
	if(-1 == i)
		return false;

	_ar.RemoveAt(i);
	return true;
}

template<typename TYPE, int stack_len>
inline int _CFreshTag::Last() const
	{	return Tags()>0 ? Tags()-1 : -1;	}

template<typename TYPE, int stack_len>
inline TYPE& _CFreshTag::operator [] (int index)
	{	return _ar[index];	}

template<typename TYPE, int stack_len>
inline const TYPE& _CFreshTag::operator [] (int index) const
	{	return _ar[index];	}

#endif //_FreshTag_h_
