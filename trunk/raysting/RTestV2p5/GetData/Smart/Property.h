// Property.h
// 通用属性对象

#ifndef _Property_h_
#define _Property_h_

#include "AutoPtr.h"

//属性变量的数据类型
enum enumValueType {
	v_u32,
	v_str,
	v_bin,
};
typedef CAutoPtr<BYTE> CBinary;

class CProperty;

struct IProperty
{
	friend class CProperty;

protected:
	virtual void Copy(const IProperty& r) = 0;
	virtual bool EqualTo(const IProperty& r) const = 0;

protected:
	enumValueType		_valueType;
	union {
		UINT			_u32;
		CString*		_pstr;
		CBinary*		_pbin;
	};
};

//下面两个函数仅供class CProperty使用
extern "C" {
	//把objbuf构造成为IProperty接口对象
	void ConstructIProperty(void* objbuf, enumValueType valueType);
	//协构IProperty接口对象
	void DestructIProperty(void* objbuf);
}

class CProperty
{
	BYTE	m_data[sizeof(IProperty)];

protected:
	IProperty& Data()
		{	return *((IProperty*)m_data);	}
	const IProperty& Data() const
		{	return *((const IProperty*)m_data);	}	

public:
	explicit CProperty(enumValueType valueType = v_u32)
		{	ConstructIProperty(m_data, valueType);	}

	explicit CProperty(UINT u32)
		{	ConstructIProperty(m_data, v_u32);
			GetU32() = u32;
		}
	explicit CProperty(const CString& str)
		{	ConstructIProperty(m_data, v_str);
			GetStr() = str;
		}
	explicit CProperty(LPCTSTR str)
		{	ConstructIProperty(m_data, v_str);
			GetStr() = str;
		}
	explicit CProperty(const CBinary& bin)
		{	ConstructIProperty(m_data, v_bin);
			GetBin().Clone(bin, true);
		}

	~CProperty()
		{	DestructIProperty(m_data);	}

public:
	CProperty& operator = (const CProperty& r)
		{	Data().Copy(r.Data());
			return *this;
		}

	CProperty(const CProperty& r)
		{	ConstructIProperty(m_data, v_u32);
			*this = r;	}

public:
	bool operator == (const CProperty& r) const
		{	return Data().EqualTo(r.Data());	}
	bool operator != (const CProperty& r) const
		{	return !(*this == r);	}

public:
	enumValueType GetValueType() const
		{	enumValueType t = Data()._valueType;
			ASSERT(v_u32<=t && t<=v_bin);
			return t;	}

	UINT& GetU32()
		{	ASSERT(v_u32 == Data()._valueType);
			return Data()._u32;	}

	CString& GetStr()
		{	ASSERT(v_str == Data()._valueType);
			return *(Data()._pstr);	}

	CBinary& GetBin()
		{	ASSERT(v_bin == Data()._valueType);
			return *(Data()._pbin);	}

	UINT GetU32() const
		{	ASSERT(v_u32 == Data()._valueType);
			return Data()._u32;	}

	const CString& GetStr() const
		{	ASSERT(v_str == Data()._valueType);
			return *(Data()._pstr);	}

	const CBinary& GetBin() const
		{	ASSERT(v_bin == Data()._valueType);
			return *(Data()._pbin);	}
};

#endif	//#ifndef _Property_h_
