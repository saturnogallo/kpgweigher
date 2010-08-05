// MatchingString.h
//
// 字符串匹配

#ifndef _MatchingString_h_
#define _MatchingString_h_

inline bool IsLetter(int n)
	{	return (_T('a') <= n && n <= _T('z') || _T('A') <= n && n <= _T('Z'));	}
inline bool IsNum(int n)
	{	return (_T('0') <= n && n <= _T('9'));	}
inline bool IsLetterNum(int n)
	{	return IsLetter(n) || IsNum(n);	}
inline bool IsEqualChar(TCHAR ch1, TCHAR ch2, bool bMatchCase = true)
	{	return bMatchCase
				? ( ch1 == ch2 )
				: ( ((_T('a')<=ch1&&ch1<=_T('z')) ? (ch1-_T('a')+_T('A')) : ch1)
					== ((_T('a')<=ch2&&ch2<=_T('z')) ? (ch2-_T('a')+_T('A')) : ch2)
				  );
	}

//功  能：去掉字符串两端的空格、Tab、\r、\n
//参  数：...；start，返回trim后的起始位置
//返回值：trim后的长度
int StrTrim0(CONST TCHAR* src, int len, int* start = NULL);

//功  能：去掉字符串两端的空格、Tab、\r、\n
//参  数：dst，目标字符buffer，保证dst足够大『dst和src可以是同一个指针』
//返回值：dst
TCHAR* StrTrim(TCHAR* dst, CONST TCHAR* src, int len);

//功  能：字符串替换
//参  数：tszTarget，目标字符串；
//		  iBufSize，目标字符串缓冲区尺寸；
//		  tszRep，tszNew，用tszNew替换tszRep，tszNew可以为NULL
//返回值：成功既tszTarget，失败返回NULL
TCHAR* StrReplace(TCHAR* tszTarget, int iBufSize, CONST TCHAR* tszRep, CONST TCHAR* tszNew);

//功  能：把整数变为对应的字符串
//参  数：iNumeralFormat，输出数字的进制；
//说  明：二进制和十六进制按照无符号数处理
LPTSTR sprintfn(LPTSTR buf, __int64 num, int iNumeralFormat = 10);

//功  能：sprintfn()的增强版
//参  数：iDigit，输出位数，不够前面用0补足，超出按照实际位数输出(默认按照实际位数输出)；
//		  iSeparateSize，两个分隔字符之间的数字个数(默认没有分隔字符)；
//		  Separater，分隔字符；
//返回值：即是buf
LPTSTR sprintfnEn(LPTSTR buf, __int64 num, int iNumeralFormat = 10, int iDigit = -1, int iSeparateSize = -1, TCHAR Separater = _T(','));

//功  能：在lpszSour中查找字符串lpszFind，
//		  lpszFind中可以包含通配字符‘?’『表示任意字符』，‘#’『表示字符'0'--'9'』
//参  数：...
//		  nSourLen，nFindLen，源串和查找串的长度，默认为0结束的字符串
//返回值：成功返回匹配位置，否则返回-1
int  FindingString(LPCTSTR lpszSour, LPCTSTR lpszFind, bool bMatchCase = true, int nSourLen = -1, int nFindLen = -1);

//功	  能：带通配符的字符串匹配
//参	  数：lpszSour是一个普通字符串；
//			  lpszMatch是一可以包含通配符的字符串；
//			  bMatchCase，true，区分大小写[完全匹配]；false，不区分大小写。
//返  回  值：匹配，返回true；否则返回false。
//通配符意义：
//		‘*’	代表任意字符串，包括空字符串；
//		‘?’	代表任意一个字符，不能为空；『2001.11.02』
//		‘#’	代表字符‘0’—‘9’		『2003.06.26』
bool MatchingString(LPCTSTR lpszSour, LPCTSTR lpszMatch, bool bMatchCase = true);

//功  能：多重匹配，不同匹配字符串之间用‘,’或‘;’隔开
//			如：“*.h,*.cpp”将依次匹配“*.h”和“*.cpp”
//参  数：bAndLogic = false, 不同匹配求或，true求与；bMatchCase, 是否大小敏感
//返回值：
bool MultiMatching(LPCTSTR lpszSour, LPCTSTR lpszMatch, bool bAndLogic = false, bool bMatchCase = true);
//同上，只是把匹配子中的 "*." 用于匹配没有扩展名的文件名
bool MultiMatching2(LPCTSTR lpszSour, LPCTSTR lpszMatch, bool bAndLogic = false, bool bMatchCase = true);

#endif	//#ifndef _MatchingString_h_
