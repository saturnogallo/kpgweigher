// SmartStr.h
// 一个简单的字符串类，用于移植，代替非MFC环境下CString 的部分功能
// 见 "PreDef.h"

#ifndef _SmartStr_h_
#define _SmartStr_h_

#include "SmartBuf.h"
#include "MatchingString.h"

class CSmartStr
{
public:
	typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> >
		TStr;

	TStr _str;

public:
	CSmartStr& operator = (LPCTSTR str)					{	_str = str; return *this;		}
	CSmartStr& operator = (const TStr& str)				{	_str = str; return *this;		}
	CSmartStr& operator = (const CSmartStr& r)			{	_str = r._str; return *this;	}

	CSmartStr() {}
	CSmartStr(LPCTSTR str)								{	_str = str;		}
	CSmartStr(const TStr& str)							{	_str = str;		}
	CSmartStr(const CSmartStr& r)						{	_str = r._str;	}

	operator LPCTSTR () const							{	return _str.c_str();	}

public:
	int Compare(LPCTSTR str) const						{	return _str.compare(str);	}
	int Compare(const CSmartStr& r) const				{	return _str.compare(r._str);	}
	int CompareNoCase(LPCTSTR str) const				{	return _tcsicmp(_str.c_str(), str);	}
	int CompareNoCase(const CSmartStr& r) const			{	return _tcsicmp(_str.c_str(), r._str.c_str());	}

public:
	int GetLength() const								{	return _str.length();	}
	bool IsEmpty() const								{	return _str.empty();	}
	void Empty()										{	_str.erase(_str.begin(), _str.end());	}

	TCHAR GetAt(int i) const							{	return _str.at(i);	}
	TCHAR operator[](int i) const						{	return _str.at(i);	}
	void SetAt(int i, TCHAR ch)							{	_str.at(i) = ch;	}

public:
	CSmartStr Mid(int nFirst, int nCount = -1) const	{	return CSmartStr(_str.substr(nFirst, -1==nCount ? _str.length() : nCount).c_str());	}
	CSmartStr Left(int nCount) const					{	return CSmartStr(_str.substr(0, nCount).c_str());	}
	CSmartStr Right(int nCount) const					{	return CSmartStr(_str.substr(_str.length()-nCount, nCount).c_str());	}

public:
	void MakeUpper()
	{
		int len = _str.length();
		for(int i=0; i<len; i++) {
			if('a' <= _str.at(i) && _str.at(i) <= 'z')
				_str.at(i) += 'A' - 'a';
		}
	}
	void MakeLower()
	{
		int len = _str.length();
		for(int i=0; i<len; i++) {
			if('A' <= _str.at(i) && _str.at(i) <= 'Z')
				_str.at(i) += 'a' - 'A';
		}
	}

public:
	int Find(TCHAR ch, int nStart = 0) const			{	return _str.find(ch, nStart);	}
	int Find(LPCTSTR lpszSub, int nStart = 0) const		{	return _str.find(lpszSub, nStart);	}
	int ReverseFind(TCHAR ch) const						{	return _str.rfind(ch);	}

	void Format(LPCTSTR lpszFormat, ...)
	{
		CSmartBuf<TCHAR, 4096> buf(_tcslen(lpszFormat) + 2048);

		va_list pars;
		va_start(pars, lpszFormat);
		_vstprintf(buf.Get(), lpszFormat, pars);
		va_end(pars);

		_str = buf.Get();
	}

public:
	void Replace(TCHAR chOld, TCHAR chNew)				{	_str.replace(_str.begin(), _str.end(), chOld, chNew);	}
	void Replace(LPCTSTR lpszOld, LPCTSTR lpszNew)		{	_str.replace(_str.begin(), _str.end(), lpszOld, lpszNew);	}
	void Remove(TCHAR chRemove)							{	TCHAR ch[2] = {chRemove}; _str.replace(_str.begin(), _str.end(), ch, "");	}

	void Insert(int nIndex, TCHAR ch)					{	_str.insert(_str.begin() + nIndex, ch);	}
	void Insert(int nIndex, LPCTSTR pstr)				{	_str.insert(nIndex, pstr);	}
	void Delete(int nIndex, int nCount = 1)				{	_str.erase(_str.begin() + nIndex, _str.begin() + nIndex + nCount);	}

public:
	void TrimLeft()
	{
		int start;
		StrTrim0(_str.c_str(), _str.length(), &start);
		Delete(0, start);
	}
	void TrimRight()
	{
		int str_len = _str.length();
		int start;
		int len = StrTrim0(_str.c_str(), str_len, &start);
		start += len;
		Delete(start, str_len-start);
	}

public:
	const CSmartStr& operator += (const CSmartStr& r)	{	_str += r._str;	return *this;	}
	const CSmartStr& operator += (LPCTSTR lpsz)			{	_str += lpsz; return *this;	}
};

inline bool operator == (const CSmartStr& s1, const CSmartStr& s2)		{	return 0 == s1.Compare(s2);	}
inline bool operator == (const CSmartStr& s1, LPCTSTR s2)				{	return 0 == s1.Compare(s2);	}
inline bool operator == (LPCTSTR s1, const CSmartStr& s2)				{	return 0 == s2.Compare(s1);	}
inline bool operator != (const CSmartStr& s1, const CSmartStr& s2)		{	return 0 != s1.Compare(s2);	}
inline bool operator != (const CSmartStr& s1, LPCTSTR s2)				{	return 0 != s1.Compare(s2);	}
inline bool operator != (LPCTSTR s1, const CSmartStr& s2)				{	return 0 != s2.Compare(s1);	}

inline CSmartStr operator + (const CSmartStr& s1, const CSmartStr& s2)	{	CSmartStr::TStr s(s1._str); s += s2._str; return CSmartStr(s);	}
inline CSmartStr operator + (const CSmartStr& s1, LPCTSTR s2)			{	CSmartStr::TStr s(s1._str); s += s2; return CSmartStr(s);	}
inline CSmartStr operator + (LPCTSTR s1, const CSmartStr& s2)			{	CSmartStr::TStr s(s1); s += s2._str; return CSmartStr(s);	}

#endif	//#ifndef _SmartStr_h_
