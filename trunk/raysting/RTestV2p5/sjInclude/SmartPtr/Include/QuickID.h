// QuickID.h

#ifndef _QuickID_h_
#define _QuickID_h_

#include "QuickMap.h"

typedef unsigned int UINT;

#ifndef _SPECIAL_BASE_ID_UINT
#define _SPECIAL_BASE_ID_UINT
_SPECIAL_BASE_ID(UINT);
#endif

// 功	能：ID管理器，可以快速的添加、删除、查询ID
template<int stack_len>
class CQuickID
{
protected:
	CQuickMap<UINT, UINT, stack_len, true> _map;

public:
	explicit CQuickID(int max_count = stack_len);

public:
	bool IsValid() const;

	int MaxCount() const;
	bool SetMaxCount(int count);

	bool Add(UINT id, bool bAutoGrow = true, int nGrowCount = 64);
	bool Del(UINT id);
	void DelAll();

	bool Find(UINT id) const;
	int Count() const;

	UINT operator [] (int index) const;
};

#define _CQuickID CQuickID<stack_len>

//=============================================================================//

template<int stack_len>
inline _CQuickID::CQuickID(int max_count /* = stack_len */)
	: _map(max_count) {}

template<int stack_len>
inline bool _CQuickID::IsValid() const
	{	return _map.IsValid();	}

template<int stack_len>
inline int _CQuickID::MaxCount() const
	{	return _map.MaxLength();	}

template<int stack_len>
inline bool _CQuickID::SetMaxCount(int count)
	{	return _map.SetMaxLength(count);	}

template<int stack_len>
inline bool _CQuickID::Add(UINT id, bool bAutoGrow /* = true */, int nGrowCount /* = 64 */)
	{	return _map.Add(id, id, bAutoGrow, nGrowCount);	}

template<int stack_len>
inline bool _CQuickID::Del(UINT id)
	{	return _map.Del(id);	}

template<int stack_len>
inline void _CQuickID::DelAll()
	{	_map.DelAll();	}

template<int stack_len>
inline bool _CQuickID::Find(UINT id) const
	{	return -1 != _map.FindByID(id);	}

template<int stack_len>
inline int _CQuickID::Count() const
	{	return _map.Maps();	}

template<int stack_len>
inline UINT _CQuickID::operator [] (int index) const
	{	return _map[index];	}

#endif	// _QuickID_h_
