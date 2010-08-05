// MatchingString.cpp

#include "..\\StdAfx.h"
#include "SmartBuf.h"
#include "MatchingString.h"

int StrTrim0(CONST TCHAR* src, int len, int* start /* = NULL */)
{
	int begin = 0;
	while( begin < len
		&& (' ' == src[begin] || '\t' == src[begin] || '\r' == src[begin] || '\n' == src[begin])
		) begin++;

	int trim_len;

	if(begin == len) {
		begin = 0;
		trim_len = 0;
	}
	else {
		int end = len - 1;
		while( end >=0
			&& (' ' == src[end] || '\t' == src[end] || '\r' == src[end] || '\n' == src[end])
			) end--;

		trim_len = end - begin + 1;
	}

	if(start)
		*start = begin;

	return trim_len;
}

TCHAR* StrTrim(TCHAR* dst, CONST TCHAR* src, int len)
{
	int start;
	int trim_len = StrTrim0(src, len, &start);
	memmove(dst, src+start, trim_len*sizeof(TCHAR));
	dst[trim_len] = 0;
	return dst;
}

TCHAR* StrReplace(TCHAR* tszTarget, int iBufSize, CONST TCHAR* tszRep, CONST TCHAR* tszNew)
{
	int iBufChars = iBufSize / sizeof(TCHAR);

	ASSERT(AfxIsValidString(tszTarget));
	ASSERT(iBufChars>0);
	ASSERT(AfxIsValidString(tszRep));
	ASSERT(NULL==tszNew || AfxIsValidString(tszNew));

	int nTargetLen = _tcslen(tszTarget);
	int nRepLen = _tcslen(tszRep);
	int nReplacementLen = NULL==tszNew ? 0 : _tcslen(tszNew);

	TCHAR* tszStart = tszTarget;
	TCHAR* tszFind;
	while(NULL != (tszFind = _tcsstr(tszStart, tszRep))) {
		int offset = tszFind - tszTarget;
		if(nReplacementLen+offset >= iBufChars)	{ //Buffer 尺寸不够
			ASSERT(false);
			return NULL;
		}

		//移动被替换字符串后面的内容
		if(nReplacementLen != nRepLen) {
			memmove(tszFind + nReplacementLen, tszFind + nRepLen,
				(nTargetLen - nRepLen - offset + 1) * sizeof(TCHAR));	//移动时包括结束符
			nTargetLen += nReplacementLen - nRepLen;
		}

		//替换字符串
		if(nReplacementLen > 0) {
			memcpy(tszFind, tszNew, nReplacementLen * sizeof(TCHAR));
		}
		tszStart = tszFind + nReplacementLen;
	}

	return tszTarget;
}

LPTSTR sprintfn(LPTSTR buf, __int64 num, int iNumeralFormat /* = 10 */)
{
	*buf = 0;

	DWORD dwl = (DWORD)num;
	DWORD dwh = *(((DWORD*)&num) + 1);

	if(2 == iNumeralFormat) {
		*(buf + 64) = 0;

		int i = 63;

		while(i >= 32) {
			*(buf + i--) = TCHAR((dwl&1) + _T('0'));
			dwl >>= 1;
		}

		while(i >= 0) {
			*(buf + i--) = TCHAR((dwh&1) + _T('0'));
			dwh >>= 1;
		}
	}
	else if(16 == iNumeralFormat) {
		_stprintf(buf, _T("%08X%08X"), dwh, dwl);
	}
	else if(3<=iNumeralFormat && iNumeralFormat<=10) {
		bool bNegative;

		if(num < 0) {
			bNegative = true;
			num = -num;
		}
		else
			bNegative = false;

		TCHAR tmpBuf[65];
		int iBit = 0;

		if(0 == num)
			*(tmpBuf + iBit++) = _T('0');
		else {
			while(num) {
				int nch = int(num%iNumeralFormat);
				*(tmpBuf + iBit++) = TCHAR(nch + _T('0'));
				num /= iNumeralFormat;
			}
		}

		int iBit2 = 0;
		if(bNegative)
			*(buf + iBit2++) = _T('-');

		while( iBit > 0 )
			*(buf + iBit2++) = *(tmpBuf + --iBit);

		*(buf + iBit2) = 0;
	}
	else {
		ASSERT(false);
	}

	return buf;
}

LPTSTR sprintfnEn(LPTSTR buf,
				 __int64 num,
				 int iNumeralFormat /* = 10 */,
				 int iDigit /* = -1 */,
				 int iSeparateSize /* = -1 */,
				 TCHAR Separater /* = _T(',') */
				 )
{
	ASSERT(buf);

	sprintfn(buf, num, iNumeralFormat);

	if(iDigit > 0 || iSeparateSize > 0) {
		TCHAR* p = (_T('-') == buf[0]) ? buf+1 : buf;
		int digits = _tcslen(p);	//数字位数

		//在前面补0
		if(iDigit > 0) {
			int diff = iDigit - digits;
			if(diff > 0) {
				int i;
				for(i=digits; i>=0; i--)
					p[i+diff] = p[i];

				for(i=0; i<diff; i++)
					p[i] = _T('0');

				digits += diff;
			}
		}

		//在数字中间加上逗号分隔符
		if(iSeparateSize > 0) {
			int separ = (digits-1) / iSeparateSize;	//逗号个数
			if(separ > 0) {
				int i = separ + digits;
				int t = 0;
				while(i >= 0) {
					p[i--] = p[i-separ];
					t++;
					if(t == iSeparateSize+1) {
						p[i--] = Separater;
						if(0 == --separ)
							break;

						t = 1;
					}
				}
			}
		}
	}

	return buf;
}

static const int STACK_SIZE = 64;	//预设的编译堆栈尺寸

int FindingString(LPCTSTR lpszSour, LPCTSTR lpszFind, bool bMatchCase /* = true */, int nSourLen /* = -1 */, int nFindLen /* = -1 */)
{
	ASSERT(AfxIsValidString(lpszSour) && AfxIsValidString(lpszFind));

	if(-1 == nSourLen)
		nSourLen = _tcslen(lpszSour);
	if(-1 == nFindLen)
		nFindLen = _tcslen(lpszFind);

	int m = nSourLen;
	int n = nFindLen;

	ASSERT(m>0 && n>=0);

	if(n > m)
		return -1;
	else if(n == 0)
		return 0;

	CSmartBuf<int, STACK_SIZE> SmartBuf(n);
	if(!SmartBuf.IsValid())
		return -1;
	int* next = SmartBuf.Get();

	//KMP算法

	//得到查找字符串的next数组
	{	n--;

		int j = 0;
		int k = -1;
		next[0] = -1;

		while(j < n) {
			//if(k == -1 || lpszFind[k] == _T('?') || IsEqualChar(lpszFind[j], lpszFind[k], bMatchCase))
			//if(k == -1 || lpszFind[k] == _T('?') || lpszFind[j] == _T('?') || IsEqualChar(lpszFind[j], lpszFind[k], bMatchCase))
			if(k == -1 || lpszFind[k] == _T('?') || lpszFind[j] == _T('?')
				||
				( lpszFind[k]==_T('#')
					? IsNum(lpszFind[j])
					: ( lpszFind[j]==_T('#')
						? IsNum(lpszFind[k])
						: IsEqualChar(lpszFind[j], lpszFind[k], bMatchCase)
					  )
				)
			  ) {
				j++;
				k++;
				next[j] = k;
			}
			else
				k = next[k];
		}

		n++;
	}

	int i = 0;
	int j = 0;
	while(i < m && j < n) {
		//if(j == -1 || lpszFind[j] == _T('?') || IsEqualChar(lpszSour[i], lpszFind[j], bMatchCase))
		if(j == -1 || lpszFind[j] == _T('?')
			||
			( lpszFind[j]==_T('#')
				? IsNum(lpszSour[i])
				: IsEqualChar(lpszSour[i], lpszFind[j], bMatchCase)
			)
		  ) {
			i++;
			j++;
		}
		else
			j = next[j];
	}

	return j>=n ? i-n : -1;
}

bool MatchingString(LPCTSTR lpszSour, LPCTSTR lpszMatch, bool bMatchCase /* = true */)
{
	ASSERT(AfxIsValidString(lpszSour) && AfxIsValidString(lpszMatch));

	if(lpszMatch[0] == 0)//Is a empty string
		return (lpszSour[0] == 0);

	int nSourceLen = _tcslen(lpszSour);

	bool bMatched = true;	//匹配指示变量
	int nIndexMatch = 0;	//匹配下标
	int nIndexSour = 0;		//...

	//开始进行匹配检查
	while(lpszMatch[nIndexMatch] != _T('\0')) {
		if(lpszMatch[nIndexMatch] == _T('*')) {
			while(lpszMatch[nIndexMatch+1] == _T('*'))
				nIndexMatch++;

			if(lpszMatch[nIndexMatch+1] == 0) {
				//lpszMatch[nIndexMatch]是最后一个字符
				//bMatched = true;
				break;
			}
			else {
				//lpszMatch[nIndexMatch+1]是_T('?')或普通字符

				int nSubIndex = nIndexMatch+1;

				while(lpszMatch[nSubIndex] != _T('*') && lpszMatch[nSubIndex])
					nSubIndex++;

				if((nSourceLen-nIndexSour) < (nSubIndex-nIndexMatch-1)) {
					//源字符串剩下的长度小于匹配串剩下要求长度
					bMatched = false;
					break;
				}

				if(!lpszMatch[nSubIndex]) { //nSubIndex is point to ender of 'lpszMatch'
					//检查剩下部分字符是否一一匹配

					nSubIndex--;
					int nTempIndex = nSourceLen-1;
					//从后向前进行匹配
					while(lpszMatch[nSubIndex] != _T('*')) {
						if( lpszMatch[nSubIndex] == _T('?')
							|| (lpszMatch[nSubIndex]==_T('#')
								? IsNum(lpszSour[nTempIndex])
								: IsEqualChar(lpszMatch[nSubIndex], lpszSour[nTempIndex], bMatchCase)
								)
						  ) {
							nSubIndex--;
							nTempIndex--;
						}
						else {
							bMatched = false;
							break;
						}
					}

					break;
				}
				else { //lpszMatch[nSubIndex] == _T('*')
					nSubIndex -= nIndexMatch+1;

					int nPos = ::FindingString( lpszSour+nIndexSour,
												lpszMatch+nIndexMatch+1,
												bMatchCase,
												nSourceLen-nIndexSour,
												nSubIndex );

					if(nPos != -1) { //在'lpszSour+nIndexSour'中找到szTempFinder
						nIndexMatch += nSubIndex;
						nIndexSour += (nPos+nSubIndex-1);
					}
					else {
						bMatched = false;
						break;
					}
				}
			}
		}		//end of "if(lpszMatch[nIndexMatch] == _T('*'))"
		else if(lpszMatch[nIndexMatch] == _T('?')) {
			if(!lpszSour[nIndexSour]) {
				bMatched = false;
				break;
			}
			if(!lpszMatch[nIndexMatch+1] && lpszSour[nIndexSour+1]) {
				//如果lpszMatch[nIndexMatch]是最后一个字符，
				//且lpszSour[nIndexSour]不是最后一个字符
				bMatched = false;
				break;
			}
			nIndexMatch++;
			nIndexSour++;
		}
		else if(lpszMatch[nIndexMatch] == _T('#')) {
			if(!IsNum(lpszSour[nIndexSour])) {
				bMatched = false;
				break;
			}
			if(!lpszMatch[nIndexMatch+1] && lpszSour[nIndexSour+1]) {
				bMatched = false;
				break;
			}
			nIndexMatch++;
			nIndexSour++;
		}
		else { //lpszMatch[nIndexMatch]为常规字符
			if(!IsEqualChar(lpszSour[nIndexSour], lpszMatch[nIndexMatch], bMatchCase)) {
				bMatched = false;
				break;
			}
			if(!lpszMatch[nIndexMatch+1] && lpszSour[nIndexSour+1]) {
				bMatched = false;
				break;
			}
			nIndexMatch++;
			nIndexSour++;
		}
	}

	return bMatched;
}

static bool MultiMatchingEx(LPCTSTR lpszSour, LPCTSTR lpszMatch, bool bAndLogic, bool bMatchCase, bool bUseForFileName)
{
	ASSERT(AfxIsValidString(lpszSour) && AfxIsValidString(lpszMatch));

	CSmartBuf<TCHAR, STACK_SIZE*4> SmartBuf(_tcslen(lpszMatch) + 1);
	if(!SmartBuf.IsValid())
		return false;
	TCHAR* szSubMatch = SmartBuf.Get();

	bool bMatchSucc;

	if(!bAndLogic) { //求或
		bMatchSucc = false;
		int i = 0;
		int j = 0;
		while(true) {
			if(lpszMatch[i] != 0 && lpszMatch[i] != _T(',') && lpszMatch[i] != _T(';'))
				szSubMatch[j++] = lpszMatch[i];
			else {
				szSubMatch[j] = 0;
				if(j != 0) {
					if(bUseForFileName && 0 == _tcscmp(szSubMatch, _T("*.")))
						bMatchSucc = 0!=lpszSour[0] && !MatchingString(lpszSour, _T("*.*"), bMatchCase);
					else
						bMatchSucc = MatchingString(lpszSour, szSubMatch, bMatchCase);

					if(bMatchSucc)
						break;
				}
				j = 0;
			}

			if(lpszMatch[i] == 0)
				break;
			i++;
		}
	}
	else { //求与
		bMatchSucc = true;
		int i = 0;
		int j = 0;
		while(true) {
			if(lpszMatch[i] != 0 && lpszMatch[i] != _T(',') && lpszMatch[i] != _T(';'))
				szSubMatch[j++] = lpszMatch[i];
			else {
				szSubMatch[j] = 0;

				if(bUseForFileName && 0 == _tcscmp(szSubMatch, _T("*.")))
					bMatchSucc = 0!=lpszSour[0] && !MatchingString(lpszSour, _T("*.*"), bMatchCase);
				else
					bMatchSucc = MatchingString(lpszSour, szSubMatch, bMatchCase);

				if(!bMatchSucc)
					break;

				j = 0;
			}

			if(lpszMatch[i] == 0)
				break;
			i++;
		}
	}

	return bMatchSucc;
}

bool MultiMatching(LPCTSTR lpszSour, LPCTSTR lpszMatch, bool bAndLogic /* = false */, bool bMatchCase /* = true */)
	{	return MultiMatchingEx(lpszSour, lpszMatch, bAndLogic, bMatchCase, false);	}

bool MultiMatching2(LPCTSTR lpszSour, LPCTSTR lpszMatch, bool bAndLogic /* = false */, bool bMatchCase /* = true */)
	{	return MultiMatchingEx(lpszSour, lpszMatch, bAndLogic, bMatchCase, true);	}
