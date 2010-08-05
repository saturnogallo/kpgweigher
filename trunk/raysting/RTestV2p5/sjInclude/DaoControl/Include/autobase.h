#if !defined(__AUTOBASE_H)
#define __AUTOBASE_H

#include <windows.h>
#include <oleauto.h>

class Bstr {	//Wrapper for BSTR
	BSTR bstr;
public:
	Bstr() {bstr = 0;}
	Bstr(char*);
	Bstr(const char*);
	Bstr(const wchar_t*);
	Bstr(CString);
	~Bstr();
	operator CString();
	operator BSTR() {return bstr;}
	Bstr& operator=(wchar_t* const &);
	Bstr& operator=(Bstr const &);
};

class ostream;
class istream;

class Currency {	//Wrapper for CURRENCY
	__int64 int64;
public:
	Currency() {int64 = 0;}
	Currency(long double);
	Currency(__int64 i) {int64 = i;}
	Currency(char*);
	Currency(Currency const & c) {int64 = c.int64;}
	__int64 i64() const {return int64;}
	__int64 operator!() {return !int64;}
	operator void*() {return (void*)int64;}
	operator CURRENCY() {CURRENCY cy; cy.int64 = int64; return cy;}
	Currency& operator+=(Currency const &);
	inline Currency& operator-=(Currency const &);
	inline Currency& operator*=(Currency const &);
	inline Currency& operator/=(Currency const &);
	Currency& operator=(Currency const &);
	inline Currency& operator-();
	friend Currency operator+(Currency const &,Currency const &);
	friend Currency operator+(Currency const &,long double const &);
	friend Currency operator+(long double const &,Currency const &);
	friend Currency operator-(Currency const &,Currency const &);
	friend Currency operator-(Currency const &,long double const &);
	friend Currency operator-(long double const &,Currency const &);
	friend Currency operator*(Currency const &,Currency const &);
	friend Currency operator*(Currency const &,long double const &);
	friend Currency operator*(long double const &,Currency const &);
	friend Currency operator/(Currency const &,Currency const &);
	friend Currency operator/(Currency const &,long double const &);
	friend Currency operator/(long double const &,Currency const &);
	friend operator==(Currency const &,Currency const &);
	friend operator!=(Currency const &,Currency const &);
	friend operator<(Currency const &,Currency const &);
	friend operator<=(Currency const &,Currency const &);
	friend operator>(Currency const &,Currency const &);
	friend operator>=(Currency const &,Currency const &);
	friend ostream& operator<<(ostream&,Currency const &);
	friend istream& operator>>(istream&,Currency&);
};

class Variant : public VARIANT {	//Wrapper for VARIANT
public:
	Variant();
  	Variant(const char* str);
	Variant(CString const &);
	Variant(short);
	Variant(int);
	Variant(long);
	Variant(double);
	Variant(CURRENCY);
	Variant(Currency);
	Variant(bool);
	Variant(VARIANT&);
	Variant(Variant const &);
	Variant(void*);
	~Variant();
	bool operator!() {return vt == VT_NULL;}
	operator CString();
	operator long() {return lVal;}
	operator short() {return iVal;}
	operator double() {
   		if(vt != VT_R8)
   			VariantChangeType(this, this, 0, VT_R8);
		return dblVal;}
	operator bool() {
   		if(vt != VT_BOOL)
      		VariantChangeType(this, this, 0, VT_BOOL);
		return boolVal != FALSE;}
	operator CURRENCY() {
		if(vt != VT_CY)
   			VariantChangeType(this, this, 0, VT_CY);
   		return cyVal;}
	operator Currency() {
		if(vt != VT_CY)
   			VariantChangeType(this, this, 0, VT_CY);
   		return cyVal.int64;}
	Variant& operator=(Variant const &);
	DATE Date();
	LPCSTR c_str();
	CString rdate();
};

class DaoBase {		//Base class for all Dao classes
protected:
	IUnknown* punk;
	void SetInterface(IUnknown* p) {punk = p;}
public:
	IUnknown* GetInterface() const {return punk;}
	DaoBase(IUnknown* p = 0, BOOL addref = FALSE);
	virtual ~DaoBase() {Destroy();}
	void Destroy();
	DaoBase& operator=(DaoBase const &);
	virtual CString GetName() {return "DaoBase";}
	CString __declspec(property(get=GetName)) Name;
};

class ErrorInfo : public DaoBase {		//Exception handling class
public:
	ErrorInfo(IErrorInfo* = 0);
	ErrorInfo(ErrorInfo&);
	ErrorInfo(LPCSTR);
	ErrorInfo& operator=(ErrorInfo const &);
	void Show(HWND = 0);
	CString GetSource() const;
	CString GetDescription() const;
	CString GetHelpFile() const;
	DWORD GetHelpContext() const;
	CString __declspec(property(get=GetSource)) Source;
	CString __declspec(property(get=GetDescription)) Description;
	CString __declspec(property(get=GetHelpFile)) HelpFile;
	DWORD __declspec(property(get=GetHelpContext)) HelpContext;
};

void cprintf(char* buffer, Currency& m, bool=false, bool=false);	
			//Helper function for currency to char conversion

LPCSTR dtos(double, int = 0);

double stod(LPCSTR);

#endif