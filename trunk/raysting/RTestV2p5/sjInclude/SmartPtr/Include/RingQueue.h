// RingQueue.h

#ifndef _RingQueue_h_
#define _RingQueue_h_

#include "SmartArray.h"

// 功	能：环形队列
// 注	意：对象尺寸不可以动态改变
template<typename TYPE, int stack_len>
class CRingQueue
{
protected:
	CSmartArray<TYPE, stack_len, false> _ar;

	int	_head;	// 指向最旧的元素
	int	_tail;	// 指向下一个添加元素位置

public:
	explicit CRingQueue(int max_len = stack_len);

	bool IsValid() const;

	// 队列可以存放的最多元素
	int ItemRooms() const;

	// 得到队列的元素个数
	int ItemCount() const;

	bool IsNull() const;
	bool IsFull() const;


	// Push()、PopOld()、PopNew() 操作说明
	// 成功返回元素索引号，失败返回-1
	// 返回的索引号应该在其它任何队列操作前使用
	// 见 operator []

	// 在队列尾查入新元素，返回-1表示队列已满『CRingQueue对象的尺寸不可以动态改变』
	int Push();

	// Pop出最旧的元素
	int PopOld();

	// Pop出最旧的元素
	int PopNew();

	// 清除所有元素
	void PopAll();

	// 得到元素引用『可以访问所有元素，包括队列外的无效元素』
	// 请使用 Push()、PopOld()、PopNew()返回的索引号
	TYPE& operator [] (int index);
	const TYPE& operator [] (int index) const;

	// index == 0，取最旧元素；index == ItemCount()-1，取最新的元素
	// 可以用于遍历队列
	TYPE& GetAtBottom(int index);
	const TYPE& GetAtBottom(int index) const;
};

#define _CRingQueue CRingQueue<TYPE, stack_len>

//=============================================================================//

template<typename TYPE, int stack_len>
inline _CRingQueue::CRingQueue(int max_len /* = stack_len */)
	: _ar(max_len), _head(0), _tail(0) {}

template<typename TYPE, int stack_len>
inline bool _CRingQueue::IsValid() const
	{	return _ar.IsValid();	};

template<typename TYPE, int stack_len>
inline int _CRingQueue::ItemRooms() const
	{	return _ar.MaxLength();	}

template<typename TYPE, int stack_len>
inline int _CRingQueue::ItemCount() const
	{	return _ar.Items();	}

template<typename TYPE, int stack_len>
inline bool _CRingQueue::IsNull() const
	{	return 0 == ItemCount();	}

template<typename TYPE, int stack_len>
inline bool _CRingQueue::IsFull() const
	{	return ItemRooms() == ItemCount();	}

template<typename TYPE, int stack_len>
int _CRingQueue::Push()
{
	if(IsFull())
		return -1;

	int index = _tail;

	_ar.SetItems(_ar.Items()+1);
	_tail++;
	if(_tail == _ar.MaxLength())
		_tail = 0;

	return index;
}

template<typename TYPE, int stack_len>
int _CRingQueue::PopOld()
{
	if(IsNull())
		return -1;

	int index = _head;

	_ar.SetItems(_ar.Items()-1);
	_head++;
	if(_head == _ar.MaxLength())
		_head = 0;

	return index;
}

template<typename TYPE, int stack_len>
int _CRingQueue::PopNew()
{
	if(IsNull())
		return -1;

	_ar.SetItems(_ar.Items()-1);
	_tail--;
	if(-1 == _tail)
		_tail = _ar.MaxLength()-1;

	return _tail;
}

template<typename TYPE, int stack_len>
inline void _CRingQueue::PopAll()
	{	_head = _tail = 0;
		_ar.SetItems(0);	}

template<typename TYPE, int stack_len>
inline TYPE& _CRingQueue::operator [] (int index)
	{	ASSERT(0<=index && index<_ar.MaxLength());
		return _ar.Get()[index];	}

template<typename TYPE, int stack_len>
inline const TYPE& _CRingQueue::operator [] (int index) const
	{	return (*(_CRingQueue*)this)[index];	}

template<typename TYPE, int stack_len>
inline TYPE& _CRingQueue::GetAtBottom(int index)
{
	ASSERT(0<=index && index<ItemCount());

	index += _head;
	if(index >= _ar.MaxLength())
		index -= _ar.MaxLength();

	return (*this)[index];
}

template<typename TYPE, int stack_len>
inline const TYPE& _CRingQueue::GetAtBottom(int index) const
	{	return ((_CRingQueue*)this)->GetAtBottom(index);	}

#endif //_RingQueue_h_
