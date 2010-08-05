// SmartArray.h

#ifndef _SmartArray_h_
#define _SmartArray_h_

#include "PreDef.h"
#include "SmartBuf.h"

#define _CArrayBase CSmartBuf<TYPE, stack_len>

// 功	能：灵巧数组
// 参	数：is_simple，类型TYPE是否为简单类型；
//				简单类型定义：该类的成员不可能引用对象自己的内存。
//				例如，所有语言基本类型、MFC的CString及CArray等都是简单类型；CSmartBuf则不是简单类型。
//			其它参数见CSmartBuf。
// 另 注意：MFC的数组模板类CArray中只能存放简单类型的对象
template<typename TYPE, int stack_len, bool is_simple /* = true */>
class CSmartArray : public _CArrayBase
{
protected:
	int			_items;	// 数组元素个数 <= _len

public:
	explicit CSmartArray(int max_len = stack_len);
	~CSmartArray();

	CSmartArray(const CSmartArray& r);
	CSmartArray& operator = (const CSmartArray& r);

public:
	int Items() const;
	int GetUpperBound() const;

	// 设置一个新的最大长度，如果 max_len < _items，这该函数调用无效
	// 该函数不会影响改变 _items
	bool SetMaxLength(int max_len);	

	// 设置元素个数『会自动调用SetMaxLength』
	void SetItems(int count);

	// bAutoGrow，是否在队列满时自动增加队列缓冲
	// nGrowCount，一次增加的数量
	bool InsertAt(int insert, const TYPE& obj, bool bAutoGrow = true, int nGrowCount = 4);
	bool Add(const TYPE& obj, bool bAutoGrow = true, int nGrowCount = 4);

	void RemoveAt(int index);
	void RemoveAll();
};

#define _CSmartArray CSmartArray<TYPE, stack_len, is_simple>

//=============================================================================//

template<typename TYPE, int stack_len, bool is_simple>
inline _CSmartArray::CSmartArray(int max_len /* = stack_len */)
	: _CArrayBase(max_len)
{
	_items = 0;
	DEBUG_EXP(_valid_cnt = &_items);

	ConstructObjs(_p, _len);
}

template<typename TYPE, int stack_len, bool is_simple>
inline _CSmartArray::~CSmartArray()
{
	DestructObjs(_p, _len);
}

template<typename TYPE, int stack_len, bool is_simple>
inline _CSmartArray::CSmartArray(const _CSmartArray& r)
	: _CArrayBase(0)
{
	_items = 0;
	DEBUG_EXP(_valid_cnt = &_items);

	*this = r;
}

template<typename TYPE, int stack_len, bool is_simple>
_CSmartArray& _CSmartArray::operator = (const _CSmartArray& r)
{
	if(this == &r)
		return *this;

	_items = 0;	// RemoveAll();

	if(_len < r._len)
		SetMaxLength(r._len);

	_items = r._items;// Set new item count.
	for(int i=0; i<_items; i++)	// Copy all right items.
		_p[i] = r._p[i];	// Must call copy function of TYPE.

	return *this;
}

template<typename TYPE, int stack_len, bool is_simple>
inline int _CSmartArray::Items() const
	{	return _items;	}

template<typename TYPE, int stack_len, bool is_simple>
inline int _CSmartArray::GetUpperBound() const
	{	return _items - 1;	}

template<typename TYPE, int stack_len, bool is_simple>
bool _CSmartArray::SetMaxLength(int max_len)
{
	ASSERT(_items <= _len);

	if(max_len < _items) {
		// 忽略
		return false;
	}
	else if(_items<=max_len && max_len<=_len) {
		bool bStatck = IsStack();

		DestructObjs(_p+max_len, _len-max_len);	// 析构多余的对象
		_len = max_len;

		if(!bStatck && _len<=StackLen()) {
			// 如果原来在全局堆中，并且新长度 <= 堆栈长度，则把对象移到堆栈buffer中

			if(is_simple)
				memcpy(_stack, _p, _len * sizeof(TYPE));
			else
				MoveConstruct((TYPE*)_stack, _p, _len);

			_p = (TYPE*)_stack;

			delete[] (int*)_heap;
			_heap = NULL;
		}
	}
	else {	// max_len > _len
		if(max_len <= StackLen()) {
			ASSERT(NULL == _heap);
			ASSERT(IsStack());

			ConstructObjs(_p+_len, max_len-_len);
			_len = max_len;
		}
		else {
			bool bStatck = IsStack();

			_heap = (TYPE*)new int[_CALC_INT_LEN(TYPE, max_len)];

			if(is_simple)
				memcpy(_heap, _p, _len * sizeof(TYPE));
			else
				MoveConstruct(_heap, _p, _len);

			ConstructObjs(_heap+_len, max_len-_len);

			if(!bStatck)
				delete[] (int*)_p;

			_p = _heap;
			_len = max_len;
		}
	}

	return true;
}

template<typename TYPE, int stack_len, bool is_simple>
inline void _CSmartArray::SetItems(int count)
{
	ASSERT(count >= 0);
	SetMaxLength(count);
	_items = count;
}

template<typename TYPE, int stack_len, bool is_simple>
bool _CSmartArray::InsertAt(int insert, const TYPE& obj, bool bAutoGrow /* = true */, int nGrowCount /* = 4 */)
{
	ASSERT(_items <= _len);
	ASSERT(0<=insert && insert<=_items);

	// 已满
	if(_items == _len) {
		if(!bAutoGrow) {
			return false;
		}
		else {	// 增加队列缓冲
			ASSERT(nGrowCount > 0);
			if(!SetMaxLength(_len + nGrowCount))
				return false;
		}
	}

	if(insert < _items) {
		if(is_simple) {
			//简单类型，直接操作对象内存
			char buf[sizeof(TYPE)];
			memcpy(buf, _p+_items, sizeof(TYPE));
			memmove(_p+insert+1, _p+insert, (_items-insert)*sizeof(TYPE));
			memcpy(_p+insert, buf, sizeof(TYPE));
		}
		else {
			//复制类型，必须调用对象的拷贝函数
			for(int i=_items-1; i>=insert; i--)
				_p[i+1] = _p[i];
		}
	}
	else
		ASSERT(insert == _items);

	_p[insert] = obj;
	_items++;

	return true;
}

template<typename TYPE, int stack_len, bool is_simple>
inline bool _CSmartArray::Add(const TYPE& obj, bool bAutoGrow /* = true */, int nGrowCount /* = 4 */)
{
	return InsertAt(_items, obj, bAutoGrow, nGrowCount);
}

template<typename TYPE, int stack_len, bool is_simple>
void _CSmartArray::RemoveAt(int index)
{
	ASSERT(0<=index && index<_items);

	if(index < _items-1) {
		if(is_simple) {
			char buf[sizeof(TYPE)];
			memcpy(buf, _p+index, sizeof(TYPE));
			memmove(_p+index, _p+index+1, (_items-1-index)*sizeof(TYPE));
			memcpy(_p+_items-1, buf, sizeof(TYPE));
		}
		else {
			for(int i=index+1; i<_items; i++)
				_p[i-1] = _p[i];
		}
	}

	_items--;
}

template<typename TYPE, int stack_len, bool is_simple>
inline void _CSmartArray::RemoveAll()
	{	_items = 0;	}

#endif //_SmartArray_h_
