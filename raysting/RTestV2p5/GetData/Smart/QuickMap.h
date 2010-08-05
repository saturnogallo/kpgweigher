// QuickMap.h

#ifndef _QuickMap_h_
#define _QuickMap_h_

#include "SmartArray.h"
#include "QSort.h"

template<typename ID_TYPE>
struct TMapItemID
{
	ID_TYPE v;

	inline int Compare(ID_TYPE r) const
		{	return v.Compare(r);	}
	inline operator ID_TYPE () const
		{	return v;	}
};

//特例化基本数据类型
template<typename ID_TYPE>
struct TMapSpecialID
{
	ID_TYPE v;

	inline int Compare(ID_TYPE r) const
		{
			if(v > r)
				return 1;
			else if(v < r)
				return -1;
			else
				return 0;
		}
	inline operator ID_TYPE () const
		{	return v;	}
};
#define _SPECIAL_BASE_ID(BASE_TYPE)	\
	template<> struct TMapItemID<BASE_TYPE> : public TMapSpecialID<BASE_TYPE> {}

#if defined(__wtypes_h__) || defined(_WINNT_)
// 对 LPCTSTR 做特例化
template<> struct TMapSpecialID<LPCTSTR>
{
	LPCTSTR v;

	inline int Compare(LPCTSTR r) const
		{	return _tcscmp(v, r);	}
	inline operator LPCTSTR () const
		{	return v;	}
};
_SPECIAL_BASE_ID(LPCTSTR);
#endif

// 用于下面的class CQuickMap
template<typename TYPE, typename ID_TYPE>
struct TMapItem
{
	TYPE					obj;
	TMapItemID<ID_TYPE>		id;

	inline TMapItem() {}

	inline TMapItem(const TMapItem& r)
		: obj(r.obj), id(r.id) {}
	inline TMapItem& operator = (const TMapItem& r)
		{	obj = r.obj;
			id = r.id;
			return *this;
		}

	inline int Compare(const TMapItem& r) const
		{	return id.Compare(r.id.v);	}
};

// 功	能：对象映射『可以通过ID迅速查找对象』
// 参	数：TYPE，需要映射的目标类型；
//			ID_TYPE，id类型，如果使用基本数据类型，
//			如int，使用宏 _SPECIAL_BASE_ID，定义如下
//				#ifndef _SPECIAL_BASE_ID_int
//				#define _SPECIAL_BASE_ID_int
//				_SPECIAL_BASE_ID(int);
//				#endif
//				注：对于LPCTSTR已定义特例化模板『调用_tcscmp()排序』
//			如果不是基本类型，则需要成员函数 int ID_TYPE::Compare(const ID_TYPE& r) const;
//			stack_len，is_simple，同CSmartArray
template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple /* = true */>
class CQuickMap
{
public:
	typedef TMapItem<TYPE, ID_TYPE> ItemType;

//protected:
	CSmartArray<ItemType, stack_len, is_simple> _ar;

protected:
	// 失败返回-1，insert返回应该插入的位置
	int _Find(ID_TYPE id, int* insert) const;

public:
	explicit CQuickMap(int max_maps = stack_len);

public:
	bool IsValid() const;
	int Maps() const;

	int MaxLength() const;
	// 设置一个buffer新长度，必须 max_maps >= _maps
	bool SetMaxLength(int max_maps);

	// bAutoGrow，是否在队列满时自动增加队列缓冲
	// nGrowCount，一次增加的数量
	// index，返回插入的索引，如果插入失败，则该值无效
	bool Add(const TYPE& obj, ID_TYPE id,
			bool bAutoGrow = true, int nGrowCount = 4, int* index = NULL);
	bool Del(ID_TYPE id);

	void DelAll();

	// 返回索引号，失败返回-1
	int FindByID(ID_TYPE id) const;

	// 返回索引号，失败返回-1
	int FindByObj(const TYPE& obj) const;

	// 把id对应的对象值设置为obj，不存在返回false
	bool Set(ID_TYPE id, const TYPE& obj);

	// 添加一个新元素并返回元素指针，失败返回NULL
	TYPE* NewElement(ID_TYPE id, bool bAutoGrow = true, int nGrowCount = 4);

	const ItemType& GetAt(int index) const;
	// 不提供非const GetAt() 操作，提醒用户不能在外面改变id值

	TYPE& operator [] (int index);
	const TYPE& operator [] (int index) const;

	// 从id映射到obj，需要保证id在CQuickMap中一定存在
	TYPE& MappingID(ID_TYPE id);
	const TYPE& MappingID(ID_TYPE id) const;
};

#define _CQuickMap CQuickMap<TYPE, ID_TYPE, stack_len, is_simple>

//=============================================================================//

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline int _CQuickMap::_Find(ID_TYPE id, int* insert) const
{
	// 利用id造一个key『避免了新构造对象的开销』
	ItemType& key = *(ItemType*)( (int)&id - offsetof(ItemType, id) );
	return BSearchObj(_ar.Get(), key, _ar.Items()-1, 0, insert);
}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline _CQuickMap::CQuickMap(int max_maps /* = stack_len */)
	: _ar(max_maps) {}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline bool _CQuickMap::IsValid() const
	{	return _ar.IsValid();	}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline int _CQuickMap::Maps() const
	{	return _ar.Items();	}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline int _CQuickMap::MaxLength() const
	{	return _ar.MaxLength();	}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline bool _CQuickMap::SetMaxLength(int max_maps)
	{	return _ar.SetMaxLength(max_maps);	}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
bool _CQuickMap::Add(const TYPE& obj, ID_TYPE id, bool bAutoGrow /* = true */, int nGrowCount /* = 4 */, int* index /* = NULL */)
{
	int insert;
	if(-1 != _Find(id, &insert))
		return false;	// 已经存在

	if(!_ar.InsertAt(insert, ItemType(), bAutoGrow, nGrowCount))
		return false;

	_ar[insert].obj = obj;
	_ar[insert].id.v = id;

	if(index)
		*index = insert;

	return true;
}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline bool _CQuickMap::Del(ID_TYPE id)
{
	int index;
	if(-1 == _Find(id, &index))
		return false;	// 不存在

	_ar.RemoveAt(index);

	return true;
}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline void _CQuickMap::DelAll()
	{	_ar.RemoveAll();	}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline int _CQuickMap::FindByID(ID_TYPE id) const
	{	return _Find(id, NULL);	}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
int _CQuickMap::FindByObj(const TYPE& obj) const
{
	for(int i=0; i<_ar.Items(); i++) {
		if(obj == _ar[i].obj)
			return i;
	}

	return -1;
}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline bool _CQuickMap::Set(ID_TYPE id, const TYPE& obj)
{
	int index;
	if(-1 == (index=_Find(id, NULL)))
		return false;	// 不存在

	_ar[index].obj = obj;
	return true;
}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline TYPE* _CQuickMap::NewElement(ID_TYPE id, bool bAutoGrow /* = true */, int nGrowCount /* = 4 */)
{
	int index;
	if(!Add(TYPE(), id, bAutoGrow, nGrowCount, &index))
		return NULL;
	return &_ar[index].obj;
}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline const _CQuickMap::ItemType& _CQuickMap::GetAt(int index) const
	{	return _ar[index];	}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline TYPE& _CQuickMap::operator [] (int index)
	{	return _ar[index].obj;	}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline const TYPE& _CQuickMap::operator [] (int index) const
	{	return _ar[index].obj;	}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline TYPE& _CQuickMap::MappingID(ID_TYPE id)
	{	return (*this)[_Find(id, NULL)];	}

template<typename TYPE, typename ID_TYPE, int stack_len, bool is_simple>
inline const TYPE& _CQuickMap::MappingID(ID_TYPE id) const
	{	return ((_CQuickMap*)this)->MappingID(id);	}

#endif //_QuickMap_h_
