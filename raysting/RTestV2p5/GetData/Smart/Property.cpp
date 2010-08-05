// Property.cpp

#include "StdAfx.h"
#include "Property.h"

class CPropertyImplement : public IProperty
{
protected:
	virtual void Copy(const IProperty& r);
	virtual bool EqualTo(const IProperty& r) const;

public:
	CPropertyImplement()
		{	 _valueType = v_u32, _u32 = 0;	}

	void Construct(enumValueType v);
	void Destruct();
};

void CPropertyImplement::Copy(const IProperty& r)
{
	if(this == &r)
		return;

	CProperty& left = (CProperty&)*this;
	const CProperty& right = (const CProperty&)r;

	if(left.GetValueType() != right.GetValueType()) {
		Destruct();
		Construct(right.GetValueType());
	}

	if(v_str == right.GetValueType()) {
		left.GetStr() = right.GetStr();
	}
	else if(v_bin == right.GetValueType()) {
		CBinary& d = left.GetBin();
		const CBinary& s = right.GetBin();
		d.Clone(s, true);
	}
	else {
		ASSERT(v_u32 == right.GetValueType());
		left.GetU32() = right.GetU32();
	}
}

bool CPropertyImplement::EqualTo(const IProperty& r) const
{
	if(this == &r)
		return true;

	const CProperty& left = (const CProperty&)*this;
	const CProperty& right = (const CProperty&)r;

	if(left.GetValueType() != right.GetValueType())
		return false;

	if(v_str == right.GetValueType()) {
		return left.GetStr() == right.GetStr();
	}
	else if(v_bin == right.GetValueType()) {
		const CBinary& lptr = left.GetBin();
		const CBinary& rptr = right.GetBin();

		if(lptr.MaxSize() != rptr.MaxSize())
			return false;

		return 0 == memcmp(lptr.Get(), rptr.Get(), lptr.MaxSize());
	}
	else {
		ASSERT(v_u32 == right.GetValueType());
		return left.GetU32() == right.GetU32();
	}
}

void CPropertyImplement::Construct(enumValueType v)
{
	_valueType = v;

	if(v_str == _valueType) {
		ASSERT(NULL == _pstr);
		_pstr = new CString;
	}
	else if(v_bin == _valueType) {
		ASSERT(NULL == _pbin);
		_pbin = new CBinary;
	}
	else {
		ASSERT(v_u32 == _valueType);
		_u32 = 0;
	}
}

void CPropertyImplement::Destruct()
{
	if(v_str == _valueType) {
		if(_pstr)
			delete _pstr;
		_pstr = NULL;
	}
	else if(v_bin == _valueType) {
		if(_pbin)
			delete _pbin;
		_pbin = NULL;
	}
	else {
		ASSERT(v_u32 == _valueType);
		_u32 = 0;
	}

	_valueType = v_u32;
}

void ConstructIProperty(void* objbuf, enumValueType valueType)
{
	ConstructObjs((CPropertyImplement*)objbuf, 1);
	((CPropertyImplement*)objbuf)->Construct(valueType);
}

void DestructIProperty(void* objbuf)
{
	((CPropertyImplement*)objbuf)->Destruct();
}
